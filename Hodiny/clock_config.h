#ifndef CLOCK_CONFIG_H
#define CLOCK_CONFIG_H
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

#define time_to_BCD(tens, ones) ( (uint8_t) ((tens) << 4) | (ones)) //toto je makro vloží se do něj dvě hodnoty desítky a jednotky a ono vrátí hodnotu
// v BCD formátu tj. pro třeba 45 sec máme 4 a 5, tens 4 ones 5 makro vezme 4 převede ho do binárního formátu což je 0100 a posune ji 
// o 4 bity doleva (<< 4) takže vznikne 0100 0000 pak je tu bitwise OR (|) což slepí dvě hodnoty doromady, 5 je v bin 0101 (4+1)
// výsledkem slepení je 0100 0101 což je 0x45 v hexu a to je BCD reprezentace čísla 45.
// takže tady to bude bcd_numbers[3] = {time_to_BCD(numbers_out[4], numbers_out[5])}
// do hodnoty 3 v poli bcd_numbers se uloží převod hodnot z numbers_out[4] a numbers_out[5] do BCD formátu což jsou sekundy
// to se pak jako celek pošle do RTC


// Forward declarations
void READ_RTC(void);
void WRITE_RTC(void);
void system_clock(void);
void timer(void);
void display_controll(void);
void My_I2C1_Interrupt(void);
void process_data_from_RTC(void);


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
uint8_t bcd_numbers[4] = {0, 0, 0}; // pole pro uchování hodnot v BCD formátu pro hodiny, minuty a sekundy
uint8_t payload[4] = {0, 0, 0, 0}; // pole pro uchování dat pro I2C komunikaci s RTC
uint8_t bcd_numbers_recieved[3] = {0, 0, 0,}; // pole pro uchování dat přijatých z RTC
volatile uint32_t ms_sync_counter = 0; // čítač pro měření času v milisekundách, inkrementuje se v přerušení od časovače
volatile uint16_t ms_counter = 0; // čítač pro měření času v milisekundách, inkrementuje se v přerušení od časovače


volatile bool I2C_trasmission_complete = false; // flag pro indikaci dokončení I2C přenosu
volatile bool sys_clock = false; // flag pro sys clock
volatile bool RTC_sync = false; // flag pro RTC sync
static bool waiting_for_data = false; // flag pro čekání na data z RTC
#endif