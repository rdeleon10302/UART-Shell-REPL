#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"


char uart2_read(void);
void uart2_rxtx_init(void);
void uart2_write(char *data);
void process_command(char *command);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t Pin, uint8_t Value);



#define SR_RXNE			(1U<<5)
#define SR_TXNE			(1U<<5)

#endif /* UART_H_ */
