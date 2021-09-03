#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mefppal.h"
#include "wifi.h"
#include "myDelay.h"
#include "comSerial.h"
#include "storageData.h"

#define MAXENVIOS	3


delay_t alive, sleeptime;

void mefPpal(void){
	iniciaMefPpal();
   	while (TRUE)
   	{	
		   accionesMef();
		   //otras Acciones

	}
}

void iniciaMefPpal(void){
	estadoPrograma=STANDBY;
}


void accionesMef(void){
	uint8_t mensaje[100], nroEnvios;

	switch (estadoPrograma)
		{
			case STANDBY:
			/**
			 * Leer datos de la memoria USB para levantar la configuración en el caso de que no sea la primera vez que 
			 * arranca el equipo.
			 * Si es la primera vez, REVISAR COMUNICACIONES y pasar a CONFIGURAWIFI si se recibio info desde la PC
			 */
				if(storageConfigCon(&espConfig)){
					estadoPrograma=CONFIGURARWIFI;
				}else{
					if (comSerialRecibe()){
						estadoPrograma=CONFIGURARWIFI;
					}
				}
			
			break;
			case CONFIGURARWIFI:
				// configurar y conectar el WIFI, pasar a READY si todo salio correctamente
				if(wifiInit(&espConfig, PASTHRUEMODE)){
					estadoPrograma=READY;
				}
			break;
			case READY:
				//EJECUTAR RUTINAS
				/**
				 * 	Leer ADC
				 * 	Convertir Datos ADC en Valores de Ingeniería
				 * 	Enviar valores al Servidor
				 *	Nunero máximo de reintentos 3
				 */
				nroEnvios=0;
				while(nroEnvios < MAXENVIOS)
				{
					if ( wifiSend(mensaje, strlen(mensaje))){
						estadoPrograma=SLEEP;
						nroEnvios=MAXENVIOS;
					}else{
						if(!conectarWifi()){
							estadoPrograma=CONFIGURARWIFI;
							nroEnvios=MAXENVIOS;
						}
					}
					nroEnvios++;
				}
			break;
			case SLEEP:
			/**
			 * Manda el micro a "Dormir" luego de x tiempo lo despierta y lo pasa a READY
			 * llamado a delayRead solo a efectos de testear la lógica
			 */
			if(delayRead(&sleeptime))
				estadoPrograma=READY;
			break;
			default:
				iniciaMefPpal();
				
		}
}