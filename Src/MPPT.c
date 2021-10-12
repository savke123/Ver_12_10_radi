/*
 * MPPT.c
 *
 *  Created on: May 7, 2021
 *  Funkcija za ocitavanje poruka sa MPPT, sve podatke koje MPPT salje smestamo u
 *  strukturu Poruka. Za komunikaciju koristimo UART i ugradjena HAL funkcije.
 *  Povezivanje
 *  STM32 F429ZI- MPPT 75/15 BlueSolar
 *  PA3 (A0 na CN9 (pin 1)) UART2_RX-> MPPT_TX (pin3)
 *  PD5 (D53 na CN9 (pin 6)) UART2_TX-> MPPT_RX (pin2)
 *  5V-> Power+ (pin 4)
 *  GND->GND (pin 1)
 *      Author: Aleksandar
 */

#include "MPPT.h"
#include <string.h>
#include "main.h"				//Ukljucijemo radi prepoznavanja HAL funkcija
#include "cmsis_os.h"
UART_HandleTypeDef huart6;		/*UART za komunikaciju MPPT-a i STM-a*/
UART_HandleTypeDef huart3;      //Virtuelni UART ispis poruke koju smo dobili na UART 2



//extern struct Poruka por={.Pid=""};
struct Poruka prazno={.Pid=""};

//uint8_t mod=1;					//Always off
//uint8_t mod=2;					//Battery life
//uint8_t mod=3;					//Conv. algoritam 1 OFF < 11.1V ON > 13.1V
//uint8_t mod=4;					//Conv. algoritam 2 OFF < 11.8V ON > 14V
//uint8_t mod=5;					//Always on
//uint8_t mod=6;					//User def. algoritam 1 OFF < 10V ON > 14V (mozemo podesavati zeljene vrednosti preko alpikacije)
//uint8_t mod=7;					//User def. algoritam 2 OFF < 10V ON > 14V

void setMpptMode(uint8_t mod)
{
	const char off[] = ":8ABED0000B5\n"; 						// Proveriti da li je isti format poruke i za ostale MPPT!!!
	const char on[] = ":8ABED0004B1\n";
	const char baterryLife[] = ":8ABED0001B4\n";
	const char ConvAl1[] = ":8ABED0002B3\n";
	const char ConvAl2[] = ":8ABED0003B2\n";
	const char UserDefAl1[] = ":8ABED0005B0\n";
	const char UserDefAl2[] = ":8ABED0006AF\n";

	switch(mod)
	{
	case 1: HAL_UART_Transmit(&huart6, &off, sizeof(off), 1000);
			break;
	case 2: HAL_UART_Transmit(&huart6, &baterryLife, sizeof(baterryLife), 1000);
			break;
	case 3: HAL_UART_Transmit(&huart6, &ConvAl1, sizeof(ConvAl1), 1000);
			break;
	case 4: HAL_UART_Transmit(&huart6, &ConvAl2, sizeof(ConvAl2), 1000);
			break;
	case 5: HAL_UART_Transmit(&huart6, &on, sizeof(on), 1000);
			break;
	case 6: HAL_UART_Transmit(&huart6, &UserDefAl1, sizeof(UserDefAl1), 1000);
			break;
	case 7: HAL_UART_Transmit(&huart6, &UserDefAl2, sizeof(UserDefAl2), 1000);
			break;
	}

}

struct Poruka getMppt()
{
		uint8_t buffer[170]={0};				//Ocitavanje primljene poruke sa MPPT-a
		uint8_t buffer2[170]={0};
		struct Poruka por={.Pid=""};
		//Sluzi da sacuva poruku iz buffer-a
		por=prazno;
		uint8_t i=0, k=0,j=0;
		int upisaoPoruku=0;
		do{
			HAL_UART_Receive(&huart6, buffer, sizeof(buffer),2000);		//Upisivanje poruke sa MPPT-a u baffer

		}while(buffer[3]!='P' && buffer[4]!='I' && buffer[5]!='D');
		strcpy(buffer2,buffer);											//Kopiranje primljene poruke u buffer2
		//HAL_UART_Transmit(&huart3, buffer, sizeof(buffer),1000);		//Ispis na virtuelni port, samo zbog nas da vidimo poruku
		memset(buffer,'\0',sizeof(buffer));

		for(i=1;i<sizeof(buffer2);i++)									//Rasclanjivanje cele poruke na delove
		{															//koje upisujemo u elemente strukture poruka
			j=0;
			if(upisaoPoruku==0)
			{
				if(buffer2[i]=='\t')									//Svaka celina pocinje sa karakterom '\t'
				{														//tako znamo da krecemo da upisujemo
					k++;
					i++;												// i i j sluze za prolazak kroz poruku karakter po karakter

					switch(k)
					{
					case 1: do											//Preko do-while petlje vrsimo upis jednog
						{												//po jednog karaktera iz buffer2 u Poruku
							por.Pid[j]=buffer2[i];					//Nailaskom na karakter '\r' znamo da smo
							i++;										//dosli do kraja jednog elementa
							j++;										// sa k++ prelazimo na sledecu rec
						}while(buffer2[i]!='\r');
						i++; 											//preskacemo karakter '\n'
						break;

					case 2: do
						{
							por.FW[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++; 											//preskacemo karakter '\n'
							break;

					case 3: do
						{
							por.SER[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;  										//preskacemo karakter '\n'
							break;

					case 4: do
						{
							por.V[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 5: do
						{
							por.I[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 6: do
						{
							por.Vpv[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 7: do
						{
							por.Ppv[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 8: do
						{
							por.Cs[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 9: do
						{
							por.Mppt[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 10: do
						{
							por.ERR[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 11: do
						{
							por.Load[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 12: do
						{
							por.Il[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 13: do
						{
							por.H19[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 14: do
						{
							por.H20[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 15: do
						{
							por.H21[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 16: do
						{
							por.H22[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 17: do
						{
							por.H23[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 18: do
						{
							por.Hsds[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							i++;
							break;

					case 19: do
						{
							por.Checksum[j]=buffer2[i];
							i++;
							j++;
						}while(buffer2[i]!='\r');
							k=0;
							i++;
							upisaoPoruku=1;
							break;
					}
				}

			}

		}

		return por;

}

