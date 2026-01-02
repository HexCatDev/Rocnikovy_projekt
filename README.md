# Ročníkový projekt S3E Dobeš Tomáš Digitální hodiny
## Zvláštní poděkování
Zvláště bych chtěl poděkovat panu Miroslavu Máchovi za rady a pomoc při výběru součástek a řešení problémů konstrukčního typu.
A také  panu Jiřímu Vinterovi za rady a kontakty.

## To-do list
- [x] Navrhnout schéma 
- [ ] Objednat součástky
- [ ] Otestovat na breadboardu
- [ ] Navrhnout PCB
- [ ] Vyrobit PCB osadit, otestovat

## Princip a postup návrhu
Původní myšlenka jak zhotovit tento obvod byla, že by se jednotlivé výstupy na zobrazovače připojily přes posuvné registry, ovšem bylo by jich zapotřebí 6 kousků (HH:mm:ss) převod časového údaje z RTC by bylo zhotoveno přímo v PIC.

Po konzultaci s panem Máchou by bylo lepší Multiplexovat číslice 1:6 tzn. že v jednom okamžiku problikne jeden segment, takto se "probliká" každý segment zvlášť v určité rychlosti(Hz), od určité rychlosti ± 30 Hz se obraz jeví jako spojitý.[1]
### Blokové schéma
<figure>
  <img src="Dokumentace/Blokove_schema.jpg" alt="Blokové schéma">
  <figcaption align="center"><i>Obrázek 1: Blokové schéma zapojení hodin</i></figcaption>
</figure>

### Součástky a jejich funkce v obvodu
V této podkapitole budou popsány hlavně podstatné součástky RTC, PIC, tranzistorové pole a samotné tranzistory (touto částí obvodu se chápe zobrazovací část. Vzhledem ke složitosti projektu zde nebude popsáno jak každý samostatný prvek funguje. 
- Mozkem obvodu je PIC18F47Q43-I-P[2] má na starosti komunikaci s RTC, multiplexování a řízení hodin. 
- RTC (Real Time Clock)[3] a Krystal[4] 
Tento IO komunikuje pomocí I2C protokolu s PIC a poskytuje časový údaj tento údaj je poskytován v dekadickém formátu. Má také externí zdroj oscilací krystal o frekvenci 32 768 Hz tato hodnota je definovaná v jeho dokumentaci[3, kap. 4.2.1].
- Tranzistorové pole ULN2804A[5] tento IO ve svém pouzdře DIP 18 obsahuje 8x NPN transistorů se spol. emitorem s max. kolektorovím  proudem 0,5A a max. napětím mezi kolektorem a emitorem 50V
- 7 Segmentové zobrazovače[6][7] 2x 20,32mm a 2x 10mm se společnou katodou obsahují 16x znaků v pouzdře 2x8 (7+DP) jsou připojeny na přes rezistory na ULN2804A[5] a jednotlivé znaky jsou připojeny přes tranzistory DTA123JCA[8]

## Komponenty použité v projektu
- Aktivní součástky
	+ MCU https://ecom.cz/eshop/detail/73426-PIC18F47Q43-I-P
	+ RTC https://ecom.cz/eshop/detail/39997-MCP7940M-I-SN
	+ T. pole https://ecom.cz/eshop/detail/5723-ULN2804A
	+ Tranzistory SMD https://ecom.cz/eshop/detail/59753-DTA123JCA-SMD
- Krystal
	+ Crystal https://www.tme.eu/cz/details/32.768k-38-sr/krystalove-rezonatory-tht/sr-passives/32-768k-3-8-sr/
- Headry:
	+ 2X https://ecom.cz/eshop/detail/84648Kondenzátry-KL-1X02-SG-62 není skladem
	+ 5X není skladem
	+ 18X není skladem
- Patice
	+ 1X4 https://ecom.cz/eshop/detail/85255-SILPZ-1X04-V13-JK
	+ 1X8 https://ecom.cz/eshop/detail/85260-SILPZ-1X08-V13-JK
	+ DIP 18 https://ecom.cz/eshop/detail/64634-DIL-18-3-NT-1
	+ DIP 40 https://ecom.cz/eshop/detail/64639-DIL-40-6-YT-1
- Zobrazovače
	+ 7 segment 2X 20,32mm: https://ecom.cz/eshop/detail/29807-LD-FYD-8021DUHR-21
	+ 7 segment 10mm: https://ecom.cz/eshop/detail/29798-LD-FYD-3921BUHR-21
- Pasivní součástky
	+ Rezistory a Kondenzátory
		* 4K7 https://ecom.cz/eshop/detail/71624-R1206-4K7-1--WRX
		* 6.8 pF https://ecom.cz/eshop/detail/14473-C0603-6-8PF-NPO-50V-DNCC
		* 100 nF https://ecom.cz/eshop/detail/21330-C1206-0-1UF-X7R-100V-KNCC
- Svorkovnice https://ecom.cz/eshop/detail/6649-DG126-5-0-02P-14-02
## Použitá literatura

- [1] WIKIPEDIA CONTRIBUTORS. *Flicker fusion threshold*. Wikipedia: The Free Encyclopedia [online]. [cit. 2026-01-02]. Dostupné z: https://en.wikipedia.org/wiki/Flicker_fusion_threshold
- [2] MICROCHIP TECHNOLOGY. *PIC18F27/47/57Q43 Data Sheet: 28/40/44/48-Pin, Low-Power, High-Performance Microcontroller with XLP Technology*. 2021. Dostupné z: https://ecom.cz/data/exportdokumentu?soubor=D73426.pdf&nazev=ECOM_73426_PIC18F47Q43-I-P.pdf 
- [3] MICROCHIP TECHNOLOGY. *MCP7940M: Low-Power I2C Real-Time Clock/Calendar with SRAM*. 2012. Dostupné z: https://ecom.cz/data/exportdokumentu?soubor=D39997.pdf&nazev=ECOM_39997_MCP7940M-I-SN.pdf
- [4] SR PASSIVES. *Crystal 32.768kHz: 32.768K-3/8-SR Specification*. Dostupné z: https://www.tme.eu/Document/4ebb629a31bbf93152d2908870dbe0d1/WX2.pdf
- [5] STMICROELECTRONICS. *ULN2801A, ULN2802A, ULN2803A, ULN2804A: Eight Darlington Transistor Arrays*. 2012. Dostupné z: https://ecom.cz/data/exportdokumentu?soubor=D05723.pdf&nazev=ECOM_05723_ULN2804A.pdf
- [6] FORYARD OPTOELECTRONICS. *FYD-8021DUHR-21: Dual Digit 7-Segment Display Specification*. Dostupné z: https://ecom.cz/data/exportdokumentu?soubor=D29806.pdf&nazev=ECOM_29807_LD-FYD-8021DUHR-21.pdf
- [7] FORYARD OPTOELECTRONICS. *FYD-3921BUHR-21: Single Digit 7-Segment Display Specification*. Dostupné z:https://ecom.cz/data/exportdokumentu?soubor=D29797.pdf&nazev=ECOM_29798_LD-FYD-3921BUHR-21.pdf
- [8] TSC - TAIWAN SEMICONDUCTORS. *DTA123JCA PNP 100mA 5OV R1 2.2K R2 47K*