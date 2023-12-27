#include "bsp_uart.h"
#include "bsp_tim3.h"
#include <string.h>

UART_HandleTypeDef UartHandle;   //���崮�ڽṹ��
UART_PARAM_STRUCT Uart1Data = {0};

void UART_Init(void)										 //���ڳ�ʼ������
{
    GPIO_InitTypeDef  GPIO_InitStruct;     //���崮������GPIO

    __HAL_RCC_USART2_CLK_ENABLE();         //��������ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();					 //����GPIOʱ��

    /*���ڲ�������*/
    UartHandle.Instance          = USART2;
    UartHandle.Init.BaudRate     = 115200;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;

    HAL_UART_Init(&UartHandle);


    /*GPIO��������*/
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);  //�����ж����ȼ�
    HAL_NVIC_EnableIRQ(USART2_IRQn);          //����NVIC�����ж�

    //__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);  //ʹ�ܴ��ڽ����ж�
}


int fputc(int ch, FILE *f) 	 						//�ض���Ϊ����printf
{
    /* Send a byte to USART */
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);

    return (ch);
}


/********************************************************************************************************
**������Ϣ ��void USART1_IRQHandler(void)
**�������� ��USART1 �����жϴ�����
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void USART1_IRQHandler(void)						//���ڽ����ж���Ӧ���������ղ���������
{
    if(__HAL_UART_GET_FLAG(&UartHandle, USART_CR1_RXNEIE))// ������յ�һ���ֽ�  USART_CR1_RXNEIE
    {
        __HAL_UART_CLEAR_FLAG(&UartHandle, USART_CR1_RXNEIE);
    }

    int ch;
    uint32_t isrflags   = READ_REG(UartHandle.Instance->SR);
    uint32_t cr1its     = READ_REG(UartHandle.Instance->CR1);

    /* UART in mode Receiver -------------------------------------------------*/
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
        __HAL_UART_CLEAR_FLAG(&UartHandle, USART_SR_RXNE);

        ch = USART1->DR;
        if( Uart1Data.RxLen > 0 && 0 == Uart1Data.RxTimer )
        {
            return;
        }
        if( Uart1Data.RxLen < UART_BUF_SIZE )
        {
            Uart1Data.RxBuf[Uart1Data.RxLen ++ ] = ch;
            Uart1Data.RxTimer = UART_RX_OVERTIME;
        }
    }

    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
    {
        __HAL_UART_CLEAR_FLAG(&UartHandle, USART_SR_TXE);
    }
}


/********************************************************************************************************
**������Ϣ ��void USART2_IRQHandler(void)
**�������� ��USART2 �����жϴ�����
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void USART2_IRQHandler(void)						//���ڽ����ж���Ӧ���������ղ���������
{
    if(__HAL_UART_GET_FLAG(&UartHandle, USART_CR1_RXNEIE))// ������յ�һ���ֽ�  USART_CR1_RXNEIE
    {
        __HAL_UART_CLEAR_FLAG(&UartHandle, USART_CR1_RXNEIE);
    }

    int ch;
    uint32_t isrflags   = READ_REG(UartHandle.Instance->SR);
    uint32_t cr1its     = READ_REG(UartHandle.Instance->CR1);

    /* UART in mode Receiver -------------------------------------------------*/
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
        __HAL_UART_CLEAR_FLAG(&UartHandle, USART_SR_RXNE);

        ch = USART1->DR;
        if( Uart1Data.RxLen > 0 && 0 == Uart1Data.RxTimer )
        {
            return;
        }
        if( Uart1Data.RxLen < UART_BUF_SIZE )
        {
            Uart1Data.RxBuf[Uart1Data.RxLen ++ ] = ch;
            Uart1Data.RxTimer = UART_RX_OVERTIME;
        }
    }

    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
    {
        __HAL_UART_CLEAR_FLAG(&UartHandle, USART_SR_TXE);
    }
}
