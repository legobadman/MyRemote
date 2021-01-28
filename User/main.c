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
#include "led.h"
#include "key.h"

//#define ENABLE_I2C
//#define I2c_Hardware
extern vu16 ADC_DMA_IN[4];	//摇杆数值存放点

#define HByte(u16_num) ((u16_num & 0xFF00) >> 8)
#define LByte(u16_num) (u16_num & 0x00FF)

//#define DEBUG_NRF24L01
//#define DEBUG_REMOTE

//uint8_t txbuf[TX_PLOAD_WIDTH] = {10,2,3,7,5,6,4,8,9,1};
uint8_t txbuf[TX_PLOAD_WIDTH] = {0};

int main (void){//主程序
	uint8_t ret,i,lock_toggle = 0;
	delay_ms(500); //上电时等待其他器件就绪
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //系统时钟初始化 
	USART1_Init(115200);
	SPI_GPIO_Init();
	SPI1_Init();
	KEY_init();
	LED_Init();
	printf("core init\r\n");

#ifdef ENABLE_I2C
#ifdef I2c_Hardware
	I2C_Configuration();//I2C初始化
#else
	IIC_Init();
#endif
#endif

#ifdef DEBUG_NRF24L01

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
			for(i=0; i<TX_PLOAD_WIDTH; i++)
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
	
	//主流程
	NRF24L01_Init();
	
	printf("nrf24l01 start!\r\n");
	while (NRF24L01_Check())
	{
		printf("nrf24l01 01 failed!\r\n");
	}
	NRF24L01_TX_Mode();


	ADC_Configuration();
	printf("ADC inited\r\n");
	
	while(1) {
		lock_toggle = 0;
		if (GPIO_ReadInputDataBit(KEY_PORT, S81_PIN) == 0 || GPIO_ReadInputDataBit(KEY_PORT, S82_PIN) == 0)
		{
			delay_ms(20);
			if (GPIO_ReadInputDataBit(KEY_PORT, S81_PIN) == 0 || GPIO_ReadInputDataBit(KEY_PORT, S82_PIN) == 0)
			{
				lock_toggle = 1;
			}
		}
		txbuf[0] = LByte(ADC_DMA_IN[0]);
		txbuf[1] = HByte(ADC_DMA_IN[0]);

		txbuf[2] = LByte(ADC_DMA_IN[1]);
		txbuf[3] = HByte(ADC_DMA_IN[1]);

		txbuf[4] = LByte(ADC_DMA_IN[2]);
		txbuf[5] = HByte(ADC_DMA_IN[2]);

		txbuf[6] = LByte(ADC_DMA_IN[3]);
		txbuf[7] = HByte(ADC_DMA_IN[3]);

		txbuf[8] = lock_toggle;
		
		ret = NRF24L01_TX_Packet(txbuf);
		if (ret == TX_OK)
		{
			for(i = 0; i < TX_PLOAD_WIDTH; i++)
			{
				printf("nrf1 send data3 is %d \r\n",txbuf[i]);
			}
		}
		else if (ret == MAX_TX)
		{
			printf("ret = MAX_TX\r\n");
		}
		else
		{
			printf("ret != TX_OK\r\n");
		}
		delay_ms(200);
	}
}
