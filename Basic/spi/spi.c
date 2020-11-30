#include "spi.h"
#include "delay.h"
#include "sys.h"

/*****************************************************************************
* 函  数：void SPI_GPIO_Init(void)
* 功  能：SPI1相关GPIO初始化配置
* 参  数：无
* 返回值：无
* 备  注：无
*****************************************************************************/
void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_CLK|SPI1_MISO|SPI1_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,SPI1_CLK|SPI1_MISO|SPI1_MOSI);
}

/*****************************************************************************
* 函  数：void SPI1_Init(void)
* 功  能：SPI1初始化配置
* 参  数：无
* 返回值：无
* 备  注：无
*****************************************************************************/
void SPI1_Init(void)
{
	SPI_InitTypeDef SPI1_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//spi1在APB2总线上
	
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式:设置为主SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//选择了串行时钟的稳态:时钟悬空低电平
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//数据捕获于第一个时钟沿
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//定义波特率预分频的值:波特率预分频值为16:72/16=4.5MHz
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI1_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	SPI_Init(SPI1,&SPI1_InitStructure);
	SPI_Cmd(SPI1,ENABLE);

	SPI1_ReadWrite_Byte(0xFF);//启动传输
}

/*****************************************************************************
* 函  数：uint8_t SPI1_ReadWrite_Byte(uint8_t tx_dat)
* 功  能：SPI1收发数据
* 参  数：tx_dat:发送的数据
* 返回值：接收的数据
* 备  注：无
*****************************************************************************/
uint8_t SPI1_ReadWrite_Byte(uint8_t tx_dat)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
		SPI_I2S_SendData(SPI1,tx_dat);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
		return SPI_I2S_ReceiveData(SPI1);
}

/*****************************************************************************
* 函  数：void SPI1_Speed_Set(uint8_t speed)
* 功  能：SPI1通信速度设置
* 参  数：speed:速度设置
* 返回值：无
* 备  注：无
*****************************************************************************/

void SPI1_Speed_Set(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7;
	SPI1->CR1 |= speed;
	SPI1->CR1 |= 1<<6;
}

/*****************************************************************************
* 函  数：void SPI2_Init(void)
* 功  能：SPI2初始化配置
* 参  数：无
* 返回值：无
* 备  注：无
*****************************************************************************/
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI2_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//空闲状态
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI2_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI2_InitStructure);
	SPI_Cmd(SPI2,ENABLE);

	SPI2_ReadWrite_Byte(0xFF);//启动传输
}

/*****************************************************************************
* 函  数：uint8_t SPI2_ReadWrite_Byte(uint8_t tx_dat)
* 功  能：SPI2收发数据
* 参  数：tx_dat:发送的数据
* 返回值：接收的数据
* 备  注：无
*****************************************************************************/
uint8_t SPI2_ReadWrite_Byte(uint8_t tx_dat)
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
		SPI_I2S_SendData(SPI2,tx_dat);
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
		return SPI_I2S_ReceiveData(SPI2);
}






