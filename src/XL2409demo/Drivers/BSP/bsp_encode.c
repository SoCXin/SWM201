/*************************************************************************/
//�������ɡ����͡���ȡ
/*************************************************************************/
#include "bsp_encode.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "bsp_rf.h"
#include "bsp_expand.h"
#include "bsp_flash.h"
#include "bsp_keyslisten.h"
#include "bsp_state_machine.h"
#include "bsp_led.h"

unsigned char gSendBuf1[TEST_BER_DATA_LEN] = "\xAB\xBB\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00"; //0 ��ʼ  1 ����/����  2-5 ��ַ  6-9 ����K  10״̬λ
//״̬λ�� 01 �����ԣ����գ�  02�����źţ����գ� 11�㶯ģʽ����� 12����ģʽ����� 13����ģʽ����ԣ������ֵ����ã�
encode_flag_block encode_flag = {0, 0, 0, {0, 0, 0, 0}};

uint8_t FlashReadBuf[FLASH_MATE_LENGTH]; //Flash��ȡ���ݻ�����

//����Flash������״̬��Ϣ
void Flash_Infor(void)
{
    uint8_t FlashSendBuf1[FLASH_INFOR_LENGTH / 2];
    if(encode_flag.t_or_r_mode_flag == 0)
    {
        FlashSendBuf1[0] = r_encode;   //����̬
    }
    if(encode_flag.t_or_r_mode_flag == 1)
    {
        FlashSendBuf1[0] = t_encode;   //����̬
    }
    FlashSendBuf1[1] = infor_flag.tr_mate_nums;
    FlashSendBuf1[2] = first;
    FlashSendBuf1[3] = 0;
    Flash_ChangeInfor(FLASH_INFOR_ADR, FlashSendBuf1);

}

//����ַ��Ϣ����Flash
void Flash_Adr(void)
{
    uint8_t Flash_Adr_Infor_Buf[FLASH_INFOR_LENGTH];
    uint8_t i;
    for(i = 0; i < 4; i++)
    {
        Flash_Adr_Infor_Buf[i] = rand_num[i];
    }

    if(encode_flag.t_or_r_mode_flag == 0)
    {
        Flash_Adr_Infor_Buf[4] = r_encode;   //����̬
    }
    if(encode_flag.t_or_r_mode_flag == 1)
    {
        Flash_Adr_Infor_Buf[4] = t_encode;   //����̬
    }
    Flash_Adr_Infor_Buf[5] = infor_flag.tr_mate_nums;
    Flash_Adr_Infor_Buf[6] = first;
    Flash_Adr_Infor_Buf[7] = 0;

    Flash_Write_8(FLASH_INFOR_ADR, Flash_Adr_Infor_Buf, FLASH_INFOR_LENGTH);
}


//������Եķ������Ϣ����Flash
void Flash_Mate_Form(void)
{
    uint8_t FlashSendBuf2[FLASH_MATE_LENGTH];
    uint8_t i = 0;
    for(i = 0; i < 4; i++)
    {
        FlashSendBuf2[i] = mRfRxBuf[2 + i];
    }
    FlashSendBuf2[4] = infor_flag.t_or_r_mode_flag;
    FlashSendBuf2[5] = 0;
    FlashSendBuf2[6] = 0;
    FlashSendBuf2[7] = 0;
    FLASH_AddMate(FLASH_MATE_ADR, FlashSendBuf2, infor_flag.tr_mate_nums); //�������Ϣ����flash
}

//�˱ܲ��������ʱ���·���
uint8_t SendDelay_Create(void)
{
    uint8_t Delaytime;
    FLASH_Read_8(FLASH_INFOR_ADR, FlashReadBuf, FLASH_INFOR_LENGTH);
    srand(FlashReadBuf[0]);  //�����ַ�ĵ�һλ��Ϊ�����ʱ������
    Delaytime = rand() % 20 + 30;
    return Delaytime;
}

//�����γ�
void Encode_Form(void)
{
    uint16_t i = 0;
    gSendBuf1[0] = head_encode;
    //gSendBuf1[0] = RfReadRssi();
    if(encode_flag.t_or_r_mode_flag == 0)
    {
        gSendBuf1[1] = r_encode;   //����̬
    }
    if(encode_flag.t_or_r_mode_flag == 1)
    {
        gSendBuf1[1] = t_encode;   //����̬
    }

    //��ַ
    switch(encode_flag.adr_change)
    {
    case 1://����̬�յ�ַ
        for(i = 0; i < 4; i++)
        {
            gSendBuf1[2 + i] = adrnull_encode;
        }
        encode_flag.adr_change = 0;
        break;

    case 2://����̬���������ַ
        FLASH_Read_8(FLASH_INFOR_ADR, FlashReadBuf, FLASH_INFOR_LENGTH);
        for(i = 0; i < 4; i++)
        {
            gSendBuf1[2 + i] = FlashReadBuf[i];
            encode_flag.adr_change = 0;
        }
        break;

    case 3://����̬ѧϰ����̬�ĵ�ַ

        for(i = 0; i < 4; i++)
        {
            gSendBuf1[2 + i] = mRfRxBuf[2 + i];
        }
        encode_flag.adr_change = 1;
        break;
    }


    //�İ���/����
    for(i = 0; i < 4; i++)
    {
        gSendBuf1[i + 6] = encode_flag.keys_flag[i];
    }

    //״̬λ
    gSendBuf1[10] = encode_flag.state_flag;
    encode_flag.state_flag = 0;
}

uint8_t busy_count = 0;
//���Ͱ�
void SendEncode(void)
{
    if(Rssi_Busy() == 1)
    {
        RfDataPackageSend(gSendBuf1, TEST_BER_DATA_LEN);
        busy_count = 0;
    }
    else
    {
        if(busy_count > 5)
        {
            busy_count = 0;
            printf("Send False\n");
        }
        else
        {
            busy_count ++;
            printf("busy_count = %d \n", busy_count);
            HAL_Delay(SendDelay_Create());
            SendEncode();
        }
    }

}


//���ռ��
void ReceiveEncode(void)
{
    LED_MODE1_OFF();
    if(mRfRxBuf[0] == head_encode)  //��һλΪָ��ֵ
    {
        pCurrentState -> pt_or_r_response();
        memset(mRfRxBuf, 0, TEST_BER_DATA_LEN);
    }
}


//������ȡflash�е�״̬
void Flash_Readstate(void)
{
    FLASH_Read_8(FLASH_INFOR_ADR, FlashReadBuf, FLASH_INFOR_LENGTH);
    uint8_t i = 0;
    for(i = 0; i < 4; i++)
    {
        gSendBuf1[2 + i] = FlashReadBuf[i];
    }
    if(FlashReadBuf[5] == 0xff)
    {
        infor_flag.tr_mate_nums = 0;

    }
    else
    {
        infor_flag.tr_mate_nums = FlashReadBuf[5];
    }
    first = FlashReadBuf[6];
    if(FlashReadBuf[4] == r_encode)
    {
        encode_flag.t_or_r_mode_flag = 0;
        encode_flag.adr_change = 1;
        pCurrentState = &R_State;
    }
    if(FlashReadBuf[4] == t_encode)
    {
        encode_flag.t_or_r_mode_flag = 1;
        pCurrentState = &T_State;
    }
    memset(FlashReadBuf, 0, FLASH_INFOR_LENGTH);
    Encode_Form();
    // SendEncode();
}
