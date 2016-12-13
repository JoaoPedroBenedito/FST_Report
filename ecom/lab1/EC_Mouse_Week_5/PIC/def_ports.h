#include <p18cxxx.h>

// DIRECTIONS
#define PINPUT			0xFF
#define POUTPUT			0x00
#define INPUT			1
#define OUTPUT			0
#define PON				0xFF
#define POFF			0x00
#define HIGH			1
#define LOW				0
#define ON				1
#define OFF				0

// PORT A
	// DIRECTION
#define DDRA			TRISA
#define DRA0			TRISAbits.TRISA0
#define DRA1			TRISAbits.TRISA1
#define DRA2			TRISAbits.TRISA2
#define DRA3			TRISAbits.TRISA3
#define DRA4			TRISAbits.TRISA4
#define DRA5			TRISAbits.TRISA5
#define DRA6			TRISAbits.TRISA6
	// OUTPUT
#define PPA				PORTA
#define PA0				PORTAbits.RA0
#define PA1				PORTAbits.RA1
#define PA2				PORTAbits.RA2
#define PA3				PORTAbits.RA3
#define PA4				PORTAbits.RA4
#define PA5				PORTAbits.RA5
#define PA6				PORTAbits.RA6
	// READ
#define LLPA			LATA
#define LPA0			LATAbits.LATA0
#define LPA1			LATAbits.LATA1
#define LPA2			LATAbits.LATA2
#define LPA3			LATAbits.LATA3
#define LPA4			LATAbits.LATA4
#define LPA5			LATAbits.LATA5
#define LPA6			LATAbits.LATA6

// PORT B
	// DIRECTION
#define DDRB			TRISB
#define DRB0			TRISBbits.TRISB0
#define DRB1			TRISBbits.TRISB1
#define DRB2			TRISBbits.TRISB2
#define DRB3			TRISBbits.TRISB3
#define DRB4			TRISBbits.TRISB4
#define DRB5			TRISBbits.TRISB5
#define DRB6			TRISBbits.TRISB6
#define DRB7			TRISBbits.TRISB7
	// OUTPUT
#define PPB				PORTB
#define PB0				PORTBbits.RB0
#define PB1				PORTBbits.RB1
#define PB2				PORTBbits.RB2
#define PB3				PORTBbits.RB3
#define PB4				PORTBbits.RB4
#define PB5				PORTBbits.RB5
#define PB6				PORTBbits.RB6
#define PB7				PORTBbits.RB7
	// READ
#define LLPB			LATB
#define LPB0			LATBbits.LATB0
#define LPB1			LATBbits.LATB1
#define LPB2			LATBbits.LATB2
#define LPB3			LATBbits.LATB3
#define LPB4			LATBbits.LATB4
#define LPB5			LATBbits.LATB5
#define LPB6			LATBbits.LATB6
#define LPB7			LATBbits.LATB7

// PORT C
	// DIRECTION
#define DDRC			TRISC
#define DRC0			TRISCbits.TRISC0
#define DRC1			TRISCbits.TRISC1
#define DRC2			TRISCbits.TRISC2
#define DRC6			TRISCbits.TRISC6
#define DRC7			TRISCbits.TRISC7
	// OUTPUT
#define PPC				PORTC
#define PC0				PORTCbits.RC0
#define PC1				PORTCbits.RC1
#define PC2				PORTCbits.RC2
#define PC4				PORTCbits.RC4
#define PC5				PORTCbits.RC5
#define PC6				PORTCbits.RC6
#define PC7				PORTCbits.RC7
	// READ
#define LLPC			LATC
#define LPC0			LATCbits.LATC0
#define LPC1			LATCbits.LATC1
#define LPC2			LATCbits.LATC2
#define LPC6			LATCbits.LATC6
#define LPC7			LATCbits.LATC7

// PORT D
	// DIRECTION
#define DDRD			TRISD
#define DRD0			TRISDDits.TRISD0
#define DRD1			TRISDbits.TRISD1
#define DRD2			TRISDbits.TRISD2
#define DRD3			TRISDbits.TRISD3
#define DRD4			TRISDbits.TRISD4
#define DRD5			TRISDbits.TRISD5
#define DRD6			TRISDbits.TRISD6
#define DRD7			TRISDbits.TRISD7
	// OUTPUT
#define PPD				PORTD
#define PD0				PORTDbits.RD0
#define PD1				PORTDbits.RD1
#define PD2				PORTDbits.RD2
#define PD3				PORTDbits.RD3
#define PD4				PORTDbits.RD4
#define PD5				PORTDbits.RD5
#define PD6				PORTDbits.RD6
#define PD7				PORTDbits.RD7
	// READ
#define LLPD			LATD
#define LPD0			LATDbits.LATD0
#define LPD1			LATDbits.LATD1
#define LPD2			LATDbits.LATD2
#define LPD3			LATDbits.LATD3
#define LPD4			LATDbits.LATD4
#define LPD5			LATDbits.LATD5
#define LPD6			LATDbits.LATD6
#define LPD7			LATDbits.LATD7

// PORT E
	// DIRECTION
#define DDRE			TRISE
#define DRE0			TRISEbits.TRISE0
#define DRE1			TRISEbits.TRISE1
#define DRE2			TRISEbits.TRISE2
	// OUTPUT
#define PPE				PORTE
#define PE0				PORTEbits.RE0
#define PE1				PORTEbits.RE1
#define PE2				PORTEbits.RE2
#define PE3				PORTEbits.RE3
	// READ
#define LLPE			LATE
#define LPE0			LATEbits.LATE0
#define LPE1			LATEbits.LATE1
#define LPE2			LATEbits.LATE2
