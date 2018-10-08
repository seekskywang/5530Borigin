/******************************************************************/
/* 名称TIM3 PWM                                                */
/* 效果：                                                        */
/* 内容：产生一个200HZ 正占空比：60.9% 负占空比：30.9%的PWM      */
/* 作者：李振                                                    */
/* 联系方式：QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "tim6.h"

vu8 resetflag;

/*****************************************************************/
/*****************************************************************/
void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* TIM3 的配置 ---------------------------------------------------
   TIM3 输入时钟(TIM3CLK) 设置为 APB2 时钟 (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock 为48 MHz */
  /* TIM16 时钟使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
  /* Time 定时基础设置*/
  TIM_TimeBaseStructure.TIM_Prescaler = 2;//时钟预分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time 定时设置为上升沿计算模式*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//开启定时器更新中断
	TIM_SetAutoreload(TIM6, 0xFF);//设置PWM分辨率
	TIM6_NVIC_Config();
  /* TIM3 计算器使能*/
  TIM_Cmd(TIM6, ENABLE);
}
/***********************************************************************/
static void TIM6_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //脵使艤 TIM3 时讚
    TIM_TimeBaseInitStructure.TIM_Period = arr; //財织讟装諛值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //吱时欠貣频
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //胁蕪輪私模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);// 脷缘始郫吱时欠 TIM3
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //脹諍循吱时欠 3 偌褌讗讖
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //吱时欠 3 讗讖
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //葊占詤袌芏 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //鞋应詤袌芏 3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);// 脺缘始郫 NVIC
    TIM_Cmd(TIM3,ENABLE); //脻使艤吱时欠 3
}
//吱时欠 3 讗讖鼐媳诏私
void TIM3_IRQHandler(void)
{
    static vu8 calert = 0;
    static vu16 resetcount;
    static vu8 read1963;
    static vu16 scancount;
    
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //缨远讗讖
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //去越讗讖要志位
//         if(page_sw != face_starter)
//         {
             if(resetflag == 1)
             {
                 if(resetcount == 1)
                 {
                     sLCD_Init();
                     sLCD_WR_REG(0xf1);
                     GUI_Init();
//                      if(page_sw == face_menu)
//                      {
//                          ResetPow();
//                      }else if(page_sw == face_cdc){
// //                         ResetCDC();
//                      }else if(page_sw == face_r){
//                          ResetR();
//                      }else if(page_sw == face_load){
//                          ResetLoad();
//                      }else if(page_sw == face_graph){
//                          ResetG();
//                      }else if(page_sw == face_set){
//                          ResetSET();
//                      }
//                     resdone = 1;
//                     resetflag = 0;
                     resetcount = 0;
                 }else{
                     resetcount++;
                 }                
             }
//         }
//         switch(page_sw)
//         {
//             case face_menu:
//             {
//                 if(pow_sw == pow_on)
//                 {
//                     bc_raw += DISS_POW_Current * 1000 * 1/3600;
//                 }else{
//                     bc_raw = 0;
//                 }
//             }break;
//             case face_cdc:
//             {
//                 if(mode_sw == mode_pow && cdc_sw == cdc_on)
//                 {
//                     bc_raw += DISS_POW_Current * 1000 * 1/3600;
//                 }else if(cdc_sw == cdc_off){
//                     bc_raw = 0;
//                 }
//             }break;
//             case face_load:
//             {
//                 if(load_sw == load_on)
//                 {
//                     if(alert_flag == 1)
//                     {
//                         calert ++;
//                         if(calert == 3)
//                         {
//                             t_onoff = 0;
//                             GPIO_SetBits(GPIOA,GPIO_Pin_15);//支負睾諛OFF
//                             mode_sw = 0;
//                             load_sw = load_off;
//                             calert = 0;                                
//                         }
//                     }
//                     bc_raw += DISS_Current * 1000 * 1/3600;
//                 }else{
//                     bc_raw = 0;
//                 }
//             }break;
//             case face_graph:
//             {
//                 if(mode_sw == mode_pow)
//                 {
//                     if(pow_sw == pow_on)
//                     {
//                         bc_raw += DISS_POW_Current * 1000 * 1/3600;
//                     }else if(mode_sw == mode_pow && cdc_sw == cdc_on)
//                     {
//                         bc_raw += DISS_POW_Current * 1000 * 1/3600;
//                     }
//                     else{
//                         bc_raw = 0;
//                     }
//                 }               
//                 
//                 if(mode_sw == mode_load)
//                 {
//                     if(load_sw == load_on)
//                     {
//                         bc_raw += DISS_Current * 1000 * 1/3600;
//                     }else{
//                         bc_raw = 0;
//                     }
//                 }
//             }break;
//             case face_r:
//             {
//                 if(oct_sw == oct_on)
//                 {
//                     if(alert_flag == 1)
//                     {
//                         calert ++;
//                         if(calert == 3)
//                         {
//                             ocstop = 1;
//                             calert = 0;
//                         }
//                     }
//                 }
//             }break;
//         }
//         GPIO_ResetBits(GPIOD,GPIO_Pin_12);
//         TM1650_SET_LED(0x48,0x71);
//         TM1650_SET_LED(0x68,0xF2);//PASS謫
     }    
    
}