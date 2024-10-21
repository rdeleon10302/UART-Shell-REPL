#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "uart.h"

#define BUFFER_SIZE 100

// Function prototypes
void process_command(char *command);
void set_pin_handler(char *args);
void read_pin_handler(char *args);

// Command structure definition
typedef struct {
    const char *command;
    void (*handler)(char *args);
} Command;

Command command_list[] = {
    {"SET", set_pin_handler},
    {"READ", read_pin_handler},
    // Add more commands here
};



// Main function
int main(void) {
    SystemInit();
    uart2_rxtx_init();

    char command_buffer[BUFFER_SIZE];

    while (1) {
    	NVIC_DisableIRQ(USART2_IRQn);
    	NVIC_EnableIRQ(USART2_IRQn);
        uart2_write("\rEnter command: ");
        int i = 0;
        NVIC_DisableIRQ(USART2_IRQn);

        // Read command from UART, make sure to process each character properly
        while (i < BUFFER_SIZE - 1) {
            char received = uart2_read(); // Blocking read
            char output[2] = {received, '\0'};  // Create a string with the character and a null terminator
            uart2_write(output);  // Write the string

            if (received == '\n' || received == '\r') {
                break; // End of command
            }

            command_buffer[i++] = received;
        }

        command_buffer[i] = '\0'; // Null-terminate the string
        NVIC_EnableIRQ(USART2_IRQn);

        // Debugging: Print the whole command received
        uart2_write("\nFull command received: ");
        uart2_write(command_buffer);
        uart2_write("\n");

        // Process the command
        process_command(command_buffer);

    }
}



// USART2 interrupt handler
void USART2_IRQHandler(void)
{
    /* Check if RXNE is set */
    if (USART2->SR & SR_RXNE)
    {
        char received = (char)(USART2->DR); // Read received data
        uart2_write((char[]){received, '\0'}); // Echo received data as a string
    }
}

// Function implementations for commands
void process_command(char *command) {
    char *token = strtok(command, " ");

    if (token == NULL) {
        uart2_write("No command entered\n");
        return;
    }

    // Debugging: Print the token received
    uart2_write("\rCommand token: ");
    uart2_write(token);
    uart2_write("\n");

    // Iterate through the command list and find a match
    for (size_t i = 0; i < sizeof(command_list) / sizeof(Command); i++) {
        if (strcmp(token, command_list[i].command) == 0) {
            // Pass the remaining part of the command as args
            command_list[i].handler(strtok(NULL, ""));
            return;
        }
    }

    uart2_write("\rUnknown command\n");
}


void set_pin_handler(char *args) {
    if (args) {
        char *pin_str = strtok(args, " ");
        if (pin_str) {
            int pin = atoi(pin_str); // Convert first argument to integer

            char *value_str = strtok(NULL, " "); // Get second argument
            if (value_str) {
                int value = atoi(value_str); // Convert second argument to integer (0 or 1)

                // Set the pin using the GPIO_WritePin function
                GPIO_WritePin(GPIOA, (1 << pin), value);  // Correct usage

                // Simulate setting the pin
                char response[50];
                sprintf(response, "\rSet pin %d to %d\n", pin, value);
                uart2_write(response);
            } else {
                uart2_write("\rMissing second argument (value)\n");
            }
        } else {
            uart2_write("\rMissing first argument (pin)\n");
        }
    } else {
        uart2_write("\rNo arguments provided for SET\n");
    }
}


void read_pin_handler(char *args) {
    if (args) {
        int pin = atoi(args); // Get the pin number
        int value = (GPIOA->ODR & (1 << pin)) ? 1 : 0; // Check the ODR instead of IDR
        char response[30];
        sprintf(response, "\rPin %d state: %d\n", pin, value);
        uart2_write(response);
    } else {
        uart2_write("\rMissing arguments for READ_PIN\n");
    }
}


// GPIO functions
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t Pin, uint8_t Value) {
    if (Value) {
        GPIOx->ODR |= Pin; // Set pin high
    } else {
        GPIOx->ODR &= ~Pin; // Set pin low
    }
}

int GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t Pin) {
    return (GPIOx->IDR & Pin) ? 1 : 0; // Return pin state
}


