#ifndef __SPI_HARAWARE__H__
#define __SPI_HARAWARE__H__
	 
#include "sys.h"


#define SPI1_CLK	GPIO_Pin_5
#define SPI1_CLK_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define SPI1_CLK_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define SPI1_MISO	GPIO_Pin_6
#define SPI1_MISO_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define SPI1_MISO_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_6) 

#define SPI1_MOSI	GPIO_Pin_7
#define SPI1_MOSI_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define SPI1_MOSI_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_7) 

/***************** SPI2 ********************************/

#define SPI2_CLK	GPIO_Pin_13
#define SPI2_CLK_HIGH	GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define SPI2_CLK_LOW	GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define SPI2_MISO	GPIO_Pin_14
#define SPI2_MISO_HIGH	GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define SPI2_MISO_LOW	GPIO_ResetBits(GPIOB,GPIO_Pin_14) 

#define SPI2_MOSI	GPIO_Pin_15
#define SPI2_MOSI_HIGH	GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define SPI2_MOSI_LOW	GPIO_ResetBits(GPIOB,GPIO_Pin_15) 


void SPI_GPIO_Init(void);//≥ı ºªØ
void SPI1_Init(void);
uint8_t SPI1_ReadWrite_Byte(uint8_t tx_dat);
void SPI1_Speed_Set(uint8_t speed);

void SPI2_Init(void);
uint8_t SPI2_ReadWrite_Byte(uint8_t tx_dat);

		 				    
#endif


