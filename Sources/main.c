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
	uint8 sendbuf[8] = {1,1,1,1,1,1,1,1};
	int counter = 0;
    int8 i8Return = 0;
    uint16 i = 100, j;
	uint8 u8Continue = 0;
	can_frm_t canmsg;
	uint8 recvbuf[8] = {0};
	uint8 dlc;
	uint32 id;

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif
		
	printf("\"QSPI\" module examples on MCF52259\n");
		
	/* QSPI Basic Initialization */
	QSPIInit(5000, 0, 8, 2, 5);

	reset_2515();
	printf("STAT = %x\n", get_status_2515());
		
	//test_2515();

	// bit timing is 5, 7, 7
	config_2515(1, 0x3, 3, 6, 7);

#if 1
	
	if(can_send_2515(0x123, sendbuf, 4) < 0)
		printf("send error\n");
	
	while(1){
		i++;
		can_send_2515(i, sendbuf, 8);
	}
#else
	dlc = can_recv_2515(&id, recvbuf);
	printf("ID=%x, DLC=%d\n", id, dlc);
	for(i = 0; i < dlc; i++)
		printf("%x\t", recvbuf[i]);
#endif
	    
	printf ("End of QSPI Examples\n");
	
	for(;;) {	   
	   	counter++;
	}
}



