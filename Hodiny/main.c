
/**
 * @file main.c
 * @author nontoxiccat
 * @date 2026-01-08
 * @brief 
 */
#include "My_MCC_Config/mcc/mcc_generated_files/system/interrupt.h"
#include <builtins.h>
#include <xc.h>
#include <pic18f47q43.h>
// Configuration bits for PIC18F47Q43
#pragma config WDTE = OFF              // Watchdog Timer disabled

#define _XTAL_FREQ 8000000             // 8 MHz internal oscillator frequency
#define RTC_ADDR 0xF6       // adress 7 bit 110 1111 
#define SEC_REG 0x00        // seconds register
#define MIN_REG 0X01        // minutes register
#define HOUR_REG 0X02       // hours register

#include "My_MCC_Config/mcc/mcc_generated_files/system/system.h"
#include "My_MCC_Config/mcc/mcc_generated_files/i2c_host/i2c1.h"




enum Segments { DPS, As, BS, CS, DS, ES, FS, GS };

void display_digit(const int segments[8]){
    LATDbits.LATD7 = segments[DPS] & 0x1;
    LATDbits.LATD6 = segments[As] & 0x1;
    LATDbits.LATD5 = segments[BS] & 0x1;
    LATDbits.LATD4 = segments[CS] & 0x1;
    LATCbits.LATC7 = segments[DS] & 0x1;
    LATCbits.LATC6 = segments[ES] & 0x1;
    LATCbits.LATC5 = segments[FS] & 0x1;
    LATDbits.LATD3 = segments[GS] & 0x1;
}

int main(void)
{
    // Configure ANSELC to digital
    ANSELC = 0x00;                     // All PORTC pins digital

    //I2C stuff
    SYSTEM_Initialize();
    INTCON0bits.GIEH = 1; 
    INTCON0bits.GIEL = 1;

    TRISBbits.TRISB0 = 0; // Nastavení výstupů pro B0-B5 transzitory PNP
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;

    // Tyto hodnoty reprezentují segmenty A_DP
    TRISDbits.TRISD7 = 0; // Nastavení výstupů pro D7-D3
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD3 = 0;
    TRISCbits.TRISC7 = 0; // Nastavení výstupů pro C7-C5
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC5 = 0;



    int DP = LATDbits.LATD7;
    int A = LATDbits.LATD6;
    int B = LATDbits.LATD5;
    int C = LATDbits.LATD4;
    int D = LATCbits.LATC7;
    int E = LATCbits.LATC6;
    int F = LATCbits.LATC5;
    int G = LATDbits.LATD3;

 
    int numbers[10][8] = {
        {0, 0, 1, 1, 1, 1, 1, 1}, //0
        {0, 0, 0, 0, 0, 1, 1, 0}, //1
        {0, 1, 0, 1, 1, 0, 1, 1}, //2
        {0, 1, 0, 0, 1, 1, 1, 1}, //3
        {0, 1, 1, 0, 0, 1, 1, 0}, //4
        {0, 1, 1, 0, 1, 1, 0, 1}, //5
        {0, 1, 1, 1, 1, 1, 0, 1}, //6
        {0, 0, 0, 0, 0, 1, 1, 1}, //7
        {0, 1, 1, 1, 1, 1, 1, 1}, //8
        {0, 1, 1, 0, 1, 1, 1, 1} //9
    };      // DP,    G,      F,    E,      D,    C,     B,     A


    while (1) {
        //__delay_us(250);
        for (int i = 0; i < 6; i++) {
            LATBbits.LATB0 = 1; //nastaví všechny tranzistory na high takže se vypnou protože PNP
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB3 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 1;
            // nastaví určitý tranzistor na low takže se zapne
            switch (i) {
                case 0: LATBbits.LATB5 = 0; display_digit(0x0); /**__delay_us(100); */ break;
                case 1: LATBbits.LATB4 = 0; display_digit(0x0); /**__delay_us(100); */ break;
                case 2: LATBbits.LATB3 = 0; display_digit(0x0); /**__delay_us(100); */ break;
                case 3: LATBbits.LATB2 = 0; display_digit(0x0); /**__delay_us(100); */ break;
                case 4: LATBbits.LATB1 = 0; display_digit(0x0); /**__delay_us(100); */ break;
                case 5: LATBbits.LATB0 = 0; display_digit(0x0); /**__delay_us(100); */ break;
            }
        };
    } 
    return 0;
}