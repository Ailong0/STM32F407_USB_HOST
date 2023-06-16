/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "fatfs.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//������������ĳ�������,���� setting->editer->encoder�е�������
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern ApplicationTypeDef Appli_state;
uint8_t status=0;
uint8_t FileBuf[100]="hello ���";//�ļ�д������
UINT bw;
uint8_t show[30];
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
	//�豸��ʼ�� gpio ���� fatfs usb
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */
	//���ڴ�ӡ����
  HAL_UART_Transmit(&huart1,(uint8_t*)"hello ���",5,100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		//�����Ե��ô˺���,�����usb��������
    MX_USB_HOST_Process();
    /* USER CODE BEGIN 3 */
		//Appli_stateΪ����usb�豸״̬,������鿴�䶨��
		 if(Appli_state == APPLICATION_READY)//usb������׼����
        {
          if(status == 0)
          {
            status = 1;
            retUSBH=f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 1);//fatfs����usb�豸,���سɹ����ӡ��������
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"Mount the USB flash drive!\r\n",sizeof("Mount the USB flash drive!\r\n"),100);
            }
    
						//���ļ�����
            HAL_UART_Transmit(&huart1,(uint8_t*)"open test!\r\n",sizeof("open test!\r\n"),100);
            retUSBH = f_open(&USBHFile,(const char*)"test.txt",FA_CREATE_ALWAYS|FA_WRITE);
						while(retUSBH!=FR_OK)
						{
							retUSBH = f_open(&USBHFile,(const char*)"test.txt",FA_CREATE_ALWAYS|FA_WRITE);
						}
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"open test successful!\r\n",sizeof("open test successful!\r\n"),100);
            }
			
						//д���ļ�����,�ɹ�д���ر��ļ�
            retUSBH = f_write(&USBHFile,FileBuf,18,&bw);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"write file successful!\r\n",sizeof("write file successful!\r\n"),100);
            }
            retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"close file successful!\r\n\r\n",sizeof("close file successful!\r\n\r\n"),100);
            }
						
						//��ȡ�ļ�����,��ȡ�ɹ����ڷ���һ�¾��,����ӡ�ļ�����
            HAL_UART_Transmit(&huart1,(uint8_t*)"read file test!\r\n",sizeof("read file test!\r\n"),100);
            retUSBH = f_open(&USBHFile,(const char*)"test.txt",FA_OPEN_EXISTING|FA_READ);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"open file successful!\r\n",sizeof("open file successful!\r\n"),100);
            }
            retUSBH = f_read(&USBHFile,FileBuf,18,&bw);
            if(retUSBH==FR_OK)
            {
							HAL_UART_Transmit(&huart1,(uint8_t*)"read file successful!\r\n",sizeof("read file successful!\r\n"),100);
              HAL_UART_Transmit(&huart1,FileBuf,bw,100);
							HAL_UART_Transmit(&huart1,"\r\n",4,100);
            }
            retUSBH = f_close(&USBHFile);//�ر��ļ�
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"close file successful!\r\n",sizeof("close file successful!\r\n"),100);
						}
		}
	}
		
		if(Appli_state == APPLICATION_IDLE)//δ��⵽u��
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"δ��⵽u��!\r\n",15,100);
			status = 0;
		}
		if(Appli_state == APPLICATION_DISCONNECT)//u�̶Ͽ�����
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"�Ͽ�����!\r\n",15,100);
			status = 0;
		}
//		sprintf((char*)show,"%d\r\n",Appli_state);
//		HAL_UART_Transmit(&huart1,show,sizeof(show),100);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
