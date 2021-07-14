/*
 * File:   LCD_interface.c
 * Author: Yuvaraj
 *
 * Created on September 6, 2020, 12:00 PM
 */
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000
#define RS RB0
#define EN RB1
 
void lcd_data(unsigned char data);
void cmd(unsigned char cmd);
void string(const char *str);
void delay(int d);
void lcd_init(void);
void main()                    
{
    TRISB=0x00;
    TRISD=0x00;

       

   lcd_init();
        
        while(1){
        cmd(0x84);
        string("Yameruka");
        delay(1000);
        cmd(0xC4);
        string("Bayaman");
        delay(1000);
        cmd(0x01);
        }
    
}
void lcd_init()
{
    cmd(0x01);
    delay(5);
    cmd(0x38);
    delay(2);
    cmd(0x0E);
    delay(2);
    cmd(0x06);
    delay(2);
    
}
void lcd_data(unsigned char data)
{
    PORTD=data;
    EN=0;
    RS=1;
    delay(1);
    EN=1;
    delay(1);
    EN=0;
  
}
void cmd(unsigned char cmd)
{
 PORTD=cmd;
 EN=0;
 RS=0;
 delay(10);
 EN=1;
 delay(10);
 EN=0;
}

void string(const char *str)
{
    while(*str)
    {
        lcd_data(*str);
        delay(1000);
        str++;
    }
    
}

void delay(int d)
{
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.T0SE=1;
    OPTION_REGbits.PS0=0;
    OPTION_REGbits.PS1=0;
    OPTION_REGbits.PS2=1;
    OPTION_REGbits.PSA=0;
    TMR0=100;
    for(int i=1;i<=d;i++){
    while(INTCONbits.TMR0IF==0);
    TMR0=101;
    INTCONbits.TMR0IF=0;
    }
}