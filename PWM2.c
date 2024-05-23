/*
 * PWM2.c
 *
 * Created: 5/20/2024 9:20:10 PM
 *  Author: samue
 */ 
#include "PWM2.h"

void init_PWM2(void){
	// PUERTOS DE SALIDA DE SALIDA 
	DDRB |= (1 << DDB3);
	DDRD |= (1 << DDD3);
	TCCR2A = 0;
	TCCR2B = 0;
	// MODO NO INVERTIDO 
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2B1);
	// MODO FAST 
	TCCR2A |= (1<<WGM21)|(1<<WGM20);
	// USAMOS PRESCALER DE 256
	TCCR2B |= (1<<CS22)|(1<<CS21);
}

void updateDutyCicle2A(uint8_t duty){
	OCR2A = duty;
}

void updateDutyCicle2B(uint8_t duty){
	OCR2B = duty;
}