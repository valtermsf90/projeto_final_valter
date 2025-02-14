#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"   
#include "hardware/pio.h"

#include "include/frames.h"
#include "include/cores.h"
#include "include/funcoes.h"

//PROTOTIPOS
void apagado(int tempo);
void ciano(int tempo);
void branco(int tempo);
void rosa(int tempo);
void amarelo(int tempo);
void vermelho(int tempo);
void azul(int tempo);
void verde(int tempo);


int main()
{
    stdio_init_all();

    while (true) {
        azul(200);
        apagado();
    }
}
