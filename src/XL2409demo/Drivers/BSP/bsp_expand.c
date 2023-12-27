/*************************************************************************/
#include "bsp_expand.h"
#include <stdio.h>
#include <string.h>
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_tim3.h"
#include "bsp_uart.h"
#include "bsp_rf.h"
#include "bsp_encode.h"
#include "bsp_pins.h"
#include "bsp_flash.h"
#include "bsp_keyslisten.h"
#include "bsp_state_machine.h"

r_infor_flag_block infor_flag = {0, 0, 0};
uint8_t tap_first[4] = {1, 1, 1, 1}; //�㶯��ʼ/����
uint8_t selfdelay[4] = {0, 0, 0, 0}; //��������������Ϣ
uint8_t interdelay[4] = {0, 0, 0, 0}; //��������������Ϣ
uint8_t rf_flag = 0;  //���ͱ��
uint8_t t_tap_first[4] = {1, 1, 1, 1}; //�㶯ģʽ�жϵ�һ�ΰ��� ����һ�η����ź�

void Ignore(void)
{
    //�պ����������в���
}

//��ӡ��־ ������
void PrintLog(void)
{
    Encode_Form();
    SendEncode();
}

//���´����޹��� ��˸����
void LedBlinkTwice(void)
{
    LedCount(LED1_PORT, LED1_PIN, 2);
}

//��״̬
void Clear_ModeFlag(void)
{
    memset(tap_first, 1, 4);
    memset(selfdelay, 0, 4);
    memset(interdelay, 0, 4);
    memset(encode_flag.keys_flag, 0, 4);
    memset(t_tap_first, 1, 4);
    rf_flag = 0;

}

//�㶯ģʽ���ն�Ϩ��
void LED_MODE1_OFF(void)
{
    uint8_t i = 0;
    for(i = 0; i < 4; i++)
    {
        if(tap_first[i] == 0)
        {
            if(tim_rfsend >= 320)
            {
                LedOff(LED2_PORT, LED2_PIN);
                tap_first[i] = 1;
            }
        }
    }
}


//�жϵ�ַ�Ƿ���ͬ
uint8_t Judge_Adr(void)
{
    uint8_t i = 0;
    uint8_t t = 0;
    FLASH_Read_8(FLASH_INFOR_ADR, FlashReadBuf, FLASH_INFOR_LENGTH);
    for(i = 0; i < 4; i++) //��ַ��ͬ
    {
        if(mRfRxBuf[2 + i] == FlashReadBuf[i])
        {
            t++;
        }
    }
    if(t == 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//�ж�flash���Ƿ��ж�Ӧ��ַ �Լ�����ģʽ��Ϣ
uint8_t Judge_Flash_Mate(uint16_t nums)
{
    uint16_t i = 0;
    uint16_t length = 8 * nums;
    uint8_t flash_matebuf[length];
    if(length > 0)
    {
        FLASH_Read_8(FLASH_MATE_ADR, flash_matebuf, length);
        for(i = 0; i < nums; i++)
        {
            if(mRfRxBuf[2] == flash_matebuf[0 + 8 * i] && mRfRxBuf[3] == flash_matebuf[1 + 8 * i] && mRfRxBuf[4] == flash_matebuf[2 + 8 * i] && mRfRxBuf[5] == flash_matebuf[3 + 8 * i])
            {
                infor_flag.t_or_r_mode_flag = flash_matebuf[4 + 8 * i];
                return 1;
            }
        }
    }
    return 0;
}

uint8_t Rssi_Busy(void)
{
    uint8_t rssi_val;
    __disable_irq();
    rssi_val = RfReadRssi();
    __enable_irq();

    if(rssi_val > 67)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
/***************************************************************/
//���ն� �㶯ģʽ ��������״̬ ѧϰ����K 1��
void R_Tap(void)
{
    infor_flag.r_reqmate_flag = 1;  //�㶯ģʽ�����
    encode_flag.state_flag = 10;  //״̬λΪ0A
    Encode_Form();
    SendEncode();
    LedOn(LED2_PORT, LED2_PIN);
}

//���ն� ����ģʽ ��������״̬ ѧϰ����K 2��
void R_Selflock(void)
{
    infor_flag.r_reqmate_flag = 2;  //����ģʽ�����
    encode_flag.state_flag = 11;  //״̬λΪ0B
    Encode_Form();
    SendEncode();
    LedOn(LED2_PORT, LED2_PIN);

}

//���ն� ����ģʽ ��������״̬ ѧϰ����K 3��
void R_Interlock(void)
{
    infor_flag.r_reqmate_flag = 3;  //����ģʽ�����
    encode_flag.state_flag = 12;  //״̬λΪ0C
    Encode_Form();
    SendEncode();
    LedOn(LED2_PORT, LED2_PIN);
}

//���ն� �л�Ϊ�����  ѧϰ����K����5��
void R_Toggle(void)
{
    infor_flag.r_reqmate_flag = 0; //�����״̬���
    KEYS_Init();  //�����ĸ�IOΪ����
    Clear_ModeFlag();  //�������ģʽ��־λ
    encode_flag.t_or_r_mode_flag = 1;  //תΪ����̬
    encode_flag.adr_change = 2;  //��ַ����Ϊ�������ַ
    //Encode_Form();
    //SendEncode();
    pCurrentState = &T_State; //�л�״̬��
    Flash_Infor();
    LedCount(LED2_PORT, LED2_PIN, 2); //��˸����
}

//���ն� ���״̬  ѧϰ����K����8��
void R_Clear(void)
{
    infor_flag.r_reqmate_flag = 0; //�����״̬���
    infor_flag.tr_mate_nums = 0;
    Clear_ModeFlag(); //�������ģʽ��־λ
    Encode_Form();
    //SendEncode();
    //********************************/����Flash
    Flash_Erase(FLASH_MATE_ADR); //����Flash
    Flash_Infor();
    LedCount(LED2_PORT, LED2_PIN, 8);
}
/***************************************************************/

//���ն���Թ���
void R_Mate_Receive(void)
{
    infor_flag.t_or_r_mode_flag = infor_flag.r_reqmate_flag;
    infor_flag.r_reqmate_flag = 0;
    encode_flag.adr_change = 3;	//�Է���˵ĵ�ַ����
    encode_flag.state_flag = 1; //�����ɱ�־λ�����ط����
    LedCount(LED2_PORT, LED2_PIN, 5);
    Encode_Form();
    SendEncode();
    //********************************/����Flash
    Flash_Mate_Form();
    infor_flag.tr_mate_nums ++;
    Flash_Infor();
}

//���ն� �㶯ģʽ ���ź���Ӧ
void R_Tap_Receive(void)
{
    uint8_t i = 0;
    for(i = 0; i < 4; i++)
    {
        if(mRfRxBuf[i + 6] == 0x01)
        {
            tim_rfsend = 0; //ÿ�յ�һ�ΰ��������źţ����Ϩ���ʱ����
        }
        if(mRfRxBuf[i + 6] == 0x01 && tap_first[i] == 1) //��Ӧ������һ�ΰ��� tap_first���������ж��Ƿ񰴼�����
        {
            //printf("111 \n");
            tap_first[i] = 0;
            encode_flag.keys_flag[i] = 1;
            encode_flag.state_flag = 2;  //״̬λ �����ź�
            encode_flag.adr_change = 3;	//�Է���˵ĵ�ַ����
            LedOn(LED2_PORT, LED2_PIN);
            Encode_Form();
            SendEncode();
        }
        if(mRfRxBuf[i + 6] == 0x00 && tap_first[i] == 0)
        {
            // printf("222 \n");
            tap_first[i] = 1;
            encode_flag.keys_flag[i] = 0;
            //SendEncode();

        }
    }
    if(tap_first[0] + tap_first[1] + tap_first[2] + tap_first[3] == 4)  ///*
    {
        // printf("333 \n");
        LedOff(LED2_PORT, LED2_PIN);   //�İ�����δ���µ�Ϩ��
    }
    Pins_Ctrl();  //��λ���Ÿߵ�

}


//���ն� ����ģʽ ���ź���Ӧ
void R_Selflock_Receive(void)
{
    uint8_t i = 0;
    for(i = 0; i < 4; i++)
    {
        if(mRfRxBuf[i + 6] == 0x01)
        {
            selfdelay[i] = 1;
        }
        if(mRfRxBuf[i + 6] == 0x00 && selfdelay[i] == 1)
        {
            selfdelay[i] = 0;
            if(encode_flag.keys_flag[i] == 0)
            {
                encode_flag.keys_flag[i] = 1;
            }
            else
            {
                encode_flag.keys_flag[i] = 0;
            }
            encode_flag.state_flag = 2;  //״̬λ �����ź�
            encode_flag.adr_change = 3;	//�Է���˵ĵ�ַ����
            Encode_Form();
            SendEncode();
            LedCount(LED2_PORT, LED2_PIN, 1);
        }
    }
    Pins_Ctrl();  //��λ���Ÿߵ�
}


//����̬ ����ģʽ ���ź���Ӧ
void R_Interlock_Receive(void)
{
    uint8_t i = 0;
    memset(encode_flag.keys_flag, 0, 4);
    for(i = 0; i < 4; i++)
    {
        if(mRfRxBuf[i + 6] == 0x01)
        {
            interdelay[i] = 1;
        }

        if(mRfRxBuf[i + 6] == 0x00 && interdelay[i] == 1)
        {
            interdelay[i] = 0;
            encode_flag.keys_flag[i] = 1;
            encode_flag.state_flag = 2;  //״̬λ �����ź�
            encode_flag.adr_change = 3;	//�Է���˵ĵ�ַ����
            Encode_Form();
            SendEncode();
            LedCount(LED2_PORT, LED2_PIN, 1);
        }
    }
    Pins_Ctrl();  //��λ���Ÿߵ�
}


//���ն� ���ź���Ӧ
void R_Receive(void)
{
    //�Է�Ϊ����̬ �� ����ȴ����
    if(mRfRxBuf[1] == t_encode && infor_flag.r_reqmate_flag > 0 && Judge_Flash_Mate(infor_flag.tr_mate_nums) == 0) //��Թ���
    {
        if(mRfRxBuf[6] + mRfRxBuf[7] + mRfRxBuf[8] + mRfRxBuf[9] == 0)
        {
            R_Mate_Receive();
        }
        //����Flash
    }
    else if(mRfRxBuf[1] == t_encode && Judge_Flash_Mate(infor_flag.tr_mate_nums) == 1) //��ƥ����
    {
        switch(infor_flag.t_or_r_mode_flag)
        {
        case 1: //Ϊ�㶯
            R_Tap_Receive();
            break;
        case 2: //Ϊ����
            R_Selflock_Receive();
            break;
        case 3: //Ϊ����
            R_Interlock_Receive();
            break;
        }
    }
}


/***************************************************************/
//����� �л�Ϊ���ն�  ѧϰ����K����5��
void T_Toggle(void)
{
    Pins_Init();  //�����ĸ�IOΪ����
    Clear_ModeFlag(); //�������ģʽ��־λ
    encode_flag.t_or_r_mode_flag = 0;  //תΪ����̬
    encode_flag.adr_change = 1;  //��ַ����Ϊ�յ�ַ
    Encode_Form();
    //SendEncode();
    pCurrentState = &R_State; //�л�״̬��
    //********************************/����Flash
    Flash_Infor();
    LedCount(LED2_PORT, LED2_PIN, 5);	 //��˸���
}

//����˰�������
void T_Keys_State(void)
{
    uint8_t i = 0;
    for(i = 0; i < 4; i++)
    {
        if(studykey_lock == 0 && keys[i] == 1 && t_tap_first[i] == 1)
        {
            RandNum();
            encode_flag.keys_flag[i] = 1, rf_flag = 1, t_tap_first[i] = 0, tim_rfsend = 300;
            LedOn(LED2_PORT, LED2_PIN);
        }
        if(keys[i] == 0 && t_tap_first[i] == 0)
        {
            t_tap_first[i] = 1;
            encode_flag.keys_flag[i] = 0;
            if(keys[0] + keys[1] + keys[2] + keys[3] == 0)
            {
                LedOff(LED2_PORT, LED2_PIN);    //���а����������Ϩ����ֹͣ����
                rf_flag = 0;
            }
            Encode_Form();
            SendEncode();
        }
    }

    if(rf_flag == 1)
    {
        //		rf_flag = 0;
        if(tim_rfsend >= 300)  //����һ��
        {
            tim_rfsend = 0;
            Encode_Form();
            SendEncode();
        }
    }
}


//����� ���ź���Ӧ
void T_Receive(void)
{
    //�Է�Ϊ����̬�ҵ�ַ��ͬ
    if(mRfRxBuf[1] == r_encode && Judge_Adr() == 1)
    {
        //  LedCount(LED1_PORT, LED1_PIN, 7); //*********����***********//
        if(mRfRxBuf[10] == 0x01)  //���յ��������ź�
        {
            LedCount(LED1_PORT, LED1_PIN, 5); //*********����***********//
        }
        if(mRfRxBuf[10] == 0x02)  //���յ������ź�
        {
            LedCount(LED1_PORT, LED1_PIN, 3); //*********����***********//
        }

    }
}

