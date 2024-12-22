#include <io.h>
#include <delay.h>
#include <i2c.h>
#include <alcd.h>
#include <stdio.h>
#include <ds1307.h>
#include <glcd.h>
#include <hardware.c>
#include <stdio.h>
#include <stdlib.h>
#define  BT1 PINB.2

int d;
bool dem_nguoc = false ;
int time_left = 9;
unsigned int dem; 
unsigned int dem_1;
unsigned char hour, minute, second;
int hour_1, hour_2, min_1, min_2;
char buf[20];
void hienThi(int i, unsigned int j);
//1ms
interrupt [TIM0_OVF] void timer0_ovf_isr(void){
 TCNT0=0x06;
 dem++;
 if (dem == 1000) d=1;
 if (dem == 2000){dem = 0; d=0;}
 if  (dem_nguoc) {
    dem_1++;
   
    if (time_left == 0) {  
        lcd_clear();
        dem_nguoc = false;   
        dem_1 = 0;
        lcd_putsf("Cho an");
    }
    if (dem_1 == 1000) {
        dem_1 = 0;
        lcd_putchar(time_left +'0');
        time_left -= 1;  
        hienThi(time_left, 0b01000000);  
        lcd_clear();
        lcd_putsf("Chuan bi");
    }    
 }
}

void hienThi(int i, unsigned int j){
 if(i==0){
  PORTF = 0x80 + j;
  PORTD = 0xF9;
  PORTA = 0x7F;
 }             
 if(i==1){
  PORTF = 0x86 + j;
  PORTD = 0xFF;
  PORTA = 0x7F;
 } 
 if(i==2){
  PORTF = 0x84 + j;
  PORTD = 0xF1;
  PORTA = 0xFF;
 } 
 if(i==3){
  PORTF = 0x84 + j;
  PORTD = 0xF5;
  PORTE = 0x00;
  PORTA = 0x7F;
 } 
 if(i==4){
  PORTF = 0x82 + j;
  PORTD = 0xF7;
  PORTA = 0x7F;
 } 
 if(i==5){
  PORTF = 0xA0 + j;
  PORTD = 0xF5;
  PORTA = 0x7F;
 } 
 if(i==6){
  PORTF = 0xA0 + j;
  PORTD = 0xF1;
  PORTA = 0x7F;
 } 
 if(i==7){
  PORTF = 0x84 + j;
  PORTD = 0xFF;
  PORTA = 0x7F;
 } 
 if(i==8){
  PORTF = 0x80 + j;
  PORTD = 0xF1;
  PORTA = 0x7F;
 } 
 if(i==9){
  PORTF = 0x80 + j;
  PORTD = 0xF5;
  PORTA = 0x7F;
 } 
 if(i==10){
  PORTF = 0x26 + j;
  PORTD = 0xFF;
  PORTA = 0xFF;
 } 
      
 delay_ms(1);  
}

void hien_thi_lcd(int hour,int minute ) {

        lcd_gotoxy(0, 0);  
    
        lcd_putchar((hour / 10) + 48);
        lcd_putchar((hour % 10) + 48);
        lcd_putchar(':');
        lcd_putchar((minute / 10) + 48);
        lcd_putchar((minute % 10) + 48);
}
void hien_thi_glcd(int hour,int minute) {
        glcd_clear(); 

        itoa(hour_1, buf);
        glcd_outtext(buf);
        itoa(hour_2, buf);
        glcd_outtext(buf); 
        glcd_putchar(':');
        itoa(min_1, buf);
        glcd_outtext(buf);
        itoa(min_2, buf);
        glcd_outtext(buf);  
      //  delay_ms(250);
}


void main(void)
{   DDRB.2 = 0;
    PORTB.2 = 1;
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA.7 = 1;
    ASSR = 0 << AS0;
    TCCR0 = (0 << WGM00)|(0<<COM01)|(0<<COM00)|(0<<WGM01)|(0<<CS02)|(1<<CS01)|(1<<CS00);
    TCNT0 = 0x06;
    OCR0 = 0x00;
    TIMSK = (0<<OCIE2)|(0<<TOIE2)|(0<<TICIE1)|(0<<OCIE1A)|(0<<OCIE1B)|(0<<TOIE1)|(0<<OCIE0)|(1<<TOIE0);
    ETIMSK = (0<<TICIE3)|(0<<OCIE3A)|(0<<OCIE3B)|(0<<TOIE3)|(0<<OCIE3C)|(0<<OCIE1C);
    i2c_init();
    init_lcd();
    rtc_init(3,1,0);
    init_glcd(4,66);
  
    #asm("sei")
   
    while (1)
    {     
        if (BT1 == 0) {
             hienThi(time_left, 0b01000000);     
            dem_nguoc = true;        
            delay_ms(250);
        
        }
         
          // Please write your application code here
          
         if (!dem_nguoc) {      
             rtc_get_time(&hour, &minute, &second);
            hour_1 = hour/10;
            hour_2 = hour - hour_1*10;
            min_1 = minute/10;
            min_2 = minute - min_1*10;
            hien_thi_lcd(hour,minute );
             hien_thi_glcd(hour,minute);
            hienThi(hour_1, 0b00000001);
            hienThi(hour_2, 0b00001000);
            
            if(d == 1) {
                hienThi(10, 0b00001000);
            } 
            hienThi(min_1, 0b00010000);
            hienThi(min_2, 0b01000000);       
            } 
    }        
    
}