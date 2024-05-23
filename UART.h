/*
 * UART.h
 *
 * Created: 5/22/2024 10:20:16 AM
 *  Author: samue
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void initUART9600(void);
void writeUART(char Caracter);
void writeUART_Text(char* texto);



#endif /* UART_H_ */