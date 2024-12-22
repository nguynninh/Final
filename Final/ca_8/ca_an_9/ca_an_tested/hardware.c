#include <delay.h>
#include <glcd.h>
#include <font5x7.h>
#include <alcd.h>

int keypad[3][3] = {1,2,3,4,5,6,7,8,9};

void init_register() {
    // ban phim ma tran
    DDRF = 0b11101010;
    // 2 led
    DDRD.4 = 1;
    DDRD.5 = 1;
    // lcd
    DDRD.7 = 1;
    DDRB.4 = 1;
    // BT 1, 2
    PORTB.2 = 1;
    PORTB.3 = 1;
    PORTD.7 = 1; 
    // servo
    DDRC.7 = 1;
}

int read() {
    char a, i, j;    
    for(j = 0; j < 3; j++ ){
        if (j == 0) PORTF = 0b11111101;
        if (j == 1) PORTF = 0b11110111;
        if (j == 2) PORTF = 0b11011111;
        for(i = 0; i < 3; i++){
            if (i == 0){
                a = PINF&0x04;
                if(a != 0x04) {
                    init_register();
                    return keypad[i][j];
                }
            }
            if (i == 1){
                a = PINF&0x10;
                if (a != 0x10) {
                    init_register();
                    return keypad[i][j];
                }
            }
            if (i == 2){
                a = PINF&0x01;
                if (a != 0x01) {
                    init_register();
                    return keypad[i][j]; 
                }
            }
        }
    }
    init_register();
    return -1;
}


void init_glcd(int bias, int vlcd) {
    GLCDINIT_t glcd_init_data;
    glcd_init_data.font=font5x7;
    glcd_init_data.temp_coef=140;
    glcd_init_data.bias=bias;
    glcd_init_data.vlcd=vlcd;
    glcd_init(&glcd_init_data);
}

void init_lcd() {
    lcd_init(16);
    lcd_clear();
}

void adjustSpeed(int speed) {
    OCR0 = 255 - speed;
}

void quay_DC() {
    adjustSpeed(120);
}

void dung_DC() {
    adjustSpeed(255);
}

void init_timer() {
    // khoi tao timer 0 cho dong co
    ASSR=0<<AS0;
    TCCR0=(1<<WGM00) | (1<<COM01) | (1<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    adjustSpeed(0);
    
    // Timer 1 (0.1 ms) cho den va servo
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
    TCNT1H=0xFC;
    TCNT1L=0xE0;
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);
    #asm("sei")
}