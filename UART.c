/*
 * UART.c
 *
 * Created: 5/22/2024 10:25:46 AM
 *  Author: samue
 */ 
#include "UART.h"

void initUART9600(void){
	// paso 1: establecer pines como entrada y salida
	//RX COMO ENTRADA Y TX COMO SALIDA
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	// PASO 2: configurar UCSR0A
	UCSR0A = 0;
	
	// PASO 3: configurar UCSR0B  DONDE SE HABILITA LA ISR DE RECEPCION
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);

	//PASO 4: CONFIGURAR UCSR0C
	UCSR0C = 0;
	//CONFIGURAR ASINCORONO Y SIN PARIDAD, 1 BIT DE STOP, 8 CARACTERES
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	//PASO 5: SE CALCULA EL VALOR DE BAUD Y VELOCIDAD: 9600
	UBRR0 = 103;
}
void writeUART(char Caracter){
	// si no es 1 quedate esperando POR LO TANTO HASTA QUE SEA 1 VA CONTINUAR
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
}
void writeUART_Text(char* Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)) );
		UDR0 = Texto[i];
	}
}
