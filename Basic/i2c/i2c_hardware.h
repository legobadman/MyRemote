#ifndef __I2C_HARDWARE_H__
#define __I2C_HARDWARE_H__	 
#include "sys.h"
//#include "NUC123.h"

typedef struct
{
    __IO uint32_t I2CON;         /* Offset: 0x00  I2C Control Register                                               */
    __IO uint32_t I2CADDR0;      /* Offset: 0x04  I2C Slave Address Register0                                        */
    __IO uint32_t I2CDAT;        /* Offset: 0x08  I2C Data Register                                                  */
    __I  uint32_t I2CSTATUS;     /* Offset: 0x0C  I2C Status Register                                                */
    __IO uint32_t I2CLK;         /* Offset: 0x10  I2C Clock Divided Register                                         */
    __IO uint32_t I2CTOC;        /* Offset: 0x14  I2C Time-out Counter Register                                      */
    __IO uint32_t I2CADDR1;      /* Offset: 0x18  I2C Slave Address Register1                                        */
    __IO uint32_t I2CADDR2;      /* Offset: 0x1C  I2C Slave Address Register2                                        */
    __IO uint32_t I2CADDR3;      /* Offset: 0x20  I2C Slave Address Register3                                        */
    __IO uint32_t I2CADM0;       /* Offset: 0x24  I2C Slave Address Mask Register0                                   */
    __IO uint32_t I2CADM1;       /* Offset: 0x28  I2C Slave Address Mask Register1                                   */
    __IO uint32_t I2CADM2;       /* Offset: 0x2C  I2C Slave Address Mask Register2                                   */
    __IO uint32_t I2CADM3;       /* Offset: 0x30  I2C Slave Address Mask Register3                                   */
    __I  uint32_t RESERVED[2];
	__IO uint32_t I2CWKUPCON;    /* Offset: 0x3C  I2C Wake-up Control Register                                       */
    __IO uint32_t I2CWKUPSTS;    /* Offset: 0x40  I2C Wake-up Status Register                                        */

} I2C_T;

/* I2C I2CON Bit Field Definitions */
#define I2C_I2CON_EI_Pos                        7                                       /*!< I2C_T::I2CON: EI Position */
#define I2C_I2CON_EI_Msk                        (1ul << I2C_I2CON_EI_Pos)               /*!< I2C_T::I2CON: EI Mask */

#define I2C_I2CON_ENS1_Pos                      6                                       /*!< I2C_T::I2CON: ENS1 Position */
#define I2C_I2CON_ENS1_Msk                      (1ul << I2C_I2CON_ENS1_Pos)             /*!< I2C_T::I2CON: ENS1 Mask */

#define I2C_I2CON_STA_Pos                       5                                       /*!< I2C_T::I2CON: STA Position */
#define I2C_I2CON_STA_Msk                       (1ul << I2C_I2CON_STA_Pos)              /*!< I2C_T::I2CON: STA Mask */

#define I2C_I2CON_STO_Pos                       4                                       /*!< I2C_T::I2CON: STO Position */
#define I2C_I2CON_STO_Msk                       (1ul << I2C_I2CON_STO_Pos)              /*!< I2C_T::I2CON: STO Mask */

#define I2C_I2CON_SI_Pos                        3                                       /*!< I2C_T::I2CON: SI Position */
#define I2C_I2CON_SI_Msk                        (1ul << I2C_I2CON_SI_Pos)               /*!< I2C_T::I2CON: SI Mask */

#define I2C_I2CON_AA_Pos                        2                                       /*!< I2C_T::I2CON: AA Position */
#define I2C_I2CON_AA_Msk                        (1ul << I2C_I2CON_AA_Pos)               /*!< I2C_T::I2CON: AA Mask */

#define I2C_I2CON_STA_SI            0x28UL

#define APB1_BASE           ((     uint32_t)0x40000000)
#define I2C0_BASE            (APB1_BASE      + 0x20000)                 /*!< I2C0 Base Address                                */
#define I2C0                ((I2C_T *) I2C0_BASE)                       /*!< I2C0 Configuration Struct                        */

#define I2CPORT		GPIOB	//定义IO接口
#define I2C_SCL		GPIO_Pin_6	//定义IO接口
#define I2C_SDA		GPIO_Pin_7	//定义IO接口

#define HostAddress	0xC0	//总线主机的器件地址
#define BusSpeed	200000	//总线速度（不高于400000）

#define I2C_START(i2c)  ((i2c)->I2CON = ((i2c)->I2CON | I2C_I2CON_SI_Msk) | I2C_I2CON_STA_Msk)
#define I2C_WAIT_READY(i2c)     while(!((i2c)->I2CON & I2C_I2CON_SI_Msk))
#define I2C_GET_STATUS(i2c) ((i2c)->I2CSTATUS)
#define I2C_SET_DATA(i2c, u8Data) ((i2c)->I2CDAT = (u8Data))
#define I2C_GET_DATA(i2c)   ((i2c)->I2CDAT)
#define I2C_I2CON_SI                0x08UL
#define I2C_I2CON_SI_AA             0x0CUL
#define I2C_I2CON_STO_SI            0x18UL
#define I2C_SET_CONTROL_REG(i2c, u8Ctrl) ((i2c)->I2CON = ((i2c)->I2CON & ~0x3c) | (u8Ctrl))

void I2C_Configuration(void);
void I2C_SAND_BUFFER(u8 SlaveAddr, u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t length);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t length);

void I2C_SAND_BYTE(u8 SlaveAddr,u8 writeAddr,u8 pBuffer);
void I2C_READ_BUFFER(u8 SlaveAddr,u8 readAddr,u8* pBuffer,u16 NumByteToRead);
u8 I2C_READ_BYTE(u8 SlaveAddr,u8 readAddr);
uint32_t I2C_WriteMultiBytesOneReg(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t u8DataAddr, const uint8_t *data, uint32_t u32wLen);
uint32_t I2C_ReadMultiBytesOneReg(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t u8DataAddr, uint8_t *rdata, uint32_t u32rLen);
		 				    
#endif
