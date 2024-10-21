# UART-Shell-REPL
UART-based shell/REPL for reading and configuring GPIO pins over UART communication

This project implements a UART-based command-line shell (REPL) that interfaces with an embedded system via UART. It allows users to read and configure GPIO pins directly from a terminal. Ideal for debugging, testing, or controlling hardware peripherals, the shell offers a simple way to interact with the microcontroller, set pin states, and retrieve status information in real-time.

Features
- Interactive UART shell for embedded systems
- Commands to read and set GPIO pin states
- Real-time communication with the board over UART
- Easily customizable for various microcontrollers (e.g., STM32, Arduino)

Hardware Used
- STM32F411RE Nucleo Board

Software Used
- STM32IDE for firmware development
- Tera Term for UART communication

Technologies Used
- C/C++ for embedded firmware
- UART communication protocol
