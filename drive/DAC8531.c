/******************** (C) COPYRIGHT 2015 AVER ********************
 * �ļ���  ��DAC8531.C
 * ����    ������
 * ����    ���⺯����
 * ����    ������16BIT DAC
 * Ӳ�����ӣ�
 * �޸����ڣ�2015-8-18
*********************************************************************/
#include "my_register.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
/********************************************************************/
#define DAC_SYNC_LOW   GPIO_ResetBits(GPIOE, GPIO_Pin_2);
#define DAC_SYNC_HIGH  GPIO_SetBits(GPIOE, GPIO_Pin_2);   
      
#define DAC_SCK_LOW   GPIO_ResetBits(GPIOE, GPIO_Pin_3);
#define DAC_SCK_HIGH  GPIO_SetBits(GPIOE, GPIO_Pin_3); 

#define DAC_SDA_LOW   GPIO_ResetBits(GPIOE, GPIO_Pin_4);
#define DAC_SDA_HIGH  GPIO_SetBits(GPIOE, GPIO_Pin_4); 
/**********************************************************************************************************
*	�� �� ��: DAC8531_GPIOCoing
*	����˵��: ���ų�ʼ��
*	��    �Σ�
*	�� �� ֵ: ��
**********************************************************************************************************/
void DAC8531_GPIOCoing(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//
	GPIO_Init(GPIOE, &GPIO_InitStructure);//
}
/**********************************************************************************************************
*	�� �� ��: DAC8531_GPIOCoing
*	����˵��: ���ų�ʼ��
*	��    �Σ�
*	�� �� ֵ: ��
**********************************************************************************************************/
void DAC8531_Send(vu16 date)
{
	vu8 cont;
	DAC_SYNC_LOW;//SYNC�õ�
	DAC_SCK_HIGH;//SCK�ø�
	NOP;
	NOP;
	NOP;
	NOP;
	DAC_SYNC_HIGH;//SYNC�ø�
	NOP;
	DAC_SCK_HIGH;//SCK�ø�
	DAC_SYNC_LOW;//SYNC�õ�
	for(cont = 0; cont < 24; cont++)
	{
		if(cont>7)
		{
			if(date & 0x8000)		// �ȴ���λ
			{
				DAC_SDA_HIGH;
			}
			else
			{
				DAC_SDA_LOW;
			}
			date=date<<1;
		}
		else//ǰ8λ����ǰ6λΪ��Чλ����2λΪ����ģʽ�趨���˴�Ϊ00����Ϊ��������ģʽ��
		{
			DAC_SDA_LOW;
		}
		DAC_SCK_LOW;//SCK�õ�
		NOP;
		NOP;
		NOP;
		DAC_SCK_HIGH;//SCK�ø�
	}
	DAC_SYNC_HIGH;//SYNC�ø�
	DAC_SCK_HIGH;//SCK�ø�
}
	
	