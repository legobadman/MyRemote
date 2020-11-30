#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "i2c_hardware.h"
#include "i2c_software.h"
#include "spi.h"

#include "math.h"
#include "nrf24l01.h"
#include "adc.h"

//#define ENABLE_I2C
//#define I2c_Hardware
extern vu16 ADC_DMA_IN[4];	//摇杆数值存放点

#define DEBUG_NRF24L01
//#define DEBUG_REMOTE

uint8_t txbuf[5]={2,2,10,14,25};
uint8_t rxbuf[5]={0,0,0,0,0};

int main (void){//主程序
	uint8_t ret,i;
	delay_ms(500); //上电时等待其他器件就绪
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //系统时钟初始化 
	USART1_Init(115200);
	printf("core init\r\n");

#ifdef ENABLE_I2C
#ifdef I2c_Hardware
	I2C_Configuration();//I2C初始化
#else
	IIC_Init();
#endif
#endif

#ifdef DEBUG_NRF24L01
	SPI_GPIO_Init();
	SPI1_Init();
	NRF24L01_Init();
	printf("nrf24l01 start!\r\n");
	while (NRF24L01_Check())
	{
		printf("nrf24l01 01 failed!\r\n");
	}
	NRF24L01_TX_Mode();
	while(1)
	{
		ret = NRF24L01_TX_Packet(txbuf);
		if (ret == TX_OK)
		{
			printf("nrf1 send is sucessed!\r\n");
			printf("\r\n");
			for(i=0;i<5;i++)
			{
				printf("nrf1 send data is %d \r\n",txbuf[i]);
			}
		}
		else if (ret == MAX_TX)
		{
			printf("ret = MAX_TX\r\n");
		}
		else {
			printf("ret != TX_OK\r\n");
		}
		delay_ms(500);
	}
#endif

	printf("CrazePony serial port test passed!\r\n");
#ifdef DEBUG_REMOTE
	ADC_Configuration();
	printf("ADC inited\r\n");
	while(1) {
		printf("Left X:%d, Y:%d\r\n", ADC_DMA_IN[0], ADC_DMA_IN[1]);
		printf("Right X:%d, Y:%d\r\n", ADC_DMA_IN[2], ADC_DMA_IN[3]);
		printf("\r\n\r\n");
		delay_ms(200);
	}
#endif
}