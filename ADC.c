/*
 * ADC.c
 *
 * Created: 5/20/2024 9:24:02 PM
 *  Author: samue
 */ 
#include "ADC.h"

void initADC(){
	ADMUX = 0;
	// VOLTAJE DE REFERENCIA 
	ADMUX |= (1<<REFS0);
	// JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	// HABILITAR EL ADC 
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADIE);
	// PRESCALER DE 128
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRB = 0;
	DIDR0 |= (1<<ADC0D) | (1<<ADC1D) | (1<<ADC2D) | (1<<ADC3D);
	
	
	// COMENZAR LA CONVERSION
	ADCSRA |= (1<<ADSC);
}
