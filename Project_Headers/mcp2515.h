/*
 * mcp2515.h
 *
 *  Created on: 2012-12-25
 *      Author: chen
 */

#ifndef MCP2515_H_
#define MCP2515_H_

#include <support_common.h>
#include "qspi.h"

typedef struct {
        unsigned int rxfsid[6];  //验收滤波设置：6个
        unsigned int rxmsid[2];  //验收屏蔽设置：2个
        unsigned char cnf1;               //位定时器设置CNF1
        unsigned char cnf2;               //位定时器设置CNF2
        unsigned char cnf3;               //位定时器设置CNF3
        unsigned char caninte;            //中断时能寄存器
}CAN_STRUCT;                      /*控制寄存器结构*/

/*
 * CAN Frame Struct
 */
typedef struct {
	unsigned char id[2];              //ID of Frame
    unsigned char length;             //Data length
    unsigned char data[8];            //Data
} can_frm_t;                     

typedef union {
	uint32 allid;
	struct {
		uint8 stdh;
		uint8 stdlexth;
		uint8 exth;
		uint8 extl;
	}std_ext_id;
} can_id_t;

#define SJW_BRP(x,y) ((x << 6) | y)
#define PS1_PRP(x,y) ((1 << 7) | (x << 3) | y)

#define GET_STDIDH(x) ((x & 0x7f8)>>3)
#define GET_STDIDL(x) ((id & 0x7)<<5)

/*
 * bit timing register
 */
#define CNF1 0x2a
#define CNF2 0x29
#define CNF3 0x28

/*
 * control register
 */
#define CANCTRL 0x0f
#define CANSTAT 0x0e
#define TXRTSCTRL 	0x0D
#define BFPCTRL 	0x0C

/*
 * Receive Mask Register
 */
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25

/*
 * Receive Filter Register
 */
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19 

/*
 * Interrupt Register
 */
#define CANINTE 0x2b
#define CANINTF 0x2c  

/*
 * Send Buffer
 */
#define TXB0CTRL 0x30
#define TXB0ID 0x31
#define TXB0DLC 0x35
#define TXB0DATA 0x36
#define TXB1ID 0x41
#define TXB1DLC 0x45
#define TXB1DATA 0x46
#define TXB2ID 0x51
#define TXB2DLC 0x55
#define TXB2DATA 0x56

/*
 * Receive Buffer
 */
#define RXB0CTRL 0x60
#define RXB0ID 0x61
#define RXB0DLC 0x65
#define RXB0DATA 0x66
#define RXB1ID 0x71
#define RXB1DLC 0x75
#define RXB1DATA 0x76


// Receive Masks
#define RXM0SIDH	0x20
#define RXM0SIDL	0x21
#define RXM0EID8	0x22
#define RXM0EID0	0x23
#define RXM1SIDH	0x24
#define	RXM1SIDL	0x25
#define RXM1EID8	0x26
#define RXM1EID0	0x27

// SPI Command
#define CAN_RESET	0xC0
#define CAN_READ	0x03
#define CAN_WRITE	0x02
#define CAN_RTS		0x80
#define CAN_RTS_TXB0	0x81
#define CAN_RTS_TXB1	0x82
#define CAN_RTS_TXB2	0x84
#define CAN_RD_STATUS	0xa0
#define CAN_BIT_MODIFY	0x05
#define CAN_RX_STATUS   0xB0
#define CAN_RD_RX_BUF	0x90
#define CAN_LOAD_TX		0x40

void test_2515();
void write_data_2515(uint8 ,uint8);
void reset_2515(void);
uint8 read_data_2515(uint8);
void bit_modify_2515(uint8 , uint8 , uint8);

void config_2515(uint8 brp, uint8 sjw, uint8 prop, uint8 ps1, uint8 ps2);
void request_send_2515(uint8);
uint8 get_status_2515(void);

void can_send_2515(uint32 id, uint8 *data, uint8 size);
uint8 can_recv_2515(uint32 *id, uint8 *buffer);

#endif /* MCP2515_H_ */
