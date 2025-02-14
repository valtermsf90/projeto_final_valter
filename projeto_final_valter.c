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
#include "include/adc.c"

// PROTOTIPOS

// VARIAVEIS
bool flag = true;
int main()
{
    // inicilização do sistema I/O, PINOS, MATRIZ LED 5X5, ADC E PWM-+
    stdio_init_all();                           //|
    inicializacao_gpio();                       //|
    inicializacao_maquina_pio(PINO_MATRIZ_LED); //|
    adc_init();                                 //|
    config_pwm(LED_G);
    config_pwm(LED_R); //|
    //--------------------------------------------------------------+

    limpar_o_buffer();
    escrever_no_buffer();
    while (true)
    {
        adc_config();
        
        pwm_set_gpio_level(LED_G, eixo_x_valor);
        pwm_set_gpio_level(LED_R, eixo_y_valor);

        printf("eixo x:%d\n", eixo_x_valor);
        printf("eixo y:%d\n", eixo_y_valor);

        /*if(flag == true){
            level = level + step;
            if(level >= WRAP){
                flag = false;
            }
        }else{
            level= level - step;
            if(level<= step){
                flag = true;
            }
        }*/
        sleep_ms(500);
    }
}
