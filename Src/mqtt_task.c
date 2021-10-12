/*
 * mqtt_task.c
 *
 *  Created on: 2020. 5. 19.
 *      Author: eziya76@gmail.com
 */

#include "main.h"
#include "mqtt_task.h"
#include "RTC.h"
#include "string.h"
#include "MPPT.h"
#include "funkcije.h"

#define SERVER_ADDR		"a36r1007e087sh-ats.iot.eu-west-1.amazonaws.com" //replace your endpoint here
#define MQTT_PORT		"8883"

//mqtt subscribe task
void MqttClientSubTask(void const *argument)
{
	while(1)
	{
		if(!mqttClient.isconnected)
		{
			//try to connect to the broker
			MQTTDisconnect(&mqttClient);
			MqttConnectBroker();
			osDelay(1000);
		}
		else
		{
			/* !!! Need to be fixed
			 * mbedtls_ssl_conf_read_timeout has problem with accurate timeout
			 */
			MQTTYield(&mqttClient, 1000);
		}
	}
}

//mqtt publish task
void MqttClientPubTask(void const *argument)
{
	//const char *str="{ \"MQTT\": \"poruka sa STM32\"}";
	//char *str=pvPortMalloc(100*sizeof(char));
	MQTTMessage message;
	float temp;
	uint8_t tempint;

	uint16_t memory;

	while(1)
	{
		char *str=pvPortMalloc(100*sizeof(char));
		temp=getTemp();
		tempint=temp/1;
		sprintf((char*)str,"{ \"MQTT\": \"poruka sa STM32\",\n \"Temperatura\" : \"%d\" \n}", tempint);
		printf("Temperatura je: %d \n",tempint);

		if(mqttClient.isconnected)
		{
			message.payload = (void*)str;
			message.payloadlen = strlen(str);

			MQTTPublish(&mqttClient, "test", &message); //publish a message to "test" topic
		}

		memory=xPortGetFreeHeapSize();
		printf("Slobodno posle poruke %d\n",memory);
		vPortFree((void*)str);
		osDelay(10000);
	}
}

void StartT2RTC(void const * argument)
{
  /* USER CODE BEGIN StartT2RTC */
	struct Vreme time;
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
	time=getTime();

	if((time.sat>=20 && time.sat<=23) || (time.sat>=0 && time.sat<1))
	{
		LedRingOn();
	}else
	{
		LedRingOff();
	}
    osDelay(7500);
  }
  /* USER CODE END StartT2RTC */
}

void StartT3MPPT(void const * argument)
{
  /* USER CODE BEGIN StartT3MPPT */
	struct Poruka PorMppt;
	int Vbat=0;
	uint16_t memory;

  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	  PorMppt=getMppt();
	  Vbat=atoi(PorMppt.V);

	  if(Vbat<12000)
	  {
		  if(Vbat<11300)
		  {
			  if(Vbat<10800)
			  {
				  ChargersOff();
			  }else
			  {
				  ChargersOn();
			  }

			  SensorsOff();
		  }else
		  {
			SensorsOn();
		  }

		  LedRingOff();
	  }else
	  {
		 LedRingOn();
		 SensorsOn();
		 ChargersOn();

	  }

	memory=xPortGetFreeHeapSize();
	printf("Slobodno %d\n",memory);
	osDelay(4000);
  }
  /* USER CODE END StartT2RTC */
}


void StartT4ADC(void const * argument)
{
	uint16_t memory;
	for(;;)
	{
		memory=xPortGetFreeHeapSize();
		printf("Slobodno %d\n",memory);

		osDelay(2000);
	}
}

int MqttConnectBroker()
{
	int ret;

	TaskHandle_t Task3MPPTHandle=NULL;
	TaskHandle_t Task2RTCHandle=NULL;
	TaskHandle_t Task4ADCHandle=NULL;

	net_clear();
	ret = net_init(&net, SERVER_ADDR);
	if(ret != MQTT_SUCCESS)
	{
		printf("net_init failed.\n");
		return -1;
	}

	ret = net_connect(&net, SERVER_ADDR, MQTT_PORT);
	if(ret != MQTT_SUCCESS)
	{
		printf("net_connect failed.\n");
		return -1;
	}

	MQTTClientInit(&mqttClient, &net, 1000, sndBuffer, sizeof(sndBuffer), rcvBuffer, sizeof(rcvBuffer));

	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = ""; //no client id required
	data.username.cstring = ""; //no user name required
	data.password.cstring = ""; //no password required
	data.keepAliveInterval = 60;
	data.cleansession = 1;

	ret = MQTTConnect(&mqttClient, &data);
	if(ret != MQTT_SUCCESS)
	{
		printf("MQTTConnect failed.\n");
		return ret;
	}

	ret = MQTTSubscribe(&mqttClient, "test", QOS0, MqttMessageArrived);
	if(ret != MQTT_SUCCESS)
	{
		printf("MQTTSubscribe failed.\n");
		return ret;
	}

	printf("MqttConnectBroker O.K.\n");

	xTaskCreate(StartT2RTC, "Task2RTC", 256, (void*)0, osPriorityNormal, &Task2RTCHandle);
	xTaskCreate(StartT3MPPT, "Task3MPPT", 512, (void*)0, osPriorityNormal, &Task3MPPTHandle);
	xTaskCreate(StartT4ADC, "Task4ADC", 128, (void*)0, osPriorityNormal, &Task4ADCHandle);

	return MQTT_SUCCESS;
}

void MqttMessageArrived(MessageData* msg)
{
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin); //toggle pin when new message arrived

	MQTTMessage* message = msg->message;
	memset(msgBuffer, 0, sizeof(msgBuffer));
	memcpy(msgBuffer, message->payload,message->payloadlen);

	printf("MQTT MSG[%d]:%s\n", (int)message->payloadlen, msgBuffer);
}
