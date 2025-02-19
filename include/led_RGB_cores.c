#include <stdio.h>
#include "pico/stdlib.h"

#define LED_R 13
#define LED_B 12
#define LED_G 11

#define TIME 300

// Funções para controle de cores

// Função para apagar todos os leds
void piscar( int cont, int mod){
  if(cont % mod == 0){
  apagado();}
}
void apagado(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 0);
 
  
}
// Função para acender o led B e G
void ciano(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 1);

}
// Função para acender o led R, G e B
void branco(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 1);
  
}
// Função para acender o led R e B
void rosa(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 0);
 
}
// Função para acender o led R e G
void amarelo(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 1);
 
}
// Função para acender o led R
void vermelho(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 0);
 
}
// Função para acender o led B
void azul(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 0);
  
}
// Função para acender o led G
void verde(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 1);
 
}
