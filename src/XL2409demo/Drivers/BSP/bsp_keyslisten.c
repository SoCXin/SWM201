/*************************************************************************/
//�涨����ѧϰ����/�İ����ķ���
/*************************************************************************/
#include <stdlib.h>
#include "bsp_keyslisten.h"
#include "stdio.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_tim3.h"
#include "bsp_encode.h"
#include "bsp_state_machine.h"
#include "bsp_flash.h"

uint8_t studykey_lock = 0; //ѧϰ�������ȼ����
uint8_t count_f = 0;  //ѧϰ�������´���
uint8_t key_f = 0;  //������
uint8_t first = 0;  //���������ַ
uint8_t rand_num[4] = {0, 0, 0, 0};

//���������ַ
void RandNum(void)
{
    if(first == 0xff)  //��һ�ΰ��°������������ַ
    {
        first = 0x55;//��Ϊ0xff����
        srand(tim_count1);
        rand_num[0] = rand() % 255;
        rand_num[1] = rand() % 255;
        rand_num[2] = rand() % 255;
        rand_num[3] = rand() % 255;
        Flash_Adr();
        if(encode_flag.t_or_r_mode_flag == 1)
        {
            encode_flag.adr_change = 2;
        }
    }
}


uint8_t study_key = 1;  //ѧϰ����
uint8_t keys[4] = {0, 0, 0, 0}; //�ĸ�����
// ��ⰴ��
void Key_Get(void)
{
//    uint8_t i;
    uint8_t study_key1 = 1, study_key2 = 0;
//    uint8_t keys1[4] = {0, 0, 0, 0};
//    uint8_t keys2[4] = {0, 0, 0, 0};

    //send����
    study_key1 = KEY_SCAN(SEND_KEY3_GPIO_PORT, SEND_KEY3_GPIO_PIN);
//    keys1[0] = KEY_SCAN(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
//    keys1[1] = KEY_SCAN(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
//    keys1[2] = KEY_SCAN(KEY3_GPIO_PORT, KEY3_GPIO_PIN);
//    keys1[3] = KEY_SCAN(KEY4_GPIO_PORT, KEY4_GPIO_PIN);
    HAL_Delay(10);
    study_key2 = KEY_SCAN(SEND_KEY3_GPIO_PORT, SEND_KEY3_GPIO_PIN);
//    keys2[0] = KEY_SCAN(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
//    keys2[1] = KEY_SCAN(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
//    keys2[2] = KEY_SCAN(KEY3_GPIO_PORT, KEY3_GPIO_PIN);
//    keys2[3] = KEY_SCAN(KEY4_GPIO_PORT, KEY4_GPIO_PIN);

    if(study_key1 == study_key2)
    {
        study_key = study_key1;
    }

//    //�İ���
//    if(encode_flag.t_or_r_mode_flag == 1) //Ϊ����״̬
//    {
//        for (i = 0; i < 4; i++)
//        {
//            if(keys1[i] == keys2[i])
//            {
//                keys[i] = keys1[i];
//            }
//        }
//    }

		if(study_key == 1)
		{
        Tim_On();
		}
}


//ѧϰ�������´���
void Study_Key_Count(void)
{
    if(study_key == 1)
    {
        RandNum();
        studykey_lock = 1;  //ѧϰ�������ȼ����
        tim_count1 = 0;
        key_f	= 1;
        LedOn(LED2_PORT, LED2_PIN); //LED2
    }

    if(study_key == 0 && key_f == 1)
    {
        key_f	= 0;
        studykey_lock = 0;
        count_f ++;

        LedOff(LED2_PORT, LED2_PIN);
    }

    if(tim_count1 >= 500 && count_f > 0)	//����һ��ѧϰ����������һ��ѧϰ��������֮���ʱ����ڹ涨ֵ
    {
        tim_count1 = 0;
        Study_Key_Response(count_f);
        count_f = 0;
    }
}

//ѧϰ��ť��Ӧ
void Study_Key_Response(uint16_t count)
{
    switch(count)
    {

    //����һ��
    case 1:
        pCurrentState -> pt_or_r_studykeynum_1();
        break;

    //��������
    case 2:
        pCurrentState -> pt_or_r_studykeynum_2();
        break;

    //��������
    case 3:
        pCurrentState -> pt_or_r_studykeynum_3();
        break;

    //�������
    case 5:
        pCurrentState -> pt_or_r_studykeynum_5();
        break;

    //�����˴�
    case 8:
        pCurrentState -> pt_or_r_studykeynum_8();
        break;

    default:
        LedCount(LED1_PORT, LED1_PIN, 2); //���´����޹��� ��˸����
    }
}




//����˰�������
void Keys_State(void)
{
    pCurrentState -> pt_or_r_keystate();
}





