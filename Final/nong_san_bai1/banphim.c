#include <io.h> 
#include <delay.h> 
#include <stdio.h> 
#include <stdint.h> 
#include <stdlib.h> 
#include <alcd.h>
#include <glcd.h>

int keypad[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }; 
char input_buffer[16]; // Luu s? nh?p v?o (t?i da 15 k? t? + 1 null-terminator)
int buffer_index = 0;  // V? tr? hi?n t?i trong b? d?m
int number = 0;
char number_str[6];

void BUTTON() { 
    char a; 
    int i, j;   
    for (j = 0; j < 3; j++) { 
        // K?ch ho?t t?ng h?ng c?a b?n ph?m
        if (j == 0) PORTF = 0b11111101;    
        if (j == 1) PORTF = 0b11110111;    
        if (j == 2) PORTF = 0b11011111;    
        
        for (i = 0; i < 3; i++) { 
            // Ki?m tra t?ng c?t
            if (i == 0) {    
                a = PINF & 0x04; 
                if (a != 0x04) { 
                    input_buffer[buffer_index++] = keypad[j][i] + '0'; // Chuy?n s? sang k? t? v? luu v?o b? d?m                
                }  
            } 
            if (i == 1) { 
                delay_ms(50);
                a = PINF & 0x10; 
                if (a != 0x10) { 
                    input_buffer[buffer_index++] = keypad[j][i] + '0';                                 
                }  
            } 
            if (i == 2) {
                delay_ms(50); 
                a = PINF & 0x01; 
                if (a != 0x01) { 
                    input_buffer[buffer_index++] = keypad[j][i] + '0';          
                } 
            } 
        } 
    }     
}

void display_input() {
    input_buffer[buffer_index] = '\0'; // K?t th?c chu?i
    number = atoi(input_buffer); // Chuy?n chu?i th?nh s? nguy?n
    sprintf(number_str, "%d", number); // Chuy?n s? nguy?n th?nh chu?i d? hi?n th?
    lcd_clear();
    lcd_putsf("NHAP TEMP:");
    lcd_puts(input_buffer); // Hi?n th? s? nguy?n l?n LCD   
    glcd_clear();
    glcd_outtextxy(0, 10, "NHAP TEMP:");
    glcd_outtext(input_buffer);
}

char get_temp() { 
    DDRD = 0xFF; 
    PORTD = 0xB6; 
    lcd_init(16); 
    DDRF = 0b11101010; 
    PORTF = 0b00010101;     
    //pull up BT3
    DDRB.2 = 0;
    PORTB.0 = 1;    
    lcd_clear();
    lcd_puts("NHAP TEMP:");  

    while (1) {          
        BUTTON();   
        display_input();
        // N?u b? d?m d?y, hi?n th? s? v? reset l?i b? d?m
        if (buffer_index >= 2) {           
            buffer_index = 0; // Reset ch? s? 
            return number;
        }   
        if (PINB.0 == 0){    
            buffer_index = 0;
            return number;
        }
    } 
}