/*
 * PWM2.h
 *
 * Created: 5/20/2024 9:19:18 PM
 *  Author: samue
 */ 


#ifndef PWM2_H_
#define PWM2_H_


#include <stdint.h>
#include <avr/io.h>

void init_PWM2(void);
void updateDutyCicle2A(uint8_t duty);
void updateDutyCicle2B(uint8_t duty);


#endif /* PWM2_H_ */