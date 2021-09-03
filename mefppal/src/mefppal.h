#ifndef _MEFPPAL_H
#define _MEFPPAL_H


typedef enum{
	STANDBY,
	CONFIGURARWIFI,
	READY,
	SLEEP
}eMefPpal;

eMefPpal estadoPrograma;

void iniciaMefPpal(void);

void mefPpal(void);

void accionesMef(void);

#endif