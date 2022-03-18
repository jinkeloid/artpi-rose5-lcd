#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "main.h"

/*ʹ�����IIC����SOFT_IIC������Ϊ1��Ӳ��IIC������Ϊ0
!!ʹ��Ӳ��IICʱ�ǳ����׳��ִ��󣬲��Ƽ�*/
#define SOFT_IIC      0

/*�趨ʹ�õĵ�����IIC�豸��ַ*/
#define GTP_ADDRESS            0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*I2C����
scl pb6
sda pb7*/
#define GTP_I2C                          I2C1
#define GTP_I2C_CLK                      RCC_APB1Periph_I2C1
//#define GTP_I2C_CLK_INIT								RCC_APB1PeriphClockCmd	

#define GTP_I2C_SCL_Pin                  GPIO_Pin_6                 
#define GTP_I2C_SCL_GPIO_Port            GPIOB                       
//#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOH
//#define GTP_I2C_SCL_SOURCE               GPIO_PinSource4
//#define GTP_I2C_SCL_AF                   GPIO_AF_I2C2

#define GTP_I2C_SDA_Pin                  GPIO_Pin_7                  
#define GTP_I2C_SDA_GPIO_Port            GPIOB                     
//#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOH
//#define GTP_I2C_SDA_SOURCE               GPIO_PinSource5
//#define GTP_I2C_SDA_AF                   GPIO_AF_I2C2

/*��λ����pd3*/
#define GTP_RST_GPIO_Port                GPIOD
//#define GTP_RST_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GTP_RST_Pin                 GPIO_Pin_11
/*�ж�����pg12*/
#define GTP_INT_GPIO_Port                GPIOG
//#define GTP_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOG
#define GTP_INT_Pin                 GPIO_Pin_12
//#define GTP_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOD
//#define GTP_INT_EXTI_PinSOURCE           EXTI_PinSource13
//#define GTP_INT_EXTI_LINE                EXTI_Line13
#define GTP_INT_EXTI_IRQ                 EXTI15_10_IRQn
/*�жϷ�����*/
#define GTP_IRQHandler                   EXTI15_10_IRQHandler

//���IICʹ�õĺ�
//#define I2C_SCL_1()  GPIO_SetBits(GTP_I2C_SCL_GPIO_Port, GTP_I2C_SCL_Pin)		/* SCL = 1 */
//#define I2C_SCL_0()  GPIO_ResetBits(GTP_I2C_SCL_GPIO_Port, GTP_I2C_SCL_Pin)		/* SCL = 0 */

//#define I2C_SDA_1()  GPIO_SetBits(GTP_I2C_SDA_GPIO_Port, GTP_I2C_SDA_Pin)		/* SDA = 1 */
//#define I2C_SDA_0()  GPIO_ResetBits(GTP_I2C_SDA_GPIO_Port, GTP_I2C_SDA_Pin)		/* SDA = 0 */

//#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GTP_I2C_SDA_GPIO_Port, GTP_I2C_SDA_Pin)	/* ��SDA����״̬ */

//�����ӿ�
void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void I2C_ResetChip(void);
void I2C_GTP_IRQDisable(void);
void I2C_GTP_IRQEnable(void);

#endif /* __I2C_TOUCH_H */
