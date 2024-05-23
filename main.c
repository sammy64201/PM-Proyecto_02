/*
 * Proyecto_02_04.c
 *
 * Created: 5/20/2024 9:10:17 PM
 * Author : samue
 */ 
#define F_CPU 4000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>
//------------------------------------ LIBRERIAS -------------------------------------
#include "ADC/ADC.h"
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"
#include "UART/UART.h"
//------------------------------------ VARIABLES -------------------------------------

uint8_t pot1_ADC = 0;
uint16_t dutyCycle1 = 0;
uint8_t pot2_ADC = 0;
uint16_t dutyCycle2 = 0;
uint8_t pot3_ADC = 0;
uint8_t dutyCycle3 = 0;
uint8_t pot4_ADC = 0;
uint8_t dutyCycle4 = 0;
uint8_t selector = 0;
uint8_t modo = 0;
uint8_t memoria = 0;

uint8_t temp_pot1_ADC = 0;
uint8_t temp_pot2_ADC = 0;
uint8_t temp_pot3_ADC = 0;
uint8_t temp_pot4_ADC = 0;
//------------------------------------ DIRECCIONES DE EEPROM -------------------------------------
#define EEPROM_POS1 0x00
#define EEPROM_POS2 0x10
#define EEPROM_POS3 0x20
#define EEPROM_POS4 0x30
//------------------------------------ DECLARACION DE FUNCIONES -------------------------------------

void setup(void);
void manual(void);
void save_position(uint8_t memoria);
void load_position(uint8_t memoria);
void save_temp_position(void);
void apply_temp_position(void);
//------------------------------------ FUNCIONES -------------------------------------
uint16_t mapADCtoServo1(uint8_t valor_ADC1){
	return (uint16_t)(((float)valor_ADC1/255)*(149.88-37.47)+37.47);
}

uint16_t mapADCtoServo2(uint8_t valor_ADC2){
	return (uint16_t)(((float)valor_ADC2/255)*(149.88-37.47)+37.47);
}

uint8_t mapADCtoServo3(uint8_t valor_ADC3){
	return (uint8_t)(((float)valor_ADC3/255)*(31-5)+5);
}

uint8_t mapADCtoServo4(uint8_t valor_ADC4){
	return (uint8_t)(((float)valor_ADC4/255)*(31-5)+5);
}

void setup(void){
	cli();
	DDRC = 0;
	// PUERTOS DE LEDS DE MENU 
	DDRD |= (1<<DDD5)|(1<<DDD6)|(1<<DDD7);
	
	// PUERTOS DE ENTRADA DE BOTONES 
	DDRC &= ~(1<<DDC0) & ~(1<<DDC1) & ~(1<<DDC2);
	PORTC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2);
	// HABILITO INTERRUPCION DE BOTON 
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT8);
	init_PWM1();
	init_PWM2();
	initADC();
	initUART9600();
	sei();
}
void manual(){
	dutyCycle1 = mapADCtoServo1(pot1_ADC);
	updateDutyCicle1A(dutyCycle1);
	
	
	dutyCycle2 = mapADCtoServo2(pot2_ADC);
	updateDutyCicle1B(dutyCycle2);
	
	
	dutyCycle3 = mapADCtoServo3(pot3_ADC);
	updateDutyCicle2A(dutyCycle3);
	
	
	dutyCycle4 = mapADCtoServo4(pot4_ADC);
	updateDutyCicle2B(dutyCycle4);
}
void save_position(uint8_t memoria) {
	uint16_t addr = 0;
	switch(memoria) {
		case 0:
		addr = EEPROM_POS1;
		break;
		case 1:
		addr = EEPROM_POS2;
		break;
		case 2:
		addr = EEPROM_POS3;
		break;
		case 3:
		addr = EEPROM_POS4;
		break;
	}
	eeprom_update_byte((uint8_t*)(addr + 0), pot1_ADC);
	eeprom_update_byte((uint8_t*)(addr + 1), pot2_ADC);
	eeprom_update_byte((uint8_t*)(addr + 2), pot3_ADC);
	eeprom_update_byte((uint8_t*)(addr + 3), pot4_ADC);
}

void load_position(uint8_t memoria) {
	uint16_t addr = 0;
	switch(memoria) {
		case 0:
		addr = EEPROM_POS1;
		break;
		case 1:
		addr = EEPROM_POS2;
		break;
		case 2:
		addr = EEPROM_POS3;
		break;
		case 3:
		addr = EEPROM_POS4;
		break;
	}
	 pot1_ADC = eeprom_read_byte((uint8_t*)(addr + 0));
	 pot2_ADC = eeprom_read_byte((uint8_t*)(addr + 1));
	 pot3_ADC = eeprom_read_byte((uint8_t*)(addr + 2));
	 pot4_ADC = eeprom_read_byte((uint8_t*)(addr + 3));
}
void save_temp_position() {
	temp_pot1_ADC = pot1_ADC;
	temp_pot2_ADC = pot2_ADC;
	temp_pot3_ADC = pot3_ADC;
	temp_pot4_ADC = pot4_ADC;
}

void apply_temp_position() {
	pot1_ADC = temp_pot1_ADC;
	pot2_ADC = temp_pot2_ADC;
	pot3_ADC = temp_pot3_ADC;
	pot4_ADC = temp_pot4_ADC;
}


//---------------------------- MAIN -------------------------------------------------
int main(void)
{
	CLKPR = (1<<CLKPCE);
	CLKPR = (1<<CLKPS1); // CLK a 4 MHz
	setup();
	while (1)
	{
		if (modo == 0)
		{
			PORTD &= ~(1<<PIND7);
			PORTD |= (1<<PIND5);
			manual();
		}
		else if (modo == 1){
			PORTD &= ~(1<<PIND5);
			PORTD |= (1<<PIND6);
			load_position(memoria);
			manual();
		}
		else if (modo == 2){
			PORTD &= ~(1<<PIND6);
			PORTD |= (1<<PIND7);
			manual();
		}
		else if (modo == 3){
			PORTD &= ~(1<<PIND5)&~(1<<PIND6)&~(1<<PIND7);
		}
		
	}
}

//----------------------------INTERRUPCIONES-------------------------------------------------
ISR(ADC_vect){
	// PARAMOS EL ADC 
	ADCSRA &= ~(1<<ADEN);
	// SELECCIONAMOS QUE TOCA LEER 
	if (selector == 0)
	{
		// SELECCIONAMOS EL ADC
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | ((1<<MUX2)|(1<<MUX1)|(1<<MUX0)));
		pot1_ADC = ADCH;
		selector +=1;
	}
	else if (selector == 1)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | ((1<<MUX2)|(1<<MUX1)));
		pot2_ADC = ADCH;
		selector +=1;
	}
	else if (selector == 2)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | ((1<<MUX2)|(1<<MUX0)));
		pot3_ADC = ADCH;
		selector +=1;
	}
	else if (selector == 3)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | (1<<MUX2));
		pot4_ADC = ADCH;
		selector = 0;
	}
	// INICIAMOS EL ADC Y LA LECTURA NUEVAMENTE 
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);
	ADCSRA |= (1<<ADIF);

}

ISR (PCINT1_vect){
	if (!(PINC & (1 << PINC0))) {
		_delay_ms(100); // Debouncing delay
		if (!(PINC & (1 << PINC0))) { // Verifica nuevamente si el botón está presionado
			modo++; // Incrementa la variable modo
			if (modo >= 4) { // Si modo alcanza 3, reinícialo a 0
				modo = 0;
			}
		}
	}
}
ISR (PCINT2_vect) {
	if (modo == 2) { // Verifica si estamos en el modo 2 para activar el botón de grabación
		if (!(PINC & (1 << PINC1))) { // Botón para grabar la posición actual en la memoria
			_delay_ms(100); // Debouncing delay
			if (!(PINC & (1 << PINC1))) { // Verifica nuevamente si el botón está presionado
				save_temp_position(); // Guardar la posición actual temporalmente
				save_position(memoria); // Guardar la posición actual en la memoria
			}
		}
	}
}
ISR(USART_RX_vect) {
	char received = UDR0;
	if (received == '1') {
		modo++;
		if (modo >= 4) {
			modo = 0;
		}
	}
}
	
