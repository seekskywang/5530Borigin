#include "TM7707.h"
#include "GPIO.h"
#include "my_register.h" 
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"

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
vu8 tm_adc_vol,tm_adc_vol2,tm_adc_vol3;
// Function name redefine.
#define TM7707_ReadPin()			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)

// Device operation Macro definition.
#define TM7707_CS_ENABLE()			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
#define TM7707_CS_DISABLE()			GPIO_SetBits(GPIOB,GPIO_Pin_6);
#define TM7707_ST_RESET()				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
#define TM7707_ST_UNRESET()			GPIO_SetBits(GPIOB,GPIO_Pin_7);

#define SCK_0()		GPIO_ResetBits(GPIOB, GPIO_Pin_3)
#define SCK_1()		GPIO_SetBits(GPIOB, GPIO_Pin_3)

#define DI_0()		GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define DI_1()		GPIO_SetBits(GPIOB, GPIO_Pin_5)

#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == Bit_SET)

enum
{
	TM7707_Pin_SPI =	0,
	TM7707_Pin_CS,
	TM7707_Pin_DRDY,
	TM7707_Pin_RESET,
	
	TM7707_Pin_Max,
};

//Simple delay function
void TM7707_Delay(vu16 nCount)
{
	vu16	i;
	for(i=0; i<nCount; i++)
	{
		NOP;
	}
}


void TM7707_InitializeSPI2(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	TM7707_CS_DISABLE();	;//CS1
  /*!< SD_SPI_CS_GPIO, SD_SPI_MOSI_GPIO, SD_SPI_MISO_GPIO, SD_SPI_DETECT_GPIO 
       and SD_SPI_SCK_GPIO Periph clock enable 
	// 	 RCC_AHBPeriphClockCmd(FLASH_CS_PIN_SCK|FLASH_SCK_PIN_SCK|FLASH_MISO_PIN_SCK | FLASH_MOSI_PIN_SCK, ENABLE);
  !< SD_SPI Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
  /*!< Configure SD_SPI pins: SPI1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI3); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI3); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI3); //PB5����Ϊ SPI1

	//����ֻ���SPI�ڳ�ʼ��
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����*/
		 
}
vu32 TM7707_ReadADCValue(vu8 uiChannelIndex)
{
	vu16 i;
	vu32 rtnADValue = 0;

	// Write channel index.
	// Wait for the conversion to complete
	if(uiChannelIndex==CH_1)//����һͨ��
	{
		
		for(i=0; i<100; i++)
		{
			while(TM7707_ReadPin() == 0)
			{
				TM7707_TransferByte(0x38);
				rtnADValue|= TM7707_TransferByte(0xFF);
				rtnADValue <<= 8;
				rtnADValue|= TM7707_TransferByte(0xFF);
				rtnADValue <<= 8;
				rtnADValue|= TM7707_TransferByte(0xFF);
				Imon1_value=rtnADValue>>6;//���ص���
				flag_TM7707_CH =0;
			}
		}
	}
	else if(uiChannelIndex==CH_2)//���ڶ�ͨ��
	{
		for(i=0; i<100; i++)
		{
			if(TM7707_ReadPin() == 0)
			{
				TM7707_TransferByte(0x39);
				rtnADValue |= TM7707_TransferByte(0xFF);
				rtnADValue <<= 8;
				rtnADValue |= TM7707_TransferByte(0xFF);
				rtnADValue <<= 8;
				rtnADValue |= TM7707_TransferByte(0xFF);
				Vmon1_value=rtnADValue>>6;//���ص�ѹ
				flag_TM7707_CH =1;
			}
		}
	}
}

/************************************************************************/
/* Function Name:	TM7707_TransferByte.								*/
/* Purpose:			Send of recive a byte.								*/
/* Resources:		SPI controler.										*/
/* Params:																*/
/*	@uiTxData:			The byte data will be sent.						*/
/* Return:			The byte data recived.								*/
/* Limitation:		None.												*/
/************************************************************************/
vu8 TM7707_TransferByte(vu8 uiTxData)
{
	vu8 uiRxData;
	TM7707_CS_ENABLE();		// CS = 1;
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI3,uiTxData);
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	TM7707_CS_DISABLE();	// CS = 0;
	uiRxData = SPI_I2S_ReceiveData(SPI3);
	return uiRxData;
}

/************************************************************************/
/* Function Name:	TM7707_SynchronizeDeviceClock.						*/
/* Purpose:			Synchronize TM7707 device clock, keep SCLK pin high	*/
/*					at least 32 cycles.									*/
/* Resources:		SPI controler and GPIOs.							*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void TM7707_SynchronizeDeviceClock(void)
{
	vu8 i;
	// Keep SCLK pin high at 40 cycles.
	for(i=0; i<5; i++)
	{
		TM7707_TransferByte(0xFF);
	}
}

/************************************************************************/
/* Function Name:	TM7707_WaitForDRDY.									*/
/* Purpose:			Wait for device DRDY signal.						*/
/* Resources:		DRDY GPIO pin.										*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void TM7707_WaitForDRDY(void)
{
		/*vu16 i;
	for(i=0; i<100; i++)
	{
		if(TM7707_ReadPin() == 0)
			break;
	}*/
while(TM7707_ReadPin() != 0);

}

/************************************************************************/
/* Function Name:	TM7707_SelfCalibration.								*/
/* Purpose:			Internal channel self-calibration.					*/
/* Resources:		SPI controler and GPIOs.							*/
/* Params:																*/
/*	@uiChannelIndex:	Channel index.									*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void TM7707_SelfCalibration(vu8	uiChannelIndex, vu8 uiGain)
{
	uiChannelIndex = uiChannelIndex % CH_MAX;
	//Write communication register to set setup register.
	TM7707_TransferByte(REG_SETUP | WRITE | uiChannelIndex);
	//Write configuration register, start self-calibration.
	TM7707_TransferByte(MD_NORMAL | uiGain | UNIPOLAR | BUF_NO | FSYNC_0);
	// Wait for operation to complete(ablut 180ms).
	TM7707_WaitForDRDY();
}

/************************************************************************/
/* Function Name:	TM7707_Initialize.									*/
/* Purpose:			Initialize TM7707 device.							*/
/* Resources:		SPI controler and GPIO pins.						*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void TM7707_Initialize(void)
{
	// Initialize SPI controler.
	TM7707_InitializeSPI2();
	TM7707_Reset();
	// Synchronize device clock.
	TM7707_SynchronizeDeviceClock();
	TM7707_SelfCalibration(CH_1, GAIN_1);
  TM7707_SelfCalibration(CH_2, GAIN_1);
	// Configure the clock registers
	TM7707_TransferByte(REG_CLOCK | WRITE | CH_1); // Write communication register to set clook register.
	TM7707_TransferByte(CLKDIS_0 | CLK_4_9152M | FS_50HZ); // External clock, frequency is 4.9152 MHz and the sampling frequency is 500Hz.
	TM7707_TransferByte(REG_CLOCK | WRITE | CH_2); // Write communication register to set clook register.
	TM7707_TransferByte(CLKDIS_0 | CLK_4_9152M | FS_50HZ); // External clock, frequency is 4.9152 MHz and the sampling frequency is 500Hz.
}
void TM7707_Reset(void)
{
		TM7707_ST_RESET();
		TM7707_Delay(5000);
		TM7707_ST_UNRESET();
		TM7707_Delay(5000);
}