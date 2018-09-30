/******************** (C) COPYRIGHT 2015 AVER********************
 * �ļ���  ��KEY.C
 * ����    ������
 * ����    ��3*2�������ɨ��
 * ����    ��ɨ����ɺ����ֵ
 * Ӳ�����ӣ�һ·3*2�������  һ·��������������
 * �޸����ڣ�2015-08-18
********************************************************************/
#include "my_register.h"
#include "beep.h"
#include "flash.h"
#include "gpio.h"
#include "key.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include  "gui.h"
#include "MainTask.h"

extern WM_HWIN CreateR(void);
extern WM_HWIN CreateWindow2(void);
extern WM_HWIN CreateWindow(void);
extern WM_HWIN hWinWind;
extern WM_HWIN hWinR;
extern WM_HWIN load_wind;//负载界面句柄
//=================================================================
extern struct bitDefine
{
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
} flagA, flagB,flagC,flagD,flagE,flagF,flagG;
//====================================================================

vu8 t_MODE;

#define MAX_VOL  3200		
#define MAX_CUR  5100	
vu8 t_KeyScan;
vu8 NewKey;
vu8 t_wei;
vu8 t_beep;
vu8 t_Str;
vu8 t_lock;
vu8 t_onoff;
vu8 t_doub;
vu8 t_key_step;//键盘扫描计数器
vu16 Key1,Key2,Key3,Key4,Key5,Key6,Key7,Key8;
vu32 Key9;
vu16 KeyCounter;
vu16 MAX_limit;//�Զ���������ֵ
vu16 MIN_limit;//�Զ���������ֵ
vu16 Test_Time;
vu16 Test_Time1;
vu16 Time_sw;//��ʱ
vu16 Test_Daley;//�Զ������жϵȴ�ʱ�䣬��ʱ���û�������
vu16 Test_C_Door;//�Զ������ż�����
vu8 OFFSET_Zore;//���ֵ
vu32 Key_Scan(void);
void Lift_Move(void);
void Right_Move(void);
void Setvalue_Add(void);
/* 键值定义 */
#define        KEY_1                		 0X4E     //
#define        KEY_2                		 0X56     //
#define        KEY_3                		 0X5E     //
#define        KEY_4                		 0X57      //
#define        KEY_5                		 0X5F      //
#define        KEY_6                		 0X76      //
#define        KEY_7                		 0X67     //
#define        KEY_8                		 0X6F      //
#define        KEY_9                		 0X77     //
#define        KEY_0                		 0X5D      //
#define        KEY_dian              		 0X65     //
#define        KEY_ESC              		 0X6D     //

#define        ENTER                     0X4D    //
#define        PUSH_Lift                 0X45     //
#define        PUSH_Right                0X55      //
#define        PUSH_Up                   0X46      //
#define        PUSH_Down                 0X74      //
#define        PUSH_DISP                 0X47      //
#define        PUSH_SETUP                0X4F     //

#define        KEY_Lock                  0X75     //
#define        KEY_BIAS                  0X6E      //
#define        KEY_Reset                 0X66      //
#define        KEY_TRIG                  0X75      //

#define        KEY_Powe                  0X6C     //
#define        KEY_Face1                 0X64      //
#define        KEY_Face2                 0X5c      //
#define        KEY_Face3                 0X54      //
#define        KEY_Face4                 0X4c      //
#define        KEY_Face5                 0X44      //

/* 定义按键动作 */
#define        KEY_NULL_VALUE             0X00    //�޶���
#define        KEY_PRESS                  0X10    //����
#define        KEY_LONG                   0X20    //����
#define        KEY_CONTINUE               0X40    //����
#define        KEY_UP                     0X80    //����̧��
/*定义按键处理步骤*/
#define        KEY_INIT_STATE               0      //��̬
#define        KEY_WOBBLE_STATE             1      //����
#define        KEY_PRESS_STATE              2      //��������
#define        KEY_LONG_STATE               3      //��������
#define        KEY_CONTINUE_STATE           4      //��������
#define        KEY_RELEASE_STATE            5      //�ָ���̬
/* 长按时间 */
#define         KEY_LONG_PERIOD             50                /* ����ʱ��1S */
#define        KEY_CONTINUE_PERIOD          10                /* ˫��ʱ��500ms */

/************************************************************************************************************************/
vu32 Key_Scan(void)
{
	static vu8 KeyState = KEY_INIT_STATE;
	static vu8 IsKeyRelease = 1;

	static vu8 KeyValueTemp = 0;
	vu8 KeyValue ;
	KeyValue= KEY_NULL_VALUE;
	
	if((NewKey>0x37)&&(IsKeyRelease==1))
	{
		if(KeyState == KEY_INIT_STATE)                    /*�ް�������*/
		{
			KeyState = KEY_WOBBLE_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_WOBBLE_STATE)                /* ���� */
		{
			KeyState = KEY_PRESS_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_PRESS_STATE)                /* �а������£����ذ���ֵ */
		{
			KeyValue = NewKey;
			KeyValueTemp = KeyValue;
			KeyState = KEY_CONTINUE_STATE;
			return KeyValue;
		}
		else if(KeyState == KEY_CONTINUE_STATE)           /*˫������*/
		{
			KeyCounter ++;
			if(KeyCounter == KEY_CONTINUE_PERIOD)
			{
				 KeyCounter = 0;
				 KeyState = KEY_LONG_STATE;
			}
		}
		else if(KeyState == KEY_LONG_STATE)                        /* �������� */
		{
			KeyCounter ++;
			if(KeyCounter == KEY_LONG_PERIOD)
			{
				KeyCounter = 0;
				Flag_Long=1; //����������־λ
			}
		}
	}
	else if(NewKey<=0x37)
	{
		KeyState = KEY_INIT_STATE;        /* �󴥷������ص���ʼ״̬ */
		IsKeyRelease = 1;
		Flag_Long=0;
	}
	return KEY_NULL_VALUE;
}
/***************************************************************************************************************************/
void Key_Funtion(void)
{
	vu32 KeyValue = 0;
	static vu8 t_KEYON;
	static vu8 a,b;
/****************************************************短按功能************************************************************/
	if(Flag_Key_Scan==1)
	{
		Flag_Key_Scan = 0;
		KeyValue = Key_Scan();
		switch(KeyValue)
		{
			case KEY_1 ://
			{
				SET_Voltage=1000;
				SET_Current=4000;
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_2 ://
			{
				SET_Voltage=2000;
				SET_Current=5100;
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_3 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_4 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_5 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;	
			
			case KEY_6 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_7 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_8 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_9 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_0 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_dian ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case KEY_ESC :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			case PUSH_Lift :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
				SET_Voltage_Laod=SET_Voltage_Laod+1000;
			}
			break;
			case PUSH_Right :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
				SET_Voltage_Laod=SET_Voltage_Laod-1000;
			}
			break;
			case PUSH_Up :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
				SET_Current_Laod=SET_Current_Laod+1000;
			}
			break;
			case PUSH_Down :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
				SET_Current_Laod=SET_Current_Laod-1000;
			}
			break;
			
			case ENTER ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case PUSH_DISP :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case PUSH_SETUP :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
				t_MODE++;
				if(t_MODE>1)
				{
					t_MODE=0;
				}
				if(t_MODE==0)
				{
					flag_Load_CC=1;//CCģʽ
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CC
					
				}
				else if(t_MODE==1)
				{
					flag_Load_CC=0;//CVģʽ
					GPIO_SetBits(GPIOC,GPIO_Pin_12);//CV
				}
			}
			break;
			case KEY_TRIG://���Ӹ���ON/OFF
			{
				t_onoff++;
				if(t_onoff>1)
				{
					t_onoff=0;
				}
				if(t_onoff==0)
				{
					Flag_Swtich_ON=0;
					GPIO_SetBits(GPIOA,GPIO_Pin_15);//OFF
					
				}
				else if(t_onoff==1)
				{
					Flag_Swtich_ON=1;
					GPIO_ResetBits(GPIOA,GPIO_Pin_15);//On
				}
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Reset://��ѹ��ԴON/OFF
			{
				static vu8 POW_t;
				POW_t++;
				if(POW_t>1)
				{
					POW_t=0;
				}
				if(POW_t==0)
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_1);//�رյ�Դ���
				}
				else if(POW_t==1)
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_1);//�򿪵�Դ���
				}
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Powe :
			{
				
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Face1 :
			{
				WM_DeleteWindow(hWinWind);
				WM_DeleteWindow(hWinR);
				CreateR();
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Face2 :
			{
				WM_DeleteWindow(hWinR);
				WM_DeleteWindow(hWinWind);
				CreateWindow2();
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Face3 :
			{
				WM_DeleteWindow(hWinR);
				WM_DeleteWindow(hWinWind);
				CreateWindow();
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Face4 :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			case KEY_Face5 :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//
			}
			break;
			
			default:
			break;
		}	
	}
}
