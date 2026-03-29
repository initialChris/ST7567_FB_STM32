#include "serial.h"

uint8_t rx_data;
volatile uint8_t rx_fifo[SERIAL_BUF_SIZE];
volatile uint16_t head, tail;

void serialBegin(UART_HandleTypeDef *huart) {
    HAL_UART_Receive_IT(huart, &rx_data, 1);
}

bool serialAvailable(void) {
    return head != tail;
}

uint8_t serialRead(void) {
    if (head == tail) return 0;
    uint8_t data = rx_fifo[tail];
    tail = (tail + 1) % SERIAL_BUF_SIZE;
    return data;
}

bool serialReadArgs(uint8_t* buf, uint8_t count, uint32_t timeout) {
    uint32_t start_time = HAL_GetTick();

    for (uint8_t i = 0; i < count; i++) {
        // Wait until a byte arrives or 100ms pass
        while (!serialAvailable()) {
            if (HAL_GetTick() - start_time > timeout) {
                return false; // Timeout: data lost or incomplete
            }
        }
        buf[i] = serialRead();
    }
    return true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        // Calcola la prossima posizione della testa
        uint16_t next_head = (head + 1) % SERIAL_BUF_SIZE;

        // Se non abbiamo superato la coda (buffer pieno), salviamo il dato
        if (next_head != tail) {
            rx_fifo[head] = rx_data;
            head = next_head;
        }

        // Riattiva l'interrupt per il prossimo byte
        HAL_UART_Receive_IT(huart, &rx_data, 1);
    }
}
