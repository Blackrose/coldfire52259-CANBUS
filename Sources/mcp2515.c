/*
 * mcp2515.c
 *
 *  Created on: 2013-1-8
 *      Author: chen
 */

#include "mcp2515.h"
#include <stdio.h>
#include <stdlib.h>

#if 0
#define CMD_VAL ((1 << 15) | MCF_QSPI_QDR_BITSE | \
		(~MCF_QSPI_QDR_DT) | MCF_QSPI_QDR_DSCK | ~(MCF_QSPI_QDR_QSPI_CS3))>>8
#endif

#define CMD_VAL (1 << 15 | 1 << 14 | 1 << 13 | 1 << 12 | 0x7 << 8)
void write_data_2515(uint8 addr, uint8 data)
{
    tQSPIBuffers *MyBuf;
	
	MyBuf = QSPI_InitFullBuffer(3);
	
	MyBuf->pu16TxData[0] = CAN_WRITE;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	MyBuf->pu16TxData[1] = addr;
	MyBuf->pu8Cmd[1] = CMD_VAL;
	MyBuf->pu16TxData[2] = data;
	MyBuf->pu8Cmd[2] = CMD_VAL;
	
	QSPIPollBufferTransfer(MyBuf);
	QSPI_FreeFullBuffer(MyBuf);
	
	return;
}

void reset_2515()
{
    tQSPIBuffers *MyBuf;
	
	MyBuf = QSPI_InitFullBuffer(1);
	//printf("%x\n", CMD_VAL);
	MyBuf->pu16TxData[0] = CAN_RESET;
	MyBuf->pu8Cmd[0] = 0x77;
	
	QSPIPollBufferTransfer(MyBuf);
	QSPI_FreeFullBuffer(MyBuf);
	
	return;
}

void bit_modify_2515(uint8 addr, uint8 mask, uint8 data)
{
    tQSPIBuffers *MyBuf;
	
	MyBuf = QSPI_InitFullBuffer(4);
	
	MyBuf->pu16TxData[0] = CAN_BIT_MODIFY;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	MyBuf->pu16TxData[1] = addr;
	MyBuf->pu8Cmd[1] = CMD_VAL;
	MyBuf->pu16TxData[2] = mask;
	MyBuf->pu8Cmd[2] = CMD_VAL;
	MyBuf->pu16TxData[3] = data;
	MyBuf->pu8Cmd[3] = CMD_VAL;
		
	QSPIPollBufferTransfer(MyBuf);
	QSPI_FreeFullBuffer(MyBuf);
	
	return;
}

uint8 read_data_2515(uint8 addr)
{
    tQSPIBuffers *MyBuf;
    int j;
    uint8 data;
	
	MyBuf = QSPI_InitFullBuffer(3);
	
	MyBuf->pu16TxData[0] = CAN_READ;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	MyBuf->pu16TxData[1] = addr;
	MyBuf->pu8Cmd[1] = CMD_VAL;
	MyBuf->pu8Cmd[2] = CMD_VAL;

	
	
	QSPIPollBufferTransfer(MyBuf);
	
	printf("Next is address %x data:\n", addr);
	for (j=0; j < MyBuf->u8Size; j++)
	{
		printf("%x\t", MyBuf->pu16RxData[j]);
	}
	data = MyBuf->pu16RxData[MyBuf->u8Size - 1];
	printf ("\n--------------------------\n");
	
	QSPI_FreeFullBuffer(MyBuf);
	
	return data;
}

void request_send_2515(uint8 number)
{
    tQSPIBuffers *MyBuf;
	
	MyBuf = QSPI_InitFullBuffer(1);
	
	MyBuf->pu16TxData[0] = CAN_RTS | number;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	
	QSPIPollBufferTransfer(MyBuf);
	QSPI_FreeFullBuffer(MyBuf);
}

void get_status_2515(void)
{
    tQSPIBuffers *MyBuf;
    int j;
	
	MyBuf = QSPI_InitFullBuffer(1);
	
	MyBuf->pu16TxData[0] = CAN_RD_STATUS;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	
	QSPIPollBufferTransfer(MyBuf);
	
	for (j=0; j < MyBuf->u8Size; j++)
	{
		printf("%x\t", MyBuf->pu16RxData[j]);
	}
	
	printf ("\n--------------------------\n");
	QSPI_FreeFullBuffer(MyBuf);
}

void config_2515()
{
	uint8 brp = 1;
	uint8 sjw = 0xc0;
	uint8 prop = 0x2;
	uint8 ps1 = (7 << 3);
	uint8 ps2 = 0x7;
	
	// enter Configure Mode
	bit_modify_2515(CANCTRL, 0xe7, 0x85);
	read_data_2515(CANCTRL);
	
	// setting bit-timing
	write_data_2515(CNF1, sjw | brp);
	write_data_2515(CNF2, ps1 | prop);
	write_data_2515(CNF3, ps2);
	
	// disable RTS and Buff-pin
	write_data_2515(TXRTSCTRL, 0);
	write_data_2515(BFPCTRL, 0);
	
	write_data_2515(RXB0CTRL, 0x60);
	write_data_2515(RXB0CTRL, 0x60);
	
	write_data_2515(RXF0SIDH, 0x00);
	write_data_2515(RXF0SIDL, 0X00);
	
	// enter Normal Mode
	bit_modify_2515(CANCTRL, 0xe8, 0x0);
	
	read_data_2515(CANSTAT);

}

void test_2515()
{
	write_data_2515(CNF1, 0x25);
	write_data_2515(CNF2, 0x98);
	//write_data_2515(CNF3, 0x3);
	
	read_data_2515(CNF1);
	//read_data_2515(CNF2);
	//read_data_2515(CNF3);
}


