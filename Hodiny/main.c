
/**
 * @file main.c
 * @author nontoxiccat
 * @date 2026-01-08
 * @brief Blink LED on RC0 (PIC18F47Q43) - MPLAB Snap Programmer Compatible with LVP
 */

#include <xc.h>
#include <pic18f47q43.h>
// Configuration bits for PIC18F47Q43
#pragma config FEXTOSC = OFF           // External Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_1MHZ  // Power-on reset uses HFINTOSC 1MHz
#pragma config WDTE = OFF              // Watchdog Timer disabled
#pragma config MCLRE = INTMCLR         // MCLR pin disabled, RE3 input enabled
#define _XTAL_FREQ 8000000             // 8 MHz internal oscillator frequency

void CLK(int H, int HH)
{
    while (1) {
        LATDbits.LATD7 = 1;
        H++;
        __delay_ms(250);
        LATDbits.LATD7 = 0;
        __delay_ms(250);

        if (H >= 10) {
            H = 0;
            HH++;
        }
        if (HH >= 24 && H >= 10) {
            H = 0;
            HH = 0;
        }
    }

}

int main(void)
{
    
    // Set system clock to 8 MHz
    OSCFRQ = 0x03;                     // HFINTOSC = 8 MHz

    // Configure ANSELC to digital
    ANSELC = 0x00;                     // All PORTC pins digital

    TRISBbits.TRISB0 = 0; // Nastavení výstupů pro B1 a B0 transzitory PNP
    TRISBbits.TRISB1 = 0;

    // Tyto hodnoty reprezentují segmenty A_DP
    TRISDbits.TRISD7 = 0; // Nastavení výstupů pro D7-D3
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD3 = 0;
    TRISCbits.TRISC7 = 0; // Nastavení výstupů pro C7-C5
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC5 = 0;

    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;

    int Segments[8] = {0, 1, 2, 3, 4, 5 , 6, 7};

    int numbers[9] = {

    };

    for (int i = 0; i < 8; i++) {
        switch(Segments[i]) {
            case 0: LATDbits.LATD7 = 0; break;
            case 1: LATDbits.LATD6 = 0; break;
            case 2: LATDbits.LATD5 = 0; break;
            case 3: LATDbits.LATD4 = 0; break;
            case 4: LATDbits.LATD3 = 0; break;
            case 5: LATCbits.LATC7 = 0; break;
            case 6: LATCbits.LATC6 = 0; break;
            case 7: LATCbits.LATC5 = 0; break;
        }
    }
    int H;
    int HH;
    H = 0;
    HH = 0;
    CLK(H, HH);

    return 0;
}