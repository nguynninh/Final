/*
 * may_giat.c
 *
 * Created: 12/28/2022 2:23:27 AM
 * Author: Who
 */

#include <io.h>
#include <delay.h>
#include <glcd.h>
#include <font5x7.h>
#include <stdio.h>
#include  <alcd.h>
int curSpeed = 0;
bool led = false;
int counter = 0;
char speedText[16];
// Interrupt c?a timer1
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    // Reinitialize Timer1 value
    TCNT1H = 0xE0C0 >> 8;
    TCNT1L = 0xE0C0 & 0xff;

    // ?i?u khi?n LED nh?p nh?y
    if (led) {
        if (counter == 2000)
            counter = 0;

        if (counter == 0) {
            PORTD.4 = 0;
            PORTD.5 = 1;
        }

        if (counter == 1000) {
            PORTD.5 = 0;
            PORTD.4 = 1;
        }
        counter++;
    }
}

// H?m di?u ch?nh t?c d? d?ng co
void adjustSpeed(int speed) {
    OCR0 = 255 - speed;
}

// H?m c?p nh?t t?c d? d?ng co
void speedUpdater(int option) {
    int newSpeed;
    if (curSpeed == 0) {
        curSpeed = 63;
    } else {
        if (option == 20)
            newSpeed = curSpeed + curSpeed / 5;
        else if (option == 100)
            newSpeed = curSpeed * 2;

        if (newSpeed > 255)
            curSpeed = 255;
        else
            curSpeed = newSpeed;
    }
    adjustSpeed(curSpeed);
}

// H?m x? l? ch? d? v?t
void vat() {
    int second = -1;

    // B?t role 2 trong 2 gi?y
    PORTC.2 = 1;
    delay_ms(2000);
    PORTC.2 = 0;

    // Quay d?ng co trong 3 gi?y
    while (second != 3) {
        speedUpdater(100);
        second++;
        delay_ms(1000);
    }

    curSpeed = 0;
    adjustSpeed(0);
    led = true;
}

// H?m x? l? ch? d? gi?t
void giat() {
    int second = -1;

    // B?t role 1 trong 2 gi?y
    PORTC.3 = 1;
    delay_ms(2000);
    PORTC.3 = 0;

    // Quay d?ng co trong 10 gi?y
    while (second != 10) {
        lcd_clear(); 
        lcd_putsf("Dang giat");  
        glcd_clear(); 
        glcd_outtextxy(10, 30, "Dang giat"); 
        lcd_gotoxy(0,1);
        speedUpdater(20);

        // Hi?n th? t?c d? hi?n t?i l?n GLCD
        
        sprintf(speedText, "Toc do: %d", curSpeed);
        glcd_outtextxy(10, 40, speedText); 
      
        lcd_puts(speedText);  
                                   
        second++;
        delay_ms(1000);
    }
    

}

void main(void)
{
    // Kh?i t?o GLCD
    GLCDINIT_t glcd_init_data;
    glcd_init_data.font = font5x7;
    glcd_init_data.temp_coef = 140;
    glcd_init_data.bias = 4;
    glcd_init_data.vlcd = 66;
    glcd_init(&glcd_init_data);
    lcd_init(16);

    // K?o l?n BT1
    DDRB.2 = 0;
    PORTB.2 = 1;

    // K?o l?n BT2
    DDRB.3 = 0;
    PORTB.3 = 1;

    // Role 1 v? 2
    DDRC.3 = 1;
    DDRC.2 = 1;

    // LED
    DDRD.5 = 1;
    DDRD.4 = 1;

    // Kh?i t?o d?ng co v? timer
    DDRB.4 = 1;
    DDRD.7 = 1;
    ASSR = 0 << AS0;
    TCCR0 = (1 << WGM00) | (1 << COM01) | (1 << COM00) | (0 << WGM01) | (1 << CS02) | (0 << CS01) | (0 << CS00);
    TCNT0 = 0x00;
    adjustSpeed(255);
    TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << COM1C1) | (0 << COM1C0) | (0 << WGM11) | (0 << WGM10);
    TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10);
    TCNT1H = 0xE0;
    TCNT1L = 0xC0;

    // Kh?i t?o ng?t cho timer1
    TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (1 << TOIE1) | (0 << OCIE0) | (0 << TOIE0);
    ETIMSK = (0 << TICIE3) | (0 << OCIE3A) | (0 << OCIE3B) | (0 << TOIE3) | (0 << OCIE3C) | (0 << OCIE1C);
    #asm("sei")

    // Hi?n th? menu ban d?u
  //  glcd_outtextxy(10, 10, "MAY GIAT");
    //glcd_outtextxy(10, 20, "BT1. GIAT");
    glcd_outtextxy(10, 30, "May giat"); 
    lcd_putsf("May giat");
    
    while (1)
    {
        if (PINB.3 == 0)
            giat();
        if (PINB.2 == 0)
            vat();
        delay_ms(250);
    }
}
