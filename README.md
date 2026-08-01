Tarea 5 - Sistemas Embebidos: Ejercicio 1 (UART2)

Este repositorio contiene la solución al Ejercicio 1 de la Tarea 5, enfocado en la implementación de comunicación serial avanzada utilizando el framework ESP-IDF en un microcontrolador ESP32.

- Descripción del Proyecto
El sistema inicializa el puerto UART2 y procesa comandos de texto enviados desde un terminal externo de forma no bloqueante utilizando una tarea de FreeRTOS. 

- Comandos Soportados:
`status`: Devuelve el estado actual del sistema.
`led on` / `led off`: Controla el encendido y apagado de un LED simulado.
`info`: Muestra información del sistema (Baud rate, puerto y contador de comandos).
`reset`: Reinicia el contador interno de comandos procesados.

Instrucciones de Compilación y Ejecución
1. Clonar o descargar este repositorio en su máquina local.
2. Abrir la carpeta del proyecto en Visual Studio Code con la extensión PlatformIO instalada.
3. Permitir que PlatformIO inicialice el entorno y descargue el framework ESP-IDF.
4. Hacer clic en el icono de Build (✓) en la barra inferior de PlatformIO para compilar el código.
5. Para probar el sistema mediante simulación, asegúrese de tener la extensión de Wokwi instalada, abra el archivo `diagram.json`, presione `F1` y ejecute el comando `Wokwi: Start Simulator`.