#include "hardware/pwm.h"

#define EIXO_Y 26 // ADC0
#define EIXO_X 27 // ADC1
#define MIC 28 //ADC2

uint16_t eixo_x_valor;
uint16_t eixo_y_valor;
int mic;
char str_x[5];
char str_y[5];
char str_mic[5];
void adc_config()
{
  adc_select_input(0);
  eixo_y_valor = adc_read();
  adc_select_input(1);
  eixo_x_valor = adc_read();
  adc_select_input(2);
  mic = adc_read();
  sprintf(str_x, "%d", eixo_x_valor); // Converte o inteiro em string
  sprintf(str_y, "%d", eixo_y_valor); // Converte o inteiro em string
  sprintf(str_mic, "%d", mic);        // Converte o inteiro em string
}