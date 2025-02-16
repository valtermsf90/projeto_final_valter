#include "hardware/pwm.h"

#define WRAP 5000
const float DIVISOR_PWN = 1000;
bool flag;
uint slice;
uint pino;
uint16_t  step = 100;
uint16_t level = 100;
void config_pwm(pino, flag){
  
  gpio_set_function(pino, GPIO_FUNC_PWM);
  slice = pwm_gpio_to_slice_num(pino);
  pwm_set_clkdiv(slice,DIVISOR_PWN);
  pwm_set_wrap(slice,WRAP);
  pwm_set_gpio_level(pino,level);
  pwm_set_enabled(slice,true);
  }