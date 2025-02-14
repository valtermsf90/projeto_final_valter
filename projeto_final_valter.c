#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "include/led_5x5.c"
#include "include/frames.c"
#include "include/led_RGB_cores.c"
#include "include/funcoes.c"
#include "include/pwm.c"

// PROTOTIPOS

int main()
{
    //inicilização do sistema I/O, PINOS, MATRIZ LED 5X5, ADC E PWM-+
    stdio_init_all();                                             //|
    inicializacao_gpio();                                         //|
    inicializacao_maquina_pio(PINO_MATRIZ_LED);                   //|
    adc_init();                                                   //|
    config_pwm(LED_B,200,50);                                                 //|
    //--------------------------------------------------------------+


    desenhar(matriz_3);
    limpar_o_buffer();
    escrever_no_buffer();
    while (true)
    {
        level = level + step;
        pwm_set_gpio_level(LED_G,level);
        sleep_ms(200);
        printf("%c\n",level);
        
        
    }
}
