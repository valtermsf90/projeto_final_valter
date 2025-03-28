#include "frames.h"

/*
R = vermelho
G = verde
B = azul
Y = amarelo
P = rosa
C = ciano
w = brando
* = apagado*/

char matriz_0[5][5] = {
    {'*', 'Y', 'Y', 'Y', '*'},
    {'*', 'Y', '*', 'Y', '*'},
    {'*', 'Y', '*', 'Y', '*'},
    {'*', 'Y', '*', 'Y', '*'},
    {'*', 'Y', 'Y', 'Y', '*'}};
char matriz_1[5][5] = {
    {'*', '*', 'W', '*', '*'},
    {'*', 'W', 'W', '*', '*'},
    {'*', '*', 'W', '*', '*'},
    {'*', '*', 'W', '*', '*'},
    {'*', 'W', 'W', 'W', '*'}};
char matriz_2[5][5] = {
    {'*', 'B', 'B', 'B', '*'},
    {'*', '*', '*', 'B', '*'},
    {'*', 'B', 'B', 'B', '*'},
    {'*', 'B', '*', '*', '*'},
    {'*', 'B', 'B', 'B', '*'}};
char matriz_3[5][5] = {
    {'*', 'R', 'R', 'R', '*'},
    {'*', '*', '*', 'R', '*'},
    {'*', '*', 'R', 'R', '*'},
    {'*', '*', '*', 'R', '*'},
    {'*', 'R', 'R', 'R', '*'}};
char matriz_4[5][5] = {
    {'*', 'G', '*', 'G', '*'},
    {'*', 'G', '*', 'G', '*'},
    {'*', 'G', 'G', 'G', '*'},
    {'*', '*', '*', 'G', '*'},
    {'*', '*', '*', 'G', '*'}};

char matriz_5[5][5] = {
    {'*', 'P', 'P', 'P', '*'},
    {'*', 'P', '*', '*', '*'},
    {'*', 'P', 'P', 'P', '*'},
    {'*', '*', '*', 'P', '*'},
    {'*', 'P', 'P', 'P', '*'}};

char matriz_6[5][5] = {
    {'*', 'C', 'C', 'C', '*'},
    {'*', 'C', '*', '*', '*'},
    {'*', 'C', 'C', 'C', '*'},
    {'*', 'C', '*', 'C', '*'},
    {'*', 'C', 'C', 'C', '*'}};

char matriz_7[5][5] = {
    {'*', 'Y', 'Y', 'Y', '*'},
    {'*', '*', '*', 'Y', '*'},
    {'*', '*', '*', 'Y', '*'},
    {'*', '*', '*', 'Y', '*'},
    {'*', '*', '*', 'Y', '*'}};
char matriz_8[5][5] = {
    {'*', 'W', 'W', 'W', '*'},
    {'*', 'W', '*', 'W', '*'},
    {'*', 'W', 'W', 'W', '*'},
    {'*', 'W', '*', 'W', '*'},
    {'*', 'W', 'W', 'W', '*'}};

char matriz_9[5][5] = {
    {'*', 'G', 'G', 'G', '*'},
    {'*', 'G', '*', 'G', '*'},
    {'*', 'G', 'G', 'G', '*'},
    {'*', '*', '*', 'G', '*'},
    {'*', 'G', 'G', 'G', '*'}};

char (*matriz[10])[5] = {matriz_0, matriz_1, matriz_2, matriz_3, matriz_4, matriz_5, matriz_6, matriz_7, matriz_8, matriz_9};
;
const uint8_t planta[] = {
    0x20,0x71, 0xF3 ,0xF7 ,0x77 ,0x5D, 0x08 ,0x1C, 0x3E};
  
  const uint8_t direita[] = {
    0xFF, 0xFF, 0x7E, 0x7E, 0x3c, 0x3c , 0x18, 0x18};
  const uint8_t esquerda[] ={
    0x18, 0x18,0x3c, 0x3c ,0x7E, 0x7E,0xFF, 0xFF
  };
  
  const uint8_t baixo[] ={
    0x03 , 0x0f,0x3f,0xff,0xff, 0x3f, 0x0f,0x03
  };
  const uint8_t cima[] ={
    0xc0, 0xf0, 0xfc,0xff,0xff, 0xfc, 0xf0,0xc0
  };
  const uint8_t gota[] = {
    0xc0,0x3c,0x3f,0xff, 0xff,0x3f,0x3c ,0xc0};