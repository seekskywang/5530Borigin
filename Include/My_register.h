/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * �ļ���  ��my_register
 * ����    ������
 * ����    ���⺯����
 * ����    ��Define register����ϵͳʹ�õļĴ�����ȫ�ֱ���
 * ע��		 ��ͷ�ļ���ֻ���������������ж���
 * �޸����ڣ�2014-12-9
*********************************************************************/

#include "STM32F4xx.h"
#ifndef __my_register_h__
#define __my_register_h__
//=============================================================================

//=============================================================================
#define NOP   __nop();	  //��ָ��

//============================================================================
extern struct bitDefine flagA,flagB,flagC,flagD,flagE,flagF,flagG;
#define   Flag_ADC_Full             (flagA.bit1)//DMA������ɱ�־λ 
#define   Flag_BEEP						    	(flagA.bit2)//������������־
#define   Flag_Key_Scan							(flagA.bit3)//������ʱɨ���־
#define   UART_SEND_flag						(flagA.bit4)//���ڷ�����ɱ�־
#define   UART_Buffer_Rece_flag			(flagA.bit5)//���ڽ�����ɱ�־
#define   Flag_Swtich_ON      			(flagA.bit6)//������������ON��־
#define   Flag_0_5S      		      	(flagA.bit7)//��ʾ���õ�ѹ��˸��־

#define   Flag_1S      		         	(flagB.bit0)//��ʾ���õ�����˸��־
#define   Flag_5S      		        	(flagB.bit1)//�˳�����״̬��־
#define   Flag_Long                 (flagB.bit2)//����������־
#define   Flag_DAC_OFF              (flagB.bit3)//是否转换DAC控制值标志
#define   Flag_SetA                 (flagB.bit5)//�������õ�����־λ
#define   Flag_SetV                 (flagB.bit6)//�������õ�ѹ��־λ 
#define   Flag_DisSet               (flagB.bit7)//��ʾ���õ�ѹ�͵�����־λ

#define   Flag_GE                   (flagC.bit0)//��λ��˸��־
#define   Flag_SHI                  (flagC.bit1)//ʮλ��˸��־
#define   Flag_BAI                  (flagC.bit2)//��λ��˸��־
#define   Flag_QIAN                 (flagC.bit3)//ǧλ��˸��־
#define   Flag_Beep_OFF             (flagC.bit6)//�������رձ�־

#define   flag_Tim_USART            (flagD.bit0)//���ڼ��������־
#define   flag_Test                 (flagD.bit1)//�Զ�������ͣ��־
#define   flag_Test_PASS            (flagD.bit2)//�Զ�����PASS��־
#define   flag_Test_FAIL            (flagD.bit3)//�Զ�����FAIL��־
#define   flag_Test_Time            (flagD.bit4)//�Զ����Եȴ�ʱ���־
#define   flag_Test_Start           (flagD.bit5)//�Զ����ԱȽϿ�ʼ��־
#define   flag_Test_Min             (flagD.bit6)//�Զ��������ޱ�־
#define   flag_Load_CC              (flagD.bit7)//����CC/CVģʽ��־λ

#define   flag_Test_Door             (flagE.bit0)//�Զ������ż���־
#define   flag_Test_SetTime          (flagE.bit1)//�Զ�����ʱ���־ 
#define   flag_OVP                   (flagE.bit2)//OVP��־
#define   flag_OCP                   (flagE.bit3)//OCP��־
#define   flag_Doub                  (flagE.bit4)//����˫��
#define   flag_t_Doub                (flagE.bit5)//����˫����ʱ��־
#define   flag_Time_SW               (flagE.bit6)//��ʱ���ܿ�����־
#define   flag_CC_MODE               (flagE.bit7)//����ģʽ��־

#define   flag_Adjust                (flagF.bit0)//
#define   flag_ADJ_ON                (flagF.bit1)//
#define   flag_ADJ_VL                (flagF.bit2)//
#define   flag_ADJ_VH                (flagF.bit3)//
#define   flag_ADJ_ALCC              (flagF.bit4)//
#define   flag_ADJ_AHCC              (flagF.bit5)//
#define   flag_ADJ_ALCV              (flagF.bit6)//
#define   flag_ADJ_AHCV              (flagF.bit7)//

#define   flag_DisVL                 (flagG.bit0)//
#define   flag_OverV                 (flagG.bit1)//
#define   flag_OverACC               (flagG.bit2)//
#define   flag_OverACV               (flagG.bit3)//
#define   flag_Just_Usart            (flagG.bit4)//
#define   flag_FAN_ON                (flagG.bit5)//
#define   flag_ADJ_FAN               (flagG.bit6)//
#define   flag_TM7707_CH						 (flagG.bit7)//
//=============================================================================
extern vu32 GE_V;
extern vu32 SHI_V;
extern vu32 BAI_V;
extern vu32 QIAN_V;  //�����ɨ�����
extern vu32 GE_A;
extern vu32 SHI_A;
extern vu32 BAI_A;
extern vu32 QIAN_A;
//--------------------------
extern vu8 NewKey;
extern vu8 DAC_Flag;
extern vu8 Beep_Flag;
extern vu8 t_beep;
extern vu8 t_KeyScan;
extern vu8 t_LEDScan;  //��ʱ������
extern vu8 t_KEYON;
extern vu16 t_0_5S;
extern vu16 t_1S;
extern vu16 t_5S;
extern vu16 t_Basket;
extern vu16 Basket_V;
extern vu16 Basket_A;
extern vu16 t_OFF_V;
extern vu16 t_ON_V;
extern vu16 t_OFF_A;
extern vu16 t_ON_A;
extern vu8 t_USART;
extern vu16 ADJ_Write;//У׼ʱ��д���ʵ��ֵ
extern vu8 OFFSET_Zore;//���ֵ
extern vu8 ADDR;
extern vu8 t_lock;
extern vu8 t_onoff;
extern vu16 MAX_limit;
extern vu16 MIN_limit;
extern vu16 Test_Time;
extern vu16 Test_Time1;
extern vu16 Time_sw;
extern vu16 Test_Daley;
extern vu16 Test_C_Door;
extern vu16 TEMP_OFFSET;//�¶Ȳ���ֵ
//--------------------------
extern vu8 UART_Buffer_Size;//���ڽ������ݳ���
extern vu8 Transmit_BUFFERsize;
//---------------------------
extern vu16 NTC_value;   //ADֵ
extern vu16 Imon_value;
extern vu32 Imon1_value;
extern vu16 Vmon_value;
extern vu32 Vmon1_value;
extern vu16 Rmon_value;

extern float DISS_Voltage;
extern  float DISS_POW_Voltage;
extern float DISS_Current;
extern float DISS_POW_Current;
extern float DISS_R;

extern vu32 Run_Control[14];	
#define onoff_ch                    Run_Control[0]  //ON/OFF
#define SET_Voltage 	    	        Run_Control[1]  //��Դ�趨��ѹ
#define SET_Current	    	    		  Run_Control[2]  //��Դ�趨����
#define SET_Voltage_Laod 					  Run_Control[3]  //�����趨��ѹ
#define SET_Current_Laod					  Run_Control[4]  //�����趨����
#define LOCK_KEY								    Run_Control[5]  //
#define BEEP_KEY								    Run_Control[6]  //
#define STRONG_KEY							    Run_Control[7]  //

#define MODE_Woke							   	  Run_Control[8]  //
#define POW_Voltage	    	          Run_Control[9]  //��Դ������ѹ
#define Voltage	    	              Run_Control[10]  //���ز�����ѹ
#define Current	    	    		      Run_Control[11]  //��Դ��������
#define Laod_Current	    		      Run_Control[12]  //���ز�������
#define R_VLUE											Run_Control[13]  //����ֵ


extern vu16 Contr_Voltage;//
extern vu16 Contr_Current;//
extern vu16 Contr_Laod;//
extern vu32 Correct_Parametet[11];
#define  REG_CorrectionV   Correct_Parametet[0]//�����ѹ��������
#define  REG_Load_A			   Correct_Parametet[1]//����CCģʽ������������
#define  SET_LoadA			   Correct_Parametet[2]//����CCģʽ���Ƶ�������
#define  REG_LoadV			   Correct_Parametet[3]//����CVģʽ���Ƶ�ѹ����
#define  SET_LoadV			   Correct_Parametet[4]//����CVģʽ���Ƶ�ѹ���Ʋ���

#define  REG_CorrectionR   Correct_Parametet[5]//�����������

#define  REG_POWERA			   Correct_Parametet[6]//��ԴCCģʽ������������
#define  SET_POWERA			   Correct_Parametet[7]//��ԴCCģʽ�������Ʋ�������
#define  REG_POWERV			   Correct_Parametet[8]//��ԴCVģʽ��ѹ��������
#define  SET_POWERV				 Correct_Parametet[9]//��ԴCVģʽ��ѹ���Ʋ���
#define  CON_POWERA				 Correct_Parametet[10]//��ԴCVģʽ������������



extern vu8  correct_por[6];
#define  Polar             correct_por[0]//�����ѹ��������
#define  Polar1            correct_por[1]//���ص��������Ϳ���
#define  Polar2						 correct_por[2]//����CVģʽ��ѹ�����Ϳ���
#define  Polar3						 correct_por[3]//��������͵�ԴCVģʽ��������
#define  Polar4						 correct_por[4]//��ԴCCģʽ���������Ϳ���
#define  Polar5						 correct_por[5]//��ԴCVģʽ��ѹ�����Ϳ���

extern vu32 Correct_Strong[11];//У׼ϵ��
#define  REG_ReadV_Offset   Correct_Strong[0]//�����ѹ��������
#define  REG_LoadA_Offset   Correct_Strong[1]//����CCģʽ������������
#define  SET_LoadA_Offset   Correct_Strong[2]//����CCģʽ���Ƶ�������
#define  REG_LoadV_Offset   Correct_Strong[3]//����CVģʽ���Ƶ�ѹ����
#define  SET_LoadV_Offset   Correct_Strong[4]//����CVģʽ���Ƶ�ѹ���Ʋ���

#define  REG_ReadR_Offset   Correct_Strong[5]//�����������

#define  REG_POWERA_Offset  Correct_Strong[6]//��ԴCCģʽ������������
#define  SET_POWERA_Offset  Correct_Strong[7]//��ԴCCģʽ�������Ʋ�������
#define  REG_POWERV_Offset  Correct_Strong[8]//��ԴCVģʽ��ѹ��������
#define  SET_POWERV_Offset	Correct_Strong[9]//��ԴCVģʽ��ѹ���Ʋ���
#define  CON_POWERA_Offset	Correct_Strong[10]//��ԴCVģʽ������������

//---------------------------
extern vu16 ADC_NTC_Filt[50];
extern vu16 ADC_Vmon_Filt[50];
extern vu16 ADC_Imon_Filt[50];
extern vu8 UART_Buffer_Rece[16];
extern vu8 UART_Buffer_Send[20];
//============================================================================= 
#define Receive_BUFFERSIZE   10
//=============================================================================
#endif
/******************* (C) COPYRIGHT 2015 KUNKIN *****END OF FILE*************************/