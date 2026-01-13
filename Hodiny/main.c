
/**
 * @file main.c
 * @author nontoxiccat
 * @date 2026-01-08
 * @brief 
 */
#include <builtins.h>
#include <xc.h>
#include <pic18f47q43.h>
#include <string.h>
// Configuration bits for PIC18F47Q43
#pragma config FEXTOSC = OFF           // External Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_1MHZ  // Power-on reset uses HFINTOSC 1MHz
#pragma config WDTE = OFF              // Watchdog Timer disabled
#pragma config MCLRE = INTMCLR         // MCLR pin disabled, RE3 input enabled
#define _XTAL_FREQ 8000000             // 8 MHz internal oscillator frequency

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
    
    // Set system clock to 8 MHz
    OSCFRQ = 0x03;                     // HFINTOSC = 8 MHz

    // Configure ANSELC to digital
    ANSELC = 0x00;                     // All PORTC pins digital


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

 
    // loop
    while (1) {
        for (int i = 0; i < 6; i++) {
            __delay_us(250);
            LATBbits.LATB0 = 1; //nastaví všechny tranzistory na high takže se vypnou protože PNP
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB3 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 1;
            // nastaví určitý tranzistor na low takže se zapne
            switch (i) {
                case 0: LATBbits.LATB5 = 0; break;
                case 1: LATBbits.LATB4 = 0; break;
                case 2: LATBbits.LATB3 = 0; break;
                case 3: LATBbits.LATB2 = 0; break;
                case 4: LATBbits.LATB1 = 0; break;
                case 5: LATBbits.LATB0 = 0; break;
            }
            display_digit(numbers[3]);
            __delay_us(250);
        };
    } 
    return 0;
}