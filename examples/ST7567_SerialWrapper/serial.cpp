#include "serial.h"
#include "cmsis_gcc.h"
#include <stdint.h>

uint8_t rx_data;
volatile uint8_t rx_fifo[SERIAL_BUF_SIZE];
volatile uint16_t head, tail;
volatile uint16_t rx_count;

void serialBegin(UART_HandleTypeDef *huart) {
    head = 0;
    tail = 0;
    HAL_UART_Receive_IT(huart, &rx_data, 1);
}

uint16_t serialAvailable(void) {
    return (head + SERIAL_BUF_SIZE - tail) % SERIAL_BUF_SIZE;
}

uint8_t serialRead(void) {
    if (head == tail) return 0;
    uint8_t data = rx_fifo[tail];
    tail = (tail + 1) % SERIAL_BUF_SIZE;
    return data;
}

bool serialReadArgs(uint8_t* buf, uint16_t count, uint32_t timeout) {
    uint32_t start_time = HAL_GetTick();

    for (uint16_t i = 0; i < count; i++) {
        // Wait until a byte arrives or 100ms pass
        uint16_t available = serialAvailable();
        while (available == 0) {
            available = serialAvailable();
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
        // Calculate the next head index
        uint16_t next_head = (head + 1) % SERIAL_BUF_SIZE;

        // If the buffer is not full, add the new byte to the buffer
        if (next_head != tail) {
            rx_fifo[head] = rx_data;
            head = next_head;
            rx_count++;
        }


        // Re enable the UART interrupt
        HAL_UART_Receive_IT(huart, &rx_data, 1);
    }
}
