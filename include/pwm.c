#include "hardware/pwm.h"

// VALORES USADOS PARA OBTER A FREQUENCIA DE 100Hz COM UM CLOCK DE 125MHz
//  PELA FORAMULA ƒpwm = ƒclock/DIVISOR X WRAP
#define WRAP 10000
const float DIVISOR_PWN = 125;

bool flag;
uint slice;
uint pino;
uint16_t step = 100;
uint16_t level = 100;
int Hz;
void config_pwm(pino, flag)
{
  gpio_set_function(pino, GPIO_FUNC_PWM);
  slice = pwm_gpio_to_slice_num(pino);
  pwm_set_clkdiv(slice, DIVISOR_PWN);
  pwm_set_wrap(slice, WRAP);
  pwm_set_gpio_level(pino, level);
  pwm_set_enabled(slice, flag);
}
void config_pwm_beep(pino, flag, Hz)
{
  long int  fClock =  12500;
  int div_beep = fClock / Hz;
  gpio_set_function(pino, GPIO_FUNC_PWM);
  slice = pwm_gpio_to_slice_num(pino);
  pwm_set_clkdiv(slice, div_beep);
  pwm_set_wrap(slice, WRAP);
  pwm_set_gpio_level(pino, level);
  pwm_set_enabled(slice, flag);
}

void desativar_pwm() {
  pwm_set_gpio_level(LED_R, 0);
  pwm_set_gpio_level(LED_G, 0);
  pwm_set_gpio_level(LED_B, 0);
  pwm_set_gpio_level(BUZZER_A, 0);
  pwm_set_gpio_level(BUZZER_B, 0);
}
