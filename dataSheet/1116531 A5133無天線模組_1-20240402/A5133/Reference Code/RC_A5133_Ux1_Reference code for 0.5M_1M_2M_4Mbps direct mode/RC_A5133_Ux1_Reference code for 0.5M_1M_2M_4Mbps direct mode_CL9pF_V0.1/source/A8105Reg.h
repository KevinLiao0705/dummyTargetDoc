/********************************************************************
*   A8105REG.h
*   RF Chip-A8105 Hardware Definitions
*
*   This file provides the constants associated with the
*   AMICCOM A8105 device.
*
********************************************************************/
#ifndef _A8105REG_h_
#define _A8105REG_h_

/************************************
* MCU SFR
************************************/
/*  BYTE Registers  */
sfr P0    = 0x80;
sfr P1    = 0x90;
sfr P2    = 0xA0;
sfr P3    = 0xB0;
sfr PSW   = 0xD0;
sfr ACC   = 0xE0;
sfr B     = 0xF0;
sfr SP    = 0x81;
sfr DPL   = 0x82;
sfr DPH   = 0x83;
sfr DPL1  = 0x84;
sfr DPH1  = 0x85;
sfr PCON  = 0x87;
sfr TCON  = 0x88;
sfr TMOD  = 0x89;
sfr TL0   = 0x8A;
sfr TL1   = 0x8B;
sfr TH0   = 0x8C;
sfr TH1   = 0x8D;
sfr CKCON = 0x8E;
sfr EIF   = 0x91;
sfr SCON  = 0x98;
sfr SBUF  = 0x99;
sfr FLASHCTRL = 0x9A;
sfr FLASHMR = 0x9B;
sfr IE    = 0xA8;
sfr PWM0CON = 0xA9;
sfr PWM0H   = 0xAA;
sfr PWM0L   = 0xAB;
sfr PWM1CON = 0xB1;
sfr PWM1H   = 0xB2;
sfr PWM1L   = 0xB3;
sfr IP      = 0xB8;
sfr PCONE  = 0xB9;
sfr RSFLAG = 0xBA;
sfr IOSEL  = 0xBB;
sfr T2CON  = 0xC8;
sfr T2IF   = 0xC9;
sfr RCAP2L   = 0xCA;
sfr RCAP2H   = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;
sfr WDCON  = 0xD8;
sfr EIE     = 0xE8; //External Interrupt Enable 
sfr SPCR    = 0xEC; 
sfr SPSR    = 0xED;  
sfr SPDR    = 0xEE; 
sfr SSCR    = 0xEF;  
sfr I2CSADR = 0xF1; //I2CS Address              
sfr I2CSCR  = 0xF2; //I2CS Control Register     
sfr I2CSBUF = 0xF3; //I2CS I/O Buffer           
sfr I2CMSA  = 0xF4; //I2CM Slave Address        
sfr I2CMCR  = 0xF5; //I2CM Control Register     
sfr I2CMBUF = 0xF6; //I2CM I/O Buffer           
sfr I2CMTP  = 0xF7; //I2CM Timer Period       
sfr EIP     = 0xF8; //External Interr. Priority

sfr P0OE    = 0xD1;
sfr P0PUN   = 0xD2;
sfr P0WUN   = 0xD3;
sfr P1OE    = 0xD9;
sfr P1PUN   = 0xDA;
sfr P1WUN   = 0xDB;
sfr P2OE    = 0xA1;
sfr P2PUN   = 0xA2;
sfr P2WUN   = 0xA3;
sfr P3OE    = 0xE1;
sfr P3PUN   = 0xE2;
sfr P3WUN   = 0xE3;

/*  BIT Registers  */
/*  PSW  */
sbit CY    = PSW^7;
sbit AC    = PSW^6;
sbit F0    = PSW^5;
sbit RS1   = PSW^4;
sbit RS0   = PSW^3;
sbit OV    = PSW^2;
sbit P     = PSW^0; //8052 only

/*  TCON  */
sbit TF1   = TCON^7;
sbit TR1   = TCON^6;
sbit TF0   = TCON^5;
sbit TR0   = TCON^4;
sbit IE1   = TCON^3;
sbit IT1   = TCON^2;
sbit IE0   = TCON^1;
sbit IT0   = TCON^0;

/*  IE  */
sbit EA    = IE^7;
sbit ET2   = IE^5; //8052 only
sbit ES    = IE^4;
sbit ET1   = IE^3;
sbit EX1   = IE^2;
sbit ET0   = IE^1;
sbit EX0   = IE^0;

/*  IP  */
sbit PT2   = IP^5;
sbit PS    = IP^4;
sbit PT1   = IP^3;
sbit PX1   = IP^2;
sbit PT0   = IP^1;
sbit PX0   = IP^0;

/*  P3  */
sbit RD    = P3^7;
sbit WR    = P3^6;
sbit T1    = P3^5;
sbit T0    = P3^4;
sbit INT1  = P3^3;
sbit INT0  = P3^2;
sbit TXD   = P3^1;
sbit RXD   = P3^0;

/*  SCON  */
sbit SM0   = SCON^7;
sbit SM1   = SCON^6;
sbit SM2   = SCON^5;
sbit REN   = SCON^4;
sbit TB8   = SCON^3;
sbit RB8   = SCON^2;
sbit TI    = SCON^1;
sbit RI    = SCON^0;

/*  P1  */
sbit T2EX  = P1^1; // 8052 only
sbit T2    = P1^0; // 8052 only

/*  T2CON  */
sbit TF2    = T2CON^7;
sbit EXF2   = T2CON^6;
sbit RCLK   = T2CON^5;
sbit TCLK   = T2CON^4;
sbit EXEN2  = T2CON^3;
sbit TR2    = T2CON^2;
sbit C_T2   = T2CON^1;
sbit CP_RL2 = T2CON^0;

/*------------------------------------------------------------------------
; BIT Register  
;-------------------------------------------------------------------------*/
//  P0  
sbit P0_7   = P0^7;
sbit P0_6   = P0^6;
sbit P0_5   = P0^5;
sbit P0_4   = P0^4;
sbit P0_3   = P0^3;
sbit P0_2   = P0^2;
sbit P0_1   = P0^1;
sbit P0_0   = P0^0;

// P1 
sbit P1_7   = P1^7;
sbit P1_6   = P1^6;
sbit P1_5   = P1^5;
sbit P1_4   = P1^4;
sbit P1_3   = P1^3;
sbit P1_2   = P1^2;
sbit P1_1   = P1^1;
sbit P1_0   = P1^0;

// P2
sbit P2_7   = P2^7;
sbit P2_6   = P2^6;
sbit P2_5   = P2^5;
sbit P2_4   = P2^4;
sbit P2_3   = P2^3;
sbit P2_2   = P2^2;
sbit P2_1   = P2^1;
sbit P2_0   = P2^0;

// P3
sbit P3_7   = P3^7;
sbit P3_6   = P3^6;
sbit P3_5   = P3^5;
sbit P3_4   = P3^4;
sbit P3_3   = P3^3;
sbit P3_2   = P3^2;
sbit P3_1   = P3^1;
sbit P3_0   = P3^0;

/************************************
* RF control register
************************************/
/*
#define MODE_REG 			0x0800
#define MODECTRL1_REG 		0x0801
#define MODECTRL2_REG 		0x0802
#define CALIBRATION_REG		0x0803
#define FIFO1_REG 			0x0804
#define FIFO2_REG 			0x0805
#define RCOSC1_REG 			0x0806
#define RCOSC2_REG 			0x0807
#define RCOSC3_REG 			0x0808
#define RCOSC4_REG 			0x0809
#define RCOSC5_REG 			0x080A
#define RCOSC6_REG 			0x080B
#define CKO_REG 			0x080C
#define GIO1_REG 			0x080D
#define GIO2_REG 			0x080E
#define CLOCK_REG 			0x080F
#define DATARATE_REG 		0x0810
#define PLL1_REG 			0x0811
#define PLL2_REG			0x0812
#define PLL3_REG 			0x0813
#define PLL4_REG			0x0814
#define PLL5_REG 			0x0815
#define TX1_REG  			0x0816
#define TX2_REG  			0x0817
#define DELAY1_REG			0x0818
#define DELAY2_REG			0x0819
#define RX_REG				0x081A
#define RXGAIN1_REG			0x081B
#define RXGAIN2_REG			0x081C
#define RXGAIN3_REG			0x081D
#define RXGAIN4_REG			0x081E
#define RSSI_REG			0x081F
#define ADC_REG  			0x0820
#define CODE1_REG 			0x0821
#define CODE2_REG 			0x0822
#define CODE3_REG 			0x0823
#define IFCAL1_REG  		0x0824
#define IFCAL2_REG  		0x0825
#define VCOCCAL_REG  		0x0826
#define VCOCAL1_REG  		0x0827
#define VCOCAL2_REG  		0x0828
#define BATTERY_REG  		0x0829
#define TXTEST_REG  		0x082A
#define RXDEM1_REG  		0x082B
#define RXDEM2_REG  		0x082C
#define CPC1_REG			0x082D
#define CRYSTALTEST_REG		0x082E
#define PLLTEST_REG   		0x082F
#define VCOTEST1_REG 		0x0830
#define VCOTEST2_REG 		0x0831
#define IFAT_REG 			0x0832
#define RFTEST1_REG			0x0833
#define RFTEST2_REG			0x0834
#define RFTEST3_REG			0x0835
#define RFTEST4_REG			0x0836
#define RFTEST5_REG			0x0837
#define CHINDEX_REG			0x0838
#define CRC1_REG			0x0839
#define CRC2_REG			0x083A
#define CRC3_REG			0x083B
#define CRC4_REG			0x083C
#define CRC5_REG			0x083D
#define CRC6_REG			0x083E
#define VCOCAL3_REG  		0x083F
#define VCODEVCAL1_REG      0x0840
#define VCODEVCAL2_REG      0x0841
#define VCODEVCAL3_REG      0x0842
#define ADCCTRL1_REG		0x0843
#define WOT_REG 			0x0844
#define CHGROUP1_REG		0x0845
#define CHGROUP2_REG		0x0846
#define CPC2_REG			0x0847
#define VCOMODDELAY_REG		0x0848
#define INTERNALCAP_REG		0x0849
#define RXDETECTION_REG		0x084A
#define ID0_REG 	 		0x084B
#define ID1_REG 	 		0x084C
#define ID2_REG 	 		0x084D
#define ID3_REG 	 		0x084E
#define DID0_REG 	 		0x084F
#define DID1_REG 	 		0x0850
#define DID2_REG 	 		0x0851
#define DID3_REG 	 		0x0852
#define EXT1_REG 	 		0x0853
#define EXT2_REG 	 		0x0854
#define EXT3_REG 	 		0x0855
#define EXT4_REG 	 		0x0856
#define ADCCTRL2_REG		0x0857
#define ADCVALUE1_REG		0x0858
#define ADCVALUE2_REG		0x0859
#define ADCVALUE3_REG		0x085A
#define TXFIFO_REG 			0x0900
#define RXFIFO_REG 			0x0980		
  
//strobe command
#define CMD_SLEEP		0x80	//1000,xxxx	SLEEP mode
#define CMD_IDLE		0x90	//1001,xxxx	IDLE mode
#define CMD_STBY		0xA0	//1010,xxxx Standby mode
#define CMD_PLL			0xB0	//1011,xxxx	PLL mode
#define CMD_RX			0xC0	//1100,xxxx	RX mode
#define CMD_TX			0xD0	//1101,xxxx	TX mode
 */

#endif