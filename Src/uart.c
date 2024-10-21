#include "uart.h"

#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define SYS_FREQ		(16000000)
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	9600

#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)
#define SR_RXNE			(1U<<5)
#define CR1_RXNEIE		(1U<<5)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart2_write(char *data); // Function prototype

void uart2_rxtx_init(void)
{
	/*********Configure UART GPIO pin********/
	/*Enable clock access to gpioa*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA2 and PA3 to alternate function mode*/
	GPIOA->MODER &=~ ((1U<<4) | (1U<<6));
	GPIOA->MODER |=  ((1U<<5) | (1U<<7));

	/* Configure PA5 as output */
	GPIOA->MODER &= ~(1U<<11);
	GPIOA->MODER |=  (1U<<10);
	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] |=  (1U<<8);
	GPIOA->AFR[0] |=  (1U<<9);
	GPIOA->AFR[0] |=  (1U<<10);
	GPIOA->AFR[0] &=~ (1U<<11);

	/*Set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] |=  (1U<<12);
	GPIOA->AFR[0] |=  (1U<<13);
	GPIOA->AFR[0] |=  (1U<<14);
	GPIOA->AFR[0] &=~ (1U<<15);

	/*********Configure UART module********/
	/*Enable clock access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure Baud Rate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE | USART_CR1_RXNEIE);

	/*Enable UART module*/
	USART2->CR1 |= CR1_UE;

	/*Enable NVIC for USART2*/
	NVIC_EnableIRQ(USART2_IRQn);

}

char uart2_read(void) {
    // Wait until the receive data register is not empty
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)(USART2->DR); // Return received character
}



void uart2_write(char *data)
{
    while (*data) {
        // Wait until the transmit data register is empty
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *data++; // Send character and increment pointer
    }
}


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate)
{
	USARTx->BRR =  compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}













