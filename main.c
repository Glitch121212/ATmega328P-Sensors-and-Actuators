
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void Servo_Init(void)
{
	DDRB |= (1 << PB1);

	// Fast PWM Mode 14
	// Non-inverting OC1A
	TCCR1A = (1 << COM1A1) | (1 << WGM11);

	// Prescaler = 8
	TCCR1B = (1 << WGM13) |
	(1 << WGM12) |
	(1 << CS11);

	ICR1 = 39999;
}

void ADC_Init(void)
{
	ADMUX = (1 << REFS0);

	ADCSRA = (1 << ADEN) |
	(1 << ADPS2) |
	(1 << ADPS1) |
	(1 << ADPS0);
}

unsigned int ADC_Read(void)
{
	ADMUX &= 0xF0;

	ADCSRA |= (1 << ADSC);

	while (ADCSRA & (1 << ADSC));

	return ADC;
}

int main(void)
{
	unsigned int adc_value;
	unsigned int servo_value;

	Servo_Init();
	ADC_Init();

	while (1)
	{
		adc_value = ADC_Read();

		servo_value = (adc_value * 2000UL) / 1023;

		OCR1A = 2000 + servo_value;

		_delay_ms(20);
	}
}
