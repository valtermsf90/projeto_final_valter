#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"   
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "include/frames.c"
#include "include/cores.c"
#include "include/funcoes.c"

//PROTOTIPOS



int main()
{
    stdio_init_all();
   
    inicializacao_gpio();
    inicializacao_maquina_pio(PINO_MATRIZ_LED);
    desenhar(matriz_0);
    escrever_no_buffer();
    while (true) {
        azul(200);
        apagado(100);
    }
}
