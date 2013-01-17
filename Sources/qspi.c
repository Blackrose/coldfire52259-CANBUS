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
tQSPIBuffers *sQSPIInterruptBuf;

void (* QSPI_SPIF_ISR) (void);
void (* QSPI_ABRT_ISR) (void);
static void QSPISetTransferCommand (unsigned char u8CS, unsigned char u8Cont);
static void QSPISetTransferData (unsigned char u8Data);

void cs_select();
void cs_deselect();

/********************************************************************/
/* 
 * Initialize the QSPI
 */
int8 QSPIInit(uint16 u16Baudrate, uint8 u8ClkAttrib, 
              uint8 u8Bits, uint8 u8ClkDly, 
              uint8 u8DlyAft) 
{
    int8 i8Ret=0;
    
    MCF_GPIO_PQSPAR = MCF_GPIO_PQSPAR_QSPI_DOUT_DOUT |
		MCF_GPIO_PQSPAR_QSPI_DIN_DIN |
		MCF_GPIO_PQSPAR_QSPI_CLK_CLK |
		//MCF_GPIO_PQSPAR_QSPI_CS0_CS0 |
		//MCF_GPIO_PQSPAR_QSPI_CS2_CS2 |
		MCF_GPIO_PQSPAR_QSPI_CS3_CS3 ;
    //MCF_GPIO_PORTQS = ~(0x0);
    //MCF_GPIO_PQSPAR = 0x1555;

    /* Set as a Master always and set CPOL & CPHA */
    MCF_QSPI_QMR = (MCF_QSPI_QMR_MSTR |         
                        ((u8ClkAttrib & 0x3) << 8));    
    
    i8Ret |= QSPISetBits(u8Bits);
    i8Ret |= QSPISetBaudrate(u16Baudrate);
    
    MCF_QSPI_QDLYR = (MCF_QSPI_QDLYR_QCD(u8ClkDly) | 
                          MCF_QSPI_QDLYR_DTL(u8DlyAft));
    /*
    MCF_QSPI_QIR = (MCF_QSPI_QIR_WCEFB | MCF_QSPI_QIR_ABRTB |
                          MCF_QSPI_QIR_ABRTL | MCF_QSPI_QIR_WCEF |               
                          MCF_QSPI_QIR_ABRT | MCF_QSPI_QIR_SPIF);
    */

    MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV;
    
    // disable interrupt
    MCF_QSPI_QIR = 0 | 0x1;
    //MCF_INTC0_ICR18 = MCF_INTC_ICR_IL(3) | MCF_INTC_ICR_IP(6);
    //MCF_INTC0_IMRH &= (0);
    //MCF_INTC0_IMRL &= (MCF_INTC_IMRL_INT_MASK18 | MCF_INTC_IMRL_MASKALL);

    return 0;
}

/* 
 * Set the Baudrate
 */
int8 QSPISetBaudrate(uint16 u16Baudrate)
{
    uint16 u16Baud; 
    
    u16Baud = ((40 * 1000 / 2) / u16Baudrate);
     
    if ((u16Baud> 1 ) && (u16Baud < 255)){
         MCF_QSPI_QMR &= ~MCF_QSPI_QMR_BAUD(0xFF);
         MCF_QSPI_QMR |= MCF_QSPI_QMR_BAUD((uint8)u16Baud);
         printf("Baud rate %d\n", u16Baud);
        return 0;
    }
    else
        return -1;
}

/* 
 * Set Bits
 */
int8 QSPISetBits(uint8 u8NumberofBits)
{
    uint8 u8Bits; 
    
    if (u8NumberofBits >7 && u8NumberofBits < 17){
        u8Bits = (u8NumberofBits & 0x0F);
        MCF_QSPI_QMR &= ~MCF_QSPI_QMR_BITS(0xF);
        MCF_QSPI_QMR |= MCF_QSPI_QMR_BITS(u8Bits);
        return 0;
    }
    else{
        return -1;
    }
}

/* 
 * Send Byte
 */
void QSPISendByteRaw(uint8 u8Byte, uint8 u8CS)
{

    MCF_QSPI_QAR = QSPI_COMMAND_ADDRESS;
    QSPISetTransferCommand(u8CS, 0);
    MCF_QSPI_QAR = QSPI_TRANSMIT_ADDRESS;
    QSPISetTransferData(u8Byte);
    
    MCF_QSPI_QWR =  MCF_QSPI_QWR_CSIV|MCF_QSPI_QWR_ENDQP(0x00)|MCF_QSPI_QWR_NEWQP(0x00);
    MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
    
}

/* 
 * Send Word
 */
void QSPISendWordRaw(uint16 u16Word, uint8 u8CS)
{

    MCF_QSPI_QAR = QSPI_COMMAND_ADDRESS;
    QSPISetTransferCommand(u8CS, 0);
    MCF_QSPI_QAR = QSPI_TRANSMIT_ADDRESS;
    MCF_QSPI_QDR = u16Word;
    
    MCF_QSPI_QWR =  MCF_QSPI_QWR_CSIV|MCF_QSPI_QWR_ENDQP(0x00)|MCF_QSPI_QWR_NEWQP(0x00);
    MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
    
}

/* 
 * Send Byte via Polling
 */
uint8 QSPIPollTransferByteRaw(uint8 u8Byte, uint8 u8CS)
{

    MCF_QSPI_QAR = QSPI_COMMAND_ADDRESS;
    QSPISetTransferCommand(u8CS, 0);
    MCF_QSPI_QAR = QSPI_TRANSMIT_ADDRESS;
    QSPISetTransferData(u8Byte);
    
    MCF_QSPI_QWR =  MCF_QSPI_QWR_CSIV|MCF_QSPI_QWR_ENDQP(0x01)|MCF_QSPI_QWR_NEWQP(0x00);
    MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
    
    while (!(MCF_QSPI_QIR & MCF_QSPI_QIR_SPIF))
        ;
    
    MCF_QSPI_QAR = QSPI_RECEIVE_ADDRESS;
    return MCF_QSPI_QDR;
}

/* Init a QSPI Buffer for SPI transfers
 *   1. Allocate memory for the Buffer
 *   2. Allocate memory for the Transmit Buffer
 *   3. Allocate memory for the Reception Buffer
 *   4. Allocate memory for the Commands
 *   5. Initialize flags 
 */
struct tQSPIBuffers* QSPI_InitFullBuffer(uint8 u8Size)
{
    tQSPIBuffers *ptr;
    
    ptr=(tQSPIBuffers*)malloc(sizeof(tQSPIBuffers));
    //memset(ptr, 0, sizeof(tQSPIBuffers));
    ptr->u8Size=u8Size;
    
    ptr->pu16TxData=(uint16*)malloc((u8Size)*sizeof(uint16));
    //memset(ptr->pu16TxData, 0, sizeof(uint16)*u8Size);
    ptr->pu16RxData=(uint16*)malloc((u8Size)*sizeof(uint16));
    memset(ptr->pu16RxData, 0, sizeof(uint16)*u8Size);
    ptr->pu8Cmd=(uint16*)malloc((u8Size)*sizeof(uint16));
    //memset(ptr->pu8Cmd, 0, sizeof(uint8)*u8Size);

    return ptr;
}

/********************************************************************/
/* Free QSPI Buffer for SPI transfers
 *   1. Free memory for the Transmit Buffer
 *   2. Free memory for the Reception Buffer
 *   3. Free memory for the Commands
 *   4. Release flags 
 *   5. Free Buffer memory
 */
int8 QSPI_FreeFullBuffer(tQSPIBuffers *sQSPIBuff)
{
    free (sQSPIBuff->pu16TxData);
    free (sQSPIBuff->pu16RxData);
    free (sQSPIBuff->pu8Cmd);
    free (sQSPIBuff);
    return 0;
}

/********************************************************************/
/* 
 * Transfer using Polling
 */
int8 QSPIPollBufferTransfer(tQSPIBuffers *sQSPIBuff)
{
    uint8 u8Counter =0;
    uint8 j;
    
    MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIF;
    

    for (j=0; j < sQSPIBuff->u8Size; j++){
    	MCF_QSPI_QAR = QSPI_COMMAND_ADDRESS+j;
        MCF_QSPI_QDR = MCF_QSPI_QDR_DATA(sQSPIBuff->pu8Cmd[j]);
        MCF_QSPI_QAR = QSPI_TRANSMIT_ADDRESS+j;
        MCF_QSPI_QDR = sQSPIBuff->pu16TxData[j];
    }
    
    MCF_QSPI_QWR = (MCF_QSPI_QWR & MCF_QSPI_QWR_CSIV) |
                        MCF_QSPI_QWR_ENDQP((sQSPIBuff->u8Size)-1) | MCF_QSPI_QWR_NEWQP(0);
    
    MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
        
	while (!(MCF_QSPI_QIR & MCF_QSPI_QIR_SPIF))
            ;

	MCF_QSPI_QAR = QSPI_RECEIVE_ADDRESS;
	for (j=0; j < sQSPIBuff->u8Size; j++){
		//printf("b:%x\n", sQSPIBuff->pu16RxData[j]);
		sQSPIBuff->pu16RxData[j] = MCF_QSPI_QDR;
	}
    MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIF;
        
    return 0;
}

/********************************************************************/
/* 
 * Set the transfer command	
 */
static void QSPISetTransferCommand (unsigned char u8CS, unsigned char u8Cont)
{
    MCF_QSPI_QDR = (u8Cont << 15)|(MCF_QSPI_QDR_BITSE | MCF_QSPI_QDR_DT |
                        MCF_QSPI_QDR_DSCK | ~(MCF_QSPI_QDR_QSPI_CS3));
    return;
}

/********************************************************************/
/* 
 * Transfer data by putting it into QDR	
 */
static void QSPISetTransferData (unsigned char u8Data)
{
    MCF_QSPI_QDR = u8Data;
    return;
}

void cs_select()
{
	MCF_GPIO_PORTQS &= ~(1 << 6);
}

void cs_deselect()
{
	MCF_GPIO_PORTQS |= (1 << 6);
}

