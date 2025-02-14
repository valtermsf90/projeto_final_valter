#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "funcoes.h"
#include "matrix.c"
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
uint8_t _intensidade_ = 255;
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

int tamanho_matriz = 5;

uint matrizint[5][5] = {
    {24, 23, 22, 21, 20},
    {15, 16, 17, 18, 19},
    {14, 13, 12, 11, 10},
    {5, 6, 7, 8, 9},
    {4, 3, 2, 1, 0}
};
//MATRIZZ------------------------


	
	char matriz_0[5][5]={
	{'*','Y','Y','Y','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','Y','Y','*'}
	};
	char matriz_1[5][5]={
	{'*','*','W','*','*'},
	{'*','W','W','*','*'},
	{'*','*','W','*','*'},
	{'*','*','W','*','*'},
	{'*','W','W','W','*'}
	};
	char matriz_2[5][5]={
	{'*','B','B','B','*'},
	{'*','*','*','B','*'},
	{'*','B','B','B','*'},
	{'*','B','*','*','*'},
	{'*','B','B','B','*'}
	};
	char matriz_3[5][5]={
	{'*','R','R','R','*'},
	{'*','*','*','R','*'},
	{'*','*','R','R','*'},
	{'*','*','*','R','*'},
	{'*','R','R','R','*'}
	};
	char matriz_4[5][5]={
	{'*','G','*','G','*'},
	{'*','G','*','G','*'},
	{'*','G','G','G','*'},
	{'*','*','*','G','*'},
	{'*','*','*','G','*'}
	};
	char matriz_5[5][5]={
	{'*','P','P','P','*'},
	{'*','P','*','*','*'},
	{'*','P','P','P','*'},
	{'*','*','*','P','*'},
	{'*','P','P','P','*'}
	};
	char matriz_6[5][5]={
	{'*','C','C','C','*'},
	{'*','C','*','*','*'},
	{'*','C','C','C','*'},
	{'*','C','*','C','*'},
	{'*','C','C','C','*'}
	};
	char matriz_7[5][5]={
	{'*','Y','Y','Y','*'},
	{'*','*','*','Y','*'},
	{'*','*','*','Y','*'},
	{'*','*','*','Y','*'},
	{'*','*','*','Y','*'}
	};
	char matriz_8[5][5]={
	{'*','W','W','W','*'},
	{'*','W','*','W','*'},
	{'*','W','W','W','*'},
	{'*','W','*','W','*'},
	{'*','W','W','W','*'}
	};
	char matriz_9[5][5]={
	{'*','G','G','G','*'},
	{'*','G','*','G','*'},
	{'*','G','G','G','*'},
	{'*','*','*','G','*'},
	{'*','G','G','G','*'}
	};
	char explosao1[5][5]={
	{'*','*','*','*','*'},
	{'*','*','*','*','*'},
	{'*','*','W','*','*'},
	{'*','*','*','*','*'},
	{'*','*','*','*','*'}
	};
	char explosao2[5][5]={
	{'*','*','*','*','*'},
	{'*','W','W','W','*'},
	{'*','W','Y','W','*'},
	{'*','W','W','W','*'},
	{'*','*','*','*','*'}
	};
	char explosao3[5][5]={
	{'W','W','W','W','W'},
	{'W','Y','Y','Y','W'},
	{'W','Y','R','Y','W'},
	{'W','Y','Y','Y','W'},
	{'W','W','W','W','W'}
	};
	char explosao4[5][5]={
	{'Y','Y','Y','Y','Y'},
	{'Y','R','R','R','Y'},
	{'Y','R','P','R','Y'},
	{'Y','R','R','R','Y'},
	{'Y','Y','Y','Y','Y'}
	} ;
	char explosao5[5][5]={
	{'R','R','R','R','R'},
	{'R','P','P','P','R'},
	{'R','P','B','P','R'},
	{'R','P','P','P','R'},
	{'R','R','R','R','R'}
	};
	char explosao6[5][5]={
	{'P','P','P','P','P'},
	{'P','B','B','B','P'},
	{'P','B','C','B','P'},
	{'P','B','B','B','P'},
	{'P','P','P','P','P'}
	};
	char explosao7[5][5]={
	{'B','B','B','B','B'},
	{'B','C','C','C','B'},
	{'B','C','G','C','B'},
	{'B','C','C','C','B'},
	{'B','B','B','B','B'}
	};
	char explosao8[5][5]={
	{'C','C','C','C','C'},
	{'C','G','G','G','C'},
	{'C','G','*','G','C'},
	{'C','G','G','G','C'},
	{'C','C','C','C','C'}
	};
	char explosao9[5][5]={
	{'G','G','G','G','G'},
	{'G','*','*','*','G'},
	{'G','*','*','*','G'},
	{'G','*','*','*','G'},
	{'G','G','G','G','G'}
	};
	char segredo0[5][5]={
	{'*','Y','Y','Y','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','Y','Y','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','*','Y','*'}
	};
	char segredo1[5][5]={
	{'*','Y','Y','*','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','Y','*','*'},
	{'*','Y','*','Y','*'},
	{'*','Y','Y','*','*'}
	};
	char segredo2[5][5]={
	{'*','Y','Y','Y','*'},
	{'*','Y','*','*','*'},
	{'*','Y','*','*','*'},
	{'*','Y','*','*','*'},
	{'*','Y','Y','Y','*'}
	};
char bola0[5][5]={
	{'*','R','R','R','*'},
	{'R','R','R','R','R'},
	{'R','R','R','R','R'},
	{'R','R','R','R','R'},
	{'*','R','R','R','*'}
	};
	char bola1[5][5]={
	{'*','Y','Y','Y','*'},
	{'Y','Y','Y','Y','Y'},
	{'Y','Y','Y','Y','Y'},
	{'Y','Y','Y','Y','Y'},
	{'*','Y','Y','Y','*'}
	};
	char bola2[5][5]={
	{'*','G','G','G','*'},
	{'G','G','G','G','G'},
	{'G','G','G','G','G'},
	{'G','G','G','G','G'},
	{'*','G','G','G','*'}
	};

char (*explosao[9])[5] = {explosao1, explosao2, explosao3, explosao4, explosao5, explosao6, explosao7, explosao8, explosao9};
char (*matriz[10])[5] = {matriz_0, matriz_1, matriz_2, matriz_3, matriz_4, matriz_5, matriz_6, matriz_7, matriz_8, matriz_9};
char (*segredo[3])[5] = {segredo0, segredo1, segredo2};
char (*bola[3])[5] = {bola0, bola1, bola2};



void desenhar(char desenho[5][5]){
	_intensidade_ = 255;
	for(int x = 0; x < tamanho_matriz; x++){
		for(int y = 0; y < tamanho_matriz; y++){
			if(desenho[x][y] == 'R'){
				atribuir_cor_ao_led(matrizint[x][y],255,0,0, _intensidade_);				
			}
			if(desenho[x][y] == 'G'){
				atribuir_cor_ao_led(matrizint[x][y],0,255,0, _intensidade_);
			}
			if(desenho[x][y] == 'B'){
				atribuir_cor_ao_led(matrizint[x][y],0,0,255, _intensidade_);
			}  
			if(desenho[x][y] == 'Y'){
				atribuir_cor_ao_led(matrizint[x][y],255,255,0, _intensidade_);
			}  
			if(desenho[x][y] == 'P'){
				atribuir_cor_ao_led(matrizint[x][y],255,0,255, _intensidade_);
			}  
			if(desenho[x][y] == 'C'){
				atribuir_cor_ao_led(matrizint[x][y],0,255,255, _intensidade_);
			}  
			if(desenho[x][y] == 'W'){
				atribuir_cor_ao_led(matrizint[x][y],255,255,255, _intensidade_);
            }
			if(desenho[x][y] == '*'){
				atribuir_cor_ao_led(matrizint[x][y],0,0,0, _intensidade_);
			} 
		}
        // Atualiza a matriz de LEDs e espera um tempo antes de mudar a cor
        escrever_no_buffer();
       sleep_ms(10); // 300ms entre as mudanças de cor
	}
}


void enigma(){
    srand(time(NULL));
    for(int i = 0; i < tamanho_enigma; i++){
        int numAleatorio = rand() % 3;
				
        switch (numAleatorio)
        {
        case 0:
          segredo[i] = segredo0;
					numEnigma[i] = numAleatorio;
            break;
        case 1:
					segredo[i] = segredo1;
					numEnigma[i] = numAleatorio;
            break;
        case 2:
					segredo[i] = segredo2;  
					numEnigma[i] = numAleatorio;
						break;  
        
        default:
            break;
        }    
    }
}

void mostrar_enigma(){
		for(int i = 0; i < tamanho_enigma; i++){
			desenhar(segredo[i]);
			escrever_no_buffer();
			sleep_ms(1000);
		}
}