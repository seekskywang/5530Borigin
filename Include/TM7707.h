/*======================================================================*/
/*= TM7707 Driver library.												*/
/*======================================================================*/
#ifndef _INCLUDE_TM7707_H_
#define _INCLUDE_TM7707_H_
#include "GPIO.h"
#include "my_register.h" 
#define TM7707_GPIO		GPIO_STRUCT

// Communication register
enum
{
	// Destination register
	REG_COMM		= 0x00,	// Communication register.
	REG_SETUP		= 0x10,	// Setting register.
	REG_CLOCK		= 0x20,	// Clock register.
	REG_DATA		= 0x30,	// Data register.
	REG_ZERO_CH1	= 0x60,	/* ƫ�ƼĴ���	*/
	REG_FULL_CH1	= 0x70,	/* �����̼Ĵ���	*/
	// Register 
	WRITE			= 0x00,	// Write register
	READ			= 0x08,	// Read register
	// Channel index
	CH_1			= 0x00,	/* AIN1+  AIN1-		*/
	CH_2			= 0x01,	/* AIN2+  AIN2-		*/
	CH_3			= 0x02,	/* AIN1-  AIN1-		*/
	CH_4			= 0x03,	/* AIN1-  AIN2-		*/
	CH_MAX			= 0x04,	// Cannot write to device register.
};

// Setting register
enum
{
	MD_NORMAL		= (0 << 6),	/* ����ģʽ		*/
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ	*/
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ	*/
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ*/

	GAIN_1			= (0 << 3),	/* ����			*/
	GAIN_2			= (1 << 3),	/* ����			*/
	GAIN_4			= (2 << 3),	/* ����			*/
	GAIN_8			= (3 << 3),	/* ����			*/
	GAIN_16			= (4 << 3),	/* ����			*/
	GAIN_32			= (5 << 3),	/* ����			*/
	GAIN_64			= (6 << 3),	/* ����			*/
	GAIN_128		= (7 << 3),	/* ����			*/

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1			/* ������ */
};

// Clock register define.
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};



// Function declare
void		TM7707_Reset		(void);
void		TM7707_InitializeSPI2(void);
void		TM7707_WaitForDRDY	(void);
vu8		TM7707_TransferByte	(vu8 uiTxData);
void		TM7707_SynchronizeDeviceClock(void);
void		TM7707_SelfCalibration(vu8	uiChannelIndex, vu8 uiGain);
void		TM7707_Initialize	(void);
vu32	TM7707_ReadADCValue(vu8 uiChannelIndex);
vu8 TM7705_Recive8Bit(void);
void TM7705_Send8Bit(vu8 _data);

#endif
