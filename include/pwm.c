#include "hardware/pwm.h"

#define WRAP 2500
const float DIVISOR_PWN = 1000;

uint pino;
uint16_t  step, level;
void config_pwm(pino,step,level){
  uint slice;
  gpio_set_function(pino, GPIO_FUNC_PWM);
  slice = pwm_gpio_to_slice_num(pino);
  pwm_set_clkdiv(slice,DIVISOR_PWN);
  pwm_set_wrap(slice,WRAP);
  pwm_set_gpio_level(pino,level);
  pwm_set_enabled(slice,true);
  }