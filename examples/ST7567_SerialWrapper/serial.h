#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include "main.h"


#define SERIAL_BUF_SIZE 256
extern uint8_t rx_data;
extern volatile uint8_t rx_fifo[SERIAL_BUF_SIZE];
extern volatile uint16_t head, tail;

void serialBegin(UART_HandleTypeDef *huart);
uint16_t serialAvailable(void);
uint8_t serialRead(void);
bool serialReadArgs(uint8_t* buf, uint16_t count, uint32_t timeout = 100);
#endif
