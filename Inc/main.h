/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USB0_CTRL_Pin GPIO_PIN_2
#define USB0_CTRL_GPIO_Port GPIOE
#define AUX_12V_CTRL_Pin GPIO_PIN_3
#define AUX_12V_CTRL_GPIO_Port GPIOE
#define USB1_CTRL_Pin GPIO_PIN_4
#define USB1_CTRL_GPIO_Port GPIOE
#define SEL0_USB_Pin GPIO_PIN_5
#define SEL0_USB_GPIO_Port GPIOE
#define SEL1_USB_Pin GPIO_PIN_6
#define SEL1_USB_GPIO_Port GPIOE
#define USER_BTN_Pin GPIO_PIN_13
#define USER_BTN_GPIO_Port GPIOC
#define SNS_RA12_Pin GPIO_PIN_3
#define SNS_RA12_GPIO_Port GPIOF
#define SNS_LL_Pin GPIO_PIN_4
#define SNS_LL_GPIO_Port GPIOF
#define ROUTER_CTRL_Pin GPIO_PIN_7
#define ROUTER_CTRL_GPIO_Port GPIOF
#define LEDRING1_CTRL_Pin GPIO_PIN_8
#define LEDRING1_CTRL_GPIO_Port GPIOF
#define LEDRING0_CTRL_Pin GPIO_PIN_9
#define LEDRING0_CTRL_GPIO_Port GPIOF
#define SNS_12VIN_Pin GPIO_PIN_0
#define SNS_12VIN_GPIO_Port GPIOC
#define SNS_WICH_Pin GPIO_PIN_2
#define SNS_WICH_GPIO_Port GPIOC
#define SNS_USB_Pin GPIO_PIN_3
#define SNS_USB_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_0
#define LED_GREEN_GPIO_Port GPIOB
#define SNS_SA5_Pin GPIO_PIN_1
#define SNS_SA5_GPIO_Port GPIOB
#define SEL0_LL_Pin GPIO_PIN_2
#define SEL0_LL_GPIO_Port GPIOB
#define SEL0_WICH_Pin GPIO_PIN_13
#define SEL0_WICH_GPIO_Port GPIOF
#define WICH1_CTRL_Pin GPIO_PIN_14
#define WICH1_CTRL_GPIO_Port GPIOF
#define WICH0_CTRL_Pin GPIO_PIN_15
#define WICH0_CTRL_GPIO_Port GPIOF
#define SEL1_RA12_Pin GPIO_PIN_0
#define SEL1_RA12_GPIO_Port GPIOG
#define SEL0_RA12_Pin GPIO_PIN_1
#define SEL0_RA12_GPIO_Port GPIOG
#define GPIO_2_Pin GPIO_PIN_8
#define GPIO_2_GPIO_Port GPIOE
#define SENSORS_CTRL_Pin GPIO_PIN_10
#define SENSORS_CTRL_GPIO_Port GPIOE
#define AUX_5V_CTRL_Pin GPIO_PIN_12
#define AUX_5V_CTRL_GPIO_Port GPIOE
#define BTN_IN_Pin GPIO_PIN_14
#define BTN_IN_GPIO_Port GPIOE
#define BTN_LED_Pin GPIO_PIN_10
#define BTN_LED_GPIO_Port GPIOB
#define RESET_555_Pin GPIO_PIN_11
#define RESET_555_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define SEL1_SA5_Pin GPIO_PIN_11
#define SEL1_SA5_GPIO_Port GPIOD
#define SEL0_SA5_Pin GPIO_PIN_12
#define SEL0_SA5_GPIO_Port GPIOD
#define SEL1_LL_Pin GPIO_PIN_13
#define SEL1_LL_GPIO_Port GPIOD
#define Btn_In_Pin GPIO_PIN_8
#define Btn_In_GPIO_Port GPIOC
#define Btn_LED_Pin GPIO_PIN_9
#define Btn_LED_GPIO_Port GPIOC
#define SEL1_12VIN_Pin GPIO_PIN_5
#define SEL1_12VIN_GPIO_Port GPIOD
#define SEL0_12VIN_Pin GPIO_PIN_6
#define SEL0_12VIN_GPIO_Port GPIOD
#define IN_12V_CTRL_Pin GPIO_PIN_7
#define IN_12V_CTRL_GPIO_Port GPIOD
#define GPIO_0_Pin GPIO_PIN_9
#define GPIO_0_GPIO_Port GPIOG
#define Button_IN_Pin GPIO_PIN_14
#define Button_IN_GPIO_Port GPIOG
#define SEL1_WICH_Pin GPIO_PIN_6
#define SEL1_WICH_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_7
#define LED_BLUE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
