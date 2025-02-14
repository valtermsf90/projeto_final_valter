#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "funcoes.h"

#include "ws2818b.pio.h"
#include <stdlib.h>
#include <time.h>

#define CONTADOR_LED 25
#define PINO_MATRIZ_LED 7
#define BUZZER_A 21
#define BUZZER_B 10
#define LED_G 11
#define LED_R 13
#define LED_B 12
#define BT_A 5
#define BT_B 6
#define BT_J 23


int tamanho_enigma = 0;
int numEnigma[5] = {0,0,0,0,0};


// Definição de uma estrutura para representar um pixel.
struct pixel_t {
	uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t LED_da_matriz;

// Definição de uma estrutura para representar uma matriz de pixels.
LED_da_matriz leds[CONTADOR_LED];
// Definições de variáveis globais.

PIO maquina_pio;
uint variavel_maquina_de_estado;

//Prototipos
void inicializacao_maquina_pio(uint pino);
void atribuir_cor_ao_led(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensidade);
void limpar_o_buffer();
void escrever_no_buffer();
void inicializacao_gpio();
void desenhar(char matriz[5][5]);



void inicializacao_gpio(){
	// gpio_init(BT);
	gpio_init(BT_A);
	gpio_set_dir(BT_A, GPIO_IN);
	gpio_pull_up(BT_A);
	gpio_init(BT_B);
	gpio_set_dir(BT_B, GPIO_IN);
	gpio_pull_up(BT_B);
	gpio_init(BT_J);
	gpio_set_dir(BT_J, GPIO_IN);
	gpio_pull_up(BT_J);
	// gpio_init(BUZZER);
	gpio_init(BUZZER_A);
	gpio_set_dir(BUZZER_A, GPIO_OUT);
	gpio_init(BUZZER_B);
	gpio_set_dir(BUZZER_B, GPIO_OUT);
	// gpio_init(LED);	
	gpio_init(LED_G);
	gpio_set_dir(LED_G, GPIO_OUT);
	gpio_init(LED_R);
	gpio_set_dir(LED_R, GPIO_OUT);
	gpio_init(LED_B);
	gpio_set_dir(LED_B, GPIO_OUT);

}

// inicializacao da maquina PIO

void inicializacao_maquina_pio(uint pino){
	uint programa_pio, i;
	// Cria programa PIO.
	programa_pio = pio_add_program(pio0, &ws2818b_program);
	maquina_pio = pio0;

	// Toma posse de uma máquina PIO.
	variavel_maquina_de_estado = pio_claim_unused_sm(maquina_pio, false);
	if (variavel_maquina_de_estado < 0) {
		maquina_pio = pio1;
		variavel_maquina_de_estado = pio_claim_unused_sm(maquina_pio, true); // Se nenhuma máquina estiver livre, panic!
	}


	// Inicia programa na máquina PIO obtida.
	ws2818b_program_init(maquina_pio, variavel_maquina_de_estado, programa_pio, pino, 800000.f);

	// Limpa buffer de pixels.
	for (i = 0; i < CONTADOR_LED; ++i) {
		leds[i].R = 0;
		leds[i].G = 0;
		leds[i].B = 0;
	}
}
 // atribuir cor ao led
void atribuir_cor_ao_led(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensidade) {
    
    if (intensidade > 255) intensidade = 255;
    if (intensidade < 0) intensidade = 0;

    // Ajusta os valores de RGB conforme a intensidade escolhida
    leds[indice].R = (r * intensidade);
    leds[indice].G = (g * intensidade);
    leds[indice].B = (b * intensidade);
}

// escrever no buffer
void escrever_no_buffer(){
	// Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
	for (uint i = 0; i < CONTADOR_LED; ++i){
		pio_sm_put_blocking(maquina_pio, variavel_maquina_de_estado, leds[i].G);
		pio_sm_put_blocking(maquina_pio, variavel_maquina_de_estado, leds[i].R);
		pio_sm_put_blocking(maquina_pio, variavel_maquina_de_estado, leds[i].B);
	}
	sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}
// limpar o buffer
void limpar_o_buffer(){	
	for (uint i = 0; i < CONTADOR_LED; ++i){
	    atribuir_cor_ao_led(i,0,0,0,0);
	}			
}

