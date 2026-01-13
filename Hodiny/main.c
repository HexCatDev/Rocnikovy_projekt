
/**
 * @file main.c
 * @author nontoxiccat
 * @date 2026-01-08
 * @brief Blink LED on RC0 (PIC18F47Q43) - MPLAB Snap Programmer Compatible with LVP
 */

#include <builtins.h>
#include <xc.h>
#include <pic18f47q43.h>
// Configuration bits for PIC18F47Q43
#pragma config FEXTOSC = OFF           // External Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_1MHZ  // Power-on reset uses HFINTOSC 1MHz
#pragma config WDTE = OFF              // Watchdog Timer disabled
#pragma config MCLRE = INTMCLR         // MCLR pin disabled, RE3 input enabled
#define _XTAL_FREQ 8000000             // 8 MHz internal oscillator frequency

void CLK(int time[5])
{
    int hh, h, mm, m, ss, s;
    time[0] = hh;
    time[1] = h;
    time[2] = mm;
    time[3] = m;
    time[4] = ss;
    time[5] = s;
    while (1) {
        LATDbits.LATD7 = 1;
        s++;
        __delay_ms(250);
        LATDbits.LATD7 = 0;
        __delay_ms(250);

        if (s >= 10) {
            s = 0;
            ss++;
        }
        if (ss >= 60 && s >= 10) {
            s = 0;
            ss++;
        }
        if (ss >= 60) {
            m++;
        }
        if (m >= 10 && ss >= 60) {
            m = 0;
            mm++;
        }
        if (mm >= 60) {
            h++;
        }
        if (h >= 10 && mm >= 60) {
            h = 0;
            hh++;
        }
        if (hh >= 24 && h >= 10 && mm >= 60 && m >= 10 && ss >= 60 && s >= 10) 
        {
        hh = 0;
        h = 0;
        mm = 0;
        m = 0;
        ss = 0;
        s = 0;
        }
    }

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


    int Segments[8] = {0, 1, 2, 3, 4, 5 , 6, 7};

    int numbers[9] = {  };

    int time[5];
    CLK(time);

    // kód
    while (1) {
        for (int i = 0; i < 6; i++) {
            LATBbits.LATB0 = 1; //nastaví všechny tranzistory na high takže se vypnou protože PNP
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB3 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 1;
            // nastaví určitý tranzistor na low takže se zapne
            switch (i) {
                case 0: LATBbits.LATB0 = 0; break;
                case 1: LATBbits.LATB1 = 0; break;
                case 2: LATBbits.LATB2 = 0; break;
                case 3: LATBbits.LATB3 = 0; break;
                case 4: LATBbits.LATB4 = 0; break;
                case 5: LATBbits.LATB5 = 0; break;
            }
        }  
            //tady se budou zadávat číslice asi...
            //tady jsou všechny segmenty nasatveny na na 0 krom DP
        for (int i = 0; i < 8; i++) {
            switch(Segments[i]) {
                case 0: LATDbits.LATD7 = 1; break;
                case 1: LATDbits.LATD6 = 0; break;
                case 2: LATDbits.LATD5 = 0; break;
                case 3: LATDbits.LATD4 = 0; break;
                case 4: LATDbits.LATD3 = 0; break;
                case 5: LATCbits.LATC7 = 0; break;
                case 6: LATCbits.LATC6 = 0; break;
                case 7: LATCbits.LATC5 = 0; break;
            }
            __delay_ms(100);
        }
    }

    return 0;
}