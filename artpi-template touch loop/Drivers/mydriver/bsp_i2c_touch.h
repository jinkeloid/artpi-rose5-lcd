#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "stm32h7xx.h"
#include "stm32h7xx_it.h"

/*ʹ�����IIC����SOFT_IIC������Ϊ1��Ӳ��IIC������Ϊ0
I2C1_SCL	PB6
I2C1_SDA	PB7
GPIO_TS_IIC_INT	PG12
GPIO_TS_RST	PD3

!!ʹ��Ӳ��IICʱ�ǳ����׳��ִ��󣬲��Ƽ�*/
#define SOFT_IIC      0

/*�趨ʹ�õĵ�����IIC�豸��ַ*/
#define GTP_ADDRESS            0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*I2C����*/
#define GTP_I2C                          I2C1
#define GTP_I2C_CLK_ENABLE()             __HAL_RCC_I2C1_CLK_ENABLE()
#define GTP_I2C_CLK_INIT				 				 RCC_APB1PeriphClockCmd	

#define GTP_I2C_SCL_PIN                  GPIO_PIN_6                 
#define GTP_I2C_SCL_GPIO_PORT            GPIOB                       
#define GTP_I2C_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define GTP_I2C_SCL_AF                   GPIO_AF4_I2C1

#define GTP_I2C_SDA_PIN                  GPIO_PIN_7                  
#define GTP_I2C_SDA_GPIO_PORT            GPIOB                     
#define GTP_I2C_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define GTP_I2C_SDA_AF                   GPIO_AF4_I2C1

/*��λ PD3 ����*/
#define GTP_RST_GPIO_PORT                GPIOD
#define GTP_RST_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define GTP_RST_GPIO_PIN                 GPIO_PIN_3
/*�ж� PG12 ����*/
#define GTP_INT_GPIO_PORT                GPIOG
#define GTP_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define GTP_INT_GPIO_PIN                 GPIO_PIN_12
#define GTP_INT_EXTI_IRQ                 EXTI15_10_IRQn



//���IICʹ�õĺ�
#define I2C_SCL_1()  HAL_GPIO_WritePin(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN,GPIO_PIN_SET)		/* SCL = 1 */
#define I2C_SCL_0()  HAL_GPIO_WritePin(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN,GPIO_PIN_RESET)		/* SCL = 0 */

#define I2C_SDA_1()  HAL_GPIO_WritePin(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN,GPIO_PIN_SET)		/* SDA = 1 */
#define I2C_SDA_0()  HAL_GPIO_WritePin(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN,GPIO_PIN_RESET)		/* SDA = 0 */

#define I2C_SDA_READ()  HAL_GPIO_ReadPin(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)	/* ��SDA����״̬ */

//�����ӿ�
void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void I2C_ResetChip(void);
void I2C_GTP_IRQDisable(void);
void I2C_GTP_IRQEnable(void);

#endif /* __I2C_TOUCH_H */
