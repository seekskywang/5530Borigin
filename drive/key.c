/******************** (C) COPYRIGHT 2015 AVER********************
 * 文件名  ：KEY.C
 * 作者    ：李振
 * 描述    ：3*2矩阵键盘扫描
 * 内容    ：扫描完成后出键值
 * 硬件连接：一路3*2矩阵键盘  一路编码器独立按键
 * 修改日期：2015-08-18
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
extern WM_HWIN load_wind;//璐熻浇鐣岄潰鍙ユ焺
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
vu8 t_key_step;//閿洏鎵弿璁℃暟鍣�
vu16 Key1,Key2,Key3,Key4,Key5,Key6,Key7,Key8;
vu32 Key9;
vu16 KeyCounter;
vu16 MAX_limit;//自动测试上限值
vu16 MIN_limit;//自动测试下限值
vu16 Test_Time;
vu16 Test_Time1;
vu16 Time_sw;//定时
vu16 Test_Daley;//自动测试判断等待时间，此时间用户可设置
vu16 Test_C_Door;//自动测试门槛电流
vu8 OFFSET_Zore;//零点值
vu32 Key_Scan(void);
void Lift_Move(void);
void Right_Move(void);
void Setvalue_Add(void);
/* 閿�煎畾涔� */
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

/* 瀹氫箟鎸夐敭鍔ㄤ綔 */
#define        KEY_NULL_VALUE             0X00    //无动作
#define        KEY_PRESS                  0X10    //单击
#define        KEY_LONG                   0X20    //长按
#define        KEY_CONTINUE               0X40    //连击
#define        KEY_UP                     0X80    //按键抬起
/*瀹氫箟鎸夐敭澶勭悊姝ラ*/
#define        KEY_INIT_STATE               0      //初态
#define        KEY_WOBBLE_STATE             1      //消抖
#define        KEY_PRESS_STATE              2      //触发单键
#define        KEY_LONG_STATE               3      //长按触发
#define        KEY_CONTINUE_STATE           4      //连续触发
#define        KEY_RELEASE_STATE            5      //恢复初态
/* 闀挎寜鏃堕棿 */
#define         KEY_LONG_PERIOD             50                /* 长按时间1S */
#define        KEY_CONTINUE_PERIOD          10                /* 双击时间500ms */

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
		if(KeyState == KEY_INIT_STATE)                    /*无按键按下*/
		{
			KeyState = KEY_WOBBLE_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_WOBBLE_STATE)                /* 消抖 */
		{
			KeyState = KEY_PRESS_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_PRESS_STATE)                /* 有按键按下，返回按键值 */
		{
			KeyValue = NewKey;
			KeyValueTemp = KeyValue;
			KeyState = KEY_CONTINUE_STATE;
			return KeyValue;
		}
		else if(KeyState == KEY_CONTINUE_STATE)           /*双击按键*/
		{
			KeyCounter ++;
			if(KeyCounter == KEY_CONTINUE_PERIOD)
			{
				 KeyCounter = 0;
				 KeyState = KEY_LONG_STATE;
			}
		}
		else if(KeyState == KEY_LONG_STATE)                        /* 长按按键 */
		{
			KeyCounter ++;
			if(KeyCounter == KEY_LONG_PERIOD)
			{
				KeyCounter = 0;
				Flag_Long=1; //长按触发标志位
			}
		}
	}
	else if(NewKey<=0x37)
	{
		KeyState = KEY_INIT_STATE;        /* 误触发，返回到初始状态 */
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
/****************************************************鐭寜鍔熻兘************************************************************/
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
                sLCD_WR_REG(0x01);
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
					flag_Load_CC=1;//CC模式
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);//CC
					
				}
				else if(t_MODE==1)
				{
					flag_Load_CC=0;//CV模式
					GPIO_SetBits(GPIOC,GPIO_Pin_12);//CV
				}
			}
			break;
			case KEY_TRIG://电子负载ON/OFF
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
			case KEY_Reset://稳压电源ON/OFF
			{
				static vu8 POW_t;
				POW_t++;
				if(POW_t>1)
				{
					POW_t=0;
				}
				if(POW_t==0)
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_1);//关闭电源输出
				}
				else if(POW_t==1)
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_1);//打开电源输出
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
