/*
 * MPPT.h
 *
 *  Created on: May 7, 2021
 *  Funkcija za ocitavanje poruka sa MPPT, sve podatke koje MPPT salje smestamo u
 *  strukturu Poruka. Za komunikaciju koristimo UART i ugradjena HAL funkcije.
 *      Author: Aleksandar
 */
#include "main.h"


struct Poruka					//	Kreiranje strucute Poruka
	{
	  char Pid[10];				//Product ID uredjaja
	  char FW[10];				//Firmware version uredjaja
	  char SER[15];				//Serijski broj uredjaja
	  char V[10];				//Trenutni napon u mV
	  char I[10];				//Trenutna struja u mA
	  char Vpv[10];				//Napon panela
	  char Ppv[10];				//Snaga panela
	  char Cs[10];				//Stanje u kome se nalazi uredjaj
	  char Mppt[10];			//Moguce vrednosti za operacije pracenja
	  char ERR[10];				//Error kod greske
	  char Load[10];			//Stanje izlaza (ON/OFF)
	  char Il[10];				//Struja opterecenja
	  char H19[10];				//Totalni prenos energije
	  char H20[10];				//Danasnji prenos energije
	  char H21[10];				//Maksmalna snaga dana
	  char H22[10];				//Prethodni dan prenos energije
	  char H23[10];				//Maksimalna snaga prethodnog dana
	  char Hsds[10];			// Istorija podataka, promena broja indikuje novi dan (0...364)
	  char Checksum[10];
	};

#ifndef INC_MPPT_H_
#define INC_MPPT_H_


struct Poruka getMppt();					//Upisivanje poruke sa MPPT
void setMpptMode(uint8_t mod);       		//Podesavanje moda u kom se mppt nalazi, odabir se vrsi u funkciji, fajl MPPT.C
#endif /* INC_MPPT_H_ */
