#include "bsp_iwdg.h"
#include "bsp_led.h"

IWDG_HandleTypeDef IwdgHandle;

void IWDG_Init(void)
{
    __HAL_RCC_LSI_ENABLE();
    while (READ_BIT(RCC->CSR, RCC_CSR_LSIRDY) == 0U);

    IwdgHandle.Instance = IWDG;                     //ѡ��IWDG
    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;  //����32��Ƶ
    IwdgHandle.Init.Reload = (3000);                //IWDG��������װ��ֵΪ1000��3s

    HAL_IWDG_Init(&IwdgHandle);                      //��ʼ��IWDG
}

void Feed_Dog(void)
{
    HAL_IWDG_Refresh(&IwdgHandle);
}
