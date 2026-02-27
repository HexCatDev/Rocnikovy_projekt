
/**
 * @file main.c
 * @author nontoxiccat
 * @date 2026-01-08
 * @brief 
 */
#include "My_MCC_Config/mcc/mcc_generated_files/system/interrupt.h"
#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>
#include <pic18f47q43.h>
// Configuration bits for PIC18F47Q43
#pragma config WDTE = OFF              // Watchdog Timer disabled

#define _XTAL_FREQ 64000000 // Define the operating frequency for delay functions
#define RTC_ADDR 0xF6       // adress 7 bit 110 1111 
#define SEC_REG 0x00        // seconds register
#define MIN_REG 0X01        // minutes register
#define HOUR_REG 0X02       // hours register
#define TL_SET PORTAbits.RA0 // tlačítko pro nastavení pozice
#define TL_ADD PORTAbits.RA1 // tlačítko pro přidání hodin
#define TL_SUB PORTAbits.RA2 // tlačítko pro odečtení hodin

#include "My_MCC_Config/mcc/mcc_generated_files/timer/tmr0.h"
#include "My_MCC_Config/mcc/mcc_generated_files/system/system.h"
#include "My_MCC_Config/mcc/mcc_generated_files/i2c_host/i2c1.h"
#include "clock_config.h" //můj soubor jsou v něm definovány segmenty pro zobrazení číslic a funkce pro zobrazení číslice a nastavení hodin


int main(void)
{ 
    //I2C stuff
    SYSTEM_Initialize();
    INTERRUPT_Initialize();
    I2C1_Host_Initialize();
    I2C1_Initialize();
    pin_init();

    TMR0_OverflowCallbackRegister(timer); //registrace callback funkce pro přerušení od časovače tzn. když nastane přerušení spustí se fukce timer()
    TMR0_Start(); //spuštění časovače

    
    while (1) {
        if (in_setup_mode == 0) { //pokud nejsme v režimu nastavení hodin, aktualizujeme čas a synchronizujeme s RTC
            if (sys_clock) {
                system_clock(); //tato funkce se bude volat každou sekundu a bude aktualizovat čas na displeji a je založena na funkci timer()
                sys_clock = false; //reset flagu pro sys clock  
            }
            if (RTC_sync) {
                INTCON0bits.GIEH = 0; //vypne globální přerušení aby se zabránilo konfliktu mezi timerem a I2C komunikací
                uint32_t snapshot_sync = ms_sync_counter; //uloží se aktuální hodnota ms_sync_counter do lokální proměnné, aby se zabránilo problému s tím, že se ms_sync_counter změní během čtení z RTC
                INTCON0bits.GIEH = 1; //znovu zapne globální přerušení
                //READ_RTC(); //tato funkce se bude volat každých 15 minut a bude synchronizovat čas s RTC
                RTC_sync = false; //reset flagu pro RTC sync
            }
        }
        display_controll();
    } 
    return 0;
}

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

void clock_init(uint8_t numbers_out[6]) {
    in_setup_mode = 1; // nastaví se, že jsme v režimu nastavení hodin
    int selected_digit = 4;
    int hold_button_time = 0, theshold = 100; // proměnné pro detekci dlouhého stisku tlačítka, threshold je počet 10ms intervalů pro detekci dlouhého stisku
    while (1) {
        if (TL_SET == 1) {
            hold_button_time++;
            __delay_ms(10);
            if (hold_button_time > theshold) {
                // tady se odešlou hodnoty z numbers out do RTC přes I2C a zapnase oscilátor
                //WRITE_RTC(); //tato funkce se bude volat při dlouhém stisku tlačítka pro nastavení hodin a odešle hodnoty z numbers_out do RTC
                while (TL_SET == 1) {
                    __delay_ms(10);
                    LATB |= 0x3F; //nastaví všechny tranzistory na high takže se vypnou protože PNP display zhasne
                }
            in_setup_mode = 0; // po uvolnění tlačítka se vypne režim nastavení hodin
            __delay_ms(100);
            TMR0_Start(); // znovu se spustí časovač aby se začal aktualizovat čas na displeji
            return; // po uvolnění tlačítka se vrátí do hlavní smyčky a začne zobrazovat čas
            }
        
        }
        else {
            if (hold_button_time > 5 && hold_button_time < theshold) { //pokud je tlačítko drženo více ne 5ms ale méně než threshold, přepne se na další číslici
                selected_digit = (selected_digit + 1) % 6; // přepínání mezi číslicemi
            }
            hold_button_time = 0; // reset času držení tlačítka
            __delay_ms(10);
        }

        if (TL_ADD == 1) {
            __delay_ms(300);
            switch (selected_digit) {
                case 0: numbers_out[0] = (numbers_out[0] >= 2) ? 0 : numbers_out[0] + 1; break; //hh 0,1,2
                case 1: numbers_out[1] = (numbers_out[1] >= 9) ? 0 : numbers_out[1] + 1; break; //h 0-9
                case 2: numbers_out[2] = (numbers_out[2] >= 5) ? 0 : numbers_out[2] + 1; break; //mm 0-5
                case 3: numbers_out[3] = (numbers_out[3] >= 9) ? 0 : numbers_out[3] + 1; break; //m 0-9
                case 4: numbers_out[4] = (numbers_out[4] >= 5) ? 0 : numbers_out[4] + 1; break; //ss 0-6
                case 5: numbers_out[5] = (numbers_out[5] >= 9) ? 0 : numbers_out[5] + 1; break; //s 0-9
            }
        }

        if (TL_SUB == 1) {
            __delay_ms(300);
            switch (selected_digit) {
                //tady jsem použil ternary operace tj. (kondice) ? (je pravda) : (není pravda) protože když odečítám 1 od 0, chci aby se číslo změnilo na maximum (např. pro minuty když je 00 a odečtu 1, chci aby se změnilo na 59)
                case 0: numbers_out[0] = (numbers_out[0] -1 ) < 0 ? 2 : numbers_out[0] - 1; break; //hh 0,1,2
                case 1: numbers_out[1] = (numbers_out[1] -1 ) < 0 ? 9 : numbers_out[1] - 1; break; //h 0-9
                case 2: numbers_out[2] = (numbers_out[2] -1 ) < 0 ? 5 : numbers_out[2] - 1; break; //mm 0-5
                case 3: numbers_out[3] = (numbers_out[3] -1 ) < 0 ? 9 : numbers_out[3] - 1; break; //m 0-9
                case 4: numbers_out[4] = (numbers_out[4] -1 ) < 0 ? 5 : numbers_out[4] - 1; break; //ss 0-6
                case 5: numbers_out[5] = (numbers_out[5] -1 ) < 0 ? 9 : numbers_out[5] - 1; break; //s 0-9
            }
        }

        for (int i = 0; i < 6; i++) {
            LATB |= 0x3F; //nastaví všechny tranzistory na high takže se vypnou protože PNP

            // nastaví určitý tranzistor na low takže se zapne
            switch (selected_digit) {
                case 0: display_digit(numbers_DP[numbers_out[0]]); LATBbits.LATB5 = 0; __delay_us(500); break; 
                case 1: display_digit(numbers_DP[numbers_out[1]]); LATBbits.LATB4 = 0; __delay_us(500); break; 
                case 2: display_digit(numbers_DP[numbers_out[2]]); LATBbits.LATB3 = 0; __delay_us(500); break; 
                case 3: display_digit(numbers_DP[numbers_out[3]]); LATBbits.LATB2 = 0; __delay_us(500); break; 
                case 4: display_digit(numbers_DP[numbers_out[4]]); LATBbits.LATB1 = 0; __delay_us(500); break; 
                case 5: display_digit(numbers_DP[numbers_out[5]]); LATBbits.LATB0 = 0; __delay_us(500); break; 
            }
        }
    }
}

void display_controll(){
    if (PORTAbits.RA0 == 1) {
        TMR0_Stop(); //zastaví časovač aby se zabránilo konfliktu mezi timerem a zobrazením číslic při nastavení hodin
        ms_sync_counter = 0; //resetuje se čítač pro synchronizaci s RTC aby se zabránilo tomu, že se během nastavení hodin synchronizuje čas s RTC a změní se nám nastavené hodnoty
        ms_counter = 0; //resetuje se čítač pro synchronizaci s RTC aby se zabránilo tomu, že se během nastavení hodin synchronizuje čas s RTC a změní se nám nastavené hodnoty
        LATB |= 0x3F; //zhasne nám display
        __delay_ms(100);
        clock_init(numbers_out);
        __delay_ms(100);
    }

    for (int i = 0; i < 6; i++) {
        // toto je bitwisová operace OR pro nastavení bitů 0-5 na 1, což znamená, že všechny tranzistory jsou vypnuté (protože jsou PNP)
        LATB |= 0x3F;
        // nastaví určitý tranzistor na low takže se zapne
        switch (i) {
            case 0: display_digit(numbers[numbers_out[0]]); LATBbits.LATB5 = 0; __delay_us(500); break; 
            case 1: display_digit(numbers[numbers_out[1]]); LATBbits.LATB4 = 0; __delay_us(500); break; 
            case 2: display_digit(numbers[numbers_out[2]]); LATBbits.LATB3 = 0; __delay_us(500); break; 
            case 3: display_digit(numbers[numbers_out[3]]); LATBbits.LATB2 = 0; __delay_us(500); break; 
            case 4: display_digit(numbers[numbers_out[4]]); LATBbits.LATB1 = 0; __delay_us(500); break; 
            case 5: display_digit(numbers[numbers_out[5]]); LATBbits.LATB0 = 0; __delay_us(500); break; 
        }
    }
}

void READ_RTC(){//přečet hodnoty z RTC převede je do arraye numbers_out a pak se zobrazí
//tohle se musí dodělat
}

void WRITE_RTC(){//tato funkce se bude volat při dlouhém stisku tlačítka pro nastavení hodin a odešle hodnoty z numbers_out do RTC
//tohle se musí dodělat

}

void system_clock(){ //tato funkce se bude volat každou sekundu a bude aktualizovat čas na displeji a je založena na funkci timer()
    numbers_out[5]++; //zvýší sekundy o 1
    if (numbers_out[5] >= 10) { //pokud jsou ss větší nebo rovny 10, resetuje se na 0 a zvýší se minuty
        numbers_out[5] = 0;
        numbers_out[4]++;
        if (numbers_out[4] >= 6){ //když s je větší než 6
            numbers_out[4] = 0;
            numbers_out[3]++;
            if (numbers_out[3] >= 10){//když je mm větší než 10
                numbers_out[3] = 0;
                numbers_out[2]++;
                if (numbers_out[2] >= 6){ //když je m větší než 6
                    numbers_out[2] = 0;
                    numbers_out[1]++;
                    if (numbers_out[1] >= 10){ //když je h větší než 10
                        numbers_out[1] = 0;
                        numbers_out[0]++;
                        if (numbers_out[0] >= 2){ //když je hh větší než 2, resetuje se na 0
                            memset(numbers_out, 0, sizeof(numbers_out)); //nastvaí všechny hodnoty v numbers_out na 0
                        }
                    }       
                }
            }
        }
    }
}

void timer(void){
    ms_sync_counter++; //inkrementace čítače pro měření času v milisekundách
    ms_counter++; //inkrementace čítače pro měření času v milisekundách
    if (ms_counter >= 1000 && in_setup_mode == 0) { //když uplyne 1 sekunda
        sys_clock = true; //nastaví se flag pro sys clock
        ms_counter = 0;
    }
    if (in_setup_mode == 0) {
        if (ms_sync_counter >= 900000) { //když uplyne 15 minut, synchronizuje se čas s RTC
            RTC_sync = true; //nastaví se flag pro RTC sync
            ms_sync_counter = 0;
        }
    }
}