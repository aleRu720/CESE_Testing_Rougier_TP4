# CESE_Testing_Rougier_TP4

Se tomó y adaptó una parte del código del proyecto, que maneja la MEF principal, esta tiene por objeto las sigueintes tareas:
- STANBY 
    - Lee datos almacenados en una memoria, si no tiene datos almacenados, espera recibir datos de conexión desde el puerto
    serie.
- CONFIGURARWIFI
    - Con los datos de configuración del Wifi, realiza la secuencia de conección del dispositivo, si todo funcionó, reporta TRUE y pasa a procesar las tareas.
- READY
    - Leer ADC
    -  Convertir Datos ADC en Valores de Ingeniería 
    - Enviar valores al Servidor 
    - Numero máximo de reintentos 3 
    - Poner el micro a "Dormir"
- SLEEP
    - Despierta el micro y lo pasa a READY