#include <stdint.h>
#include <math.h>

#define R_REF 12129.0
#define A1 3.354016e-3
#define B1 2.569850e-4
#define C1 2.620131e-6
#define D1 6.383091e-8

#define AVERAGES 3000

void setup() {
	Serial.begin(9600);
}

void loop()
{
	double thermistor_r_sum = 0.0;

	for(uint16_t i=0; i<AVERAGES; i++) {
		uint16_t reading = analogRead(0);
		/* Small value readings are caused by erros */
		if (reading < 63 || reading == 1023) {
			i--;
			continue;
		}
		double thermistor_r = R_REF * (1023/(double)reading -1);
		thermistor_r_sum += thermistor_r;
	}

	double r_avg = thermistor_r_sum / AVERAGES;
	double log_calc = log(r_avg / R_REF);
	double temperature = 1 / (A1 + B1 * log_calc + C1 * pow(log_calc, 2) + D1 * pow(log_calc, 3)) - 275.15;

	Serial.println(temperature);
}
