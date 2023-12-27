#include "bsp_tim3.h"

TIM_HandleTypeDef  TimHandle;

void TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE() ;
    TimHandle.Instance = TIM3;                                           //ѡ��TIM3
    TimHandle.Init.Period            = 1000 - 1;                         //�Զ���װ��ֵ
    TimHandle.Init.Prescaler         = 1000 - 1;                         //Ԥ��ƵΪ1000-1
    TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           //ʱ�Ӳ���Ƶ
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;               //���ϼ���
    TimHandle.Init.RepetitionCounter = 1 - 1;                            //���ظ�����
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;//TIM_AUTORELOAD_PRELOAD_DISABLE;   //�Զ���װ�ؼĴ���û�л���

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 2);                //�����ж����ȼ�
    HAL_NVIC_EnableIRQ(TIM3_IRQn);                        //ʹ���ж�
    __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_UPDATE);        //����жϱ�־
    //Tim_On();   //������ʱ��
    Clear_Tim();
}

uint16_t tim_rfsend = 0; //���ڵ㶯����������ź�
uint16_t tim_led1 = 0; //����led1��˸
uint16_t tim_led2 = 0;  //����led2��˸
uint16_t tim_count1 = 0;  //��������/������ʱ��
//��ʱ���жϻص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    tim_rfsend ++;
    tim_led1 ++;
    tim_led2 ++;
    tim_count1 ++;  //��������/������ʱ��
    if(tim_rfsend > 2000) tim_rfsend = 0;
    if(tim_led1 > 2000) tim_led1 = 0;
    if(tim_led2 > 2000) tim_led2 = 0;
    if(tim_led2 > 2000) tim_led2 = 0;
    if(tim_count1 > 2000) tim_count1 = 0;
    //HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
}

void Tim_On(void)
{
    HAL_TIM_Base_Start_IT(&TimHandle);
}

void Clear_Tim(void)
{
    HAL_TIM_Base_Stop_IT(&TimHandle);                 //�رռ���
    __HAL_TIM_SET_COUNTER(&TimHandle, 0);							//�������
}





