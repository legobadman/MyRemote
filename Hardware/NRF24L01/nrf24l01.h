#ifndef __NRF24L01_H__
#define __NRF24L01_H__
	 
#include "sys.h"


#define NRF_CE	GPIO_Pin_15
#define NRF_CE_HIGH	GPIO_SetBits(GPIOA, NRF_CE)
#define NRF_CE_LOW	GPIO_ResetBits(GPIOA, NRF_CE)

#define NRF_CS	GPIO_Pin_4 
#define NRF_CS_HIGH	GPIO_SetBits(GPIOA, NRF_CS)
#define NRF_CS_LOW	GPIO_ResetBits(GPIOA, NRF_CS)

#define NRF_IRQ	GPIO_Pin_11
#define NRF_IRQ_STATUS	(GPIO_ReadInputDataBit(GPIOA, NRF_IRQ)) //PAin(1)

//命令宏定义
#define R_REGISTER			0x00
#define W_REGISTER			0x20
#define R_RX_PAYLOAD		0x61
#define W_TX_PAYLOAD		0XA0
#define FLUSH_TX			0xE1
#define FLUSH_RX			0xE2
#define RESUE_TX_PL			0xE3
#define ACTIVATE			0x50
#define R_RX_PL_WID			0x60
#define W_AC_PAYLOAD		0xA8
#define W_TX_PAYLOAD_NOACK	0xB0
#define NRF24L01_NOP		0xFF

//NRF24L01寄存器
#define	NRF24L01_CONFIG		0x00	//配置寄存器:bit0:0-TX;1-RX;bit1:上电使能选择;bit2:CRC模式;bit3:CRC使能
									//bit4:最大重发次数中断(MAX_RT)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA				0x01
#define EN_RXADDR			0x02
#define SETUP_AW			0x03
#define SETUP_PETR			0x04
#define RF_CH				0x05
#define RF_SETUP			0x06
#define NRF24L01_STATUS		0x07
#define OBSERVE_TX			0x08
#define RPD					0X09			
#define RX_ADDR_P0			0x0A
#define RX_ADDR_P1			0x0B
#define RX_ADDR_P2			0x0C
#define RX_ADDR_P3			0x0D
#define RX_ADDR_P4			0x0E
#define RX_ADDR_P5			0x0F
#define TX_ADDR				0x10
#define RX_PW_P0			0x11
#define RX_PW_P1			0x12
#define RX_PW_P2			0x13
#define RX_PW_P3			0x14
#define RX_PW_P4			0x15
#define RX_PW_P5			0x16
#define FIFO_STATUS			0x17
#define DYNPD				0x1C
#define NRF24L01_FEATURE	0x1D

//nrf24l01发送数据宽度定义
#define TX_ADR_WIDTH		5	//5字节的地址宽度
#define RX_ADR_WIDTH		5	//5字节的地址宽度
#define TX_PLOAD_WIDTH		5//32	//32字节的用户数据宽度
#define RX_PLOAD_WIDTH		5//32	//32字节的用户数据宽度

#define MAX_TX				0x10	//达到最大发送次数中断
#define TX_OK				0x20	//发送完成中断
#define RX_OK				0x40	//接收数据中断

void NRF24L01_GPIO_Init(void);
uint8_t NRF24L01_Write_Reg(uint8_t res,uint8_t value);
uint8_t NRF24L01_Read_Reg(uint8_t res);
uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf);
uint8_t NRF24L01_Check(void);
void NRF24L01_Power_Down_Mode(void);
void NRF24L01_TX_Mode(void);
void NRF24L01_RX_Mode(void);
uint8_t NRF24L01_TX_Packet(uint8_t *txbuf);
uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf);
void NRF24L01_Init(void);//初始化

#endif







