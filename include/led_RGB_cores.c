#include <stdio.h>
#include "pico/stdlib.h"

#define LED_R 13
#define LED_B 12
#define LED_G 11

#define TIME 300

// Funções para controle de cores

// Função para apagar todos os leds
void apagado(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 0);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led B e G
void ciano(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 1);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led R, G e B
void branco(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 1);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led R e B
void rosa(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 0);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led R e G
void amarelo(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 1);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led R
void vermelho(int tempo)
{
  gpio_put(LED_R, 1);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 0);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led B
void azul(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 1);
  gpio_put(LED_G, 0);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
// Função para acender o led G
void verde(int tempo)
{
  gpio_put(LED_R, 0);
  gpio_put(LED_B, 0);
  gpio_put(LED_G, 1);
  if (tempo > 0)
  { // Se o tempo for maior que 0, o programa espera o tempo definido
    sleep_ms(tempo);
  }
}
