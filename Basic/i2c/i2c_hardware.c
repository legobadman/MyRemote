

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202309 创建。


*/



#include "i2c_hardware.h"
#include <stdio.h>


void I2C_GPIO_Init(void){ //I2C接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); //启动I2C功能 
    GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA; //选择端口号                      
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(I2CPORT, &GPIO_InitStructure);
}

void I2C_Configuration(void){ //I2C初始化
	I2C_InitTypeDef  I2C_InitStructure;
	I2C_GPIO_Init(); //先设置GPIO接口的状态
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//设置为I2C模式
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = HostAddress; //主机地址（从机不得用此地址）
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//允许应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //7位地址模式
	I2C_InitStructure.I2C_ClockSpeed = BusSpeed; //总线速度设置 	
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_Cmd(I2C1,ENABLE);//开启I2C					
}

void I2C_SAND_BUFFER(u8 SlaveAddr,u8 WriteAddr,u8* pBuffer,u16 NumByteToWrite){ //I2C发送数据串（器件地址，内部地址，寄存器，数量）
	I2C_GenerateSTART(I2C1,ENABLE);//产生起始位
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //清除EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr,I2C_Direction_Transmitter);//发送器件地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//清除EV6
	I2C_SendData(I2C1,WriteAddr); //内部功能地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//移位寄存器非空，数据寄存器已空，产生EV8，发送数据到DR既清除该事件
	while(NumByteToWrite--){ //循环发送数据	
		I2C_SendData(I2C1,*pBuffer); //发送数据
		pBuffer++; //数据指针移位
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//清除EV8
	}
	I2C_GenerateSTOP(I2C1,ENABLE);//产生停止信号
}

int8_t i2c_reg_write(uint8_t SlaveAddr, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	I2C_GenerateSTART(I2C1,ENABLE); //发送开始信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //等待完成
	
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //发送从器件地址及状态（写入）
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //等待完成	
	I2C_SendData(I2C1,WriteAddr); //发送从器件内部寄存器地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待完成	
	I2C_SendData(I2C1,pBuffer[0]); //发送要写入的内容
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待完成	
	I2C_GenerateSTOP(I2C1,ENABLE); //发送结束信号	
	return 0;
}

void I2C_SAND_BYTE(u8 SlaveAddr,u8 writeAddr,u8 pBuffer){ //I2C发送一个字节（从地址，内部地址，内容）
	I2C_GenerateSTART(I2C1,ENABLE); //发送开始信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //等待完成	
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //发送从器件地址及状态（写入）
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //等待完成	
	I2C_SendData(I2C1,writeAddr); //发送从器件内部寄存器地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待完成	
	I2C_SendData(I2C1,pBuffer); //发送要写入的内容
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待完成	
	I2C_GenerateSTOP(I2C1,ENABLE); //发送结束信号
}
void I2C_READ_BUFFER(u8 SlaveAddr,u8 readAddr,u8* pBuffer,u16 NumByteToRead){ //I2C读取数据串（器件地址，寄存器，内部地址，数量）
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1,ENABLE);//开启信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));	//清除 EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //写入器件地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//清除 EV6
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C1,readAddr); //发送读的地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //清除 EV8
	I2C_GenerateSTART(I2C1,ENABLE); //开启信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //清除 EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr,I2C_Direction_Receiver); //将器件地址传出，主机为读
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //清除EV6
	while(NumByteToRead){
		if(NumByteToRead == 1){ //只剩下最后一个数据时进入 if 语句
			I2C_AcknowledgeConfig(I2C1,DISABLE); //最后有一个数据时关闭应答位
			I2C_GenerateSTOP(I2C1,ENABLE);	//最后一个数据时使能停止位
		}
		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){ //读取数据
			*pBuffer = I2C_ReceiveData(I2C1);//调用库函数将数据取出到 pBuffer
			pBuffer++; //指针移位
			NumByteToRead--; //字节数减 1 
		}
	}
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}

int8_t i2c_reg_read(uint8_t SlaveAddr, uint8_t readAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		printf("1\r\n");
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);//开启信号
	
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))	//清除 EV5
	{
		printf("2\r\n");
	}	
	
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //写入器件地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//清除 EV6
	{
		printf("3\r\n");
	}	
	
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C1,readAddr); //发送读的地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //清除 EV8
	I2C_GenerateSTART(I2C1,ENABLE); //开启信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //清除 EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr,I2C_Direction_Receiver); //将器件地址传出，主机为读
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //清除EV6
	while(NumByteToRead){
		if(NumByteToRead == 1){ //只剩下最后一个数据时进入 if 语句
			I2C_AcknowledgeConfig(I2C1,DISABLE); //最后有一个数据时关闭应答位
			I2C_GenerateSTOP(I2C1,ENABLE);	//最后一个数据时使能停止位
		}
		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){ //读取数据
			*pBuffer = I2C_ReceiveData(I2C1);//调用库函数将数据取出到 pBuffer
			pBuffer++; //指针移位
			NumByteToRead--; //字节数减 1 
		}
	}
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	return 0;
}



u8 I2C_READ_BYTE(u8 SlaveAddr,u8 readAddr){ //I2C读取一个字节
	u8 a;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); 
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C1,readAddr);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2C1,DISABLE); //最后有一个数据时关闭应答位
	I2C_GenerateSTOP(I2C1,ENABLE);	//最后一个数据时使能停止位
	a = I2C_ReceiveData(I2C1);
	return a;
}

uint32_t I2C_ReadMultiBytesOneReg(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t u8DataAddr, uint8_t *rdata, uint32_t u32rLen)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0; 
    uint32_t u32rxLen = 0;
  
    I2C_START(i2c);                                                /* Send START */ 
    while(u8Xfering && (u8Err == 0))
    { 
        I2C_WAIT_READY(i2c);
        switch(I2C_GET_STATUS(i2c))
        {
            case 0x08:
                I2C_SET_DATA(i2c, (u8SlaveAddr << 1 | 0x00));      /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                             /* Clear SI */                   
                break;
            case 0x18:                                             /* Slave Address ACK */
                I2C_SET_DATA(i2c, u8DataAddr);                     /* Write Lo byte address of register */
                break;
            case 0x20:                                             /* Slave Address NACK */
            case 0x30:                                             /* Master transmit data NACK */                
                u8Ctrl = I2C_I2CON_STO_SI;                         /* Clear SI and send STOP */                
                u8Err = 1;                
                break;               
            case 0x28:
                u8Ctrl = I2C_I2CON_STA_SI;                         /* Send repeat START */
                break;
            case 0x10:
                I2C_SET_DATA(i2c, ((u8SlaveAddr << 1) | 0x01));    /* Write SLA+R to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                             /* Clear SI */     
                break;
            case 0x40:                                             /* Slave Address ACK */
                u8Ctrl = I2C_I2CON_SI_AA;                          /* Clear SI and set ACK */
                break;
            case 0x48:                                             /* Slave Address NACK */
                u8Ctrl = I2C_I2CON_STO_SI;                         /* Clear SI and send STOP */                
                u8Err = 1;                
                break;            
            case 0x50:
                rdata[u32rxLen++] = (uint8_t) I2C_GET_DATA(i2c);   /* Receive Data */
                if(u32rxLen<(u32rLen-1))
                    u8Ctrl = I2C_I2CON_SI_AA;                      /* Clear SI and set ACK */
                else
                    u8Ctrl = I2C_I2CON_SI;                         /* Clear SI */
                break;              
            case 0x58:
                rdata[u32rxLen++] = (uint8_t) I2C_GET_DATA(i2c);   /* Receive Data */
                u8Ctrl = I2C_I2CON_STO_SI;                         /* Clear SI and send STOP */
                u8Xfering = 0;
                break;
            case 0x38:                                             /* Arbitration Lost */
            default:                                               /* Unknow status */
                u8Ctrl = I2C_I2CON_STO_SI;                         /* Clear SI and send STOP */                  
                u8Err = 1;
                break;                
        }
        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                          /* Write controlbit to I2C_CTL register */
    } 
    return u32rxLen;                                               /* Return bytes length that have been received */
}

uint32_t I2C_WriteMultiBytesOneReg(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t u8DataAddr, const uint8_t *data, uint32_t u32wLen)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0;
    uint32_t u32txLen = 0;
  
    I2C_START(i2c);                                              /* Send START */
    while(u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(i2c);
        switch(I2C_GET_STATUS(i2c))
        {
            case 0x08:
                I2C_SET_DATA(i2c, (u8SlaveAddr << 1 | 0x00));    /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                 
                break;
            case 0x18:                                           /* Slave Address ACK */
                I2C_SET_DATA(i2c, u8DataAddr);                   /* Write Lo byte address of register */
                break;
            case 0x20:                                           /* Slave Address NACK */
            case 0x30:                                           /* Master transmit data NACK */                
                u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */                
                u8Err = 1;                
                break;             
            case 0x28:
                if(u32txLen < u32wLen)
                    I2C_SET_DATA(i2c, data[u32txLen++]);
                else
                {
                    u8Ctrl = I2C_I2CON_STO_SI;                   /* Clear SI and send STOP */
                    u8Xfering = 0;  
                }                 
                break;
            case 0x38:                                           /* Arbitration Lost */
            default:                                             /* Unknow status */
                u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */                  
                u8Err = 1;  
                break;
        }
        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                        /* Write controlbit to I2C_CTL register */ 
    } 
    
    return u32txLen;                                             /* Return bytes length that have been transmitted */
}

 
/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/


/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
