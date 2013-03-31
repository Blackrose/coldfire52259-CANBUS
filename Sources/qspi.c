/*
 * File:    qspi.c
 * Purpose: Driver for the Queued Serial Peripheral Interface    
 * 
 * License:     All software covered by license agreement in -
 *              docs/Freescale_Software_License.pdf
 */

#include "qspi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Initialize Variables     
 */
tQSPIBuffers *qspi_crt_buf;

void handle_qspi_int(void);
uint8 qspi_isfin(void);
uint8 qspi_setbits(uint8 bits);
uint8 qspi_setbaudrate(uint16 baudrate);


/* 
 * Initialize the QSPI
 */
int8 qspi_init(uint16 baudrate, uint8 clk_attr, 
              uint8 bits, uint8 clk_dly, 
              uint8 dly_aft, CS_PIN pin) 
{
    int8 i8Ret=0;
    uint8 cs;
    
    // use FB_A6 as CS pin
#ifdef SPI_IN_CAN
    MCF_GPIO_DDRTE |= (1 << 6);
    //MCF_GPIO_PORTTE |= (0 << 6);
    MCF_GPIO_PTEPAR = (0 << 6);
    //MCF_GPIO_SETTE |= (0 << 6);
#endif 
    
    switch(pin){
    case CS_PIN0:
    	cs = MCF_GPIO_PQSPAR_QSPI_CS0_CS0;
    	break;
    case CS_PIN2:
    	cs = MCF_GPIO_PQSPAR_QSPI_CS2_CS2;
    	break;
    case CS_PIN3:
    	cs = MCF_GPIO_PQSPAR_QSPI_CS3_CS3;
    	break;
    case OTHER:
    	cs = 0;
    	break;
    default:
    	cs = 0;
    	break;
    }
    	
    
    MCF_GPIO_PQSPAR = MCF_GPIO_PQSPAR_QSPI_DOUT_DOUT |
		MCF_GPIO_PQSPAR_QSPI_DIN_DIN |
		MCF_GPIO_PQSPAR_QSPI_CLK_CLK |
		MCF_GPIO_PQSPAR_QSPI_CS3_CS3 ;
    
    //MCF_GPIO_PQSPAR = 0x1555;
    

    /* Set as a Master always and set CPOL & CPHA */
    MCF_QSPI_QMR = (MCF_QSPI_QMR_MSTR | 
    		((clk_attr & 0x3) << 8));    
    
    i8Ret |= qspi_setbits(bits);
    i8Ret |= qspi_setbaudrate(baudrate);
    
    MCF_QSPI_QDLYR = (MCF_QSPI_QDLYR_QCD(clk_dly) | 
    		MCF_QSPI_QDLYR_DTL(dly_aft));


    MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV;

    // enable interrupt
    MCF_INTC0_IMRH &= ~(0);
    MCF_INTC0_IMRL &= ~(MCF_INTC_IMRL_INT_MASK18 | MCF_INTC_IMRL_MASKALL);
    MCF_INTC0_ICR18 = MCF_INTC_ICR_IL(7) | MCF_INTC_ICR_IP(7);
    MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIFE | MCF_QSPI_QIR_SPIF;
    
    /*
    MCF_QSPI_QIR = (MCF_QSPI_QIR_WCEFB | MCF_QSPI_QIR_ABRTB |
                          MCF_QSPI_QIR_ABRTL | MCF_QSPI_QIR_WCEF |               
                          MCF_QSPI_QIR_ABRT | MCF_QSPI_QIR_SPIF);
    */
    
    return 0;
}

void PIT1_Init(void)
{
  
	//Configure the interrupt level and priority
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIF; //Clear the flag  
	
	MCF_PIT1_PCSR = 0|MCF_PIT_PCSR_PRE(0) | MCF_PIT_PCSR_RLD | 
			MCF_PIT_PCSR_PIE | MCF_PIT_PCSR_OVW;
	
	//参阅PIT章节具体的计算公式 1ms定时  
	MCF_PIT1_PMR = MCF_PIT_PMR_PM(19);
	
	//PIT1初始化
	MCF_INTC0_ICR56 = MCF_INTC_ICR_IL(6) | MCF_INTC_ICR_IP(6);
	
	//Open the local mask 参阅中断章节
	MCF_INTC0_IMRH &= ~MCF_INTC_IMRH_INT_MASK56;    
	MCF_INTC0_IMRL &= ~MCF_INTC_IMRL_MASKALL; // Open the maskall

	
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_EN;//使能
}

/* 
 * Set the Baudrate
 */
uint8 qspi_setbaudrate(uint16 baudrate)
{
    uint16 u16Baud; 
    
    u16Baud = ((40 * 1000 / 2) / baudrate);
     
    if ((u16Baud> 1 ) && (u16Baud < 255)){
         MCF_QSPI_QMR &= ~MCF_QSPI_QMR_BAUD(0xFF);
         MCF_QSPI_QMR |= MCF_QSPI_QMR_BAUD((uint8)u16Baud);
         printf("Baud rate div = %d\n", u16Baud);
        return 0;
    }
    else
        return -1;
}

/* 
 * Set Bits
 */
uint8 qspi_setbits(uint8 bits)
{
    uint8 u8Bits; 
    
    if (bits >7 && bits < 17){
        u8Bits = (bits & 0x0F);
        MCF_QSPI_QMR &= ~MCF_QSPI_QMR_BITS(0xF);
        MCF_QSPI_QMR |= MCF_QSPI_QMR_BITS(u8Bits);
        return 0;
    }
    else{
        return -1;
    }
}

/* 
 * Init a QSPI Buffer for SPI transfers
 */
struct tQSPIBuffers* qspi_init_buffer(uint8 size)
{
    tQSPIBuffers *ptr;
    
    ptr=(tQSPIBuffers*)malloc(sizeof(tQSPIBuffers));
    memset(ptr, 0, sizeof(tQSPIBuffers));
    ptr->size = size;
    
    ptr->tx_data=(uint16*)malloc((size)*sizeof(uint16));
    memset(ptr->tx_data, 0, sizeof(uint16)*size);
    ptr->rx_data=(uint16*)malloc((size)*sizeof(uint16));
    memset(ptr->rx_data, 0, sizeof(uint16)*size);
    ptr->cmd=(uint16*)malloc((size)*sizeof(uint16));
    memset(ptr->cmd, 0, sizeof(uint8)*size);

    ptr->stat = QSPI_BUFFSTAT_IDLE;
    return ptr;
}


/* 
 * Free QSPI Buffer for SPI transfers
 */
int8 qspi_free_buffer(tQSPIBuffers *sQSPIBuff)
{
    free (sQSPIBuff->tx_data);
    free (sQSPIBuff->rx_data);
    free (sQSPIBuff->cmd);
    free (sQSPIBuff);
    return 0;
}

/* 
 * Transfer using Polling
 */
int8 QSPIPollBufferTransfer(tQSPIBuffers *sQSPIBuff)
{
    uint8 u8Counter =0;
    uint8 j;
    
    MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIF;
    
    qspi_crt_buf = sQSPIBuff;
    
    for (j=0; j < sQSPIBuff->size; j++){
    	MCF_QSPI_QAR = QSPI_COMMAND_ADDRESS+j;
        MCF_QSPI_QDR = MCF_QSPI_QDR_DATA(sQSPIBuff->cmd[j]);
        MCF_QSPI_QAR = QSPI_TRANSMIT_ADDRESS+j;
        MCF_QSPI_QDR = sQSPIBuff->tx_data[j];
    }
        
    MCF_QSPI_QWR = (MCF_QSPI_QWR & MCF_QSPI_QWR_CSIV) |
		MCF_QSPI_QWR_ENDQP((sQSPIBuff->size)-1) | 
		MCF_QSPI_QWR_NEWQP(0);
    
#ifdef SPI_IN_CAN
    MCF_GPIO_PORTTE &= (0 << 6);
#endif
    MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
        
    while(!qspi_isfin()){
    	if(j++ == 100){
    		printf("Transfer error\n");
    		return -1;
    	}
    }
#ifdef SPI_IN_CAN 
    MCF_GPIO_PORTTE |= (1 << 6);
#endif
    
    return 0;
}

/* 
 * Check to transfer queue is finish	
 */
uint8 qspi_isfin(void)
{
    if (qspi_crt_buf->stat == QSPI_BUFFSTAT_FIN) 
        return 1;
    else
        return 0;
}

void handle_qspi_int(void)
{
	int j;
	
	switch(qspi_crt_buf->stat){
	case QSPI_BUFFSTAT_TXRDY:
		qspi_crt_buf->stat = QSPI_BUFFSTAT_FIN;
		break;
	case QSPI_BUFFSTAT_RXRDY:
		{
			MCF_QSPI_QAR = QSPI_RECEIVE_ADDRESS;
			for (j=0; j < qspi_crt_buf->size; j++)
			{
				qspi_crt_buf->rx_data[j] = MCF_QSPI_QDR;
			}
		}
		qspi_crt_buf->stat = QSPI_BUFFSTAT_FIN;
		break;
	default:
		break;
	}
}

__declspec(interrupt) void qspi_isr(void)
{		
	MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIF;
	
	handle_qspi_int();
}

__declspec(interrupt) 
void PIT1_Interrupt(void){
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIF;
	printf("i gott\n");
}
