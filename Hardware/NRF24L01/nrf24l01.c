#include "nrf24l01.h"
#include "delay.h"
#include "sys.h"
#include "spi.h"
#include "usart.h"

const uint8_t TX_ADDRESS_X[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//发送端地址
const uint8_t RX_ADDRESS_X[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//接收端地址


/*****************************************************************************
* 函  数：void NRF24L01_GPIO_Init(void)
* 功  能：NRF24L01相关GPIO初始化
* 参  数：void
* 返回值：无
* 备  注：无
*****************************************************************************/
void NRF24L01_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);

	//SPI1上还挂了flash和sd卡,一直片选置高,排除干扰
	
	GPIO_InitStructure.GPIO_Pin = NRF_CE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NRF_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//中断上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NRF_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,NRF_IRQ|NRF_CE);
	GPIO_SetBits(GPIOA,NRF_CS);
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_Write_Reg(uint8_t res,uint8_t value)
* 功  能：从寄存器写入一字节数据
* 参  数：res:寄存器地址;value:写入的值
* 返回值：status:读取的寄存器状态值
* 备  注：无
*****************************************************************************/
uint8_t NRF24L01_Write_Reg(uint8_t res,uint8_t value)
{
	uint8_t status;
	
	NRF_CS_LOW;
	status = SPI1_ReadWrite_Byte(res);
	SPI1_ReadWrite_Byte(value);
	NRF_CS_HIGH;

	return status;	//返回寄存器状态值
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_Read_Reg(uint8_t res)
* 功  能：从寄存器读取一字节数据
* 参  数：res:寄存器地址
* 返回值：ret:读取的值
* 备  注：无
*****************************************************************************/
uint8_t NRF24L01_Read_Reg(uint8_t res)
{
	uint8_t ret;
	
	NRF_CS_LOW;
	SPI1_ReadWrite_Byte(res);
	ret = SPI1_ReadWrite_Byte(0xFF);	//在读取数据前稳定MISO的电平,防止触发slave设备
	NRF_CS_HIGH;

	return ret;
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
* 功  能：在指定位置读取一定长度的数据
* 参  数：res:指定位置;len:数据长度;*pBuf:指定数据的首地址
* 返回值：status:寄存器的状态
* 备  注：无
*****************************************************************************/
uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
{
	uint8_t status,i;

	NRF_CS_LOW;
	status = SPI1_ReadWrite_Byte(res);
	for(i=0;i<len;i++)
		{
			pBuf[i] = SPI1_ReadWrite_Byte(0xFF);
	}
	NRF_CS_HIGH;

	return status;
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_Write_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
* 功  能：在指定位置写入一定长度的数据
* 参  数：res:指定位置;len:数据长度;*pBuf:指定数据的首地址
* 返回值：status:寄存器的状态
* 备  注：无
*****************************************************************************/
uint8_t NRF24L01_Write_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
{
	uint8_t status,i;

	NRF_CS_LOW;
	status = SPI1_ReadWrite_Byte(res);
	for (i = 0; i < len; i++)
		{
			SPI1_ReadWrite_Byte(*pBuf++);
			//SPI1_ReadWrite_Byte(pBuf[i]);
		}
	NRF_CS_HIGH;
	
	return status;
}

void NRF24L01_Power_Down_Mode(void)
{
	NRF_CS_LOW;
	NRF24L01_Write_Reg(NRF24L01_CONFIG, 0x00);
	NRF_CS_HIGH;
	
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_Check(void)
* 功  能：检测NRF24L01是否存在
* 参  数：无
* 返回值：0:成功；1:失败
* 备  注：往NRF24L01的发送地址寄存器写入5字节数据再读出来,判断24L01是否已正常工作
*****************************************************************************/
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0xa5,0xa5,0xa5,0xa5,0xa5};
	uint8_t i;

	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,5,buf);//写入5字节的地址
	NRF24L01_Read_Buf(TX_ADDR,5,buf);
	for(i=0;i<5;i++)
		{
			if(buf[i]!=0xa5)
				break;
	}
	if(i!=5)
		return 1;
	return 0;
}

/*****************************************************************************
* 函  数：void NRF24L01_TX_Mode(void)
* 功  能：NRF24L01发送模式配置
* 参  数：无
* 返回值：无
* 备  注：无
*****************************************************************************/
void NRF24L01_TX_Mode(void)
{
	NRF_CE_LOW;
	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,TX_ADR_WIDTH,(uint8_t *)TX_ADDRESS_X);//写TX节点地址
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0,RX_ADR_WIDTH, (uint8_t *) RX_ADDRESS_X);//写RX节点地址,为了自动使能ACK
	NRF24L01_Write_Reg(W_REGISTER+EN_AA, 0x01);//使能通道0自动应答
	NRF24L01_Write_Reg(W_REGISTER+EN_RXADDR, 0x01);//使能通道0接收地址
	NRF24L01_Write_Reg(W_REGISTER+SETUP_PETR, 0x1a);//设置自动重发间隔时间:500us+86us,最大重大次数:10次
	NRF24L01_Write_Reg(W_REGISTER+RF_CH, 40);//设置通道为40
	NRF24L01_Write_Reg(W_REGISTER+RF_SETUP, 0x0f);//设置发射参数:0dB增益;2Mnps;低噪声增益开启
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_CONFIG,0x0e);//基本参数:PWR_UP;EN_CRC;16BIT_CRC;发送模式;开启所有中断
	NRF_CE_HIGH;//NRF_CE为高,10us后启动发送数据
	delay_us(12);
}

/*****************************************************************************
* 函  数：void NRF24L01_TX_Mode(void)
* 功  能：NRF24L01发送模式配置
* 参  数：无
* 返回值：无
* 备  注：无
*****************************************************************************/
void NRF24L01_RX_Mode(void)
{
	NRF_CE_LOW;
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0, RX_ADR_WIDTH, (uint8_t *) RX_ADDRESS_X);//写RX地址节点
	NRF24L01_Write_Reg(W_REGISTER+EN_AA,0x01);//使能通道0自动应答
	NRF24L01_Write_Reg(W_REGISTER+EN_RXADDR,0x01);//使能通道0接收地址
	NRF24L01_Write_Reg(W_REGISTER+RF_CH, 40);//设置RF通信频率
	NRF24L01_Write_Reg(W_REGISTER+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0有效数据宽度
	NRF24L01_Write_Reg(W_REGISTER+RF_SETUP, 0x0f);//设置TX发射参数:0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_CONFIG, 0x0f);//基本参数:PWR_UP;EN_CRC;16BIT_CRC;接收模式;开启所有中断
	NRF_CE_HIGH;//CE为高,进入接收模式
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_TX_Packet(uint8_t *txbuf)
* 功  能：NRF24L01发送一次数据
* 参  数：*txbuf：等待发送数据的首地址
* 返回值：MAX_TX:最大重发次数;TX_OK:发送完成;0xFF:发送失败
* 备  注：无
*****************************************************************************/
uint8_t NRF24L01_TX_Packet(uint8_t *txbuf)
{
	uint8_t ret;

	NRF_CE_LOW;
	NRF24L01_Write_Buf(W_TX_PAYLOAD, TX_PLOAD_WIDTH, txbuf);//写数据到txbuf,32字节
	NRF_CE_HIGH;//启动发送

	while (NRF_IRQ_STATUS)//等待发送完成
	{
		
	}
	
	ret = NRF24L01_Read_Reg(NRF24L01_STATUS);//读取状态寄存器的值
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_STATUS, ret);//清除TX_DS or MAX_RT的中断标志
	if(ret&MAX_TX)//达到最大重发次数
		{
			NRF24L01_Write_Reg(FLUSH_TX, 0XFF);//清除TX FIFO	寄存器
			return MAX_TX;
	}
	if(ret&TX_OK)	//发送完成
		{
			return TX_OK;
	}
	return 0xFF;//发送失败
	
}

/*****************************************************************************
* 函  数：uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf)
* 功  能：NRF24L01接收一次数据
* 参  数：*rxbuf：等待接收数据的首地址
* 返回值：0:接收成功;1:接收数据失败
* 备  注：无
*****************************************************************************/
uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf)
{
	uint8_t ret;


	NRF_CE_HIGH;
	while(NRF_IRQ_STATUS);
	NRF_CE_LOW;

	ret = NRF24L01_Read_Reg(NRF24L01_STATUS);//读取状态寄存器的值
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_STATUS, ret);//清除TX_DS or MAX_RT的中断标志
	if(ret&RX_OK)//接收到数据
		{
			NRF24L01_Read_Buf(R_RX_PAYLOAD,RX_PLOAD_WIDTH,rxbuf);//读取数据
			NRF24L01_Write_Reg(FLUSH_RX,0xFF);//清除RX FIFO寄存器
			return 0;
	}
	return 1;//没有接收到数据
}

/*****************************************************************************
* 函  数：void NRF24L01_Init(void)
* 功  能：NRF24L01初始化
* 参  数：无
* 返回值：无
* 备  注：对应GPIO设置
*****************************************************************************/
void NRF24L01_Init(void)
{
	NRF24L01_GPIO_Init();
	
	NRF_CS_HIGH;
	NRF_CE_LOW;
}

