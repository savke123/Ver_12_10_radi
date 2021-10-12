/*
 * RTC.c
 *
 *  Created on: May 6, 2021
 *  Funkcije za podesavanje vremena na RTC, ocitavanje vremena sa RTC i zapis
 *  u decimalnom formatu
 *  Povezivanje
 *  STM NUCLEO F429ZI-> RTC DS1307
 *  PB8 (D15 na CN7 (pin2)) I2C1 SCL-> SCL RTC
 *  PB9 (D14 na CN7 (pin4)) I2C1 SDA-> SDA RTC
 *  5V->VCC
 *  GND->GND
 *      Author: Aleksandar
 */
#include "RTC.h"
I2C_HandleTypeDef hi2c1;							// inicijalizacija za I2C komunikaciju
//extern struct Vreme trenutnoVreme={0};	// Structura trenutnoVreme u koju se upisuju vrednosti sa RTC
struct Vreme prazno2={0};

int bcdToDec(uint8_t val)							//Konverzija iz binarnog u decimalni brojevni sistem, RTC prima i salje podatke samo binarno
 {
   return (int)( (val/16*10) + (val%16) );
 }

uint8_t decToBcd(int val)							//Konverzija iz decimalnog u binarni brojevni sistem
{
  return (uint8_t)( (val/10*16) + (val%10) );
}

 struct Vreme getTime()
 {

	 uint8_t primljeno[7]={0};						// Promenjiva Primljeno sluzi za upis podataka koje dobije preko I2C
	 struct Vreme trenutnoVreme={0};
	 trenutnoVreme=prazno2;


	 HAL_I2C_Mem_Read(&hi2c1, ((0x68<<1) | 0x01), 0x00, 1, primljeno, 7, 50); 		//Preko HAL fukcije vrsicitanje podataka iz memorije RTC-a
	 trenutnoVreme.sec=bcdToDec(primljeno[0]);										//Vrsimo upis u elemente strukure i u isto vremem konverziju u decimalni breojeve
	 trenutnoVreme.min=bcdToDec(primljeno[1]);
	 trenutnoVreme.sat=bcdToDec(primljeno[2]);
	 trenutnoVreme.dow=bcdToDec(primljeno[3]);
	 trenutnoVreme.dom=bcdToDec(primljeno[4]);
	 trenutnoVreme.mon=bcdToDec(primljeno[5]);
	 trenutnoVreme.god=bcdToDec(primljeno[6]);

	 return trenutnoVreme;
//	 HAL_Delay(500);
 }



 void setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)    //postavljamo reme od kog RTC pocinje da broji
 {

		uint8_t trVreme[7];						//niz koji sadrzi podatke o trenutnom vremenu
		trVreme[0] = decToBcd(sec);				//Konverzija podataka u binarne vrednostikoje saljeme RTC
		trVreme[1] = decToBcd(min);
		trVreme[2] = decToBcd(hour);
		trVreme[3] = decToBcd(dow);
		trVreme[4] = decToBcd(dom);
		trVreme[5] = decToBcd(month);
		trVreme[6] = decToBcd(year);

	 HAL_I2C_Mem_Write(&hi2c1, (0x68<<1), 0x00, 1, trVreme, sizeof(trVreme), 100);			//Vrsimo upis Trenutnih vrednosti u memoriju RTC-a

 }

float getTemp()
{
	int16_t vrednost;
	HAL_StatusTypeDef ret;
	uint8_t buf[12];
	float temperatura;

	ret=HAL_I2C_Master_Transmit(&hi2c1, (0x48<<1), 0x00, 1, HAL_MAX_DELAY);
	if(ret!=HAL_OK)
	{
		temperatura=-254;
	}

	ret=HAL_I2C_Master_Receive(&hi2c1, (0x48<<1), buf, 2, HAL_MAX_DELAY);
	if(ret!=HAL_OK)
	{
		temperatura=-200;
	}else
	{
		vrednost=((int16_t)buf[0]<<4) | (buf[1]>>4);

		if(vrednost>0x7FF)
		{
			vrednost|=0xF000;
		}

		temperatura=vrednost*0.0625;
		return temperatura;
	}
}
