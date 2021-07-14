/*
 * File:   4bit_mode_lcd.c
 * Author: Yuvaraj
 *
 * Created on September 12, 2020, 4:38 PM
 */
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = ON         // Data EEPROM Memory Code Protection bit (Data EEPROM code-protected)
#pragma config WRT = 256        // Flash Program Memory Write Enable bits (0000h to 00FFh write-protected; 0100h to 1FFFh may be written to by EECON control)
#pragma config CP = ON          // Flash Program Memory Code Protection bit (All program memory code-protected)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

                              //4 bit LCD Mode//
#include <xc.h>
#define _XTAL_FREQ 20000000

#define RS PORTBbits.RB0  /*PIN 0 of PORTB is assigned for register select Pin of LCD*/
#define EN PORTBbits.RB1  /*PIN 1 of PORTB is assigned for enable Pin of LCD */
#define ldata PORTD  /*PORTB(PB4-PB7) is assigned for LCD Data Output*/ 
#define LCD_Port TRISD  /*define macros for PORTB Direction Register*/

/*********************Proto-Type Declaration*****************************/

void MSdelay(unsigned int );       /*Generate delay in ms*/
void LCD_Init();                   /*Initialize LCD*/
void LCD_Command(unsigned char );  /*Send command to LCD*/
void LCD_Char(unsigned char x);    /*Send data to LCD*/
void LCD_String(const char *);     /*Display data string on LCD*/
void LCD_String_xy(char, char , const char *);
void LCD_Clear();                  /*Clear LCD Screen*/
int i=0;
unsigned char mod1;
unsigned char mod2;
int change1;
int change2;
int main(void)
{   TRISB=0x00; 
	LCD_Init();  /*Initialize LCD to 5*8 matrix in 4-bit mode*/  
    while(1){
        __delay_ms(300);
        i=i+1;

        
         if(i<10){
            change1=i;
        }
        else if(i<100){
           change1=i%10;
        change2=i/10; 
        }   
        else if(i>=100)
        {
            change1=0;
            change2=0;
            i=0;
        }
        
        mod1=change1+0X30;
        mod2=change2+0X30;
             LCD_Command(0x80);
             LCD_Char('k');
             LCD_Char(mod1);
    }
}

/****************************Functions********************************/

void LCD_Init()
{
    LCD_Port = 0;       /*PORT as Output Port*/
    MSdelay(15);        /*15ms,16x2 LCD Power on delay*/
    LCD_Command(0x02);  /*send for initialization of LCD 
                          for nibble (4-bit) mode */
    LCD_Command(0x28);  /*use 2 line and 
                          initialize 5*8 matrix in (4-bit mode)*/
	LCD_Command(0x01);  /*clear display screen*/
    LCD_Command(0x0c);  /*display on cursor off*/
	LCD_Command(0x06);  /*increment cursor (shift cursor to right)*/	   
}

void LCD_Command(unsigned char cmd )
{
	ldata = (ldata & 0x0f) |(0xF0 & cmd);  /*Send higher nibble of command first to PORT*/ 
	RS = 0;  /*Command Register is selected i.e.RS=0*/ 
	EN = 1;  /*High-to-low pulse on Enable pin to latch data*/ 
	__delay_ms(1);
	EN = 0;
	__delay_ms(1);
    ldata = (ldata & 0x0f) | (cmd<<4);  /*Send lower nibble of command to PORT */
	EN = 1;
	__delay_ms(1);
	EN = 0;
	__delay_ms(1);
}

void LCD_Char(unsigned char dat)
{
	ldata = (ldata & 0x0f) | (0xF0 & dat);  /*Send higher nibble of data first to PORT*/
	RS = 1;  /*Data Register is selected*/
	EN = 1;  /*High-to-low pulse on Enable pin to latch data*/
	__delay_ms(10);
	EN = 0;
	__delay_ms(10);
    ldata = (ldata & 0x0f) | (dat<<4);  /*Send lower nibble of data to PORT*/
	EN = 1;  /*High-to-low pulse on Enable pin to latch data*/
	__delay_ms(10);
	EN = 0;
	__delay_ms(10);
}
void LCD_String(const char *msg)
{
	while((*msg)!=0)
	{		
	  LCD_Char(*msg);    //send the data present in pointer to function
	  msg++;	         //increments the address
    }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f);  /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f);  /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    

    LCD_String(msg);

}
void LCD_Clear()
{
   	LCD_Command(0x01);  /*clear display screen*/
    __delay_ms(3);
}

void MSdelay(unsigned int val)
{
 unsigned int i,j;
 for(i=0;i<val;i++)
     for(j=0;j<165;j++);  /*This count Provide delay of 1 ms for 8MHz Frequency */
 }