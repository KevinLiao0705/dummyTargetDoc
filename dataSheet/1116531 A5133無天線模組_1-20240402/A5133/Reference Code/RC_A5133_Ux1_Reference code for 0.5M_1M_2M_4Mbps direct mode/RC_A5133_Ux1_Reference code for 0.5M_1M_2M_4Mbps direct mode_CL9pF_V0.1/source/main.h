#ifndef _MAIN_H_
#define _MAIN_H_

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

#endif