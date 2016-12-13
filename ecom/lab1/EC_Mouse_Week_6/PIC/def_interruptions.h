#include <p18cxxx.h>

// INTERRUPTIONS
#define INTPRIEN		RCONbits.IPEN
	// CONFIGURATION
#define HIGHP			1
#define LOWP			0
#define REDGE			1
#define FEDGE			0
#define ENABLE			1
#define DISABLE			0
#define YESINT			1
#define NOINT			0
	// INTCON
#define GINTEN			INTCONbits.GIE
#define PEIE			INTCONbits.PEIE
#define TMR0IE			INTCONbits.TMR0IE
#define INT0IE			INTCONbits.INT0IE
#define RBIE			INTCONbits.RBIE
#define TMR0IF			INTCONbits.TMR0IF
#define INT0IF			INTCONbits.INT0IF
#define RBIF			INTCONbits.RBIF
	// INTCON2
#define RBPU			INTCON2bits.RBPU
#define INTEDG0			INTCON2bits.INTEDG0
#define INTEDG1			INTCON2bits.INTEDG1
#define INTEDG2			INTCON2bits.INTEDG2
#define TMR0IP			INTCON2bits.TMR0IP
#define RBIP			INTCON2bits.RBIP
	// INTCON3
#define INT2IP			INTCON3bits.INT2IP
#define INT1IP			INTCON3bits.INT1IP
#define INT2IE			INTCON3bits.INT2IE
#define INT1IE			INTCON3bits.INT1IE
#define INT2IF			INTCON3bits.INT2IF
#define INT1IF			INTCON3bits.INT1IF
