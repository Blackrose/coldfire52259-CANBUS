/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

#include "mcp2515.h"
//#include "qspi.h"

int main(void)
{
	int counter = 0;
    tQSPIBuffers *MyBuf;
    int8 i8Return = 0;
    uint8 i = 0, j;
	uint8 u8Continue = 0;
	can_msg_t canmsg;
	uint8 data;

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif
		
	printf("\"QSPI\" module examples on MCF52259\n");
		
	/* QSPI Basic Initialization */
	QSPIInit(500, 0, 8, 5, 15);

#if 0
	MyBuf = QSPI_InitFullBuffer(1);
	        
	MyBuf->pu16TxData[0] = CAN_RESET;
	MyBuf->pu8Cmd[0] = CMD_VAL;

	QSPIPollBufferTransfer(MyBuf);
	QSPI_FreeFullBuffer(MyBuf);
#else
	reset_2515();
	get_status_2515();
	

	//bit_modify_2515(CANCTRL, 0xe0, 0x0);

#endif
	
#if 0
	MyBuf = QSPI_InitFullBuffer(3);
	MyBuf->pu16TxData[0] = CAN_WRITE;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	MyBuf->pu16TxData[1] = CNF2;
	MyBuf->pu8Cmd[1] = CMD_VAL;
	MyBuf->pu16TxData[2] = 0x26;
	MyBuf->pu8Cmd[2] = CMD_VAL;
	QSPIPollBufferTransfer(MyBuf);
	QSPI_FreeFullBuffer(MyBuf);
#endif
	
	//test_2515();

	config_2515();

	
#if 0
	MyBuf = QSPI_InitFullBuffer(3);
	MyBuf->pu16TxData[0] = CAN_READ;
	MyBuf->pu8Cmd[0] = CMD_VAL;
	MyBuf->pu16TxData[1] = CNF1;
	MyBuf->pu8Cmd[1] = CMD_VAL;
	MyBuf->pu8Cmd[2] = CMD_VAL;
	QSPIPollBufferTransfer(MyBuf);	
	
	for (j=0; j < MyBuf->u8Size; j++)
	{
		printf("%x\n", MyBuf->pu16TxData[j]);
	}
	printf("OK\n"); 
	QSPI_FreeFullBuffer(MyBuf);
#endif


	
#if 1
	while(1){
		i++;
	bit_modify_2515(TXB0CTRL, 0x8, 0x0);

		write_data_2515(TXB0ID, i);
		write_data_2515(TXB0ID+1, 0x0);
		write_data_2515(TXB0DLC, 1);
		write_data_2515(TXB0DATA, 1);

	request_send_2515(1);
	
	while(data = read_data_2515(TXB0CTRL) & 0x8){
		printf("Failed data=%x\n", data);
	}
	
	}
#endif
	    
	printf ("End of QSPI Examples\n");
	
	for(;;) {	   
	   	counter++;
	}
}



