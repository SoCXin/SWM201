#include "bsp_flash.h"
#include <math.h>

//д��FLASH (4λ)
void Flash_Write(uint32_t WriteAddr, uint32_t *pBuffer)
{

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;
    uint32_t flash_program_start = WriteAddr;                              //flashд��ʼ��ַ
    uint32_t flash_program_end = (WriteAddr + sizeof(pBuffer));           //flashд������ַ
    uint32_t *src = pBuffer;                                                          //����ָ��

    HAL_FLASH_Unlock();                                                                 //����FLASH
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGEERASE;                            //��д����FLASH_TYPEERASE_PAGEERASE=Page��, FLASH_TYPEERASE_SECTORERASE=Sector��
    EraseInitStruct.PageAddress = WriteAddr;                                //��д��ʼ��ַ
    EraseInitStruct.NbPages  = 2;                                                       //��Ҫ��д��ҳ����
    HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);                                    //ִ��page����,PAGEError���ز�д�����page,����0xFFFFFFFF,��ʾ��д�ɹ�

    do
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, flash_program_start, src) == HAL_OK) //ִ��Program; FLASH_TYPEPROGRAM_PAGE�β���Ч,��Ҫ�޸Ŀ�
        {
            flash_program_start += 128;                                                     //flash��ʼָ��ָ���һ��page
            src += 32;                                                                      //��������ָ��
        }
    }
    while (flash_program_start < flash_program_end);
    HAL_FLASH_Lock();                                                                   //����FLASH
}

//д��FLASH (1λ)
void Flash_Write_8(uint32_t WriteAddr, uint8_t *pBuffer1, uint16_t length)
{
    uint8_t mid_pBuffer[length + 3];
    uint16_t flash_length = ceil((float)length / 4.0);
    uint32_t pBuffer2[flash_length];
    uint16_t i;
    for(i = 0; i < length + 3; i++)
    {
        if(i < length)
        {
            mid_pBuffer[i] = pBuffer1[i];
        }
        else
        {
            mid_pBuffer[i] = 0xff;
        }

    }
    for(i = 0; i < flash_length ; i++)
    {
        pBuffer2[i] = (mid_pBuffer[0 + i * 4] << 24) | (mid_pBuffer[1 + i * 4] << 16) | (mid_pBuffer[2 + i * 4] << 8) | mid_pBuffer[3 + i * 4];
    }
    Flash_Write(WriteAddr, pBuffer2);
}


//�������ַ��ʼ��ȡ���ݣ����ض����ĸõ�ַ������
uint32_t FLASH_ReadWord(uint32_t faddr)
{
    return *(uint32_t *)faddr;
}


//���ݶ�ȡ4λ lengthΪ��Ҫ�ĳ���
void FLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint16_t length)
{
    uint16_t i;
    for(i = 0; i < length; i++)
    {
        pBuffer[i] = FLASH_ReadWord(ReadAddr);//���ֽڶ�ȡ
        ReadAddr += 4; //��ַ���ƫ��
    }
}

//���ݶ�ȡ1λ lengthΪ��Ҫ��pBuffer1�ĳ���
void FLASH_Read_8(uint32_t ReadAddr, uint8_t *pBuffer1, uint16_t length)
{
    uint16_t flash_length = ceil((float)length / 4.0);
    uint32_t pBuffer2[flash_length];
    FLASH_Read(ReadAddr, pBuffer2, flash_length);
    uint16_t i;
    for(i = 0; i < flash_length ; i++)
    {
        if((0 + 4 * i) < length)
        {
            pBuffer1[0 + 4 * i] = (pBuffer2[i] >> 24) & 0xff;
        }
        if((1 + 4 * i) < length)
        {
            pBuffer1[1 + 4 * i] = (pBuffer2[i] >> 16) & 0xff;
        }
        if((2 + 4 * i) < length)
        {
            pBuffer1[2 + 4 * i] = (pBuffer2[i] >> 8) & 0xff;
        }
        if((3 + 4 * i) < length)
        {
            pBuffer1[3 + 4 * i] = pBuffer2[i] & 0xff;
        }
    }

}

//��������Ϣ numsΪ�ܼƶ��ٸ���Եķ����
void FLASH_AddMate(uint32_t WriteAddr, uint8_t *pBuffer, uint16_t nums)
{
    uint32_t flash_mid[32];
    FLASH_Read(WriteAddr, flash_mid, 32);
    flash_mid[nums * 2] =  (pBuffer[0] << 24) | (pBuffer[1] << 16) | (pBuffer[2] << 8) | pBuffer[3];
    flash_mid[nums * 2 + 1] =  (pBuffer[4] << 24) | (pBuffer[5] << 16) | (pBuffer[6] << 8) | pBuffer[7];
    Flash_Write(WriteAddr, flash_mid);
}

//�޸�������Ϣ
void Flash_ChangeInfor(uint32_t WriteAddr, uint8_t *pBuffer)
{
    uint32_t flash_mid[32];
    FLASH_Read(WriteAddr, flash_mid, 32);
    flash_mid[1] = (pBuffer[0] << 24) | (pBuffer[1] << 16) | (pBuffer[2] << 8) | pBuffer[3];
    Flash_Write(WriteAddr, flash_mid);
}

//����
void Flash_Erase(uint32_t UserAddr)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;
    HAL_FLASH_Unlock();                                                                 //����FLASH
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGEERASE;                            //��д����FLASH_TYPEERASE_PAGEERASE=Page��, FLASH_TYPEERASE_SECTORERASE=Sector��
    EraseInitStruct.PageAddress = UserAddr;                                             //��д��ʼ��ַ
    EraseInitStruct.NbPages  = 2;                                                       //��Ҫ��д��ҳ����
    HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);                                    //ִ��page����,PAGEError���ز�д�����page,����0xFFFFFFFF,��ʾ��д�ɹ�
    HAL_FLASH_Lock();
}

