/**
  ******************************************************************************
  * @file    xl32f0xx_it.c
  * @author  MCU Application Team
  * @Version V1.0.0
  * @Date    2020-10-19
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "xl32f0xx_it.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers         */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/* XL32F0xx Peripheral Interrupt Handlers                                     */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_xl32f003xx.s).                   */
/******************************************************************************/

/**
  * @brief This function handles SPI1.
  */
void SPI1_IRQHandler(void)
{
  /* ��� ISR �Ĵ����е� RXNE ��־ֵ */
  if(LL_SPI_IsActiveFlag_RXNE(SPI1))
  {
    /* ���ý��ջص� */
    APP_SpiRxCallback();
  }
  /* Check RXNE flag value in ISR register */
  else if(LL_SPI_IsActiveFlag_TXE(SPI1))
  {
    /* ���÷��ͻص� */
    APP_SpiTxCallback();
  }
  /* ��� ISR �Ĵ����е� STOP ��־ֵ */
  else if(LL_SPI_IsActiveFlag_OVR(SPI1))
  {
    /* ���ô����� */
    APP_SpiTransferErrorCallback();
  }
}

/************************ (C) COPYRIGHT Xinling *****END OF FILE****/
