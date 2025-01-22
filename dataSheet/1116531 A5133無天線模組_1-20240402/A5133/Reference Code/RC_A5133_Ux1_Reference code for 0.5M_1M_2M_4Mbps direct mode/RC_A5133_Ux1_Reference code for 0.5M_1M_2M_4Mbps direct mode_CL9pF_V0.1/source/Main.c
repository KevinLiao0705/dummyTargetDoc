/*********************************************************************                                                                   **
**  Device:		A5133
**  File:			main.c
**  Author:		JPH
**  Target:		A8xxx
**  Tools:		ICE
**  Created:	
**  Description:
**  This file is a sample code for your reference.
**
**  Copyright (C) 2007 AMICCOM Electronics Corp.
**
**  Revision history:
**
*********************************************************************/
#include "define.h"
#include "A8105reg.h"
#include "A5133reg.h"
#include "A5133Config.h"
#include "Uti.h"

/*********************************************************************
**  I/O Declaration
*********************************************************************/
#define SCS       P0_3		//spi SCS
#define SCK      	P0_0		//spi SCK
#define SDIO      P0_1		//spi SDIO
#define CKO       P1_3		//CKO
#define GIO1      P0_2		//GPIO1
#define GIO2    	P1_2		//GPIO2
#define Button    P3_2		//test Button

/*********************************************************************
**  Constant Declaration
*********************************************************************/
#define TIMEOUT     	   50
#define t0hrel      	   1333//1000
#define BaudRate38400   (65536-((16000000)/(32*38400)))

/************************************************************************
**  Global Variable Declaration
************************************************************************/
Uint8		data		 timer;
Uint16  data     TimerCnt;
Uint8		data     Slot_TX;
Uint8		data 	   Flag_Timeout;
Uint16	idata 	 RxCnt;
Uint8   data     *Uartptr;
Uint8		data	   UartSendCnt;
Uint8		data	   CmdBuf[25];
Uint8		idata	   tmpbuf[80];
Uint16	idata 	 Err_Loss;
Uint16  idata    Err_Frame;
Uint32	idata	   Err_BitCnt;
Uint8		data	   Flag_Report;
Uint8   idata    Flag_FirstLink;
Uint8   idata    Flag_MASTER;
Uint8   idata    Mem_RH;
Uint8   idata    Mem_RL;

const Uint8 code BitCount_Tab[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
const Uint8 code ID_Tab[8]={0xC3,0x3C,0x95,0x6A,0x36,0x75,0xC5,0x2A};//ID code
//const Uint8 //code ID_Tab[8]={0x34,0x75,0xC5,0x2A,0xC7,0x33,0x45,0xEA};//ID code
//const Uint8 code ID_Tab[8]={0x55,0x55,0x55,0x55,0x34,0x75,0xC5,0x6A}; //ID2 code
//const Uint8 code ID_Tab[8]={0xFF,0xFF,0xFF,0xFF,0x36,0x75,0xC5,0xBA}; //ID2 code
//const Uint8 code ID_Tab[8]={0x34,0x75,0xC5,0x2A,0x34,0x75,0xC5,0x2A}; //ID2 code
const Uint8 code KeyData_Tab[16]={0x00,0x00,0x00,0x00,0x00,0x0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //keyData code
const Uint8 code FCB_Tab[20]={0x00,0x00,0x00,0x00,0x00,0x15,0x20,0x25,0x30,0x35,0x40,0x45,0x50,0x55,0x60,0x65,0x70,0x75,0x80,0x85}; //keyData code
const Uint8 code PN9_Tab[]=
{   0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0,
};

/*********************************************************************
**  function Declaration
*********************************************************************/
void MCU_Init(void);
void Timer0_Init(void);
void ISR_Timer0 (void);
void Uart0_Init_WithTimer2(void);
void ISR_Uart0(void);
void Clr_timer(void);
void Err_State(void);
void RF_Reset(void);
void RF_WriteID(Uint8* ptr);
void RF_ReadID(Uint8* ptr);
Uint8 RF_ReadPage(Uint8 addr, Uint8 page);
void RF_WritePage(Uint8 addr, Uint8 wbyte, Uint8 page);
void RF_WriteReg(Uint8, Uint8);
Uint8 RF_ReadReg(Uint8);
void ByteSend(Uint8 src);
Uint8 ByteRead(void);
void RF_SetCH(Uint8);
Uint8 RF_Init(void);
void RF_FIFOWrite(Uint8);
void RF_StrobeCmd(Uint8);
void RxPacket(void);
Uint8 RF_Cal_CHGroup(Uint8 ch);
Uint8 RF_Cal(void);
void RF_Config(void);
void RF_FCB(void);
void RF_KeyData(void);
void RF_FIFOLength(Uint16 len);
void RF_TrimmedValue_Init(void);
Sint16 RF_RSSI_Read(void);
void RF_PM_SleepMode(void);
Uint8 RF_LVR_Check(void);
void RF_WOR_En(void);
void RF_AdvFIFO_Init(void);
Uint8 Is_Vpoak(void);
void main_prog_ACK(void);

/*********************************************************************
* main loop
*********************************************************************/
void main(void)
{
	MCU_Init();
	Timer0_Init();
	Uart0_Init_WithTimer2();
	Delay10ms(10);
	
	if(RF_Init())/* init RF */
		Err_State();
	
	RF_WriteReg(MODECTRL_REG, 0x60);/* Direct mode */
	RF_SetCH(80);/* ch=80, freq=5805.001MHz */
	/*	To set RF channel																																								 */
	/* 	RF base Freq : 5725.001MHz																																			 */
	/* 	Channel step : 1MHz																																							 */
	/* 	SetCH        : 80																																								 */
	/* 	RF Frequency = RF base Freq + (Channel step * SetCH) = 5725.001MHz + ( 1MHz * 80 ) = 5805.001MHz */

	if(P3_5)
		Flag_MASTER = 1;//master device
	else	
		Flag_MASTER = 0;//slave device
	
	if(Flag_MASTER) 
	{  
		RF_WriteReg(CKO_REG, 0x02);/* cko-dck */
		RF_WriteReg(GIO1_REG, 0x1D);/* gio1-TRXD */
		RF_WriteReg(GIO2_REG, 0x09);/* gio2-TMEO */
		
		while(1)
		{	
			RF_StrobeCmd(CMD_TX);//entry tx
								 
			/* data transmit procedure */
			while(~GIO2);/* waiting TX modulation start */
			//use GIO1 pin send data follow CKO pin...
			//...
			RF_StrobeCmd(CMD_STBY);//entry standby
			Delay10ms(5);
		}	
	}
	else
	{
		RF_WriteReg(CKO_REG, 0x02);/* cko-dck */
		RF_WriteReg(GIO1_REG, 0x1D);/* gio1-TRXD */
		RF_WriteReg(GIO2_REG, 0x05);/* gio2-Fsync */

		while(1)
		{	
			RF_StrobeCmd(CMD_RX);//entry rx

			/* data receive procedure */
			//After GPIO2-Fsync=1, use the CKO signal of CKO-dck pin to latch RXD with the rising edge of cko 
			//...
			RF_StrobeCmd(CMD_STBY);//entry standby
			 
			Delay10ms(3);	
		}
	}
}

/************************************************************************
**  MCU_Init
************************************************************************/
void MCU_Init(void)
{
	//initHW
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;

	P0OE = 0x7B;
	P1OE = 0xF3;
	P2OE = 0xFF;
	P3OE = 0xFF;

	P0PUN = ~0x00;
	P1PUN = ~0x00;
	P2PUN = ~0x00;
	P3PUN = ~0x00;
   
	P0WUN	= ~0x00;
	P1WUN	= ~0x00;
	P2WUN	= ~0x00;
	P3WUN	= ~0x00;
}

/************************************************************************
**  Timer0_Init
************************************************************************/
void Timer0_Init(void)
{
	TR0 = 0;
	TMOD =(TMOD & 0xF0)|0x01; //timer0 mode=1
	TH0 = (65536-t0hrel)>>8; // Reload Timer0 high byte,low byte
	TL0 = 65536-t0hrel;
	TF0 = 0; // Clear any pending Timer0 interrupts
	ET0 = 1; // Enable Timer0 interrupt
}

/************************************************************************
**  ISR_Timer0
************************************************************************/
void ISR_Timer0 (void) interrupt 1
{
	TF0 = 0; // Clear Timer0 interrupt
	TH0 = (65536-t0hrel)>>8; // Reload Timer0 high byte,low byte
	TL0 = 65536-t0hrel;

	timer++;
	if (timer == TIMEOUT)
		Flag_Timeout=1;

	if (timer==2)
		Slot_TX=1;

	TimerCnt++;
	if (TimerCnt >=500)
	{
		TimerCnt=0;
		CmdBuf[0]=0xF1;
		memcpy(&CmdBuf[1], &RxCnt, 2);
		memcpy(&CmdBuf[3], &Err_Loss, 2);
		memcpy(&CmdBuf[5], &Err_Frame, 2);
		memcpy(&CmdBuf[7], &Err_BitCnt, 4);

		UartSendCnt=12;
		Uartptr=&CmdBuf[0];
		SBUF=CmdBuf[0];
	}
}

/************************************************************************
**  Uart0_Init_WithTimer2
************************************************************************/
void Uart0_Init_WithTimer2(void)
{
	IOSEL |= 0x01;//enable BBIOS + UART
	PCON = 0x00;
	SCON=0x40;
	T2CON=0x30;
	RCAP2H = BaudRate38400 / 256;
	RCAP2L = BaudRate38400 % 256;
	TR2=1;
	REN = 1;
	ES=1;//enable uart0 interrupt
}

/************************************************************************
**  ISR_Uart0
************************************************************************/
void ISR_Uart0(void) interrupt 4 using 3
{
	if (TI==1)
	{
		TI=0;
		UartSendCnt--;
		if(UartSendCnt !=0)
		{
			 Uartptr++;
			 SBUF = *Uartptr;
		}
	}

	if (RI==1)
	{
		RI=0;
	}
}

/************************************************************************
**  Clr_timer
************************************************************************/
void Clr_timer(void)
{
	TR0 = 0;
	TH0 = (65536-t0hrel-5)>>8; // Reload Timer0 high byte,low byte
	TL0 = 65536-t0hrel-5;
	timer = 2;
	TR0 = 1;
}

/*********************************************************************
** Err_State
*********************************************************************/
void Err_State(void)
{
	//ERR display
	//Error Proc...
	//...
	while(1);
}
   
/************************************************************************
**  Reset_RF
************************************************************************/
void RF_Reset(void)
{
	RF_WriteReg(MODE_REG, 0x00); //reset RF chip
}

/************************************************************************
**  RF_WriteID
************************************************************************/
void RF_WriteID(Uint8* ptr)
{
	Uint8 i;

	SCS = 0;
	ByteSend(IDCODE_REG);
	for (i=0; i < 8; i++)
		ByteSend(*ptr++);
	SCS = 1;
}

/************************************************************************
**  RF_ReadID
************************************************************************/
void RF_ReadID(Uint8* ptr)
{
	Uint8 i;

	SCS=0;
	ByteSend(IDCODE_REG | 0x40);
	for (i=0; i<8; i++)
		*ptr++ = ByteRead();
	SCS=1;
}

/*********************************************************************
** RF_ReadPage
*********************************************************************/
Uint8 RF_ReadPage(Uint8 addr, Uint8 page)
{
	Uint8 tmp;

	RF_WriteReg(RFANALOG_REG, (A5133_RFConfigTab_Main[0x35]&0x0F) | page<<4);
	tmp = RF_ReadReg(addr);
	return tmp;
}

/*********************************************************************
** RF_WritePage
*********************************************************************/
void RF_WritePage(Uint8 addr, Uint8 wbyte, Uint8 page)
{
	RF_WriteReg(RFANALOG_REG, (A5133_RFConfigTab_Main[0x35]&0x0F) | page<<4);
	RF_WriteReg(addr, wbyte);
}

/************************************************************************
**  RF_WriteReg
************************************************************************/
void RF_WriteReg(Uint8 addr, Uint8 dataByte)
{
	SCS = 0;
	ByteSend(addr);//bit7 cmd=0, bit6 r/w=0
	ByteSend(dataByte);
	SCS = 1;
}

/************************************************************************
**  RF_ReadReg
************************************************************************/
Uint8 RF_ReadReg(Uint8 addr)
{
	Uint8 tmp;

	SCS = 0;
	ByteSend(addr | 0x40);//bit7 cmd=0,bit6 r/w=1
	tmp = ByteRead();
	SCS = 1;

	return tmp;
}

/************************************************************************
**  ByteSend
************************************************************************/
void ByteSend(Uint8 src)
{
	Uint8 i;

	for(i = 0; i < 8; i++)
	{
		if(src & 0x80)
			 SDIO = 1;
		else
			 SDIO = 0;

		_nop_();
		SCK = 1;
		_nop_();
		SCK = 0;
		src = src << 1;
	}
}

/************************************************************************
**  ByteRead
************************************************************************/
Uint8 ByteRead(void)
{
	Uint8 i,tmp;

	SDIO = 1; //sdio pull high
	P0OE = P0OE & (~0x02);
	for(i = 0; i < 8; i++)
	{
		if(SDIO)
			 tmp = (tmp << 1) | 0x01;
		else
			 tmp = tmp << 1;

		SCK = 1;
		_nop_();
		SCK = 0;
	}
	P0OE = P0OE | (0x02);
	return tmp;
}

/*********************************************************************
** RF_SetCH
*********************************************************************/
void RF_SetCH(Uint8 ch)
{
	RF_WriteReg(PLL1_REG, ch); //RF freq = RFbase + (CH_Step * ch)
}

/*********************************************************************
** initRF
*********************************************************************/
Uint8 RF_Init(void)
{
	Uint8 i,id[8];

	/* init io pin */
	SCS = 1;
	SCK = 0;
	SDIO = 1;
	CKO = 1;
	GIO1 = 1;
	GIO2 = 1;

	RF_Reset();/* reset RF chip */
	RF_WriteID(ID_Tab);/* write ID code */
	RF_ReadID(id);
	for (i=0; i<8; i++)
	{
		if (id[i] ^ ID_Tab[i])
			return 1;/* fail */
	}
		
	RF_Config();/* config A7157 chip */
  RF_TrimmedValue_Init();/* load trimming value */
	if(RF_Cal())/* rf calibration */
		return 1;

	return 0;
}

/*********************************************************************
** RF_FIFOWrite
*********************************************************************/
void RF_FIFOWrite(Uint8 len)
{
	Uint8 i;

	RF_StrobeCmd(CMD_TFR);//TX fifo pointer reset
	SCS=0;
	ByteSend(FIFO_REG);//send address 0x05, bit7 cmd=0, bit6 r/w=0
	for(i=0; i <len; i++)
		ByteSend(PN9_Tab[i]);
	SCS=1;	
}

/*********************************************************************
** RF_StrobeCmd
*********************************************************************/
void RF_StrobeCmd(Uint8 cmd)
{
	SCS = 0;
	ByteSend(cmd);
	SCS = 1;
}

/*********************************************************************
** RxPacket
*********************************************************************/
void RxPacket(void)
{
	Uint8 i,recv,tmp,err;

	RxCnt++;
	err=0;

	RF_StrobeCmd(CMD_RFR);//RX fifo pointer reset
	SCS=0;
	ByteSend(FIFO_REG | 0x40);//address 0x05, bit7 cmd=0, bit6 r/w=1
	for(i=0; i <64; i++)
	{
		recv = ByteRead();
		tmpbuf[i]=recv;
		if((recv ^ PN9_Tab[i])!=0)
		{
			tmp = recv ^ PN9_Tab[i];
			Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
			err=1;
		}
	}
	SCS=1;

	if (err)//packet error
		Err_Frame++;
}

/*********************************************************************
** RF_Cal_CHGroup
*********************************************************************/
Uint8 RF_Cal_CHGroup(Uint8 ch)
{
	Uint8 tmp;
	Uint8 vb,vbcf,vcb,vccf;
	Uint8 deva,adev;

	RF_WriteReg(PLL1_REG, ch);
	RF_WriteReg(CALIBRATION_REG, 0x1C);
	do{
			 tmp = RF_ReadReg(CALIBRATION_REG)&0x1C;
	}while (tmp);

	//for check
	tmp = RF_ReadReg(VCOCCAL_REG);
	vcb = tmp & 0x0F;
	vccf = (tmp>>4) & 0x01;

	tmp = RF_ReadReg(VCOCAL1_REG);
	vb = tmp & 0x0F;
	vbcf = (tmp >>4) & 0x01;

	tmp = RF_ReadReg(VCODEVCAL1_REG);
	deva = tmp;

	tmp = RF_ReadReg(VCODEVCAL2_REG);
	adev = tmp;

	if(vbcf)
		return 1;//error   

	return 0;
}

/*********************************************************************
** calibration
*********************************************************************/
Uint8 RF_Cal(void)
{
	Uint8 tmp;
	Uint8 rhc,rlc,fb,fbcf,fcd;

	RF_StrobeCmd(CMD_PLL); //calibration @PLL state
	RF_WriteReg(RFANALOG_REG, 0);

	//IF,RSSI,RC procedure
	RF_WriteReg(CALIBRATION_REG, 0x23);
	do{
			 tmp = RF_ReadReg(CALIBRATION_REG)&0x23;
	}while(tmp);	

	//calibration VBC,VDC procedure
	if(RF_Cal_CHGroup(20)) //calibrate channel group Bank I
		return 1;
	if(RF_Cal_CHGroup(60)) //calibrate channel group Bank II
		return 1;
	if(RF_Cal_CHGroup(100)) //calibrate channel group Bank III
		return 1;

	RF_StrobeCmd(CMD_STBY); //return to STBY state

	//for check
	tmp = RF_ReadReg(IFCAL1_REG);
	fb = tmp & 0x0F;
	fbcf = (tmp>>4) & 0x01;

	tmp = RF_ReadReg(IFCAL2_REG);
	fcd = tmp & 0x1F;

	rhc = RF_ReadReg(RXGAIN2_REG);
	rlc = RF_ReadReg(RXGAIN3_REG);
	Mem_RH = rhc;
	Mem_RL = rlc;

	if(fbcf)
		return 1;//error

	return 0;
}

/*********************************************************************
** RF_Config
*********************************************************************/
void RF_Config(void)
{
	Uint8 i;

	//0x00 mode register, for reset
	//0x05 fifo data register
	//0x06 id code register
	//0x36 key data, 16 bytes
	//0x3D FCB register,4 bytes
	//0x3F USID register, read only

  for (i=0x01; i<=0x04; i++)
    RF_WriteReg(i, A5133_RFConfigTab_Main[i]);

	for (i=0x07; i<=0x1F; i++)
		RF_WriteReg(i, A5133_RFConfigTab_Main[i]);

	for (i=0; i<=12; i++)//0x20 code1
		RF_WritePage(0x20, A5133_RFConfigTab_Addr0x20[i], i);

	for (i=0; i<=12; i++)//0x21 code2
		RF_WritePage(0x21, A5133_RFConfigTab_Addr0x21[i], i);

	for (i=0; i<=5; i++)//0x22 code3
		RF_WritePage(0x22, A5133_RFConfigTab_Addr0x22[i], i);

	for (i=0x23; i<=0x29; i++)
		RF_WriteReg(i, A5133_RFConfigTab_Main[i]);

	for (i=0; i<=12; i++)//0x2A DAS
		RF_WritePage(0x2A, A5133_RFConfigTab_Addr0x2A[i], i);

	for (i=0x2B; i<=0x35; i++)
		RF_WriteReg(i, A5133_RFConfigTab_Main[i]);

	RF_WriteReg(0x37, A5133_RFConfigTab_Main[0x37]);

	for (i=0; i<=11; i++)//0x38 ROM
		RF_WritePage(0x38, A5133_RFConfigTab_Addr0x38[i], i);

	for (i=0x39; i<=0x3C; i++)
		RF_WriteReg(i, A5133_RFConfigTab_Main[i]);

	RF_WriteReg(0x3E, A5133_RFConfigTab_Main[0x3E]);
}

/*********************************************************************
** RF_FCB
*********************************************************************/
void RF_FCB(void)
{
	Uint8 i;

	SCS = 0;
	ByteSend(FCB_REG);//address 0x3D, bit7 cmd=0, bit6 r/w=0
	for (i=0; i < 20; i++)
		ByteSend(FCB_Tab[i]);
	SCS = 1;
}

/*********************************************************************
** A7157_KeyData
*********************************************************************/
void RF_KeyData(void)
{
	Uint8 i;

	SCS = 0;
	ByteSend(KEYDATA_REG);//address 0x36, bit7 cmd=0, bit6 r/w=0
	for (i=0; i < 16; i++)
		ByteSend(KeyData_Tab[i]);
	SCS = 1;
}

/*********************************************************************
** RF_FIFOLength
*********************************************************************/
void RF_FIFOLength(Uint16 len)
{
	SCS = 0;
	ByteSend(FIFO1_REG);
	ByteSend(len & 0xFF);//low byte
  ByteSend(len>>8);//high byte
	SCS = 1;
}

/*********************************************************************
** RF_TrimmedValue_Init
*********************************************************************/
void RF_TrimmedValue_Init(void)
{
	Uint8 i;
	Uint8 trimValue[8];
	//Uint8 tmp_checksum;

	//trimValue[0]=FBG
	//trimValue[1]=CTR
	//trimValue[2]=BDC
	//trimValue[3]=STM
	//trimValue[4]=Checksum for trimvalue[0]~trimvalue[3]
	//trimValue[5]=CSXTL
	//trimValue[6]=FBG_CP
	//trimValue[7]=Checksum for customer

	RF_WritePage(ROMP_REG, A5133_RFConfigTab_Addr0x38[9] | 0xA0, 9);//enable EFSW=1, EFRE=1
	
	Delay1ms(5);	//wait for stability
	
	SCS = 0;
	ByteSend(USID_REG | 0x40);
	for (i=0; i < 8; i++)
		trimValue[i] = ByteRead();
	SCS = 1;
	RF_WritePage(ROMP_REG, A5133_RFConfigTab_Addr0x38[9], 9);//disable EFSW=1, EFRE=1

	if((trimValue[0] + trimValue[1]) == trimValue[4]) //case1-only FT
	{
		if((trimValue[0]!=0) && (trimValue[1]!=0))
		{
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[1] & 0xE0) | trimValue[0], 1);//FBG
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[2] & 0xC0) | trimValue[1], 2);//CTR          
		}
		else
			Err_State();
   }
	else if((trimValue[0] + trimValue[1] + trimValue[2] + trimValue[3]) == trimValue[4]) //case2-CP+FT
	{
		if((trimValue[0]!=0) && (trimValue[1]!=0) && (trimValue[2]!=0) && (trimValue[3]!=0)) 
		{
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[1] & 0xE0) | trimValue[0], 1);//FBG
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[2] & 0xC0) | trimValue[1], 2);//CTR      
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[0] & 0x03) | (trimValue[2]<<2), 0);//BDC
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[4] & 0x40) | trimValue[3], 4);//STM
		}
		else
			Err_State();
	}
	else //only CP
	{
		if((trimValue[0]==0) && (trimValue[1]!=0) && (trimValue[2]!=0) && (trimValue[3]!=0) && (trimValue[4]==0) && (trimValue[6]!=0)) 
		{
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[1] & 0xE0) | trimValue[6], 1);//FBG
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[2] & 0xC0) | trimValue[1], 2);//CTR      
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[0] & 0x03) | (trimValue[2]<<2), 0);//BDC
			RF_WritePage(ROMP_REG, (A5133_RFConfigTab_Addr0x38[4] & 0x40) | trimValue[3], 4);//STM
		}
		else
			Err_State();     
	}
}
  
/*********************************************************************
** RF_RSSI_Read
*********************************************************************/
Sint16 RF_RSSI_Read(void)
{
	Uint8 rssi;
	Sint16 tmp;

	rssi= RF_ReadReg(RSSI_REG);	//read RSSI value(wanted signal RSSI)

	//RF power in(dBm) = (RSSI - RL) / (RH - RL)  * 12  -  80dbm - 3
	tmp = ((rssi*1.0 - Mem_RL) / (Mem_RH - Mem_RL) * 12.0) - 80 - 3; 
	return tmp;
}

/*********************************************************************
** RF_PM_SleepMode
*********************************************************************/
void RF_PM_SleepMode(void)
{
	RF_StrobeCmd(CMD_SLEEP);
	SCK = 1;
	SDIO = 1;
}

/*********************************************************************
** RF_Low Voltage Reset_Check
*********************************************************************/
Uint8 RF_LVR_Check(void)
{
	Uint8 tmp;

	tmp = RF_ReadReg(VCOCAL2_REG);
	if(tmp == 0xFF)//default reset value 0xFF
		return 1;
	else
		return 0;
}

/*********************************************************************
** RF_WOR_En
*********************************************************************/
void RF_WOR_En(void)
{
	RF_WriteReg(GIO1_REG, 0x01);//WTR
	RF_WriteReg(GIO2_REG, 0x11);//MCU wakeup signal
	RF_WriteReg(CKO_REG, 0x02);   

	RF_WriteReg(RCOSC1_REG, 0x0B);//WOR_SL=11, SL time=(11+1)*32*(1/4KHz)
	RF_WriteReg(RCOSC2_REG, 0x0B);//WOR_AC=11, AC time(wakeup time + RX time)=(11+1)*(1/4KHz)
	RF_WriteReg(RCOSC3_REG, 0x0C);//RCOSC enable

	while(1)
	{ 
		RF_WriteReg(MODECTRL_REG, A5133_RFConfigTab_Main[1] | (1<<3));	//WOR enable
		while(~GIO2);//waitting to wakeup until packet is received 
		
		RF_StrobeCmd(CMD_PLL);
		//Wakeup process...
		//RxPacket();
		//...;
	}
}

/*********************************************************************
** RF_AdvFIFO_Init
*********************************************************************/
void RF_AdvFIFO_Init(void)
{
	RF_WriteReg(FCR_REG, 0x0F);/* EARTS=1, EACKS=1, ARC=3, FCL=0 */
	RF_WriteReg(ARD_REG, 0x02);/* ARD=2, ARD=600us */	
	RF_WriteReg(AFEP_REG, 0x40);/* spss=1 */
	RF_WriteReg(KEYC_REG, 0x20);/* ARTMS=1, fixed interval */
	RF_WriteReg(USID_REG, 0x00);/* Random seed=0, fixed interval=200us */
}

/*********************************************************************
** Is_Vpoak
*********************************************************************/
Uint8 Is_Vpoak(void)
{
	Uint8 tmp;
	
	tmp = (RF_ReadReg(FCR_REG) >>6) & 0x01;/* read vpoak bit */
	return tmp;
}

/*********************************************************************
** main_prog_ACK
*********************************************************************/
void main_prog_ACK(void)
{
	/* A sample code for auto-resend & auto-ack function */

	RF_AdvFIFO_Init();/* init advance fifo with auto-resend & auto-ack */
	
	if (Flag_MASTER) 
	{  
		while(1)
		{
			RF_WriteReg(GIO1_REG,0x01);/* gio1-wtr */
			RF_WriteReg(GIO2_REG,0x1D);/* gio2-trxd */
			RF_WriteReg(CKO_REG,0x02);/* cko-dck */
			
			RF_FIFOLength(64-1);/* fifo length 64 bytes */ 
			RF_StrobeCmd(CMD_PLL);
			RF_SetCH(80);/* ch=80, freq=5805.001MHz */
			/*	To set RF channel																																								 */
			/* 	RF base Freq : 5725.001MHz																																			 */
			/* 	Channel step : 1MHz																																							 */
			/* 	SetCH        : 80																																								 */
			/* 	RF Frequency = RF base Freq + (Channel step * SetCH) = 5725.001MHz + ( 1MHz * 80 ) = 5805.001MHz */
   
			RxCnt = 0;
			Err_Loss=0;
			Err_Frame=0;
			Err_BitCnt = 0;
			timer=0;
			TR0=1;
			EA=1; 
		
			while(1)
			{	
				RF_FIFOWrite(64); /* write data to tx fifo */
				if(RF_LVR_Check())/* check if BOD reset has occurred */
					break;/* exit loop & re-RF init */
				else
					RF_StrobeCmd(CMD_TX);/* transmit packet */
				
				Delay10us(1);
				while(GIO1);

				/* check vpoak */
				if(Is_Vpoak())
					P3 = P3 ^ 0x04;/* transmit completed, toggle I/O */
				else
					_nop_();/* resend fail */
						
				Delay10ms(5);
			}
    }
	}
	else
	{ 
		while(1)
		{
			RF_WriteReg(GIO1_REG,0x01);/* gio1-wtr */
			RF_WriteReg(GIO2_REG,0x1D);/* gio2-trxd */
			RF_WriteReg(CKO_REG,0x02);/* cko-dck */

			RF_FIFOLength(64-1);/* fifo length 64 bytes */ 
			RF_StrobeCmd(CMD_PLL);
			RF_SetCH(80);/* ch=80, freq=5805.001MHz */
                  
			RxCnt = 0;
			Err_Loss=0;
			Err_Frame=0;
			Err_BitCnt = 0;
			timer=0;
			TR0=1;
			EA=1;

			while(1)
			{
				if(RF_LVR_Check())/* check if BOD reset has occurred */
					break;/* exit loop & re-RF init */
				else
					RF_StrobeCmd(CMD_RX);
            
				Delay10us(1);
				while(GIO1);
				
				/* check vpoak */
				if(Is_Vpoak())
				{
					P3 = P3 ^ 0x04;/* valid packet, toggle I/O */
					RxPacket();
				}
				else
					_nop_();/* invalid packet */

				Delay10ms(4);
      }
    }
  }	
}
