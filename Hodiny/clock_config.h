#ifndef CLOCK_CONFIG_H
#define CLOCK_CONFIG_H
#include "My_MCC_Config/mcc/mcc_generated_files/system/clock.h"
#define _XTAL_FREQ 64000000 //definice frekvence pro delay funkce

#include <language_support.h>
#include <pic18f47q43.h>
#include <stdint.h>
#include <xc.h>
#include <stdbool.h>

#define DP = LATDbits.LATD7;
#define A = LATDbits.LATD6;
#define B = LATDbits.LATD5;
#define C = LATDbits.LATD4;
#define D = LATCbits.LATC7;
#define E = LATCbits.LATC6;
#define F = LATCbits.LATC5;
#define G = LATDbits.LATD3;

// Forward declarations
void READ_RTC();
void WRITE_RTC();
void system_clock(void);
void timer(void);
void display_controll(void);

const int numbers[10][8] = {
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

const int numbers_DP[10][8] = {
    {1, 0, 1, 1, 1, 1, 1, 1}, //0
    {1, 0, 0, 0, 0, 1, 1, 0}, //1
    {1, 1, 0, 1, 1, 0, 1, 1}, //2
    {1, 1, 0, 0, 1, 1, 1, 1}, //3
    {1, 1, 1, 0, 0, 1, 1, 0}, //4
    {1, 1, 1, 0, 1, 1, 0, 1}, //5
    {1, 1, 1, 1, 1, 1, 0, 1}, //6
    {1, 0, 0, 0, 0, 1, 1, 1}, //7
    {1, 1, 1, 1, 1, 1, 1, 1}, //8
    {1, 1, 1, 0, 1, 1, 1, 1} //9
};      // DP,    G,      F,    E,      D,    C,     B,     A

static inline void pin_init() { //nastavení pinů jako výstupy a vstupy
    // Configure allpins to digital
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00; 

    TRISB &= ~0x3F; //nastavení výstupů pro B0-B5 transzistory PNP bitwisová operace AND s negací 0x3F (0011 1111) nastaví bity 0-5 na 0 (výstup) a zbytek ponechá beze změny

    // Tyto hodnoty reprezentují segmenty A_DP
    TRISDbits.TRISD7 = 0; // Nastavení výstupů pro D7-D3
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD3 = 0;
    TRISCbits.TRISC7 = 0; // Nastavení výstupů pro C7-C5
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC5 = 0;

    //nastavení vstupů pro tlačítka
    TRISAbits.TRISA0 = 1; // hodiny zpět 
    TRISAbits.TRISA1 = 1; // výběr pozice
    TRISAbits.TRISA2 = 1; // hodiny vpřed

    INTCON0bits.IPEN = 1; // povolení priorit přerušení
    INTCON0bits.GIEH = 1; // povolení globálních přerušení s vysokou prioritou
    INTCON0bits.GIEL = 1; // povolení globálních přerušení s nízkou prioritou
}

enum Segments { DPS, As, BS, CS, DS, ES, FS, GS };

uint8_t numbers_out[6] = {0, 0, 0, 0, 0, 0}; // pole pro uchování aktuálních čísel na displeji
uint8_t in_setup_mode = 0; // proměnná pro indikaci, zda jsme v režimu nastavení hodin

volatile uint32_t ms_sync_counter = 0; // čítač pro měření času v milisekundách, inkrementuje se v přerušení od časovače
volatile uint16_t ms_counter = 0; // čítač pro měření času v milisekundách, inkrementuje se v přerušení od časovače
volatile bool sys_clock = false; // flag pro sys clock
volatile bool RTC_sync = false; // flag pro RTC sync
#endif