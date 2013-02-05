/*
 * File:    qspi.h
 * Purpose: Header for the Queued Serial Peripheral Interface
 * 
 * License:     All software covered by license agreement in -
 *              docs/Freescale_Software_License.pdf
 */
 
 
#ifndef _QSPI_H_
#define _QSPI_H_

//#include "common.h"
#include "support_common.h"

/*
 * QSPI Macros
 */
#define QSPI_GPIO_INIT()  MCF_GPIO_PQSPAR = 0x1555;
#define QSPI_ENABLE_SPIF()  (MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIFE)
#define QSPI_DISABLE_SPIF()  (MCF_QSPI_QIR &= ~MCF_QSPI_QIR_SPIFE)
#define QSPI_ENABLE_ABRT()  (MCF_QSPI_QIR |= MCF_QSPI_QIR_ABRTE)
#define QSPI_DISABLE_ABRT()  (MCF_QSPI_QIR &= ~MCF_QSPI_QIR_ABRTE)
#define QSPI_ENABLE_WCEF()  (MCF_QSPI_QIR |= MCF_QSPI_QIR_WCEFE)
#define QSPI_DISABLE_WCEF()  (MCF_QSPI_QIR &= ~MCF_QSPI_QIR_WCEFE)
#define QSPI_ENABLE_ABRTLOCK() (MCF_QSPI_QIR |= MCF_QSPI_QIR_ABRTL)
#define QSPI_DISABLE_ABRTLOCK() (MCF_QSPI_QIR &= ~MCF_QSPI_QIR_ABRTL)
#define QSPI_ABORT_TX_SPE() (MCF_QSPI_QDLYR &= ~MCF_QSPI_QDLYR_SPE)



/*
 * Constants
 */
#define QSPI_IDLE   0x00
#define QSPI_BUSY   0x01

#define QSPI_BUFFSTAT_IDLE      0x00
#define QSPI_BUFFSTAT_TXRDY     0x01
#define QSPI_BUFFSTAT_BUSY      0x02
#define QSPI_BUFFSTAT_RXRDY     0x03
#define QSPI_BUFFSTAT_FIN   	0x05
#define QSPI_BUFFSTAT_ABORTED   0x04
#define QSPI_TRANSMIT_ADDRESS 	0x00
#define QSPI_RECEIVE_ADDRESS 	0x10
#define QSPI_COMMAND_ADDRESS 	0x20

/*
 * QSPI Buffer structure definition
 */
typedef struct tQSPIBuffers
{
    uint8 size;         
    uint16 *tx_data;
    uint16 *rx_data;
    uint16  *cmd;
    uint8 stat;
}tQSPIBuffers;


/*
 * Functions provided by this driver
 */
int8 qspi_init(uint16 u16Baudrate, uint8 u8ClkAttrib, 
              uint8 u8Bits, uint8 u8ClkDly, uint8 u8DlyAft);
void PIT1_Init(void);

int8 QSPIPollBufferTransfer(tQSPIBuffers *sQSPIBuff);

struct tQSPIBuffers* qspi_init_buffer(uint8 u8Size);
int8 qspi_free_buffer(tQSPIBuffers *sQSPIBuff);

int8 QSPIIntBufferTransfer(tQSPIBuffers *sQSPIBuff);

void QSPI_Enable_Abort(void);
void QSPI_Disable_Abort(void);


__declspec(interrupt) void qspi_isr(void);
__declspec(interrupt) void PIT1_Interrupt(void);

#endif /* _QSPI_H_ */
