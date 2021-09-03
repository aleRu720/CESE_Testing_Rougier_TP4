#include "unity.h"
#include "mefppal.h"
#include "mock_wifi.h"
#include "mock_comSerial.h"
#include "mock_myDelay.h"
#include "mock_storageData.h"

typedef enum{
	STANDBY_TEST,
	CONFIGURARWIFI_TEST,
	READY_TEST,
	SLEEP_TEST
}eMefPpalTest;


#define ESTADOINVALIDO  -1

/* Verifica que la MEF se iniciliza correctamente*/
void test_inicializar_MEF_principal(void){
    estadoPrograma=READY_TEST;
    iniciaMefPpal();
    TEST_ASSERT_EQUAL(STANDBY_TEST,estadoPrograma);
}

/* Si la MEF toma un estado inválido, tiene que poder reiniciarse*/
void test_MEF_estado_invalido(void){
    estadoPrograma=ESTADOINVALIDO;
    accionesMef();
    TEST_ASSERT_EQUAL(STANDBY_TEST,estadoPrograma);
}

/* Verifica que cambia al estado de configurarwifi si no tiene datos almacenados y recibe datos desde una PC*/
void test_recibe_datos_configuracion_via_pc(void){
    estadoPrograma=STANDBY_TEST;
    storageConfigCon_IgnoreAndReturn(FALSE);
    comSerialRecibe_IgnoreAndReturn(TRUE);
    accionesMef();
    TEST_ASSERT_EQUAL(CONFIGURARWIFI_TEST,estadoPrograma);
}

/* Verifica que cambia al estado de configurarwifi si tiene datos almacenados y puede iniciar la conexión*/
void test_lee_datos_configuracion_memoria_usb(void){
    estadoPrograma=STANDBY_TEST;
    storageConfigCon_IgnoreAndReturn(TRUE);
    accionesMef();
    TEST_ASSERT_EQUAL(CONFIGURARWIFI_TEST,estadoPrograma);
}

/* Verifica que manda al micro a "Dormir" si pudo transmitir los datos correctamente */
void test_mandar_micro_sleep(void){
    estadoPrograma=READY_TEST;
    wifiSend_IgnoreAndReturn(TRUE);
    accionesMef();
    TEST_ASSERT_EQUAL(SLEEP_TEST,estadoPrograma);
}

/* Verifica que retrasnmite los datos, si falla y luego manda al micro a "Dormir" */
void test_perdida_comunicacion_wifi_recuperacion_comunicacion(void){
    estadoPrograma=READY_TEST;
    wifiSend_IgnoreAndReturn(FALSE);
    conectarWifi_IgnoreAndReturn(TRUE);
    wifiSend_IgnoreAndReturn(TRUE);
    accionesMef();
    TEST_ASSERT_EQUAL(SLEEP_TEST,estadoPrograma);
}

/* Verifica que si no logra reestablecer la comunicación, manda a realizar la conexión desde el principio*/
void test_perdida_comunicacion_wifi_sin_recuperacion_comunicacion(void){
    estadoPrograma=READY_TEST;
    wifiSend_IgnoreAndReturn(FALSE);
    conectarWifi_IgnoreAndReturn(FALSE);
    accionesMef();
    TEST_ASSERT_EQUAL(CONFIGURARWIFI_TEST,estadoPrograma);
}

/* Verifica que cumplido el tiempo de sleep, la MEF vuelve a READY */
void test_sacar_micro_sleep_tiempo_cumplido(void){
    estadoPrograma=SLEEP_TEST;
    delayRead_IgnoreAndReturn(TRUE);
    accionesMef();
    TEST_ASSERT_EQUAL(READY_TEST,estadoPrograma);
}

/* Verifica que si no se ha cumplido el tiempo de Sleep, la MEF no cambie de estado*/
void test_sacar_micro_sleep_tiempo_restante(void){
    estadoPrograma=SLEEP_TEST;
    delayRead_IgnoreAndReturn(FALSE);
    accionesMef();
    TEST_ASSERT_EQUAL(SLEEP_TEST,estadoPrograma);
}