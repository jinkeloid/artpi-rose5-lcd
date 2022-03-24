/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "EventRecorder.h"
#include "perf_counter.h"

#include "bsp_i2c_touch.h"
#include "bsp_touch_gtxx.h"
#include "core_delay.h"  

#include "lvgl.h"
#include "demos/lv_demos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void MX_SDRAM_InitEx(void);

void lv_port_indev_init(void);
void lv_port_disp_init(void);

extern void GTP_TouchProcess(void);

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned int *dispbuf  = (unsigned int*)0xc0000000;
unsigned int tpx=0,tpy=0,tppressed = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
extern void QSPI_MemoryMapped(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	SystemCoreClock = 400000000ul;//400MHz
	init_cycle_counter(false);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();	
	MX_QUADSPI_Init();
	
  MX_FMC_Init();
  MX_UART4_Init();
  MX_I2C1_Init();
  MX_DMA2D_Init();
  MX_CRC_Init();
  MX_LTDC_Init();
  /* USER CODE BEGIN 2 */	
	
	HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,1);
	printf("START!\r\n");
	GTP_Init_Panel();
	
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
	int32_t time=get_system_ms();
	int32_t time5ms=get_system_ms();
	int32_t time500ms=get_system_ms();
	int i;
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
memset(dispbuf,0x55,800*480*2);
lv_demo_music();
//lv_demo_widgets();
//lv_demo_stress();
//lv_demo_benchmark();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		time=get_system_ms();
		
		if(time>=(time5ms+5))
		{
			lv_tick_inc(5);
			lv_timer_handler();
			time5ms=get_system_ms();
		}
		
		if(time>=(time500ms+100))
		{
//			memset(dispbuf,0xff,800*480*2);		
//			memset(dispbuf,i++,800*480*2);
			
			HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
			time500ms=get_system_ms();
		}
		
//		HAL_GPIO_TogglePin(lcd_bl_GPIO_Port,lcd_bl_Pin);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_CSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
	uint32_t psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	uint32_t timeout=0; 
	uint16_t offline;
	uint32_t addr; 
	//坐标系转换
//	if(lcdltdc.dir)	//横屏
//	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
//	}else			//竖屏
//	{
//		psx=sy;psy=lcdltdc.pheight-ex-1;
//		pex=ey;pey=lcdltdc.pheight-sx-1;
//	}
	
	//width 800 
	offline=800-(pex-psx+1);
	addr=((unsigned int)dispbuf[0]+2*(800*psy+psx));
	RCC->AHB1ENR|=1<<23;			//使能DM2D时钟
	DMA2D->CR=0<<16;				//存储器到存储器模式
	DMA2D->FGPFCCR=0X02;	//设置颜色格式 rgb565
	DMA2D->FGOR=0;					//前景层行偏移为0
	DMA2D->OOR=offline;				//设置行偏移 
	DMA2D->CR&=~(1<<0);				//先停止DMA2D
	DMA2D->FGMAR=(unsigned int)color;		//源地址
	DMA2D->OMAR=addr;				//输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器 
	DMA2D->CR|=1<<0;				//启动DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//超时退出
	} 
	DMA2D->IFCR|=1<<1;				//清除传输完成标志  	
} 
#if 0
#define GLCD_WIDTH     800
#define GLCD_HEIGHT    480
#define LCD_DB_ADDR   0xC0000000
#define LCD_DB_PTR    ((volatile uint16_t *)LCD_DB_ADDR)

int32_t GLCD_DrawBitmap (uint32_t x, 
                         uint32_t y, 
                         uint32_t width, 
                         uint32_t height, 
                         const uint8_t *bitmap) 
{
    volatile uint16_t *phwDes = LCD_DB_PTR + y * GLCD_WIDTH + x;
    const uint16_t *phwSrc = (const uint16_t *)bitmap;
    for (int_fast16_t i = 0; i < height; i++) {
        memcpy ((uint16_t *)phwDes, phwSrc, width * 2);
        phwSrc += width;
        phwDes += GLCD_WIDTH;
    }

    return 0;
}
#endif
void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{

		uint32_t  height = area->y2 - area->y1;
		uint32_t  width= area->x2 - area->x1; 
		uint32_t  x=area->x1;
		uint32_t  y=area->y1;
	
//		GLCD_DrawBitmap(x,width,y,height, (const uint8_t *)color_p);	
//		LTDC_Color_Fill(area->x1,area->x2,area->y1,area->y2,(uint16_t*)color_p);

    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}


void lv_port_disp_init(void)
{	
	#define DISP_HOR_RES 800//800
	#define DISP_VER_RES 480//480
	
	static lv_disp_draw_buf_t draw_buf;
	//__attribute__((section(".ARM.__at_0x24000000")))
__attribute__((section(".ARM.__at_0xc0000000")))	static lv_color_t buf1[DISP_HOR_RES * DISP_VER_RES] ;// __attribute__((at(0Xc0001000)));
	/*Declare a buffer for 1/10 screen size*/
	lv_disp_draw_buf_init(&draw_buf, buf1, NULL, DISP_HOR_RES * DISP_VER_RES );
	/*Initialize the display buffer.*/
	
	static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
	disp_drv.flush_cb = my_disp_flush;    /*Set your driver function*/
	disp_drv.draw_buf = &draw_buf;        /*Assign the buffer to the display*/
	disp_drv.hor_res = DISP_HOR_RES;   /*Set the horizontal resolution of the display*/
	disp_drv.ver_res = DISP_VER_RES;   /*Set the vertical resolution of the display*/
	disp_drv.full_refresh = 1;
	disp_drv.direct_mode = 1;
	lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/
}

/*------------------
 * Touchpad
 * -----------------*/
lv_indev_t * indev_touchpad;

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
	if(tppressed == 1)
		return true;
    /*Your code comes here*/
	else
    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
	*x = tpx;
	*y = tpy;
    /*Your code comes here*/

//    (*x) = 0;
//    (*y) = 0;
}

void lv_port_indev_init(void)
{
	static lv_indev_drv_t indev_drv;
	
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = touchpad_read;
	indev_touchpad = lv_indev_drv_register(&indev_drv);
}





/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

