/*
 * main implementation: use this sample to create your own application
 *
 */

#include "flexcan.h"
#include "uart.h"


#include "support_common.h" /* include peripheral declarations and more */
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

static void printArray(uint8 *data, uint8 len);

int main(void)
{
	int counter = 0;

    uint8 aDummy[] = "hellowor";
    uint8 aDummyR[8] = { 0};
    uint8 i,u8Len, ret;
    uint8 sendmb = 0, recvmb = 1;
    uint16 id = 0;
	
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n");
#endif
	
	printf("FlexCAN module examples on MCF52259\n");
	
    /* FlexCAN Basic Initialization */
    FlexCANInit();
    
    //MCF_FlexCAN_DISABLE_LOOPBACK();
    
    //MCF_FlexCAN_ENABLE_LOOPBACK();
        

    
    for (i = 123; i < 300; i++)
    //while(1)
    {
    	id = i;
    
    	//FlexCANSetMBforRx (recvmb, FLEXCAN_STANDARDID(id));   
    	printf("********************************\n");
#if 1
    	ret = FlexCANSendDataPoll(aDummy, 8, FLEXCAN_STANDARDID(id), sendmb);
        if (ret) 
        {
        	printf("send error\n");
        }
        
        printf("Transmit: ID:%d\n", id);
#else
        
        u8Len = FlexCANReceiveDataPoll (recvmb, (uint8 *)aDummyR);
        //printf("Message Received: Msg=%s\n", aDummyR);
        printArray(aDummyR, u8Len);
#endif
        printf("********************************\n");
   }
    
    //FlexCANSendDataNoPoll(sendbuf, 1, FLEXCAN_STANDARDID(0x123), sendmb);
    
}

static void printArray(uint8 *data, uint8 len){
	int i;
	
	for(i = 0; i < len; i++){
		printf("%d\t", i, data[i]);
	}
	
	printf("\n");
	
}
