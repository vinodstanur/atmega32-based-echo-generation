////////////////////////////////////////////////////////////////////////////
//Atmega32 based echo generator                                           //
//visit my blog http://blog.vinu.co.in for more details                   //
//http://blog.vinu.co.in/2012/05/generating-audio-echo-using-atmega32.html//
//vinodstanur@gmail.com                                                   //
////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

uint8_t buf[1900];

void pwm_init();
void adc_init();
uint16_t adc_read();
void switch_enable();

void main()
{
	int i,j;
	int8_t echo_level = 4;
	uint16_t rd;
	adc_init();
	pwm_init();
	switch_enable();
	i = 0;j=1;
	while(1) {
		rd = adc_read() +  buf[j]*echo_level;
		rd/=8;
		if(rd>255)rd=255;
		_delay_us(100);
		buf[i] = rd;
		OCR0 = rd;
		if(!(PINB & (1<<PB1))) {
			if(echo_level < 4) { 
				echo_level++;
				_delay_ms(300);
			}
		} else if(!(PINB & (1<<PB2))) {
			if(echo_level > 0) {
				echo_level--;
				_delay_ms(300);
			}
		}
		i++;
		if(i==1899)i=0;
		j++;
		if(j==1899)j=0;
	}

}

void adc_init()
{
	ADMUX = 0b11000000;
	ADCSRA =0b10000010;
}

void switch_enable()
{
	DDRB &= ~((1<<PB1)|(1<<PB2));
	PORTB = (1<<PB1)|(1<<PB2);
}

uint16_t adc_read()
{
	uint16_t retl,reth;
	ADCSRA |= 1<<ADSC;
	while(!ADIF);
	ADCSRA |= 1<<ADIF;
	retl = ADCL;
	reth = ADCH;
	reth<<=8;
	reth|=retl;
	return reth;
}

void pwm_init()
{
    TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00);
    DDRB|=(1<<PB3);
}

