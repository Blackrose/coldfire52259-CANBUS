/*
 * mcp2515.c
 *
 *  Created on: 2013-1-8
 *      Author: chen
 */

#include "mcp2515.h"
#include <stdio.h>
#include <stdlib.h>

// each entry of queue must be fill this CMD
#ifdef SPI_IN_CAN

#define CMD_VAL (1 << 15 | 1 << 14 | 1 << 13 | 1 << 12 | 0xf << 8)

#else

#define CMD_VAL (1 << 15 | 1 << 14 | 1 << 13 | 1 << 12 | 0x7 << 8)

#endif

void write_data_2515(uint8 addr, uint8 data)
{
	int i;
    tQSPIBuffers *MyBuf = NULL;
	
	MyBuf = qspi_init_buffer(3);
	
	MyBuf->tx_data[0] = CAN_WRITE;
	MyBuf->cmd[0] = CMD_VAL;
	
	MyBuf->tx_data[1] = addr;
	MyBuf->cmd[1] = CMD_VAL;
	
	MyBuf->tx_data[2] = data;
	MyBuf->cmd[2] = CMD_VAL;
	
	MyBuf->stat = QSPI_BUFFSTAT_TXRDY;
	
	QSPIPollBufferTransfer(MyBuf);
	qspi_free_buffer(MyBuf);
	
	return;
}


void reset_2515()
{
    tQSPIBuffers *MyBuf = NULL;
	
	MyBuf = qspi_init_buffer(1);
	MyBuf->tx_data[0] = CAN_RESET;
	MyBuf->cmd[0] = CMD_VAL;
	MyBuf->stat = QSPI_BUFFSTAT_TXRDY;
	
	QSPIPollBufferTransfer(MyBuf);
	qspi_free_buffer(MyBuf);
	
	return;
}

void bit_modify_2515(uint8 addr, uint8 mask, uint8 data)
{
    tQSPIBuffers *MyBuf = NULL;
	
	MyBuf = qspi_init_buffer(4);
	
	MyBuf->tx_data[0] = CAN_BIT_MODIFY;
	MyBuf->cmd[0] = CMD_VAL;
	MyBuf->tx_data[1] = addr;
	MyBuf->cmd[1] = CMD_VAL;
	MyBuf->tx_data[2] = mask;
	MyBuf->cmd[2] = CMD_VAL;
	MyBuf->tx_data[3] = data;
	MyBuf->cmd[3] = CMD_VAL;
	
	MyBuf->stat = QSPI_BUFFSTAT_TXRDY;
		
	QSPIPollBufferTransfer(MyBuf);
	qspi_free_buffer(MyBuf);
	
	return;
}

uint8 read_data_2515(uint8 addr)
{
    tQSPIBuffers *MyBuf;
    int j;
    uint16 tmp, ret;
	
	MyBuf = qspi_init_buffer(3);
	
	MyBuf->tx_data[0] = CAN_READ;
	MyBuf->cmd[0] = CMD_VAL;
	MyBuf->tx_data[1] = addr;
	MyBuf->cmd[1] = CMD_VAL;
	MyBuf->cmd[2] = CMD_VAL;

	MyBuf->stat = QSPI_BUFFSTAT_RXRDY;
	
	QSPIPollBufferTransfer(MyBuf);
	
	tmp = MyBuf->rx_data[2];
	
#if 0
	printf ("\n----------start------------\n");
	printf("Next is address %x data:\n", addr);
	for (j=0; j < MyBuf->size; j++)
	{
		printf("%x\t", MyBuf->rx_data[j]);
	}
	printf ("\n-----------end--------------\n");
#endif

	qspi_free_buffer(MyBuf);
	return tmp;
}

void request_send_2515(uint8 number)
{
    tQSPIBuffers *MyBuf = NULL;
	
    // mcp2515 has three transmit buffer 1, 2, 3
    if(number < 4 && number > 0){
    
    	MyBuf = qspi_init_buffer(1);
	
    	MyBuf->tx_data[0] = CAN_RTS | number;
    	MyBuf->cmd[0] = CMD_VAL;
    	
    	MyBuf->stat = QSPI_BUFFSTAT_TXRDY;
	
    	QSPIPollBufferTransfer(MyBuf);
    	qspi_free_buffer(MyBuf);
    }
    return;
}

uint8 get_status_2515(void)
{
    tQSPIBuffers *MyBuf = NULL;
    int j;
    uint8 buf;
	
	MyBuf = qspi_init_buffer(1);
	
	MyBuf->tx_data[0] = CAN_RD_STATUS;
	MyBuf->cmd[0] = CMD_VAL;
	
	MyBuf->stat = QSPI_BUFFSTAT_RXRDY;
	
	QSPIPollBufferTransfer(MyBuf);
	buf = MyBuf->rx_data[0];
#if 0
	for (j=0; j < MyBuf->size; j++)
	{
		printf("%x\t", MyBuf->rx_data[j]);
	}
#endif
	qspi_free_buffer(MyBuf);
	
	return buf;
}

void config_2515(uint8 brp, uint8 sjw, uint8 prop, uint8 ps1, uint8 ps2)
{
	
	// enter Configure Mode
	bit_modify_2515(CANCTRL, 0xe7, 0x85);
	printf("CANCTRL = %x\n", read_data_2515(CANCTRL));
	
	// setting bit-timing
	write_data_2515(CNF1, SJW_BRP(sjw, brp));
	write_data_2515(CNF2, PS1_PRP(ps1, prop));
	write_data_2515(CNF3, ps2);
	
	// disable RTS and Buff-pin
	write_data_2515(TXRTSCTRL, 0);
	write_data_2515(BFPCTRL, 0);
	
	write_data_2515(RXB0CTRL, 0x60);
	write_data_2515(RXB0CTRL, 0x60);
	
	write_data_2515(RXF0SIDH, 0x00);
	write_data_2515(RXF0SIDL, 0X00);
	
	// enable interrupt
	write_data_2515(CANINTE, 0x5);
	
	// enter Normal Mode
	bit_modify_2515(CANCTRL, 0xe8, 0x0);
	
	printf("CANSTAT = %x\n", read_data_2515(CANSTAT));

}

void test_2515()
{
	bit_modify_2515(CANCTRL, 0xe7, 0x85);
	printf("CANCTRL = %x\n", read_data_2515(CANCTRL));
	
	write_data_2515(CNF1, 0x25);
	write_data_2515(CNF2, 0x98);
	write_data_2515(CNF3, 0x3);
	
	printf("%x\n", read_data_2515(CNF1));
	printf("%x\n", read_data_2515(CNF2));
	printf("%x\n", read_data_2515(CNF3));
}

uint8 can_send_2515(uint32 id, uint8 *data, uint8 size)
{
	uint8 ret;
	uint8 i;
	
	bit_modify_2515(TXB0CTRL, 0x8, 0x0);
	
	do{
		ret = read_data_2515(CANINTF) & 0x4;
	}while(ret);

	if(!(id >> 11)){
		// Standard ID
		
		write_data_2515(TXB0ID, GET_STDIDH(id));
		write_data_2515(TXB0ID+1, GET_STDIDL(id));
		write_data_2515(TXB0DLC, size);
		
		for(i = 0; i < size; i++){
			write_data_2515(TXB0DATA+i, data[i]);
		}
	}else{
		// Extend ID
	}
	request_send_2515(1);

	i = 1;
	do{
		ret = read_data_2515(CANINTF) & 0x4;
		if(ret)
			bit_modify_2515(CANINTF, 0x4, 0);
		
		if(i++ == 10)
			return -1;
		
	}while(ret);
	
	//bit_modify_2515(CANINTF, 0x4, 0);

	
	return 0;
}

uint8 can_recv_2515(uint32 *id, uint8 *buffer)
{
	uint8 dlc;
	uint8 i;
	
	while(!(read_data_2515(CANINTF) & 0x1))
		;

	// Standard ID
	*id = (read_data_2515(RXB0ID) << 3) | read_data_2515(RXB0ID+1)>>5;
	
	dlc = read_data_2515(RXB0DLC) & 0xff;
	
	for (i = 0; i < dlc; i++)
		buffer[i] = read_data_2515(RXB0DATA + i);
		
	// clear flag
	bit_modify_2515(CANINTF, 0x1, 0);
	return dlc;
}

void enable_irq_2515(void)
{
	// set gpio for irq3
	MCF_GPIO_PNQPAR |= (1 << 7);
}
