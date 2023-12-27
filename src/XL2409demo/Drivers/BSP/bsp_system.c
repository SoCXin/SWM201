#include "bsp_system.h"
#include "main.h"

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /*����ʱ��ԴHSE/HSI/LSE/LSI*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                                      //����HSI
    //RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_4MHz;                            //����HSI���ʱ��Ϊ4MHz
    //RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;                            //����HSI���ʱ��Ϊ8MHz
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_16MHz;                           //����HSI���ʱ��Ϊ16MHz
    //RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_22p12MHz;                        //����HSI���ʱ��Ϊ22.12MHz
    //RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;                             //����HSI���ʱ��Ϊ24MHz
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                                      //HSI����Ƶ
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                                     //�ر�HSE
    RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;                                                 //HSE����Ƶ�ʷ�Χ16M~32M
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                                                     //�ر�LSI

    HAL_RCC_OscConfig(&RCC_OscInitStruct);                                                    //RCC������ʼ��

    //��ʼ��CPU,AHB,APB����ʱ��
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; //RCCϵͳʱ������
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;                                        //SYSCLK��Դѡ��ΪHSI
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                            //APHʱ�Ӳ���Ƶ
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                                              //APBʱ�Ӳ���Ƶ

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);                                 //��ʼ��RCCϵͳʱ��
}


