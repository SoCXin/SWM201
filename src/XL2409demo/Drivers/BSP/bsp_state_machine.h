#ifndef __BSP_STATE_MACHINE_H
#define __BSP_STATE_MACHINE_H

typedef struct
{
    void (* pt_or_r_studykeynum_1)(); //ѧϰ��������1�η���
    void (* pt_or_r_studykeynum_2)(); //ѧϰ��������2�η���
    void (* pt_or_r_studykeynum_3)(); //ѧϰ��������3�η���
    void (* pt_or_r_studykeynum_5)(); //ѧϰ��������5�η���
    void (* pt_or_r_studykeynum_8)(); //ѧϰ��������8�η���
    void (* pt_or_r_keystate)();  //�ĸ��������·���������ˣ�
    void (* pt_or_r_response)();  //�յ��źŴ���
} State;

void Init_State_Machine(void);
extern State *pCurrentState;

extern State R_State;
extern State T_State;

#endif
