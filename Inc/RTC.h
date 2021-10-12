/*
 * RTC.h
 *
 *  Created on: May 6, 2021
 *  Funkcije za podesavanje vremena na RTC, ocitavanje vremena sa RTC i zapis
 *  u decimalnom formatu
 *      Author: Aleksandar
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_
#include "main.h"

extern struct Vreme          //Structura u koju se upisuju podaci
 {
	 uint8_t sec;
	 uint8_t min;
	 uint8_t sat;
	 uint8_t dow;
	 uint8_t dom;
	 uint8_t mon;
	 uint8_t god;

 };




struct Vreme getTime(void);      																							//Ucitavanja vremena sa RTC
int bcdToDec(uint8_t val);																							//Konverzija ic binarnog u decimalni brojevni sistem
void setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);		//Podesavanje pocetnog vremena na RTC(koristi se samo prvi put, posle se komentarise)
uint8_t decToBcd(int val);																							//Konverzija iz decimalnog u binarni brojevni sistem
float getTemp(void);

#endif /* INC_RTC_H_ */
