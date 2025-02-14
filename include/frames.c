#include "frames.h"

#define CONTADOR_LED 25
#define PINO_MATRIZ_LED 7

/*
R = vermelho
G = verde
B = azul
Y = amarelo
P = rosa
C = ciano
w = brando
* = apagado*/
uint8_t _intensidade_ = 255;
int tamanho_matriz = 5;

uint matrizint[5][5] = {
    {24, 23, 22, 21, 20},
    {15, 16, 17, 18, 19},
    {14, 13, 12, 11, 10},
    {5, 6, 7, 8, 9},
    {4, 3, 2, 1, 0}
};
//MATRIZZ------------------------

	
	





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
  

  





