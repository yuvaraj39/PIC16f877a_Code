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
#include<stdio.h>
#define _XTAL_FREQ 4000000
void UART_config(void);
signed int count=0; //this variable will incremented or decremented on encoder rotation

#define REA PORTBbits.RB4 // Rotary encoder pin definition
#define REB PORTBbits.RB5


void pbchange(void )
{
unsigned char state;
static unsigned char oldstate; // this variable need to be static as it has to retain the value between calls
__delay_ms(1); // delay for 1ms here for debounce
state= (REB<<1 | REA); // combine the pin status and assign to state variable
if(oldstate==0x0){
if( state ==0x1)
{
count--; //decrement the count
//lcd_out(1,8,count);
}else if( state == 0x2)
{

count++; //decrement the count
//Lcd_Out(1,8,count);
}

}
oldstate = state; // store the current state value to oldstate value this value will be used in next call

PORTB = PORTB; // read or Any read or write of PORTB,This will end the mismatch condition
INTCONbits.RBIF = 0; // clear the porb change intrrupt flag
}
void __interrupt() isr(void)
{

if(INTCONbits.RBIF==1) //check for PortB change interrupt
{
pbchange(); //call the routine
}
}

void main() {
TRISBbits.TRISB4=1; // set rotary encoder pins to input
TRISBbits.TRISB5=1;
OPTION_REGbits.nRBPU =0; // enable pullups
INTCONbits.RBIF = 0; // clear the interrupt flag
INTCONbits.RBIE = 1; // enable PORTB change interrupt
INTCONbits.GIE = 1; // enable the global interrupt
INTCONbits.PEIE=1;
UART_config();
while(1)
{
//UART1_Write_Text(text);
printf("%d \n",count);
printf("\r");

}

}
void UART_config(){
     SYNC = 0;//
     BRGH = 0;//
     TXEN = 1; // Enable UART Transmission 
    TRISCbits.TRISC6=0;  
    RCSTA = 0X90;//
     TXSTAbits.TXEN = 1;//
    SPBRG = 6;
    }
 void putch(char data){
  while(!TRMT);
  TXREG = data;     
  __delay_ms(2);
}