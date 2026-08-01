#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

// Definición de pines y parámetros[cite: 8, 9]
#define UART_NUM UART_NUM_2
#define TXD_PIN 17
#define RXD_PIN 16
#define LED_PIN 2 // Pin del LED integrado
#define BUF_SIZE 1024

int cmd_count = 0; // Contador de comandos[cite: 8]

// Función para configurar el UART2[cite: 8]
void init_uart(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    // Configuramos los parámetros del UART2
    uart_param_config(UART_NUM, &uart_config);
    // Asignamos los pines 16 y 17[cite: 9]
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // Instalamos el driver (buffers de recepción y transmisión)
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
}

// Función para configurar el LED[cite: 8]
void init_led(void) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0); // Apagado por defecto
}

// Tarea principal para procesar comandos de forma no bloqueante[cite: 8]
void uart_task(void *arg) {
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    
    while (1) {
        // Leer datos del UART2 de forma no bloqueante (espera max 20 ticks)
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, 20 / portTICK_PERIOD_MS);
        
        if (len > 0) {
            data[len] = '\0'; // Convertir a string
            cmd_count++;      // Aumentar contador de comandos
            
            // Limpiar saltos de línea para comparar bien
            char* cmd = (char*)data;
            cmd[strcspn(cmd, "\r\n")] = 0; 
            
            // Evaluamos los comandos[cite: 8]
            if (strcmp(cmd, "status") == 0) {
                const char* resp = "Estado: Sistema Operando Correctamente\r\n";
                uart_write_bytes(UART_NUM, resp, strlen(resp));
            } 
            else if (strcmp(cmd, "led on") == 0) {
                gpio_set_level(LED_PIN, 1);
                const char* resp = "Accion: LED Encendido\r\n";
                uart_write_bytes(UART_NUM, resp, strlen(resp));
            } 
            else if (strcmp(cmd, "led off") == 0) {
                gpio_set_level(LED_PIN, 0);
                const char* resp = "Accion: LED Apagado\r\n";
                uart_write_bytes(UART_NUM, resp, strlen(resp));
            } 
            else if (strcmp(cmd, "info") == 0) {
                char resp[100];
                sprintf(resp, "Info: Baud Rate 115200, Puerto UART2, Comandos procesados: %d\r\n", cmd_count);
                uart_write_bytes(UART_NUM, resp, strlen(resp));
            } 
            else if (strcmp(cmd, "reset") == 0) {
                cmd_count = 0;
                const char* resp = "Accion: Contador de comandos reiniciado a 0\r\n";
                uart_write_bytes(UART_NUM, resp, strlen(resp));
            } 
            else {
                const char* resp = "Error: Comando no reconocido\r\n";
                uart_write_bytes(UART_NUM, resp, strlen(resp));
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Evitar que la tarea bloquee el CPU
    }
    free(data);
}

void app_main(void) {
    init_uart();
    init_led();
    
    // Imprimimos un mensaje de bienvenida en UART2
    const char* welcome = "Sistema Iniciado. Esperando comandos...\r\n";
    uart_write_bytes(UART_NUM, welcome, strlen(welcome));
    
    // Creamos la tarea que procesará el UART
    xTaskCreate(uart_task, "uart_task", 4096, NULL, 10, NULL);
}
