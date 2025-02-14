#include "hardware/pwm.h"

#define EIXO_Y 26 // ADC0
#define EIXO_X 27 // ADC1

uint16_t eixo_x_valor;
uint16_t eixo_y_valor;
void adc_config()
{
  adc_select_input(0);
  eixo_y_valor = adc_read();
  adc_select_input(1);
  eixo_x_valor = adc_read();
}