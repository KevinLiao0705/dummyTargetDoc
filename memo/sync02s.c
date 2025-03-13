#include <stdio.h>
#include <sys/unistd.h>
#include <io.h>
#include <string.h>
#include "system.h"
#include "altera_avalon_uart_regs.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "sys/alt_flash.h"
#include "A7130reg.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include <altera_avalon_spi_regs.h>
#include <altera_avalon_spi.h>
#include "syncadr.h"

#define rdata_lim_k 512
#define nios_ver_k 22





typedef alt_u8  U8;
typedef alt_u16 U16;
typedef alt_u32 U32;



typedef struct uart_rxtemp
{
	alt_u16 command ;
	alt_u8 data[64];
	alt_u16 len;
	alt_u16 pindex;
} UART_RXTEMP ;


const int InTimer1 = 25000;  //250 us


U8 test_422_cnt=0;


U8 HA_rf_en=1;
U8 HB_rf_en=1;
U8 swpab_cnt=0;


U8 pri_pcnt=0;

U8 swp_pus_cnt=0;
U8 bst_pus_cnt1=0;
U8 bst_pus_cnt0=0;

U8 TXrx_data4;
U8 TXrx_data5;
U8 TXrx_f;

U8 GPSrx_data[100];
U8 GPSrx_f;

U8 var_freq_mod_cnt=0;
U8 var_freq_mod_cnt0=0;


U16 urx_dcc_la_tim=0;
U16 urx_dcc_rf1_tim=0;
U16 urx_dcc_rf2_tim=0;
U16 urx_dcc_fb1_tim=0;
U16 urx_dcc_fb2_tim=0;
U16 urx_dcc_422_tim=0;
U16 urx_dcc_gps_tim=0;

U16 hc_pri0_buf=0;
U16 s1_pri0_buf=0;
U16 s2_pri0_buf=0;


U16 txreq_err_tim=65000;

/*****************************************************************************************/
static void UART_ISR_Init(void); //初始化串口
static void initINT(void);
U8 uart_rx_proc(char* rxbuf,U16 rx_cnt,UART_RXTEMP* urpack);
U16 asc2num(U8* cp);
U8 asc2numc(U8 cp);

U8 asc2nip(U8* cp,U8* retb);
U8 asc2byte(U8* cp,U8* retb);
U8 asc2word(U8* cp,U16* retb);
int uart_ui_send_byte(unsigned char data);
U8 uart_ui_send_pack(U8 force_tx);
static void initTIMER(void);
static void ISR_timer1(void *context, alt_u32 id);
static void INT_IrqA_Handler(void);
static void INT_IrqB_Handler(void);

void keybo_prg(void);

void uruitx_recedtx(void);
void loadui_uarttx_start(void);
void loadui_uarttx_end(void);
void loadui_uarttx_byte(unsigned char ch);
void initSPI(void);
void initRF(void);
static void UART_TX_Irq_Handler(void *context, alt_u32 id);
static void UART_GPS_Irq_Handler(void *context, alt_u32 id);
static void UART_DCC_Irq_Handler(void *context, alt_u32 id);
void loadtx_uarttx_cmd(U8 cmd);
void loadtx_uarttx_req(void);


void RF0_WriteReg(alt_u8 addr, alt_u8 dataByte);
void RF0_Reset(void);
alt_u8 ByteSend0(alt_u8 txdata);
alt_u8 ByteRead0(void);
U8 RF0_TESTID(void);
void RF0_Cal(void);
void CHGroupCal0(alt_u8 ch);
void Err_State(void);
void StrobeCmd0(alt_u8 cmd);
alt_u8 RF0_ReadReg(alt_u8 addr);
void RF0_Config(void);
void SetCH0(alt_u8 ch);

void RF1_WriteReg(alt_u8 addr, alt_u8 dataByte);
void RF1_Reset(void);
alt_u8 ByteSend1(alt_u8 txdata);
alt_u8 ByteRead1(void);
U8 RF1_TESTID(void);
void RF1_Cal(void);
void CHGroupCal1(alt_u8 ch);
void StrobeCmd1(alt_u8 cmd);
alt_u8 RF1_ReadReg(alt_u8 addr);
void RF1_Config(void);
void SetCH1(alt_u8 ch);
void led_prg(void);
void uart_dcc_proc(void);


void loadtx_uartdcc_la(void);
void loadtx_uartdcc_rf1(void);
void loadtx_uartdcc_rf2(void);
void loadtx_uartdcc_fb1(void);
void loadtx_uartdcc_fb2(void);
void loadtx_uartdcc_422(void);
void loadtx_uartdcc_gps(void);


UART_RXTEMP urui_pack;


static void UART_UI_Irq_Handler(void *context, alt_u32 id);




void dispatch_uirx(void);
void uruitx_reced(void);
void rwmem(void);



char hextoasc[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


const alt_u8 A7130Config[]=
{

	0x00, //RESET register,			only reset, not use on config
	0x60, //MODE register,
	0x00, //CALIBRATION register,
	0x3F, //FIFO1 register,
	0x00, //FIFO2 register,
	0x00, //FIFO register,			for fifo read/write
	0x00, //IDDATA register,		for idcode

	0x00, //RCOSC1 register,
	0x00, //RCOSC2 register,
	0x00, //RCOSC3 register,
//	0x02, //CKO register,
  	0x8a, //CKO register,kevin

//	0x1D, //GPIO1 register
	0x21, //GPIO1 register rxd kevin

//	0x05, //GPIO2 register,
	0x25, //0X0C GPIO2 register,//TXD kevin


	0x9F, //DATARATECLOCK register,

	0x00, //PLL1 register,
	0x0E, //PLL2 register, 			RFbase 2400.001MHz
//	0x9E, //0x0F PLL2 register,		kevin


	0x96, //PLL3 register,
//	0x4B, //0x10 PLL3 register,kevin


	0x00, //PLL4 register,
	0x04, //PLL5 register,'
//	0x02, //0x12 PLL5 register,kevin

	0x3C, //ChannelGroup1 register,
	0x78, //ChannelGroup1 register,
	0xAF, //TX1 register,'
//	0xEE, //0x15 TX1 register,kevin

	0x40, //TX2 register,
	0x10, //DELAY1 register,
	0x40, //DELAY2 register,
//	0x60, //0x18 DELAY2 register,kevin


	0x70, //RX register,
//	0xFA, //0x19 RX register,kevin


	0x6F, //RXGAIN1 register,
	0xC2, //RXGAIN2 register,
//	0xD2, //RXGAIN2 register,reference


	0x3D, //RXGAIN3 register,
	0xE7, //RXGAIN4 register,
	0x00, //RSSI register,
	0xF1, //ADC register,
//	0x03, //CODE1 register,
	0xff, //CODE1 register,test

//	0x07, //CODE1 register,reference

	0x06, //CODE2 register,
//	0x07, //CODE2 register,
//	0x03, //0X21 CODE2 register,kevin

	0x2A, //CODE3 register,
//	0x00, //0x22 CODE3 register,kevin


	0x60, //IFCAL1 register,
	0x7F, //IFCAL2 register,
//	0xFF, //0x24 IFCAL2 register,reference
//	0xFF, //0x24 IFCAL2 register,kevin



	0x80, //VCOCCAL register,
	0xD0, //VCOCAL1 register,
	0x00, //VCOCAL2 register,
	0x70, //VCO deviation 1 register,
	0x00, //VCO deviation 2 register,

	0x00, //DSA register,

	0xDC, //VCO Modulation delay register,
	0xF0, //BATTERY register,
	0x37, //TXTEST register,
	0x27, //RXDEM1 register,
//	0x47, //RXDEM1 register,reference

	0xF7, //RXDEM2 register,
	0xF0, //CPC1 register,
	0x37, //CPC2 register,
	0x55, //CRYSTAL register,
//	0x51, //CRYSTAL register,reference
	0x15, //PLLTEST register,
	0x15, //VCOTEST register,
	0x00, //RF Analog register,
	0x00, //Key data register,
	0x77, //Channel select register,
	0x00, //ROM register,
	0x00, //DataRate register,
	0x00, //FCR register,
	0x00, //ARD register,
	0x00, //AFEP register,
	0x00, //FCB register,
	0x00, //KEYC register,
	0x00,  //USID register,
	///////////////////////////////////


	0x00, //0 RESET register,			only reset, not use on config

	0x60, //1 MODE register,
	0x00, //2 CALIBRATION register,
	0x3F, //3 FIFO1 register,
	0x00, //4 FIFO2 register,

	0x00, //5 FIFO register,			for fifo read/write
	0x00, //6 IDDATA register,		for idcode


	0x00, //7 RCOSC1 register,
	0x00, //8 RCOSC2 register,
	0x00, //9 RCOSC3 register,

//	0x02, //0xA CKO register,
//	0xea, //0xA CKO register,
	0xaa, //0xA CKO register,

//	0x1D, //0X0B GPIO1 register
	0x21, //0X0B GPIO1 register	//RXD


//	0x05, //0X0C GPIO2 register,
	0x25, //0X0C GPIO2 register,//TXD

	0x9F, //0X0D DATARATECLOCK register,

	0x00, //0x0E PLL1 register,
	0x9E, //0x0F PLL2 register,
	0x4B, //0x10 PLL3 register,


	0x00, //0x11 PLL4 register,
	0x02, //0x12 PLL5 register,

	0x3C, //0x13 0x0E ChannelGroup1 register,
	0x78, //0x14 ChannelGroup1 register,


	0xEE, //0x15 TX1 register,
	0x40, //0x16 TX2 register,

	0x10, //0x17 DELAY1 register,
	0x60, //0x18 DELAY2 register,

//	0x70, //0x19 RX register,
	0xFA, //0x19 RX register,


	0x6F, //0x1A RXGAIN1 register,
	0xC2, //0x1B RXGAIN2 register,
	0x3D, //0x1C RXGAIN3 register,
	0xE7, //0x1D RXGAIN4 register,


	0x00, //0x1E RSSI register,
	0xF1, //0x1F ADC register,

	0x03, //0x20 CODE1 register,
	0x03, //0X21 CODE2 register,
	0x00, //0x22 CODE3 register,

	0x60, //0x23 IFCAL1 register,
	0xFF, //0x24 IFCAL2 register,

	0x80, //0x25 VCOCCAL register,
	0xD0, //0x26 VCOCAL1 register,
	0x00, //0x27 VCOCAL2 register,

	0x70, //0x28 VCO deviation 1 register,
	0x00, //0x29 VCO deviation 2 register,


	0x00, //0x2A DSA register,

	0xDC, //0x2B VCO Modulation delay register,

	0xF0, //0X2C BATTERY register,
	0x37, //0x2D TXTEST register,

	0x27, //0X2E RXDEM1 register,
	0xF7, //0X2F RXDEM2 register,

	0xF0, //0X30 CPC1 register,
	0x37, //0X31 CPC2 register,
	0x55, //0X32 CRYSTAL register,
	0x15, //0X33 PLLTEST register,
	0x15, //0X34 VCOTEST register,
	0x00, //0X35 RF Analog register,

	0x00, //0X36 Key data register,

	0x77, //0X37 Channel select register,

	0x00, //0X38 ROM register,
	0x00, //0X39 DataRate register,
	0x00, //0X3A FCR register,
	0x00, //ARD register,
	0x00, //AFEP register,
	0x00, //FCB register,
	0x00, //KEYC register,
	0x00  //USID register,
};


const alt_u8 A7130_Addr2A_Config[]=
{
	0x74, //page0,
	0x49, //page1,
	//0x41, //page1,
	0x00, //Page2,
	0x80, //page3,
	0x80, //page4,
	0x00, //page5,
	0x0A, //page6,
	//0x00, //page6,
	0x00, //page7,




	0x74, //page0,
	0x49, //page1,
	0x00, //Page2,
	0x80, //page3,
	0x80, //page4,
	0x00, //page5,
	0x0A, //page6,
	0x00, //page7,
};

const alt_u8 A7130_Addr38_Config[]=
{
	0x00, //page0,
	0x10, //page1,
	0x20, //page2,
	0x24, //page3,
	0x20, //page4,
};
const alt_u8 PageTab[8]={0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70};//page select


int rxa_con_tim;
int rxb_con_tim;

U16 GPSrx_con_tim;


U8 rwmem_f=0;

U8 rdata_init[512];
U8 rdata_flag[512];
U8 rdata_value[512];
U8 rdata_temp[512];
U8 rdata_init_f=0;
U16 uarttx_ui_chksum;

U8 ui_setdata[32];
U8 mda_setdata[32];
U8 mdb_setdata[32];
U8 mds_setdata[32];

U8 mda_setdata_cnt;
U8 mdb_setdata_cnt;
U8 mds_setdata_cnt;

U16 mda_trans_cnt=0;
U16 mdb_trans_cnt=0;
U16 mds_trans_cnt=0;


U8 renew_ui_rdata_cnt;
U8 renew_ui_rdata_f=0;

int rdata_trans_cnt=0;

U16 muicon_tim;
U8 flash_f;
U16 rwmem_led_tim;
U8 led_flag;

U8 uart_dcc_tx_f = 0;
U8 uart_dcc_tx_cnt = 0;
U8 UiRx[256];
U8 uirxb[256];
U8 uirxb_cnt;
U8 uirxb_f;



U8 uarttx_dcc_len=0;
U8 uarttx_dcc_cnt=0;
U8 uarttx_dcc_start_f=0;



U8 uarttx_dcc_buf[16];


U8 uarttx_tx_buf[256];
U8 uarttx_tx_len=0;
U8 uarttx_tx_cnt=0;
U8 uarttx_tx_start_f=0;
U16 txreq_tim=0;


U8 uarttx_ui_buf[256];
U8 uarttx_ui_len=0;
U8 uarttx_ui_cnt=0;
U8 uarttx_ui_start_f=0;



U8 keybo_chk_f=0;
U8 keyp_f=0;
U8 keyr_f=0;
U8 keyc_f=0;

U8 testdata=0x55;
U8 poa_flag=0x00;
U8 pob_flag=0x00;

U8 pina_flag;

U8 Atxrx_sw_f;
U8 Btxrx_sw_f;


U8 Amem[64];
U8 Bmem[64];

U8 rf_enableA_f=1;
U8 rf_enableB_f=1;


U16 Adly1588_filt_buf[4];
U16 Adly1588_buf;
U16 Adly1588_filt_cnt;
U16 Adly1588_all_cnt;
U32 Adly1588_all;


U16 Bdly1588_filt_buf[4];
U16 Bdly1588_buf;
U16 Bdly1588_filt_cnt;
U16 Bdly1588_all_cnt;
U32 Bdly1588_all;







U32 tsync_delt_tim=0;
U32 tsync_delt_set=0;
int tsync_delt_delt=0;


U8 lvgint_f;
U8 lswp_pus_cnt;
U8 lbst_pus_cnt1;
U8 lbst_pus_cnt0;
U8 lvar_freq_mod_cnt;
U8 local_f;
U8 lvgout_en_f;
U8 pus_remote_f;


U8 brf_err_tim=0;


U8 uarttx_dcc_id=0;
U8 uarttx_dcc_idcnt=0;

U32 vg_delt_cnt=0;
U32 vg_delt_buf=0;


int main()
{
	int i;
	U8 buf;
	U32 nout32;
	usleep(1000);

reset:
	poa_flag=0;
//	printf("nios start!\n\r");
	UART_ISR_Init();//init UART
	initINT();//init INT
	initSPI();
	initRF();
	initTIMER();
	Atxrx_sw_f=0;
	Btxrx_sw_f=0;

	while(1)
	{
		rdata_value[nios_ver]=nios_ver_k;

		led_prg();
		if(uirxb_f)
		{
			uirxb_f=0;
			if(!uart_rx_proc(uirxb,uirxb_cnt,&urui_pack))
			{
				dispatch_uirx();
				muicon_tim=0;
			}
		}

		if(rdata_value[err_flag]&1)
		  poa_flag&=0xfd;
		else
		  poa_flag|=0x02;



		if(rdata_value[sync_sys]==1)
		{
			if(rdata_value[s1_tx_stat1]&0x04)
				poa_flag|=0x08;
			else
				poa_flag&=0xf7;
		}

		if(rdata_value[sync_sys]==2)
		{
			if(rdata_value[s2_tx_stat1]&0x04)
				poa_flag|=0x08;
			else
				poa_flag&=0xf7;
		}


		IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);



		if(rdata_init_f==0)
			continue;


		if(uart_dcc_tx_f)
		{
			uart_dcc_tx_f=0;
			uart_dcc_proc();
		}

		if(urx_dcc_la_tim>1000)
		  rdata_value[la_grp3]&=0xfe;
		if(urx_dcc_rf1_tim>1000)
		  rdata_value[la_grp3]&=0xfd;
		if(urx_dcc_rf2_tim>1000)
		  rdata_value[la_grp3]&=0xfb;
		if(urx_dcc_fb1_tim>1000)
		  rdata_value[la_grp3]&=0xf7;
		if(urx_dcc_fb2_tim>1000)
		  rdata_value[la_grp3]&=0xef;
		if(urx_dcc_422_tim>1000)
		  rdata_value[la_grp3]&=0xdf;
		if(urx_dcc_gps_tim>1000)
		  rdata_value[la_grp3]&=0xbf;


		if(rdata_value[otonios])
		{
			switch(rdata_value[otonios])
			{
			  case 1:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 2:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 3:
				if(!RF0_TESTID())
					rdata_value[testflag1]=rdata_value[otonios];
				else
					rdata_value[testflag1]=0;
			    break;
			  case 4:
					if(!RF1_TESTID())
						rdata_value[testflag1]=rdata_value[otonios];
					else
						rdata_value[testflag1]=0;
			    break;
			  case 5:
				if(GPSrx_con_tim<5000)
					rdata_value[testflag1]=rdata_value[otonios];
				else
					rdata_value[testflag1]=0;
			    break;
			  case 6:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 7:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 8:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 9:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 10:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 11:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;
			  case 12:
			    rdata_value[testflag1]=rdata_value[otonios];
			    break;

			}
			rdata_value[otonios]=0;
			renew_ui_rdata_f=1;
			renew_ui_rdata_cnt=otonios;

		}


		if(txreq_tim>=50)//unit 0.1
		{
			txreq_tim=0;
			loadtx_uarttx_req();
		}





//generate wave powon anf rf on
		if(rdata_value[sync_sys]==0)
		  buf=rdata_value[s1_waveg_sw];
		if(rdata_value[sync_sys]==1)
		  buf=rdata_value[s1_waveg_sw];
		if(rdata_value[sync_sys]==2)
		  buf=rdata_value[s2_waveg_sw];
		buf=buf<<2;
		buf=buf&0x0c;
		pob_flag&=0xf3;
		pob_flag|=buf;


		if(rdata_value[sync_sys]==1)
		{
			if(rdata_value[s1_pus_src]==1)
				pus_remote_f=1;
			else
				pus_remote_f=0;
		}
		if(rdata_value[sync_sys]==2)
		{
			if(rdata_value[s2_pus_src]==1)
				pus_remote_f=1;
			else
				pus_remote_f=0;
		}

		buf=pob_flag ^ (pus_remote_f<<4);
		buf&=0x10;
		pob_flag=pob_flag ^ buf;

		buf=pob_flag ^ (rdata_value[inhib]<<5);
		buf&=0x20;
		pob_flag=pob_flag ^ buf;


		IOWR(POUT_B_BASE,0,pob_flag );



//********************************************
		if(lvgint_f)
		{
			lvgint_f=0;
			buf=0;
			if(rdata_value[sync_sys]==1)
			  buf=rdata_value[s1_pus_mod_set];
			if(rdata_value[sync_sys]==2)
			  buf=rdata_value[s2_pus_mod_set];
			switch(buf)
			{
			  case 0:
				  nout32=(rdata_value[fix_pus_x1_set1]<<8)+rdata_value[fix_pus_x1_set0];
				  break;
			  case 1:
				  nout32=(rdata_value[fix_pus_x2_set1]<<8)+rdata_value[fix_pus_x2_set0];
				  break;
			  case 2:
				  nout32=(rdata_value[fix_pus_x3_set1]<<8)+rdata_value[fix_pus_x3_set0];
				  break;
			  case 3:
				  nout32=rdata_value[swp_pus_sa1_set1+lswp_pus_cnt]<<8;
				  nout32+=rdata_value[swp_pus_sa1_set0+lswp_pus_cnt];
				  lswp_pus_cnt+=2;
				  if(lswp_pus_cnt>=14)
					lswp_pus_cnt=0;
				  break;
			  case 4:
				  nout32=rdata_value[swp_pus_sb1_set1+lswp_pus_cnt]<<8;
				  nout32+=rdata_value[swp_pus_sb1_set0+lswp_pus_cnt];
				  lswp_pus_cnt+=2;
				  if(lswp_pus_cnt>=14)
					lswp_pus_cnt=0;
				  break;
			  case 5:
				  nout32=rdata_value[bst_pus_b1_set1+lbst_pus_cnt1]<<8;
				  nout32+=rdata_value[bst_pus_b1_set0+lbst_pus_cnt1];
				  lbst_pus_cnt0+=1;
				  if(lbst_pus_cnt0>=4)
				  {
					lbst_pus_cnt0=0;
					lbst_pus_cnt1+=2;
					if(lbst_pus_cnt1>=6)
						lbst_pus_cnt1=0;
				  }
				  break;
			  case 6:
				  nout32=0x4e20;
				  break;


			}

			buf=0;
			if(rdata_value[sync_sys]==1)
			  buf=rdata_value[s1_freq_mod_set];
			if(rdata_value[sync_sys]==2)
			  buf=rdata_value[s2_freq_mod_set];

			if(buf==0)
				buf=rdata_value[fix_freq_mod_set];
			else
			{
			  buf=rdata_value[var_freq_mod_set01+lvar_freq_mod_cnt];
			  lvar_freq_mod_cnt++;
			  if(lvar_freq_mod_cnt>=16)
				  lvar_freq_mod_cnt=0;
			}
			if(rdata_value[inhib])
				buf|=0x80;
			else
				buf&=0x7f;


			nout32=nout32+(buf<<16);
			IOWR(POUT_32_BASE,0,nout32);


		}


//********************************************
		if(rwmem_f)
		{
			rwmem_led_tim=0;
			led_flag|=1;
			/////////////////////////////////////
			poa_flag|=0x04;
			IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
			rwmem();
			poa_flag&=0xfb;
			IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
		}
//********************************************
		if(TXrx_f)
		{
			TXrx_f=0;
			txreq_err_tim=60000;
			if(rdata_value[sync_sys]==0)
			{
				rdata_value[s1_tx_stat1]=TXrx_data5;
				rdata_value[s1_tx_stat0]=TXrx_data4 | 0x01;
			}
			if(rdata_value[sync_sys]==1)
			{
				rdata_value[s1_tx_stat1]=TXrx_data5;
				rdata_value[s1_tx_stat0]=TXrx_data4 | 0x01;
			}
			if(rdata_value[sync_sys]==2)
			{
				rdata_value[s2_tx_stat1]=TXrx_data5;
				rdata_value[s2_tx_stat0]=TXrx_data4 | 0x01;
			}
		}
		if(txreq_err_tim==20)
		{
			if(rdata_value[sync_sys]==1)
				rdata_value[s1_tx_stat0]=TXrx_data4 & 0xfe;
			if(rdata_value[sync_sys]==2)
				rdata_value[s2_tx_stat0]=TXrx_data4 | 0xfe;
		}

//********************************************
		if(GPSrx_f)
		{
			GPSrx_f=0;
			GPSrx_con_tim=0;
			for(i=0;i<26;i++)
				rdata_value[gps_rx00+i]=GPSrx_data[i];
		}
//********************************************
		pina_flag = IORD_8DIRECT(PIN_A_BASE,0);
		if(rdata_value[sync_sys]==0)
		{
			buf=rdata_value[la_grp4] ^ (pina_flag>>3);//WG_PWOK,RFOK
			buf&=0x03;
			rdata_value[la_grp4]=rdata_value[la_grp4] ^ buf;
		}
		if(rdata_value[sync_sys]==1)
		{
			buf=rdata_value[s1_stat] ^ (pina_flag>>2);//WG_PWOK,RFOK
			buf&=0x06;
			rdata_value[s1_stat]=rdata_value[s1_stat] ^ buf;
		}
		if(rdata_value[sync_sys]==2)
		{
			buf=rdata_value[s2_stat] ^ (pina_flag>>2);//WG_PWOK,RFOK
			buf&=0x06;
			rdata_value[s2_stat]=rdata_value[s2_stat] ^ buf;
		}
//********************************************
		if(rf_enableA_f)
		{
			if(pina_flag&0x02)//rf1rx
			{
				if(Atxrx_sw_f==0)
				{
					Atxrx_sw_f=1;
					poa_flag&=0xcf;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
					SetCH0(rdata_value[s1_rf_ch]); //freq = 2450.001MHz
					StrobeCmd0(CMD_STBY);
					usleep(20);
					StrobeCmd0(CMD_RX);
					poa_flag|=0x20;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
				}
			}
			else//rf1tx
			{
				if(Atxrx_sw_f==1)
				{
					Atxrx_sw_f=0;
					poa_flag&=0xcf;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
					SetCH0(rdata_value[s1_rf_ch]); //freq = 2450.001MHz
					StrobeCmd0(CMD_STBY);
					usleep(20);
					StrobeCmd0(CMD_TX); //entry tx & transmit
					poa_flag|=0x10;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
				}
			}
		}
//********************************************
		if(rf_enableB_f)
		{
			if(rdata_value[sync_sys]==0)
				buf=s2_rf_ch;
			if(rdata_value[sync_sys]==1)
				buf=s1_rf_ch;
			if(rdata_value[sync_sys]==2)
				buf=s2_rf_ch;


			if(pina_flag&0x04)//rf2rx
			{
				if(brf_err_tim>80)
				{
					brf_err_tim=0;
					if(rdata_value[sync_sys]==1)
					  if(rdata_value[s1_con_mod])
						  Btxrx_sw_f=0;
					if(rdata_value[sync_sys]==2)
					  if(rdata_value[s2_con_mod])
						  Btxrx_sw_f=0;
				}
				if(Btxrx_sw_f==0)
				{
					Btxrx_sw_f=1;
					poa_flag&=0x3f;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
					SetCH1(rdata_value[buf]); //freq = 2450.001MHz
					StrobeCmd1(CMD_STBY);
					usleep(20);
					StrobeCmd1(CMD_RX);
					poa_flag|=0x80;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
				}
			}
			else//rf2tx
			{
				brf_err_tim=0;
				if(Btxrx_sw_f==1)
				{
					Btxrx_sw_f=0;
					poa_flag&=0x3f;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
					SetCH1(rdata_value[buf]); //freq = 2450.001MHz
					StrobeCmd1(CMD_STBY);
					usleep(20);
					StrobeCmd1(CMD_TX); //entry tx & transmit
					poa_flag|=0x40;
					IOWR_8DIRECT(POUT_A_BASE,0,poa_flag);
				}
			}
		}

		keybo_prg();
		if(keyp_f==0x01)
		{
			goto reset;
		}
		if(keyp_f==0x02)
		{
		}
		if(keyp_f==0x04)
		{
		}
		if(keyp_f==0x08)
		{
		}
	}
	return 0;
}




void led_prg(void)
{
	led_flag|=2;
	switch(rdata_value[sync_sys])//system_set
	{
		case 0:
			if(rdata_value[con_ab_stat])
				led_flag|=4;
			else
				led_flag&=0xfb;
			break;
		case 1:
			if(rdata_value[con_cd_stat])
				led_flag|=4;
			else
				led_flag&=0xfb;
			break;
		case 2:
			if(rdata_value[con_ef_stat])
				led_flag|=4;
			else
				led_flag&=0xfb;
			break;
	}

	if(rdata_value[con_bc_stat])
		led_flag|=8;
	else
		led_flag&=0xf7;

	if(rdata_value[con_be_stat])
		led_flag|=0x10;
	else
		led_flag&=0xef;

	if(rdata_value[hc_vgin_stat])
		led_flag|=0x20;
	else
		led_flag&=0xdf;


	if(flash_f)
		IOWR_8DIRECT(POUT_LED_BASE,0,led_flag^255);
	else
		IOWR_8DIRECT(POUT_LED_BASE,0,0xff);

}


/***********************************************/
static void UART_ISR_Init(void)
{
//	IOWR_ALTERA_AVALON_UART_STATUS(UART_BASE, 0x0); //清除與"通訊埠"中斷向量旗標
//	alt_irq_register(UART_IRQ, NULL, UART_Irq_Handler); //註冊中斷
//	IOWR_ALTERA_AVALON_UART_CONTROL(UART_BASE, 0x0080); //允許"通訊埠"接收中斷

	IOWR_ALTERA_AVALON_UART_STATUS(UART_DCC_BASE, 0x0); //清除與"MCU通訊"中斷向量旗標
	alt_irq_register(UART_DCC_IRQ, NULL, UART_DCC_Irq_Handler); //註冊中斷
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x0080); //允許"MCU通訊"接收中斷

	IOWR_ALTERA_AVALON_UART_STATUS(UART_GPS_BASE, 0x0); //清除與"GPS"中斷向量旗標
	alt_irq_register(UART_GPS_IRQ, NULL, UART_GPS_Irq_Handler); //註冊中斷
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_GPS_BASE, 0x0080); //允許"GPS"接收中斷

	IOWR_ALTERA_AVALON_UART_STATUS(UART_TX_BASE, 0x0); //清除與"TX通訊"中斷向量旗標
	alt_irq_register(UART_TX_IRQ, NULL, UART_TX_Irq_Handler); //註冊中斷
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_TX_BASE, 0x0080); //允許"TX通訊"接收中斷

	IOWR_ALTERA_AVALON_UART_STATUS(UART_UI_BASE, 0x0); //清除與"PC通訊"中斷向量旗標
	alt_irq_register(UART_UI_IRQ, NULL, UART_UI_Irq_Handler); //註冊中斷
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_UI_BASE, 0x0080); //允許"PC通訊"接收中斷
//	IOWR_ALTERA_AVALON_UART_CONTROL(UART_UI_BASE, 0x0040); //允許"PC通訊"發送中斷
//	IOWR_ALTERA_AVALON_UART_CONTROL(UART_UI_BASE, 0x00c0); //允許"PC通訊"接收發送中斷
}

/***********************************************/

// ************** 外部中斷初始化 **************************
static void initINT(void)
{
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIN_A_BASE,0x01);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIN_A_BASE,0x00);
	alt_irq_register(PIN_A_IRQ, NULL, INT_IrqA_Handler); //註冊中斷
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIN_B_BASE,0x01);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIN_B_BASE,0x00);
	alt_irq_register(PIN_B_IRQ, NULL, INT_IrqB_Handler); //註冊中斷
}

// ************** INT_Irq_Handler **************************
static void INT_IrqA_Handler(void)
{

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIN_A_BASE,0x00);
	rwmem_f=1;
}

static void INT_IrqB_Handler(void)
{
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIN_B_BASE,0x00);
	lvgint_f=1;
}


void rwmem(void)//40us
{
	static U8 rxa_cnt_pre;
	static U8 rxb_cnt_pre;
	int i,j,k;
	U8 buf;

	rwmem_f=0;
	for(i=0;i<48;i++)
		Bmem[i]=IORD_8DIRECT(MYBPORT_BASE,128*2+ 1+i );
	////////////////////////////////////////////////////////////
	if(rdata_value[sync_sys]==0)//系統(主控/誘標1/誘標2)
	{
		j=0;
		for(i=0;i<8;i++)
		{
			if(mda_setdata[i*4+0]!=0)
			{
				Amem[4]=mda_setdata[i*4+0];
				Amem[5]=mda_setdata[i*4+1];
				Amem[6]=mda_setdata[i*4+2];
				Amem[7]=mda_setdata[i*4+3];
				mda_setdata[i*4+0]=0;
				j++;
				break;
			}
		}
		k=0;
		for(i=0;i<8;i++)
		{
			if(mdb_setdata[i*4+0]!=0)
			{
				Amem[8]=mdb_setdata[i*4+0];
				Amem[9]=mdb_setdata[i*4+1];
				Amem[10]=mdb_setdata[i*4+2];
				Amem[11]=mdb_setdata[i*4+3];
				mdb_setdata[i*4+0]=0;
				k++;
				break;
			}
		}
		if(j==0)
		{
			for(i=0;i<rdata_lim_k;i++)
			{
				if(rdata_flag[mda_trans_cnt]&0x4)
				{
					Amem[4]=0xb0;		//sync data
					Amem[5]=(mda_trans_cnt>>8)&0x01;
					Amem[6]=mda_trans_cnt&0xff;
					Amem[7]=rdata_value[mda_trans_cnt];
					mda_trans_cnt++;
					mda_trans_cnt&=0x1ff;
					break;
				}
				mda_trans_cnt++;
				mda_trans_cnt&=0x1ff;
			}
		}

		if(k==0)
		{
			for(i=0;i<rdata_lim_k;i++)
			{
				if(rdata_flag[mdb_trans_cnt]&0x8)
				{
					Amem[8]=0xb0;		//sync data
					Amem[9]=(mdb_trans_cnt>>8)&0x01;
					Amem[10]=mdb_trans_cnt&0xff;
					Amem[11]=rdata_value[mdb_trans_cnt];
					mdb_trans_cnt++;
					mdb_trans_cnt&=0x1ff;
					break;
				}
				mdb_trans_cnt++;
				mdb_trans_cnt&=0x1ff;
			}
		}




	}
	else
	{
		j=0;
		for(i=0;i<8;i++)
		{
			if(mds_setdata[i*4+0]!=0)
			{
				Amem[0]=mds_setdata[i*4+0];
				Amem[1]=mds_setdata[i*4+1];
				Amem[2]=mds_setdata[i*4+2];
				Amem[3]=mds_setdata[i*4+3];
				mds_setdata[i*4+0]=0;
				j++;
				break;
			}
		}
		if(j==0)
		{
			for(i=0;i<rdata_lim_k;i++)
			{
				if(rdata_flag[mds_trans_cnt]&0x4)
				{
					Amem[0]=0xb0;		//sync data
					Amem[1]=(mds_trans_cnt>>8)&0x01;
					Amem[2]=mds_trans_cnt&0xff;
					Amem[3]=rdata_value[mds_trans_cnt];
					mds_trans_cnt++;
					mds_trans_cnt&=0x1ff;
					break;
				}
				mds_trans_cnt++;
				mds_trans_cnt&=0x1ff;

			}
		}
	}





//*******************************************************************
	buf=Amem[12] ^ rdata_value[sync_sys];//系統(主控/誘標1/誘標2)
	buf&=0x03;
	Amem[12]=Amem[12] ^ buf;
//*******************************************************************
	buf=Amem[12] ^ (rdata_value[one_con_trs]<<2);//單擊傳輸/連續傳輸
	buf&=0x04;
	Amem[12]=Amem[12] ^ buf;
//*******************************************************************
	if(rdata_value[sync_mod]<2)//副控一同步模式
		Amem[12]&=0xf7;
	else
		Amem[12]|=0x08;
//*******************************************************************
	buf=Amem[12] ^ (rdata_value[tim_stune]<<4);//時間微調
	buf&=0x10;
	Amem[12]=Amem[12] ^ buf;
//*******************************************************************
	buf=rdata_value[hc_op_mod];
	if(rdata_value[hc_op_mod]==3)
	  if(rdata_value[hc_pus_src]==0)
		buf=2;//test mode
	buf=Amem[12] ^ (buf<<6);//主控VGI模式
	buf&=0xc0;
	Amem[12]=Amem[12] ^ buf;
//*******************************************************************


	if(rdata_value[s1_trs_sw]==0)//ch1 transfer off/on
		Amem[13]&=0xfe;
	else
		Amem[13]|=0x01;

	if(rdata_value[s2_trs_sw]==0)//ch2 transfer off/on
		Amem[13]&=0xfd;
	else
		Amem[13]|=0x02;


	if(rdata_value[sync_sys]==1)
	  buf=Amem[13] ^ (rdata_value[s1_op_mod]<<2);//s1通訊方式
	if(rdata_value[sync_sys]==2)
	  buf=Amem[13] ^ (rdata_value[s2_op_mod]<<2);//s1通訊方式
	buf&=0x04;
	Amem[13]=Amem[13] ^ buf;


	buf=Amem[13] ^ (HA_rf_en<<4);
	buf&=0x10;
	Amem[13]=Amem[13] ^ buf;

	buf=Amem[13] ^ (HB_rf_en<<5);
	buf&=0x20;
	Amem[13]=Amem[13] ^ buf;

	buf=Amem[13] ^ (rdata_value[s1_con_mod]<<6);//s1通訊方式
	buf&=0x40;
	Amem[13]=Amem[13] ^ buf;

	buf=Amem[13] ^ (rdata_value[s2_con_mod]<<7);//s2通訊方式
	buf&=0x80;
	Amem[13]=Amem[13] ^ buf;



	Amem[14]=rdata_value[timsync_vgo_offs1];//Hvg_tim_off
	Amem[15]=rdata_value[timsync_vgo_offs0];
	Amem[16]=rdata_value[timsync_vgo_offs1];//Svgrt_tim_off
	Amem[17]=rdata_value[timsync_vgo_offs0];




	if(rdata_value[sync_sys]==2)
	{
		Amem[18]=rdata_value[s2_dlysync_op_offs1];  //Svgrx_bas_off
		Amem[19]=rdata_value[s2_dlysync_op_offs0];
		if(rdata_value[s2_con_mod]==0)
		{
			Amem[20]=rdata_value[s2_fbsync_trs_offs1];  //Svgrx_dly_off
			Amem[21]=rdata_value[s2_fbsync_trs_offs0];
		}
		else
		{
			Amem[20]=rdata_value[s2_rfsync_trs_offs1];  //Svgrx_dly_off
			Amem[21]=rdata_value[s2_rfsync_trs_offs0];
		}

	}
	else
	{
		Amem[18]=rdata_value[s1_dlysync_op_offs1];  //Svgrx_bas_off
		Amem[19]=rdata_value[s1_dlysync_op_offs0];
		if(rdata_value[s1_con_mod]==0)
		{
			Amem[20]=rdata_value[s1_fbsync_trs_offs1];  //Svgrx_dly_off
			Amem[21]=rdata_value[s1_fbsync_trs_offs0];
		}
		else
		{
			Amem[20]=rdata_value[s1_rfsync_trs_offs1];  //Svgrx_dly_off
			Amem[21]=rdata_value[s1_rfsync_trs_offs0];
		}
	}


	Amem[22]=rdata_value[timsync_max_delt1];//Svgrt_delt_lim
	Amem[23]=rdata_value[timsync_max_delt0];

	Amem[24]=0;					//Svgrt_adj_off
	Amem[25]=rdata_value[tim_fix_offs];


	Amem[28]=rdata_value[la_grp5];//adj_tim_0
	Amem[29]=rdata_value[la_grp6];//adj_tim_1
	Amem[30]=rdata_value[la_grp7];//adj_tim_0
	Amem[31]=rdata_value[la_grp8];//adj_tim_1


//	Amem[28]=rdata_value[tim_adj3];//adj_tim_0
//	Amem[29]=rdata_value[tim_adj2];//adj_tim_1
//	Amem[30]=rdata_value[tim_adj1];//adj_tim_2
//	Amem[31]=rdata_value[tim_adj0];//adj_tim_3

	Amem[32]=rdata_value[i1588_offs1];//a1588 adj
	Amem[33]=rdata_value[i1588_offs0];//a1588 adj

	Amem[34]=(rdata_value[s2_con_mod]<<4)+rdata_value[s1_con_mod];//tr mode;



//--------------------------------------------------
	buf=6;
	if(rdata_value[sync_sys]==0)
	{
	  if(rdata_value[hc_op_mod]==3)
		if(rdata_value[hc_pus_src]==0)
	      buf=rdata_value[hc_freq_mod_set];
	}
	if(rdata_value[sync_sys]==1)
	  buf=rdata_value[s1_freq_mod_set];
	if(rdata_value[sync_sys]==2)
	  buf=rdata_value[s2_freq_mod_set];


	if(buf==0)
		Amem[35]=rdata_value[fix_freq_mod_set];
	if(buf==1)
	{
	  Amem[35]=rdata_value[var_freq_mod_set01+var_freq_mod_cnt];
	  var_freq_mod_cnt++;
	  if(var_freq_mod_cnt>=16)
		  var_freq_mod_cnt=0;
	}
	if(buf==2)
	{
	  Amem[35]=rdata_value[var_freq_mod_set01+var_freq_mod_cnt];
	  var_freq_mod_cnt0++;
	  if(var_freq_mod_cnt0>=7)
	  {
		var_freq_mod_cnt0=0;
	    var_freq_mod_cnt++;
	    if(var_freq_mod_cnt>=16)
		  var_freq_mod_cnt=0;
	  }
	}
	if(rdata_value[inhib])
		Amem[35]|=0x80;
	else
		Amem[35]&=0x7f;



//--------------------------------------------------
	if(rdata_value[sync_sys]==0)
	{
	  if(rdata_value[hc_op_mod]==3)
	  {
		if(rdata_value[hc_pus_src]==0)
	      buf=rdata_value[hc_pus_mod_set];
	  }
	  else
		buf=6;
	}

	if(rdata_value[sync_sys]==1)
	  buf=rdata_value[s1_pus_mod_set];
	if(rdata_value[sync_sys]==2)
	  buf=rdata_value[s2_pus_mod_set];


	switch(buf)
	{
	  case 0:
		  Amem[36]=rdata_value[fix_pus_x1_set1];
		  Amem[37]=rdata_value[fix_pus_x1_set0];;
		  break;
	  case 1:
		  Amem[36]=rdata_value[fix_pus_x2_set1];
		  Amem[37]=rdata_value[fix_pus_x2_set0];;
		  break;
	  case 2:
		  Amem[36]=rdata_value[fix_pus_x3_set1];
		  Amem[37]=rdata_value[fix_pus_x3_set0];;
		  break;
	  case 3:
		  Amem[36]=rdata_value[swp_pus_sa1_set1+swp_pus_cnt];
		  Amem[37]=rdata_value[swp_pus_sa1_set0+swp_pus_cnt];
		  swp_pus_cnt+=2;
		  if(swp_pus_cnt>=14)
			swp_pus_cnt=0;
		  break;
	  case 4:
		  Amem[36]=rdata_value[swp_pus_sb1_set1+swp_pus_cnt];
		  Amem[37]=rdata_value[swp_pus_sb1_set0+swp_pus_cnt];
		  swp_pus_cnt+=2;
		  if(swp_pus_cnt>=14)
			swp_pus_cnt=0;
		  break;
	  case 5:
		  Amem[36]=rdata_value[bst_pus_b1_set1+bst_pus_cnt1];
		  Amem[37]=rdata_value[bst_pus_b1_set0+bst_pus_cnt1];
		  bst_pus_cnt0+=1;
		  if(bst_pus_cnt0>=4)
		  {
			bst_pus_cnt0=0;
			bst_pus_cnt1+=2;
			if(bst_pus_cnt1>=6)
				bst_pus_cnt1=0;
		  }
		  break;
	  case 6:
		  Amem[36]=0x4e;
		  Amem[37]=0x20;
		  break;


	}

	Amem[38]=rdata_value[la_grp0] | (rdata_value[la_grp1]<<4);


	if(rdata_value[p4c_data_set]==2)
	  j=c_i_data2;
	else if(rdata_value[p4c_data_set]==1)
	  j=b_i_data2;
	else
	  j=a_i_data2;

	Amem[40]=rdata_value[j+0];
	Amem[41]=rdata_value[j+1];
	Amem[42]=rdata_value[j+3];
	Amem[43]=rdata_value[j+4];
	Amem[44]=rdata_value[j+6];
	Amem[45]=rdata_value[j+7];
	Amem[46]=rdata_value[j+9];
	Amem[47]=rdata_value[j+10];
	Amem[48]=rdata_value[wg_offseth1];
	Amem[49]=rdata_value[wg_offseth0];
	Amem[50]=rdata_value[wg_offsetl1];
	Amem[51]=rdata_value[wg_offsetl0];




	for(i=0;i<8;i++)
	{
		Amem[52+i]=rdata_value[p4c_offset1+i];
	}




	for(i=0;i<64;i++)
		IOWR_8DIRECT(MYBPORT_BASE,128*1+ 1+i,Amem[i] );


	rdata_value[rxa0]=Bmem[0];	//;Arxdata1h/
	rdata_value[rxa1]=Bmem[1];	//;Arxdata1h/
	rdata_value[rxa2]=Bmem[2];	//;Arxdata2h/
	rdata_value[rxa3]=Bmem[3];	//;Arxdata2h/
	rdata_value[rxa4]=Bmem[4];	//;Arxdata3h/time sync time
	rdata_value[rxa5]=Bmem[5];	//;Arxdata3h/time sync time
	rdata_value[rxa6]=Bmem[6];	//;Arxdata4h/
	rdata_value[rxa7]=Bmem[7];	//;Arxdata4h/
	rdata_value[rxb0]=Bmem[8];	//;Brxdata1h/
	rdata_value[rxb1]=Bmem[9];	//;Brxdata1h/
	rdata_value[rxb2]=Bmem[10];	//;Brxdata2h/
	rdata_value[rxb3]=Bmem[11];	//;Brxdata2h/
	rdata_value[rxb4]=Bmem[12];	//;Brxdata3h/
	rdata_value[rxb5]=Bmem[13];	//;Brxdata3h/
	rdata_value[rxb6]=Bmem[14];	//;Brxdata4h/
	rdata_value[rxb7]=Bmem[15];	//;Brxdata4h/


	if(pri_pcnt<24)
	{
		rdata_value[pus01_lat1+pri_pcnt+pri_pcnt]=Bmem[21];
		rdata_value[pus01_lat0+pri_pcnt+pri_pcnt]=Bmem[22];
		rdata_value[freq_lat00+pri_pcnt]=Bmem[40];
		pri_pcnt++;
	}

	if(rdata_value[sync_sys]==0)
	{
		if(mda_trans_cnt==hc_pus_pri0 || mdb_trans_cnt==hc_pus_pri0 || rdata_trans_cnt==hc_pus_pri0)
		{
		}
		else
		{
		  rdata_value[hc_pus_pri1]=Bmem[21];	//vg period m
		  rdata_value[hc_pus_pri0]=Bmem[22];	//vg period l
		}
		rdata_value[hc_vgin_stat]=Bmem[25]&0x01;//external vgin on

	}
	if(rdata_value[sync_sys]==1)
	{
		if(mds_trans_cnt==s1_pus_pri0 || rdata_trans_cnt==s1_pus_pri0)
		{
		}
		else
		{
			rdata_value[s1_pus_pri1]=Bmem[21];	//vg period m
			rdata_value[s1_pus_pri0]=Bmem[22];	//vg period l
		}
	}
	if(rdata_value[sync_sys]==2)
	{
		if(mds_trans_cnt==s2_pus_pri0 || rdata_trans_cnt==s2_pus_pri0)
		{
		}
		else
		{
			rdata_value[s2_pus_pri1]=Bmem[21];	//vg period m
			rdata_value[s2_pus_pri0]=Bmem[22];	//vg period l
		}
	}

	rdata_value[debug_mod]=Bmem[25]&0x06;//rlp1_f

	if(rdata_value[sync_sys]==0)
	{
		rdata_value[s1_trs_corat1]=Bmem[30];//Acorrect_cnt_h
		rdata_value[s1_trs_corat0]=Bmem[31];//Acorrect_cnt_l
		rdata_value[s2_trs_corat1]=Bmem[32];//Bcorrect_cnt_h
		rdata_value[s2_trs_corat0]=Bmem[33];//Bcorrect_cnt_l


		rdata_value[s1_txpack_count]=Bmem[23];	//Avgin_count(h)
		rdata_value[s1_rxpack_count]=Bmem[16];	//Arx_count(h)
		rdata_value[s1_retlos_count]=Bmem[23]*256+Bmem[24]-Bmem[16]*256-Bmem[17];//tx_count-Arx_count
		rdata_value[s2_txpack_count]=Bmem[26];	//Bvgin_count(h)
		rdata_value[s2_rxpack_count]=Bmem[18];	//Brx_count(h)
		rdata_value[s2_retlos_count]=Bmem[26]*256+Bmem[27]-Bmem[18]*256-Bmem[19];//tx_count-Arx_count



		Adly1588_buf=Bmem[28]*256+Bmem[29];
		Adly1588_filt_cnt++;
		if(Adly1588_filt_cnt>=4)
			Adly1588_filt_cnt=0;
		Adly1588_filt_buf[Adly1588_filt_cnt]=Adly1588_buf;
		if(Adly1588_filt_cnt==0)
		{
			if(Adly1588_filt_buf[0]>Adly1588_filt_buf[1])
			{
				Adly1588_buf=Adly1588_filt_buf[0];
				Adly1588_filt_buf[0]=Adly1588_filt_buf[1];
				Adly1588_filt_buf[1]=Adly1588_buf;
			}
			if(Adly1588_filt_buf[1]>Adly1588_filt_buf[2])
			{
				Adly1588_buf=Adly1588_filt_buf[1];
				Adly1588_filt_buf[1]=Adly1588_filt_buf[2];
				Adly1588_filt_buf[2]=Adly1588_buf;
			}
			if(Adly1588_filt_buf[2]>Adly1588_filt_buf[3])
			{
				Adly1588_buf=Adly1588_filt_buf[2];
				Adly1588_filt_buf[2]=Adly1588_filt_buf[3];
				Adly1588_filt_buf[3]=Adly1588_buf;
			}

			if(Adly1588_filt_buf[1]>Adly1588_filt_buf[2])
			{
				Adly1588_buf=Adly1588_filt_buf[1];
				Adly1588_filt_buf[1]=Adly1588_filt_buf[2];
				Adly1588_filt_buf[2]=Adly1588_buf;
			}
			if(Adly1588_filt_buf[0]>Adly1588_filt_buf[1])
			{
				Adly1588_buf=Adly1588_filt_buf[0];
				Adly1588_filt_buf[0]=Adly1588_filt_buf[1];
				Adly1588_filt_buf[1]=Adly1588_buf;
			}

			Adly1588_buf=Adly1588_filt_buf[1]+Adly1588_filt_buf[2];
			Adly1588_all=Adly1588_all+Adly1588_buf;
			Adly1588_all_cnt++;
			Adly1588_buf=1<<rdata_value[avg_1588];
			if(Adly1588_all_cnt>=Adly1588_buf)
			{
				Adly1588_all=Adly1588_all>>(rdata_value[avg_1588]+1);
				Adly1588_all_cnt=0;
				rdata_value[s1_1588_dly1]=Adly1588_all>>8;//1588 DLY(H)
				rdata_value[s1_1588_dly0]=Adly1588_all&0x00ff;//1588 DLY(L)
				Adly1588_all=0;
			}
		}


		Bdly1588_buf=Bmem[34]*256+Bmem[35];
		Bdly1588_filt_cnt++;
		if(Bdly1588_filt_cnt>=4)
			Bdly1588_filt_cnt=0;
		Bdly1588_filt_buf[Bdly1588_filt_cnt]=Bdly1588_buf;
		if(Bdly1588_filt_cnt==0)
		{
			if(Bdly1588_filt_buf[0]>Bdly1588_filt_buf[1])
			{
				Bdly1588_buf=Bdly1588_filt_buf[0];
				Bdly1588_filt_buf[0]=Bdly1588_filt_buf[1];
				Bdly1588_filt_buf[1]=Bdly1588_buf;
			}
			if(Bdly1588_filt_buf[1]>Bdly1588_filt_buf[2])
			{
				Bdly1588_buf=Bdly1588_filt_buf[1];
				Bdly1588_filt_buf[1]=Bdly1588_filt_buf[2];
				Bdly1588_filt_buf[2]=Bdly1588_buf;
			}
			if(Bdly1588_filt_buf[2]>Bdly1588_filt_buf[3])
			{
				Bdly1588_buf=Bdly1588_filt_buf[2];
				Bdly1588_filt_buf[2]=Bdly1588_filt_buf[3];
				Bdly1588_filt_buf[3]=Bdly1588_buf;
			}

			if(Bdly1588_filt_buf[1]>Bdly1588_filt_buf[2])
			{
				Bdly1588_buf=Bdly1588_filt_buf[1];
				Bdly1588_filt_buf[1]=Bdly1588_filt_buf[2];
				Bdly1588_filt_buf[2]=Bdly1588_buf;
			}
			if(Bdly1588_filt_buf[0]>Bdly1588_filt_buf[1])
			{
				Bdly1588_buf=Bdly1588_filt_buf[0];
				Bdly1588_filt_buf[0]=Bdly1588_filt_buf[1];
				Bdly1588_filt_buf[1]=Bdly1588_buf;
			}

			Bdly1588_buf=Bdly1588_filt_buf[1]+Bdly1588_filt_buf[2];
			Bdly1588_all=Bdly1588_all+Bdly1588_buf;
			Bdly1588_all_cnt++;
			Bdly1588_buf=1<<rdata_value[avg_1588];
			if(Bdly1588_all_cnt>=Bdly1588_buf)
			{
				Bdly1588_all=Bdly1588_all>>(rdata_value[avg_1588]+1);
				Bdly1588_all_cnt=0;
				rdata_value[s2_1588_dly1]=Bdly1588_all>>8;//1588 DLY(H)
				rdata_value[s2_1588_dly0]=Bdly1588_all&0x00ff;//1588 DLY(L)
				Bdly1588_all=0;
			}
		}

		vg_delt_cnt++;
		if(vg_delt_cnt>=256)
		{
			vg_delt_cnt=0;
			rdata_value[vg_delt1]=vg_delt_buf>>16;
			rdata_value[vg_delt0]=vg_delt_buf>>8;
			vg_delt_buf=0;
		}
		vg_delt_buf=vg_delt_buf+(Bmem[36]<<8)+Bmem[37];

//		rdata_value[vg_delt1]=Bmem[36];
//		rdata_value[vg_delt0]=Bmem[37];


	}






	if(rdata_value[sync_sys]==0)
	{
		if(Bmem[0]==0xb0)	//;SYNC DATA
		{
			Bmem[0]=0x00;
			Bmem[1]=Bmem[1]&0x01;
			rdata_value[(Bmem[1]<<8)+Bmem[2]]=Bmem[3];
		}
		if(Bmem[0]==0xb1)	//;set DATA
		{
			Bmem[0]=0x00;
			if(Bmem[1]&0x10)
			{
				Bmem[1]=Bmem[1]&0x01;
				rdata_value[(Bmem[1]<<8)+Bmem[2]]=Bmem[3];
			}
			if(Bmem[1]&0x20)
			{
				ui_setdata[0]=0xb1;
				ui_setdata[1]=Bmem[1];
				ui_setdata[2]=Bmem[2];
				ui_setdata[3]=Bmem[3];
			}
		}


		if(Bmem[8]==0xb0)	//;SYNC DATA
		{
			Bmem[8]=0x00;
			Bmem[9]=Bmem[9]&0x01;
			rdata_value[(Bmem[9]<<8)+Bmem[10]]=Bmem[11];
		}
		rdata_value[hc_freq_cod]=Bmem[40];
		if(Bmem[17]!=rxa_cnt_pre)
		{
		  rxa_cnt_pre=Bmem[17];
		  rdata_value[con_bc_stat]=1;
		  rxa_con_tim=0;
		}

		if(rdata_value[sync_sys]==0)
		{
			if(Bmem[19]!=rxb_cnt_pre)
			{
				rxb_cnt_pre=Bmem[19];
				rdata_value[con_be_stat]=1;
				rxb_con_tim=0;
			}
		}
	}


	if(rdata_value[sync_sys]==1)
	{
		if(Bmem[8]==0xb0)	//;SYNC DATA
		{
			Bmem[8]=0x00;
			Bmem[9]=Bmem[9]&0x01;
			rdata_value[(Bmem[9]<<8)+Bmem[10]]=Bmem[11];
		}

		if(Bmem[8]==0xb1)	//;set DATA
		{
			Bmem[8]=0x00;
			if(Bmem[9]&0x10)
			{
				Bmem[9]=Bmem[9]&0x01;
				rdata_value[(Bmem[9]<<8)+Bmem[10]]=Bmem[11];
			}
			if(Bmem[9]&0x20)
			{
				ui_setdata[0]=0xb1;
				ui_setdata[1]=Bmem[9];
				ui_setdata[2]=Bmem[10];
				ui_setdata[3]=Bmem[11];
			}
		}

		if(Bmem[8]==0xb2)	//;set DATA
		{
			Bmem[8]=0x00;
			loadtx_uarttx_cmd(Bmem[11]);
		}


		rdata_value[s1_freq_cod]=Bmem[40];
		if(Bmem[19]!=rxa_cnt_pre)
		{
		  rxa_cnt_pre=Bmem[19];
		  rdata_value[con_bc_stat]=1;
		  rxa_con_tim=0;
		}
	}

	if(rdata_value[sync_sys]==2)
	{
		if(Bmem[8]==0xb0)	//;SYNC DATA
		{
			Bmem[8]=0x00;
			Bmem[9]=Bmem[9]&0x01;
			rdata_value[(Bmem[9]<<8)+Bmem[10]]=Bmem[11];
		}

		if(Bmem[8]==0xb1)	//;set DATA
		{
			Bmem[8]=0x00;
			if(Bmem[9]&0x40)
			{
				Bmem[9]=Bmem[9]&0x01;
				rdata_value[(Bmem[9]<<8)+Bmem[10]]=Bmem[11];
			}
			if(Bmem[9]&0x80)
			{
				ui_setdata[0]=0xb1;
				ui_setdata[1]=Bmem[9];
				ui_setdata[2]=Bmem[10];
				ui_setdata[3]=Bmem[11];
			}
		}

		if(Bmem[8]==0xb2)	//;set DATA
		{
			Bmem[8]=0x00;
			loadtx_uarttx_cmd(Bmem[11]);
		}


		rdata_value[s2_freq_cod]=Bmem[40];
		if(Bmem[19]!=rxa_cnt_pre)
		{
		  rxa_cnt_pre=Bmem[19];
		  rdata_value[con_be_stat]=1;
		  rxb_con_tim=0;
		}
	}




/*
	tsync_delt_tim=tsync_delt_tim+(Bmem[34]<<16)+(Bmem[35]<<8)+Bmem[36];
	tsync_delt_set=(rdata_value[tim_adj3]<<24)+(rdata_value[tim_adj2]<<16)+(rdata_value[tim_adj1]<<8)+rdata_value[tim_adj0];
	if(Bmem[37]&0x80)
		tsync_delt_delt=tsync_delt_delt+(Bmem[37]<<8)+Bmem[38]-65536;
	else
		tsync_delt_delt=tsync_delt_delt+(Bmem[37]<<8)+Bmem[38];
	rdata_value[tim_adj3]=(tsync_delt_set>>24) & 0xff;
	rdata_value[tim_adj2]=(tsync_delt_set>>16) & 0xff;
	rdata_value[tim_adj1]=(tsync_delt_set>> 8) & 0xff;
	rdata_value[tim_adj0]=tsync_delt_set & 0xff;
	if(tsync_delt_tim>80000000)
	{
		rdata_value[tsysn_delt_tim3]=(tsync_delt_tim>>24) & 0xff;
		rdata_value[tsysn_delt_tim2]=(tsync_delt_tim>>16) & 0xff;
		rdata_value[tsysn_delt_tim1]=(tsync_delt_tim>> 8) & 0xff;
		rdata_value[tsysn_delt_tim0]=tsync_delt_tim & 0xff;
		//////////////////////////////////////////////////////////
		rdata_value[tsysn_delt_delt3]=(tsync_delt_delt>>24) & 0xff;
		rdata_value[tsysn_delt_delt2]=(tsync_delt_delt>>16) & 0xff;
		rdata_value[tsysn_delt_delt1]=(tsync_delt_delt>> 8) & 0xff;
		rdata_value[tsysn_delt_delt0]=tsync_delt_delt & 0xff;
		tsync_delt_tim=0;
		tsync_delt_delt=0;
	}
*/

	if(rdata_value[sync_sys]==1)
		rdata_value[s1_pack_los_count]=Bmem[39];
	if(rdata_value[sync_sys]==2)
		rdata_value[s2_pack_los_count]=Bmem[39];









	if((poa_flag&0x01)==0)
	{
		if(rdata_init_f==1)
		{
			poa_flag|=0x01;
		}
	}






}



/***********************************************/
static void UART_UI_Irq_Handler(void *context, alt_u32 id)
{
	static U16 UiRx_cnt=0;
	int uart_status;
	alt_u8 uart_data;
	int i;


	uart_status=IORD_ALTERA_AVALON_UART_STATUS(UART_UI_BASE);
	if(uart_status&0x80)//rx_interrupt
	{
		uart_status&=0xff7f;
		uart_data=IORD_ALTERA_AVALON_UART_RXDATA(UART_UI_BASE); //讀取接收數據
		if(UiRx_cnt>=200)
			UiRx_cnt=200;
		if(uart_data=='$')
			UiRx_cnt=0;
		else if(uart_data=='#')
		{
			if(UiRx_cnt<200)
			{
				for(i=0;i<UiRx_cnt;i++)
					uirxb[i]=UiRx[i];
				uirxb_cnt=UiRx_cnt;
				uirxb_f=1;


			}
		}
		else
		{
			UiRx[UiRx_cnt]=uart_data;
			UiRx_cnt++;

		}
	}
	if(uart_status&0x40)//tx_interrupt
	{
		uart_status&=0xffbf;
		if(uarttx_ui_start_f)
		{
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_UI_BASE,uarttx_ui_buf[uarttx_ui_cnt]);
			uarttx_ui_cnt++;
			if(uarttx_ui_cnt>=uarttx_ui_len)
				uarttx_ui_start_f=0;
		}
		else
		{
			IOWR_ALTERA_AVALON_UART_CONTROL(UART_UI_BASE, 0x0080); //允許接收發送中斷
		}

	}
	uart_status&=0x00c0;
	IOWR_ALTERA_AVALON_UART_STATUS(UART_UI_BASE, uart_status);
}

int uart_ui_send_byte(unsigned char data)
{
	while (!(IORD_ALTERA_AVALON_UART_STATUS(UART_UI_BASE) & 0x0040));
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_UI_BASE, data);
	return 0;
}


U8 uart_ui_send_pack(U8 force_tx)
{
	if(!force_tx)
	{
		if(uarttx_ui_start_f)
			return 0;
	}
	uarttx_ui_start_f=1;
	uarttx_ui_cnt=0;
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_UI_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
	return 1;
}







//txmod:
//bit10=chksum byte,00=no chksum,01:1byte,10:2byte
//bit32=package index byte,00=no index,01:1byte,10:2byte;
//bit4=command byte,0=:1byte,1:2byte;
//bit5=data lenght,0=:not exist,1:exist;

U8 uart_rx_proc(char* rxbuf,U16 rx_cnt,UART_RXTEMP* urpack)
{
	U16 i;
	U16 chksum;
	U16 chksum_r;
	U8 chksum_f;
	U8 data_len;
	U8 chr;
	U16 pcnt;
	U8 txmod;


	pcnt=0;
	if(asc2byte(&rxbuf[pcnt],&chr))
		return 1;
	pcnt+=2;
	txmod=chr;
	chksum=chr;

	if(txmod&0x10)
	{
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->command=chr*256;
		chksum+=chr;
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->command+=chr;
		chksum+=chr;
	}
	else
	{
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->command=chr;
		chksum+=chr;
	}

	data_len=0;
	if(txmod&0x20)
	{
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		data_len=chr;
		chksum+=chr;
	}

	if(txmod&0x08)
	{
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->pindex=chr*256;
		chksum+=chr;
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->pindex+=chr;
		chksum+=chr;
	}
	else if(txmod&0x04)
	{
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->pindex=chr;
		chksum+=chr;
	}
	else
	{
		urpack->pindex=0;
	}
//=======================================
	chksum_f=0;
	if(txmod&0x02)
	{
		if(asc2byte(&rxbuf[rx_cnt-4],&chr))
			return 1;
		chksum_r=chr*256;
		if(asc2byte(&rxbuf[rx_cnt-2],&chr))
			return 1;
		chksum_r+=chr;
		rx_cnt-=4;
		chksum_f=1;
	}
	else if(txmod&0x01)
	{
		if(asc2byte(&rxbuf[rx_cnt-2],&chr))
			return 1;
		chksum_r=chr;
		rx_cnt-=2;
		chksum_f=1;
	}
	else
	{

	}
//=======================================
	i=0;
	urpack->len=0;
	while(pcnt<rx_cnt)
	{
		if(asc2byte(&rxbuf[pcnt],&chr))
			return 1;
		pcnt+=2;
		urpack->data[i]=chr;
		chksum+=chr;
		urpack->len++;
		i++;
		if(urpack->len>=64)
			return 1;
	}

	if(chksum_f==1 && chksum!=chksum_r)
		return 1;
	if(data_len!=0 && urpack->len!=data_len )
		return 1;
	return 0;
}






U8 asc2numc(U8 cp)
{
	if(cp<='9' && cp>='0')
		return (cp-'0');
	else if(cp<='F' && cp>='A')
		return (cp-'A'+10);
	else if(cp<='f' && cp>='a')
		return (cp-'a'+10);
	return 0;
}



U16 asc2num(U8* cp)
{
	if(cp[0]<='9' && cp[0]>='0')
		return (cp[0]-'0');
	else if(cp[0]<='F' && cp[0]>='A')
		return (cp[0]-'A'+10);
	else if(cp[0]<='f' && cp[0]>='a')
		return (cp[0]-'a'+10);
	else
	{
		return 0xff00;
	}
}


U8 asc2nip(U8* cp,U8* retb)
{
	U16 buf;
	buf=asc2num(&cp[0]);
	if(buf&0xfff0)
	  return 1;
	else
	{
	  *retb=buf;
	  return 0;
	}
}

U8 asc2byte(U8* cp,U8* retb)
{
	U16 buf;
	buf=asc2num(&cp[0]);
	if(buf&0xfff0)
	  return 1;
	else
	  *retb=buf*16;
	buf=asc2num(&cp[1]);
	if(buf&0xfff0)
	  return 1;
	else
	{
	  *retb=*retb+buf;
	  return 0;
	}
}

U8 asc2word(U8* cp,U16* retb)
{
	U16 buf;
	buf=asc2num(&cp[0]);
	if(buf&0xfff0)
	  return 1;
	else
	  *retb=buf*4096;

	buf=asc2num(&cp[1]);
	if(buf&0xfff0)
	  return 1;
	else
	  *retb=*retb+buf*256;

	buf=asc2num(&cp[2]);
	if(buf&0xfff0)
	  return 1;
	else
	  *retb=*retb+buf*16;

	buf=asc2num(&cp[3]);
	if(buf&0xfff0)
	  return 1;
	else
	{
	  *retb=*retb+buf;
	  return 0;
	}
}


void initTIMER(void)
{
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_US_BASE, 0x00);//清除Timer中斷暫存器
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_US_BASE,InTimer1);//設置Timer1週期，這裡是輸入周期數
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_US_BASE, InTimer1 >> 16);
	alt_irq_register(TIMER_US_IRQ, TIMER_US_BASE, ISR_timer1);//註冊Timer中斷
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_US_BASE, 0x07);//允許Timer中斷
}


static void ISR_timer1(void *context, alt_u32 id)
{
	static int uart_dcc_tx_tim;
	static int keybo_chk_tim;
	static int flash_tim;
	static int swpab_tim;
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_US_BASE, 0x00);
	uart_dcc_tx_tim++;
	int buf;
	if(uart_dcc_tx_tim>=125)
	{
		uart_dcc_tx_tim=0;
		uart_dcc_tx_f=1;
	}

	keybo_chk_tim++;
	if(keybo_chk_tim>=4)
	{
		keybo_chk_tim=0;
		keybo_chk_f=1;
	}
///////////////////////////////
	GPSrx_con_tim++;
	if(GPSrx_con_tim>60000)
		GPSrx_con_tim=60000;

	rxa_con_tim++;
	rxb_con_tim++;
	urx_dcc_la_tim++;
	urx_dcc_rf1_tim++;
	urx_dcc_rf2_tim++;
	urx_dcc_fb1_tim++;
	urx_dcc_fb2_tim++;
	urx_dcc_422_tim++;
	urx_dcc_gps_tim++;


	if(rxa_con_tim>100)
	{
		if(rdata_value[sync_sys]==0)
			rdata_value[con_bc_stat]=0;
		if(rdata_value[sync_sys]==1)
			rdata_value[con_bc_stat]=0;
	}
	if(rxb_con_tim>100)
	{
		if(rdata_value[sync_sys]==0)
			rdata_value[con_be_stat]=0;
		if(rdata_value[sync_sys]==2)
			rdata_value[con_be_stat]=0;
	}

	brf_err_tim++;
	muicon_tim++;
	if(muicon_tim>8000)
	{
		if(rdata_value[sync_sys]==0)//system_set
			rdata_value[con_ab_stat]=0;	//ma-mb
		if(rdata_value[sync_sys]==1)//system_set
			rdata_value[con_cd_stat]=0;//mc-md
		if(rdata_value[sync_sys]==2)//system_set
			rdata_value[con_ef_stat]=0;//me-mf
	}
	flash_tim++;
	if(flash_tim>=400)
	{
		flash_f=!flash_f;
		flash_tim=0;
		txreq_tim++;
		if(txreq_err_tim<60000)
			txreq_err_tim++;
	}
	rwmem_led_tim++;
	if(rwmem_led_tim>400)
		led_flag&=0xfe;
/*
	if(rdata_value[hc_op_mod]==0)
	{
		HA_rf_en=0;
		HB_rf_en=0;
	}
	else if(rdata_value[hc_op_mod]==2)
	{
		HA_rf_en=0;
		HB_rf_en=0;
	}
	else if((rdata_value[hc_op_mod]==3)&&(rdata_value[hc_vgin_stat]==0))
	{
		HA_rf_en=0;
		HB_rf_en=0;
	}
	else
	{
*/
	if(rdata_value[att_targ_mod]==0)
	{
		HA_rf_en=1;
		HB_rf_en=1;
	}
	else
	{
		swpab_tim++;
		if(swpab_tim>=400)
		{
			swpab_tim=0;
			swpab_cnt++;
			buf=rdata_value[rdata_value[att_targ_mod]+att_targ_mod];
			if(swpab_cnt>=buf)
			{
				swpab_cnt=0;
				if(HA_rf_en)
				{
					HA_rf_en=0;
					HB_rf_en=1;
				}
				else
				{
					HA_rf_en=1;
					HB_rf_en=0;
				}
			 }
		}

	}
}

void keybo_prg(void)
{
	static U8 keypre;
	U8 keybuf;
	static U32 yeskey_tim;
	static U32 nokey_tim;

	keyp_f=0;
	keyr_f=0;
	keyc_f=0;

	if(!keybo_chk_f)
		return;
	keybo_chk_f=0;

	IOWR( POUT_LED_BASE, 0, testdata );
	testdata^=0xff;


	keybuf = IORD_8DIRECT(PIN_SW_BASE,0);
	keybuf ^= 255;
	keybuf &= 0x0f;
	if(keybuf==0)
	{
		nokey_tim++;
		yeskey_tim=0;
		if(nokey_tim==10)
		{
			keyr_f=keypre;
			keypre=0;
		}

	}
	else
	{
		nokey_tim=0;
		if(keybuf==keypre)
		{
			yeskey_tim++;
			if(yeskey_tim==4)
			{
				keyp_f=keybuf;
			}
			if(yeskey_tim==1000)
			{
				keyc_f=keybuf;
			}
		}
		else
		{
			yeskey_tim=0;
			keypre=keybuf;
		}
	}

}




//===============================================================================
void dispatch_uirx(void)
{
	int i;
	U8 mda_setdata_cnt;
	U8 mdb_setdata_cnt;
	U16 buf;


	if(urui_pack.command==0x00a0)//load set
	{
		if(urui_pack.len>32)
			return;
		if(urui_pack.pindex>=32)
			return;
		if(urui_pack.pindex<16)
		{
			for(i=0;i<urui_pack.len;i++)
			{
				rdata_init[i+urui_pack.pindex*32]=urui_pack.data[i];
			}
		}
		else
		{
			for(i=0;i<urui_pack.len;i++)
			{
				rdata_flag[i+(urui_pack.pindex-16)*32]=urui_pack.data[i];
			}
		}
		if(urui_pack.pindex==31)
		{
//			if(!rdata_init_f)
//			{
				for(i=0;i<512;i++)
				{
					rdata_value[i]=rdata_init[i];
					rdata_temp[i]=rdata_init[i];
				}
				rdata_init_f=1;
//			}
		}
		uruitx_reced();
	}


	if(urui_pack.command==0x00a1)//trans data
	{
		if(urui_pack.len!=32)
			return;
		mda_setdata_cnt=0;
		mdb_setdata_cnt=0;
		for(i=0;i<8;i++)
		{
			if(urui_pack.data[i*4+0]==0xb0)//sync data
			{
				buf=urui_pack.data[i*4+1]&0x01;
				buf=buf<<8;
				rdata_value[buf+urui_pack.data[i*4+2]]=urui_pack.data[i*4+3];
			}


			if(urui_pack.data[i*4+0]==0xb1)//set data
			{
				if(rdata_value[sync_sys]==0)//system_set
				{
					if(urui_pack.data[i*4+1]&0x8)
					{
						buf=urui_pack.data[i*4+1]&0x01;
						buf=buf<<8;
						rdata_value[buf+urui_pack.data[i*4+2]]=urui_pack.data[i*4+3];
						renew_ui_rdata_f=1;
						renew_ui_rdata_cnt=buf+urui_pack.data[i*4+2];
					}
					if(urui_pack.data[i*4+1]&0x30)
					{
						mda_setdata_cnt=mda_setdata_cnt&7;
						mda_setdata[mda_setdata_cnt*4+0]=urui_pack.data[i*4+0];
						mda_setdata[mda_setdata_cnt*4+1]=urui_pack.data[i*4+1];
						mda_setdata[mda_setdata_cnt*4+2]=urui_pack.data[i*4+2];
						mda_setdata[mda_setdata_cnt*4+3]=urui_pack.data[i*4+3];
						mda_setdata_cnt++;
					}
					if(urui_pack.data[i*4+1]&0xc0)
					{
						mdb_setdata_cnt=mdb_setdata_cnt&7;
						mdb_setdata[mdb_setdata_cnt*4+0]=urui_pack.data[i*4+0];
						mdb_setdata[mdb_setdata_cnt*4+1]=urui_pack.data[i*4+1];
						mdb_setdata[mdb_setdata_cnt*4+2]=urui_pack.data[i*4+2];
						mdb_setdata[mdb_setdata_cnt*4+3]=urui_pack.data[i*4+3];
						mdb_setdata_cnt++;
					}
				}
				else
				{
					if(urui_pack.data[i*4+1]&0x8)
					{
						buf=urui_pack.data[i*4+1]&0x01;
						buf=buf<<8;
						rdata_value[buf+urui_pack.data[i*4+2]]=urui_pack.data[i*4+3];
					}
					if(urui_pack.data[i*4+1]&0x30)
					{
						mds_setdata_cnt=mds_setdata_cnt&7;
						mds_setdata[mds_setdata_cnt*4+0]=urui_pack.data[i*4+0];
						mds_setdata[mds_setdata_cnt*4+1]=urui_pack.data[i*4+1];
						mds_setdata[mds_setdata_cnt*4+2]=urui_pack.data[i*4+2];
						mds_setdata[mds_setdata_cnt*4+3]=urui_pack.data[i*4+3];
						mds_setdata_cnt++;
					}
				}
			}


			if(urui_pack.data[i*4+0]==0xb2)//tx command
			{
				if(rdata_value[sync_sys]==0)//system_set
				{
					if(urui_pack.data[i*4+1]&0x8)
					{
						loadtx_uarttx_cmd(urui_pack.data[i*4+3]);
					}
					if(urui_pack.data[i*4+1]&0x30)
					{
						mda_setdata_cnt=mda_setdata_cnt&7;
						mda_setdata[mda_setdata_cnt*4+0]=urui_pack.data[i*4+0];
						mda_setdata[mda_setdata_cnt*4+1]=urui_pack.data[i*4+1];
						mda_setdata[mda_setdata_cnt*4+2]=urui_pack.data[i*4+2];
						mda_setdata[mda_setdata_cnt*4+3]=urui_pack.data[i*4+3];
						mda_setdata_cnt++;
					}
					if(urui_pack.data[i*4+1]&0xc0)
					{
						mdb_setdata_cnt=mdb_setdata_cnt&7;
						mdb_setdata[mdb_setdata_cnt*4+0]=urui_pack.data[i*4+0];
						mdb_setdata[mdb_setdata_cnt*4+1]=urui_pack.data[i*4+1];
						mdb_setdata[mdb_setdata_cnt*4+2]=urui_pack.data[i*4+2];
						mdb_setdata[mdb_setdata_cnt*4+3]=urui_pack.data[i*4+3];
						mdb_setdata_cnt++;
					}
				}
				else
				{
					if(urui_pack.data[i*4+1]&0x8)
					{
						loadtx_uarttx_cmd(urui_pack.data[i*4+3]);
					}
					if(urui_pack.data[i*4+1]&0x30)
					{
						mds_setdata_cnt=mds_setdata_cnt&7;
						mds_setdata[mds_setdata_cnt*4+0]=urui_pack.data[i*4+0];
						mds_setdata[mds_setdata_cnt*4+1]=urui_pack.data[i*4+1];
						mds_setdata[mds_setdata_cnt*4+2]=urui_pack.data[i*4+2];
						mds_setdata[mds_setdata_cnt*4+3]=urui_pack.data[i*4+3];
						mds_setdata_cnt++;
					}
				}
			}



			if(urui_pack.data[i*4+0]==0xb3)//command	test;
			{
				TXrx_data5=urui_pack.data[i*4+2];
				TXrx_data4=urui_pack.data[i*4+3];
				TXrx_f=1;
			}

			if(urui_pack.data[i*4+0]==0xb4)//set rdata_flag
			{
				buf=urui_pack.data[i*4+1]&0x01;
				buf=buf<<8;
				rdata_flag[buf+urui_pack.data[i*4+2]]=urui_pack.data[i*4+3];
			}
		}
		uruitx_recedtx();
	}


}
//===============================================================================



//test_ok
//===============================================================================
void uruitx_reced(void)
{
	loadui_uarttx_start();
	loadui_uarttx_byte(0x02);
	loadui_uarttx_byte(0x20);
	loadui_uarttx_byte(urui_pack.command);
	loadui_uarttx_byte(urui_pack.len);
	loadui_uarttx_byte(urui_pack.pindex);
	loadui_uarttx_end();
	uart_ui_send_pack(1);
}
//===============================================================================


void uruitx_recedtx(void)
{
	int i,j;
	loadui_uarttx_start();
	loadui_uarttx_byte(0x02);
	loadui_uarttx_byte(0x21);
	loadui_uarttx_byte(urui_pack.command);
	loadui_uarttx_byte(urui_pack.len);
	loadui_uarttx_byte(urui_pack.pindex);
	///////////////////////////////////////////
	j=0;
	for(i=0;i<8;i++)
	{
		if(ui_setdata[i*4+0]!=0)
		{
			loadui_uarttx_byte(ui_setdata[i*4+0]);
			loadui_uarttx_byte(ui_setdata[i*4+1]);
			loadui_uarttx_byte(ui_setdata[i*4+2]);
			loadui_uarttx_byte(ui_setdata[i*4+3]);
			ui_setdata[i*4+0]=0;
			j++;
		}
	}
	if(j<8)
	{
		if(renew_ui_rdata_f)
		{
			renew_ui_rdata_f=0;
			rdata_trans_cnt=renew_ui_rdata_cnt;
		}
		for(i=0;i<rdata_lim_k;i++)
		{
			if(rdata_flag[rdata_trans_cnt]&0x02)
			{

				if(rdata_trans_cnt==hc_pus_pri1)
					hc_pri0_buf=rdata_value[hc_pus_pri0];
				if(rdata_trans_cnt==s1_pus_pri1)
					s1_pri0_buf=rdata_value[s1_pus_pri0];
				if(rdata_trans_cnt==s2_pus_pri1)
					s2_pri0_buf=rdata_value[s2_pus_pri0];
				loadui_uarttx_byte(0xb0);		//sync data
				loadui_uarttx_byte(rdata_trans_cnt>>8);
				loadui_uarttx_byte(rdata_trans_cnt);
				if(rdata_trans_cnt==hc_pus_pri0)
					  loadui_uarttx_byte(hc_pri0_buf);
				else if(rdata_trans_cnt==s1_pus_pri0)
					  loadui_uarttx_byte(s1_pri0_buf);
				else if(rdata_trans_cnt==s2_pus_pri0)
					  loadui_uarttx_byte(s2_pri0_buf);
				else
				  loadui_uarttx_byte(rdata_value[rdata_trans_cnt]);
				j++;
			}
			rdata_trans_cnt++;
			if(rdata_trans_cnt>=rdata_lim_k)
			{
				rdata_trans_cnt=0;
				pri_pcnt=0;
			}
			if(j>=8)
				break;
		}
	}
	loadui_uarttx_end();
	uart_ui_send_pack(1);
}


//===============================================================================



void uart_dcc_proc(void)
{
	uart_dcc_tx_cnt++;
	if(uart_dcc_tx_cnt>=7)
		uart_dcc_tx_cnt=0;
	switch(uart_dcc_tx_cnt)
	{
		case 0:
			loadtx_uartdcc_la();
			break;
		case 1:
			loadtx_uartdcc_rf1();
			break;
		case 2:
			loadtx_uartdcc_rf2();
			break;
		case 3:
			loadtx_uartdcc_fb1();
			break;
		case 4:
			loadtx_uartdcc_fb2();
			break;
		case 5:
			loadtx_uartdcc_422();
			break;
		case 6:
			loadtx_uartdcc_gps();
			break;
	}
}


void loadtx_uartdcc_la(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x02)
	  err_f=1;
  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='A';
  uarttx_dcc_buf[uarttx_dcc_len++]='B';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+rdata_value[la_grp2];;
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xab;
  uarttx_dcc_idcnt=0;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}

void loadtx_uartdcc_rf1(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x04)
	  err_f=1;
  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='A';
  uarttx_dcc_buf[uarttx_dcc_len++]='C';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+rdata_value[la_grp2];;
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xac;
  uarttx_dcc_idcnt=1;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}

void loadtx_uartdcc_rf2(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x08)
	  err_f=1;

  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='A';
  uarttx_dcc_buf[uarttx_dcc_len++]='D';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]='0';
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xad;
  uarttx_dcc_idcnt=2;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}

void loadtx_uartdcc_fb1(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x10)
	  err_f=1;


  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='A';
  uarttx_dcc_buf[uarttx_dcc_len++]='E';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]='0';
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xae;
  uarttx_dcc_idcnt=3;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}

void loadtx_uartdcc_fb2(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x20)
	  err_f=1;



  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='A';
  uarttx_dcc_buf[uarttx_dcc_len++]='F';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]='0';
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xaf;
  uarttx_dcc_idcnt=4;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}


void loadtx_uartdcc_422(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x40)
	  err_f=1;

  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='B';
  uarttx_dcc_buf[uarttx_dcc_len++]='0';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]=hextoasc[rdata_value[test_422_mod]&0x0f];
  uarttx_dcc_buf[uarttx_dcc_len++]=hextoasc[rdata_value[test_422_set]>>4];
  uarttx_dcc_buf[uarttx_dcc_len++]=hextoasc[rdata_value[test_422_set]&0x0f];;
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xb0;
  uarttx_dcc_idcnt=5;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}


void loadtx_uartdcc_gps(void)
{
  U8 err_f=0;
  if(rdata_value[err_flag]&0x80)
	  err_f=1;


  uarttx_dcc_len=0;
  uarttx_dcc_buf[uarttx_dcc_len++]='$';
  uarttx_dcc_buf[uarttx_dcc_len++]='B';
  uarttx_dcc_buf[uarttx_dcc_len++]='1';
  uarttx_dcc_buf[uarttx_dcc_len++]='0'+err_f;
  uarttx_dcc_buf[uarttx_dcc_len++]='0';
  uarttx_dcc_buf[uarttx_dcc_len++]='#';
  uarttx_dcc_start_f=1;
  uarttx_dcc_cnt=0;
  uarttx_dcc_id=0xb1;
  uarttx_dcc_idcnt=6;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x00c0); //允許"TX通訊"接收發送中斷
}




void loadtx_uarttx_cmd(U8 cmd)
{
  uarttx_tx_len=0;
  uarttx_tx_buf[uarttx_tx_len++]=0x55;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x01;
  uarttx_tx_buf[uarttx_tx_len++]=0x00;
  uarttx_tx_buf[uarttx_tx_len++]=cmd;
  uarttx_tx_buf[uarttx_tx_len++]=0x10;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x55;

  uarttx_tx_buf[uarttx_tx_len++]=0x55;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x01;
  uarttx_tx_buf[uarttx_tx_len++]=0x00;
  uarttx_tx_buf[uarttx_tx_len++]=cmd;
  uarttx_tx_buf[uarttx_tx_len++]=0x20;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x55;

  uarttx_tx_buf[uarttx_tx_len++]=0x55;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x01;
  uarttx_tx_buf[uarttx_tx_len++]=0x00;
  uarttx_tx_buf[uarttx_tx_len++]=cmd;
  uarttx_tx_buf[uarttx_tx_len++]=0x30;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x55;

  uarttx_tx_start_f=1;
  uarttx_tx_cnt=0;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_TX_BASE, 0x00c0); //允許"TX通訊"接收發送中斷

}




void loadtx_uarttx_req(void)
{
  if(uarttx_tx_start_f!=0)
	  return;

  uarttx_tx_len=0;
  uarttx_tx_buf[uarttx_tx_len++]=0x55;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x02;
  uarttx_tx_buf[uarttx_tx_len++]=0x00;
  uarttx_tx_buf[uarttx_tx_len++]=0x00;
  uarttx_tx_buf[uarttx_tx_len++]=0x00;
  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
  uarttx_tx_buf[uarttx_tx_len++]=0x55;

//  uarttx_tx_buf[uarttx_tx_len++]=0x55;
//  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
//  uarttx_tx_buf[uarttx_tx_len++]=0x02;
//  uarttx_tx_buf[uarttx_tx_len++]=0x00;
//  uarttx_tx_buf[uarttx_tx_len++]=0x00;
//  uarttx_tx_buf[uarttx_tx_len++]=0x20;
//  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
//  uarttx_tx_buf[uarttx_tx_len++]=0x55;

//  uarttx_tx_buf[uarttx_tx_len++]=0x55;
//  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
//  uarttx_tx_buf[uarttx_tx_len++]=0x02;
//  uarttx_tx_buf[uarttx_tx_len++]=0x00;
//  uarttx_tx_buf[uarttx_tx_len++]=0x00;
//  uarttx_tx_buf[uarttx_tx_len++]=0x30;
//  uarttx_tx_buf[uarttx_tx_len++]=0xaa;
//  uarttx_tx_buf[uarttx_tx_len++]=0x55;

  txreq_tim=0;
  txreq_err_tim=0;
  uarttx_tx_start_f=1;
  uarttx_tx_cnt=0;
  IOWR_ALTERA_AVALON_UART_CONTROL(UART_TX_BASE, 0x00c0); //允許"TX通訊"接收發送中斷

}





void loadui_uarttx_start(void)
{
  uarttx_ui_len=0;
  uarttx_ui_chksum=0;
  uarttx_ui_buf[uarttx_ui_len++]='$';
}
void loadui_uarttx_end(void)
{
  uarttx_ui_buf[uarttx_ui_len++]=hextoasc[(uarttx_ui_chksum>>12)&0x0f];
  uarttx_ui_buf[uarttx_ui_len++]=hextoasc[(uarttx_ui_chksum>>8)&0x0f];
  uarttx_ui_buf[uarttx_ui_len++]=hextoasc[(uarttx_ui_chksum>>4)&0x0f];
  uarttx_ui_buf[uarttx_ui_len++]=hextoasc[uarttx_ui_chksum&0x0f];
  uarttx_ui_buf[uarttx_ui_len++]='#';
}
void loadui_uarttx_byte(unsigned char ch)
{
  uarttx_ui_buf[uarttx_ui_len++]=hextoasc[(ch>>4)&0x0f];
  uarttx_ui_buf[uarttx_ui_len++]=hextoasc[ch&0x0f];
  uarttx_ui_chksum+=ch;
}
//===============================================================================


// ************** initSPI **************************
void initSPI(void)
{
	IOWR_32DIRECT(SPI_RFA_BASE,12,0xc0);  //設定RF0_SPI的control暫存器。
	//0xc0即bit6=1,bit7=1
	//Bit6 ITYDY 置1則致能發送等待旗標位(TRDY)
	//Bit7 IRYDY 置1則致能接收等待旗標位(RRDY)
	IOWR_32DIRECT(SPI_RFA_BASE,8,0x60);  //RF0_SPI的status暫存器清除為零。
	//0x60即bit5=1,bit6=1
	//Bit5 TMT傳輸移位暫存器空旗標。(Master mode下)當TMT=0時表示傳輸正在進行。
	//TMT=1表示移位暫存器空。
	//Bit6 TRDY發送等待。TRDY=1表示txdata暫存器空，可以發起新的一次寫入。
	//==================================================================
	IOWR_32DIRECT(SPI_RFB_BASE,12,0xc0);  //設定RF1_SPI的control暫存器。
	IOWR_32DIRECT(SPI_RFB_BASE,8,0x60);  //RF1_SPI的status暫存器清除為零。
}
















// ************** initRF **************************
void initRF(void)
{
//	RF0_Reset();
//	RF1_Reset();
//	while(1)
//	{
//		RF0_TESTID();
//		RF1_TESTID();
//	}

	if(rf_enableA_f)
	{
		RF0_Reset();
		usleep(10000);
		RF0_TESTID();
		usleep(10000);
		RF0_Config();
		usleep(10000);
		RF0_Cal();
	}
	if(rf_enableB_f)
	{
		RF1_Reset();
		usleep(10000);
		RF1_TESTID();
		usleep(10000);
		RF1_Config();
		usleep(10000);
		RF1_Cal();
	}
}



// ************** RF0_WriteReg **************************
void RF0_WriteReg(alt_u8 addr, alt_u8 dataByte)
{
	pob_flag|=0x01;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out
	// initiate SS_N
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);

	//send address byte
	addr |= 0x00; //bit cmd=0,r/w=0
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, addr);
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));
	//send data byte
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, dataByte);
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));

	// release SS_N
    IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);

	pob_flag&=0xfe;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in

}



// ************** RF0_WriteReg **************************
void RF1_WriteReg(alt_u8 addr, alt_u8 dataByte)
{
	pob_flag|=0x02;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out
	// initiate SS_N
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);

	//send address byte
	addr |= 0x00; //bit cmd=0,r/w=0
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, addr);
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));
	//send data byte
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, dataByte);
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));

	// release SS_N
    IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);

	pob_flag&=0xfd;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in

}










// ************** Reset_RF0 **************************
void RF0_Reset(void)
{
	RF0_WriteReg(MODE_REG, 0x00); //reset @RF0
}
// ************** Reset_RF1 **************************
void RF1_Reset(void)
{
	RF1_WriteReg(MODE_REG, 0x00); //reset @RF0
}





// ************************************************
alt_u8 ByteSend0(alt_u8 txdata)
{
	pob_flag|=0x01;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out
	// transmit a byte, set write enable latch
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, txdata);
	// wait for all bytes to be send
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));
	pob_flag&=0xfe;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in
	return 0;
}


// ************************************************
alt_u8 ByteSend1(alt_u8 txdata)
{
	pob_flag|=0x02;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out
	// transmit a byte, set write enable latch
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, txdata);
	// wait for all bytes to be send
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));
	pob_flag&=0xfd;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in
	return 0;
}










// ************** ByteRead0 **************************
alt_u8 ByteRead0(void)
{
	alt_u8 tmp;

	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, 0x00);//產生MISO運作需要的CLK
	//read data
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	tmp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFA_BASE);
	return tmp;
}

// ************** ByteRead0 **************************
alt_u8 ByteRead1(void)
{
	alt_u8 tmp;

	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, 0x00);//產生MISO運作需要的CLK
	//read data
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	tmp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFB_BASE);
	return tmp;
}



// ************** RF0_WriteID **************************
U8 RF0_TESTID(void)
{
	alt_u8 addr;
	alt_u8 temp;
	alt_u8 error;
	addr = IDCODE_REG; //send address 0x06, bit cmd=0, r/w=0
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N
	ByteSend0(addr);
	ByteSend0(0xAB);
	ByteSend0(0xCD);
	ByteSend0(0xEF);
    IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N
	addr = IDCODE_REG | 0x40; //send address 0x06, bit cmd=0, r/w=1
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N
	ByteSend0(addr);
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	temp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFA_BASE);
	temp = ByteRead0();
	error=0;
	if(temp!=0xab)
		error=1;
	temp = ByteRead0();
	if(temp!=0xcd)
		error=1;
	temp = ByteRead0();
	if(temp!=0xef)
		error=1;
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N

	return error;

}





// ************** RF0_WriteID **************************
U8 RF1_TESTID(void)
{
	alt_u8 addr;
	alt_u8 temp;
	alt_u8 error;
	addr = IDCODE_REG; //send address 0x06, bit cmd=0, r/w=0
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N
	ByteSend1(addr);
	ByteSend1(0xAB);
	ByteSend1(0xCD);
	ByteSend1(0xEF);
    IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N
	addr = IDCODE_REG | 0x40; //send address 0x06, bit cmd=0, r/w=1
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N
	ByteSend1(addr);
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	temp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFB_BASE);
	temp = ByteRead1();
	error=0;
	if(temp!=0xab)
		error=1;
	temp = ByteRead1();
	if(temp!=0xcd)
		error=1;
	temp = ByteRead1();
	if(temp!=0xef)
		error=1;
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N

	return error;


}















// ************** RF0_WriteReg_Page **************************
void RF0_WriteReg_Page(alt_u8 addr, alt_u8 wbyte, alt_u8 page)
{
	RF0_WriteReg(RFANALOG_REG, A7130Config[RFANALOG_REG] | PageTab[page]);//page select
	RF0_WriteReg(addr, wbyte);
}
// ************** RF0_WriteReg_Page **************************
void RF1_WriteReg_Page(alt_u8 addr, alt_u8 wbyte, alt_u8 page)
{
	RF1_WriteReg(RFANALOG_REG, A7130Config[RFANALOG_REG] | PageTab[page]);//page select
	RF1_WriteReg(addr, wbyte);
}




// ************** RF0_Config **************************
void RF0_Config(void)
{
	alt_u8 i;


	//0x00 mode register, for reset
	//0x05 fifo data register
	//0x06 id code register
	//0x3F USID register, read only
	//0x36 key data, 16 bytes
	//0x3D FCB register,4 bytes

	for (i=0x01; i<=0x04; i++)
		RF0_WriteReg(i, A7130Config[i]);

	for (i=0x07; i<=0x29; i++)
		RF0_WriteReg(i, A7130Config[i]);

	for (i=0; i<=7; i++)//0x2A DAS
		RF0_WriteReg_Page(0x2A, A7130_Addr2A_Config[i], i);

	for (i=0x2B; i<=0x35; i++)
		RF0_WriteReg(i, A7130Config[i]);

	RF0_WriteReg(0x37, A7130Config[0x37]);

	for (i=0; i<=4; i++)//0x38 ROM
		RF0_WriteReg_Page(0x38, A7130_Addr38_Config[i], i);

	for (i=0x39; i<=0x3C; i++)
		RF0_WriteReg(i, A7130Config[i]);

	RF0_WriteReg(0x3E, A7130Config[0x3E]);

}



// ************** RF0_Config **************************
void RF1_Config(void)
{
	alt_u8 i;


	//0x00 mode register, for reset
	//0x05 fifo data register
	//0x06 id code register
	//0x3F USID register, read only
	//0x36 key data, 16 bytes
	//0x3D FCB register,4 bytes

	for (i=0x01; i<=0x04; i++)
		RF1_WriteReg(i, A7130Config[i]);

	for (i=0x07; i<=0x29; i++)
		RF1_WriteReg(i, A7130Config[i]);

	for (i=0; i<=7; i++)//0x2A DAS
		RF1_WriteReg_Page(0x2A, A7130_Addr2A_Config[i], i);

	for (i=0x2B; i<=0x35; i++)
		RF1_WriteReg(i, A7130Config[i]);

	RF1_WriteReg(0x37, A7130Config[0x37]);

	for (i=0; i<=4; i++)//0x38 ROM
		RF1_WriteReg_Page(0x38, A7130_Addr38_Config[i], i);

	for (i=0x39; i<=0x3C; i++)
		RF1_WriteReg(i, A7130Config[i]);

	RF1_WriteReg(0x3E, A7130Config[0x3E]);

}




// ************** StrobeCmd0 **************************
void StrobeCmd0(alt_u8 cmd)
{

	pob_flag|=0x01;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out


	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N

	// transmit a byte, set write enable latch
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, cmd);

	// wait for all bytes to be send
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));

	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N

	pob_flag&=0xfe;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in
}

// ************** StrobeCmd0 **************************
void StrobeCmd1(alt_u8 cmd)
{

	pob_flag|=0x02;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out


	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N

	// transmit a byte, set write enable latch
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, cmd);

	// wait for all bytes to be send
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));

	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N

	pob_flag&=0xfd;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in
}



// ************** Err_State **************************
void Err_State(void)
{
	int i;
	i++;
	//ERR display
	//Error Proc...
	//...
}


// ************** RF0_ReadReg **************************
alt_u8 RF0_ReadReg(alt_u8 addr)
{
	alt_u8 tmp;

	pob_flag|=0x01;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out

	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N
	addr |= 0x40; //bit cmd=0,r/w=1
	// transmit a byte, set write enable latch
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, addr);

	// wait for all bytes to be send
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));

	pob_flag&=0xfe;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in

	usleep(100);

	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFA_BASE, 0x00);//產生MISO運作需要的CLK
	//read data
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	tmp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFA_BASE);
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFA_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	tmp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFA_BASE);

	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFA_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N
	return tmp;
}



// ************** RF0_ReadReg **************************
alt_u8 RF1_ReadReg(alt_u8 addr)
{
	alt_u8 tmp;

	pob_flag|=0x02;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為Out

	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// initiate SS_N
	addr |= 0x40; //bit cmd=0,r/w=1
	// transmit a byte, set write enable latch
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, addr);

	// wait for all bytes to be send
	while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_TMT_MSK));

	pob_flag&=0xfd;
	IOWR(POUT_B_BASE,0,pob_flag ); //MDIO設定為in

	usleep(100);

	IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_RFB_BASE, 0x00);//產生MISO運作需要的CLK
	//read data
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	tmp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFB_BASE);
	while (!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_RFB_BASE) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	tmp = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_RFB_BASE);

	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_RFB_BASE, ~ALTERA_AVALON_SPI_CONTROL_SSO_MSK);// release SS_N
	return tmp;
}


// ************** CHGroupCal0 **************************
void CHGroupCal0(alt_u8 ch)
{
	alt_u8 tmp;
	alt_u8 vb,vbcf;
	alt_u8 deva,adev;

	RF0_WriteReg(PLL1_REG, ch);

	RF0_WriteReg(CALIBRATION_REG, 0x0C);
	do
	{
		tmp = RF0_ReadReg(CALIBRATION_REG);
		tmp &= 0x0C;
	}
	while (tmp);

	//for check
	tmp = RF0_ReadReg(VCOCAL1_REG);
	vb = tmp & 0x07;
	vbcf = (tmp >>3) & 0x01;

	tmp = RF0_ReadReg(VCODEVCAL1_REG);
	deva = tmp;

	tmp = RF0_ReadReg(VCODEVCAL2_REG);
	adev = tmp;

	if (vbcf)
		Err_State();//error
}

// ************** CHGroupCal0 **************************
void CHGroupCal1(alt_u8 ch)
{
	alt_u8 tmp;
	alt_u8 vb,vbcf;
	alt_u8 deva,adev;

	RF1_WriteReg(PLL1_REG, ch);

	RF1_WriteReg(CALIBRATION_REG, 0x0C);
	do
	{
		tmp = RF1_ReadReg(CALIBRATION_REG);
		tmp &= 0x0C;
	}
	while (tmp);

	//for check
	tmp = RF1_ReadReg(VCOCAL1_REG);
	vb = tmp & 0x07;
	vbcf = (tmp >>3) & 0x01;

	tmp = RF1_ReadReg(VCODEVCAL1_REG);
	deva = tmp;

	tmp = RF1_ReadReg(VCODEVCAL2_REG);
	adev = tmp;

	if (vbcf)
		Err_State();//error
}





// ************** RF0_Cal **************************
void RF0_Cal(void)
{
	alt_u8 tmp, loopCnt;
	alt_u8 fb, fbcf, fcd, vcb, vccf, rhc, rlc;

	StrobeCmd0(CMD_PLL); //calibration @PLL state

	//calibration IF procedure
	loopCnt = 0;
	RF0_WriteReg(RXGAIN3_REG, 0x1D);
	while(1)
	{
		RF0_WriteReg(CALIBRATION_REG, 0x02);
		do
		{
			tmp = RF0_ReadReg(CALIBRATION_REG);
			tmp &= 0x02;
		}
		while(tmp);

		tmp = RF0_ReadReg(IFCAL1_REG);
		fb = tmp & 0x0F;
		if (fb>3 && fb<9)
//		if (fb>0 && fb<9)
			break;

		loopCnt++;
		if (loopCnt>=10)
			break;
	}
	RF0_WriteReg(RXGAIN3_REG, 0x3D);

	if (loopCnt >=10)
		Err_State();

	//calibration RSSI, VCC procedure
	RF0_WriteReg(CALIBRATION_REG, 0x11);
	do
	{
		tmp = RF0_ReadReg(CALIBRATION_REG);
		tmp &= 0x11;
	}
	while(tmp);

	//calibration VBC,VDC procedure
	CHGroupCal0(30); //calibrate channel group Bank I
	CHGroupCal0(90); //calibrate channel group Bank II
	CHGroupCal0(150); //calibrate channel group Bank III
	StrobeCmd0(CMD_STBY); //return to STBY state

	//for check
	tmp = RF0_ReadReg(IFCAL1_REG);
	fb = tmp & 0x0F;
	fbcf = (tmp >>4) & 0x01;

	tmp = RF0_ReadReg(IFCAL2_REG);
	fcd = tmp & 0x1F;

	tmp = RF0_ReadReg(VCOCCAL_REG) & 0x1F;
	vcb = tmp & 0x0F;
	vccf = (tmp >>4) & 0x01;

	rhc = RF0_ReadReg(RXGAIN2_REG);
	rlc = RF0_ReadReg(RXGAIN3_REG);

	if (fbcf || vccf)
		Err_State();
}


// ************** RF0_Cal **************************
void RF1_Cal(void)
{
	alt_u8 tmp, loopCnt;
	alt_u8 fb, fbcf, fcd, vcb, vccf, rhc, rlc;

	StrobeCmd1(CMD_PLL); //calibration @PLL state

	//calibration IF procedure
	loopCnt = 0;
	RF1_WriteReg(RXGAIN3_REG, 0x1D);
	while(1)
	{
		RF1_WriteReg(CALIBRATION_REG, 0x02);
		do
		{
			tmp = RF1_ReadReg(CALIBRATION_REG);
			tmp &= 0x02;
		}
		while(tmp);

		tmp = RF1_ReadReg(IFCAL1_REG);
		fb = tmp & 0x0F;
		if (fb>3 && fb<9)
//		if (fb>0 && fb<9)
			break;

		loopCnt++;
		if (loopCnt>=10)
			break;
	}
	RF1_WriteReg(RXGAIN3_REG, 0x3D);

	if (loopCnt >=10)
		Err_State();

	//calibration RSSI, VCC procedure
	RF1_WriteReg(CALIBRATION_REG, 0x11);
	do
	{
		tmp = RF1_ReadReg(CALIBRATION_REG);
		tmp &= 0x11;
	}
	while(tmp);

	//calibration VBC,VDC procedure
	CHGroupCal1(30); //calibrate channel group Bank I
	CHGroupCal1(90); //calibrate channel group Bank II
	CHGroupCal1(150); //calibrate channel group Bank III
	StrobeCmd1(CMD_STBY); //return to STBY state

	//for check
	tmp = RF1_ReadReg(IFCAL1_REG);
	fb = tmp & 0x0F;
	fbcf = (tmp >>4) & 0x01;

	tmp = RF1_ReadReg(IFCAL2_REG);
	fcd = tmp & 0x1F;

	tmp = RF1_ReadReg(VCOCCAL_REG) & 0x1F;
	vcb = tmp & 0x0F;
	vccf = (tmp >>4) & 0x01;

	rhc = RF1_ReadReg(RXGAIN2_REG);
	rlc = RF1_ReadReg(RXGAIN3_REG);

	if (fbcf || vccf)
		Err_State();
}






// ************** SetCH0 **************************
void SetCH0(alt_u8 ch)
{
	RF0_WriteReg(PLL1_REG, ch); //RF freq = RFbase + (CH_Step * ch)
}
// ************** SetCH0 **************************
void SetCH1(alt_u8 ch)
{
	RF1_WriteReg(PLL1_REG, ch); //RF freq = RFbase + (CH_Step * ch)
}








static void UART_TX_Irq_Handler(void *context, alt_u32 id)
{


	int uart_status;
	static U8 rx_data[8];


	uart_status=IORD_ALTERA_AVALON_UART_STATUS(UART_TX_BASE);
	if(uart_status&0x80)//rx_interrupt
	{
		uart_status&=0xff7f;
		rx_data[0]=rx_data[1];
		rx_data[1]=rx_data[2];
		rx_data[2]=rx_data[3];
		rx_data[3]=rx_data[4];
		rx_data[4]=rx_data[5];
		rx_data[5]=rx_data[6];
		rx_data[6]=rx_data[7];
		rx_data[7]=IORD_ALTERA_AVALON_UART_RXDATA(UART_TX_BASE); //讀取接收數據
		if(rx_data[0]==0x55 && rx_data[1]==0xAA)
		if(rx_data[6]==0xAA && rx_data[7]==0x55)
		{
			TXrx_data4=rx_data[4];
			TXrx_data5=rx_data[5];
			TXrx_f=1;
		}
	}
	if(uart_status&0x40)//tx_interrupt
	{
		uart_status&=0xffbf;
		if(uarttx_tx_start_f)
		{
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_TX_BASE,uarttx_tx_buf[uarttx_tx_cnt]);
			uarttx_tx_cnt++;
			if(uarttx_tx_cnt>=uarttx_tx_len)
				uarttx_tx_start_f=0;
		}
		else
		{
			IOWR_ALTERA_AVALON_UART_CONTROL(UART_TX_BASE, 0x0080); //允許接收發送中斷
		}
	}
	uart_status&=0x00c0;
	IOWR_ALTERA_AVALON_UART_STATUS(UART_TX_BASE, uart_status);




}

/***********************************************/
static void UART_GPS_Irq_Handler(void *context, alt_u32 id)
{
	static U8 rx_data[100];
	static U8 rx_data_cnt=0;
	static U8 rxbuf[2];
	int uart_status;
	static U8 rxda[100];
	static U8 rxdacnt=0;
	U8 flag;
	U16 buf;

	int i=0;

	uart_status=IORD_ALTERA_AVALON_UART_STATUS(UART_GPS_BASE);
	if(uart_status&0x80)//rx_interrupt
	{
		rxbuf[1]=rxbuf[0];
		rxbuf[0]=IORD_ALTERA_AVALON_UART_RXDATA(UART_GPS_BASE); //讀取接收數據
		if(rxbuf[1]==0x8f && rxbuf[0]==0xAC)
			rx_data_cnt=0;
		rx_data[rx_data_cnt]=rxbuf[0];
		rx_data_cnt++;
		if(rx_data_cnt>=100)
			rx_data_cnt=99;
		if(rx_data_cnt==67)
		{
			GPSrx_f=1;
			GPSrx_data[0]=rx_data[2];
			GPSrx_data[1]=rx_data[12];
			for(i=0;i<24;i++)
				GPSrx_data[i+2]=rx_data[i+36];
		}

		if(rxbuf[0]==0x24)
			rxdacnt=0;
		rxda[rxdacnt]=rxbuf[0];
		rxdacnt++;
		if(rxdacnt>=100)
			rxdacnt=99;
		if(rxbuf[1]==0x0d && rxbuf[0]==0x0a)
		{
			flag=0;
			i=0;
			if(rxda[i++]!='$')
				flag++;
			if(rxda[i++]!='G')
				flag++;
			if(rxda[i++]!='P')
				flag++;
			if(rxda[i++]!='G')
				flag++;
			if(rxda[i++]!='G')
				flag++;
			if(rxda[i++]!='A')
				flag++;
			if(flag==0)
			{
			  if(rxdacnt<70)
			  {
  			    GPSrx_data[0]=1;
				goto exitgps;
			  }
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  //time
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }


			  GPSrx_data[2]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[2];
			  GPSrx_data[3]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[3];
			  i++;
			  GPSrx_data[4]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[4];
			  GPSrx_data[5]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[5];
			  GPSrx_data[6]=asc2numc(rxda[i++]);
			  flag=GPSrx_data[6];
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  //NS
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  GPSrx_data[10]=asc2numc(rxda[i++]);
			  GPSrx_data[11]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[11];
			  GPSrx_data[12]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[12];
			  i++;
			  GPSrx_data[13]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[13];
			  GPSrx_data[14]=asc2numc(rxda[i++])*10+asc2numc(rxda[i++]);
			  flag=GPSrx_data[14];
			  GPSrx_data[15]=asc2numc(rxda[i++]);
			  flag=GPSrx_data[15];
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  //EW
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  GPSrx_data[1]=20;
			  if(rxda[i++]!='0')
				  GPSrx_data[0]=0;
			  else
				  GPSrx_data[0]=1;
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  //settler quantity
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  //HDOP
			  while(rxda[i++]!=',')
			  {
				if(i>=100)
					goto exitgps;
			  }
			  buf=0;
			  while(rxda[i]!='.')
			  {
				 buf=buf*10+asc2numc(rxda[i++]);
				 if(i>=100)
					 break;
			  }

			  GPSrx_data[18]=(buf>>8)&255;
			  GPSrx_data[19]=buf&255;
 			  GPSrx_f=1;











			}
		}
	}
exitgps:
	IOWR_ALTERA_AVALON_UART_STATUS(UART_GPS_BASE, 0x00);
}






static void UART_DCC_Irq_Handler(void *context, alt_u32 id)
{
	int uart_status;
	static U8 rx_data[8];
	U8 chr_id,chr_rv;
	uart_status=IORD_ALTERA_AVALON_UART_STATUS(UART_DCC_BASE);
	if(uart_status&0x80)//rx_interrupt
	{
		uart_status&=0xff7f;
		rx_data[0]=rx_data[1];
		rx_data[1]=rx_data[2];
		rx_data[2]=rx_data[3];
		rx_data[3]=rx_data[4];
		rx_data[4]=rx_data[5];
		rx_data[5]=rx_data[6];
		rx_data[6]=rx_data[7];
		rx_data[7]=IORD_ALTERA_AVALON_UART_RXDATA(UART_DCC_BASE); //讀取接收數據
		if(rx_data[2]=='$' && rx_data[7]=='#')
		{
			if(!asc2byte(&rx_data[3],&chr_id))
			if(!asc2byte(&rx_data[5],&chr_rv))
			if(chr_id==uarttx_dcc_id)
			{
				switch(uarttx_dcc_idcnt)
				{
					case 0:
						if(chr_rv==rdata_value[la_grp2])
						{
							rdata_value[la_grp3]|=1;
							urx_dcc_la_tim=0;
						}
						break;
					case 1:
						rdata_value[la_grp3]|=2;
						urx_dcc_rf1_tim=0;
						break;
					case 2:
						rdata_value[la_grp3]|=4;
						urx_dcc_rf2_tim=0;
						break;
					case 3:
						rdata_value[la_grp3]|=8;
						urx_dcc_fb1_tim=0;
						break;
					case 4:
						rdata_value[la_grp3]|=16;
						urx_dcc_fb2_tim=0;
						break;
					case 5:
						rdata_value[la_grp3]|=32;
						rdata_value[test_422_val]=chr_rv;
						urx_dcc_422_tim=0;
						break;
					case 6:
						rdata_value[la_grp3]|=64;
						urx_dcc_gps_tim=0;
						break;



				}




			}
		}
	}
	if(uart_status&0x40)//tx_interrupt
	{
		uart_status&=0xffbf;
		if(uarttx_dcc_start_f)
		{
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_DCC_BASE,uarttx_dcc_buf[uarttx_dcc_cnt]);
			uarttx_dcc_cnt++;
			if(uarttx_dcc_cnt>=uarttx_dcc_len)
				uarttx_dcc_start_f=0;
		}
		else
		{
			IOWR_ALTERA_AVALON_UART_CONTROL(UART_DCC_BASE, 0x0080); //允許接收發送中斷
		}
	}
	uart_status&=0x00c0;
	IOWR_ALTERA_AVALON_UART_STATUS(UART_DCC_BASE, uart_status);
}









