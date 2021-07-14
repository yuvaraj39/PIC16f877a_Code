/*
 * File:   seg7_Int.c
 * Author: Yuvaraj
 *
 * Created on August 29, 2020, 9:49 PM
 */
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = ON         // Data EEPROM Memory Code Protection bit (Data EEPROM code-protected)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000
unsigned char count[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
void main(void) {
    TRISD=0X00;
    TRISA=0X00;
    OPTION_REG=0x080;
    TRISBbits.TRISB0=1;
    INTCONbits.GIE=1;
    INTCONbits.INTE=1;
    INTCONbits.PEIE=1;
    while(1)
    {
        for(int i=0;i<=9;i++)
        {
            PORTD=count[i];
            __delay_ms(500);
        }
    }
    
    return;
}
void __interrupt() isr()
{
   
        
        PORTD=0x80;PORTA=0X01;
        __delay_ms(500);
        PORTA=0X00;
        INTCONbits.INTF=0;
    
}
