#include "bsp_state_machine.h"
#include <stdio.h>
#include "bsp_expand.h"
#include "bsp_encode.h"

State *pCurrentState;

//����״̬
/**************************/
//���ն�
State R_State =
{
    R_Tap,
    R_Selflock,
    R_Interlock,//
    R_Toggle,
    R_Clear,//
    Ignore,
    R_Receive,
};

/**************************/
//�����
State T_State =
{
    LedBlinkTwice,
    LedBlinkTwice,
    LedBlinkTwice,//
    T_Toggle,
    LedBlinkTwice,//
    T_Keys_State,
    T_Receive
};


/***********************************************
��ʼ��ϵͳ��ǰ״ָ̬�룬ָ��ϵͳ����ʼ״̬
************************************************/
void Init_State_Machine(void)
{
	//��ʼΪ����
    pCurrentState = &R_State;
	
  //��ʼΪ����
    // pCurrentState = &T_State;
    // encode_flag.t_or_r_mode_flag = 1;
}


