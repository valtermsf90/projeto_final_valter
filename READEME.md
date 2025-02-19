Nome do Projeto

Sistema de Monitoramento e Controle com Matriz de LED e Sensores

Descrição

Este projeto implementa um sistema embarcado utilizando o Raspberry Pi Pico W para monitoramento e controle de variáveis ambientais e fisiológicas. Ele inclui sensores para temperatura, umidade do solo, radiação UV e sinais vitais, além de controle de LEDs RGB e matriz de LED para exibição de informações.

Tecnologias e Bibliotecas Utilizadas

Plataforma: Raspberry Pi Pico W

Linguagem: C

Bibliotecas:

pico/stdlib.h (para controle geral)

hardware/adc.h (para leitura de sensores analógicos)

hardware/pwm.h (para controle de LEDs e buzzers)

hardware/i2c.h (para comunicação com display OLED)

hardware/timer.h (para temporização)

Outros Arquivos Incluídos:

led_5x5.c, frames.c, led_RGB_cores.c, funcoes.c, pwm.c, adc.c

Funcionalidades

Monitoramento de temperatura, umidade do solo e radiação UV

Exibição de sinais vitais (frequência cardíaca, respiração, pressão arterial)

Controle de LEDs RGB e buzzer com base nos sensores

Exibição gráfica de dados no display OLED

Modo automático de controle de irrigação e abastecimento de tanque

Modos de Operação

Modo 1 - Monitoramento ambiental: Exibe informações sobre temperatura, umidade do solo e radiação UV, além de permitir a ativação do modo automático para controle da irrigação e abastecimento do tanque.

Botão A: Alterna o modo automático de irrigação.

Botão B: Alterna o status do sistema.

Modo 2 - Monitoramento fisiológico: Exibe dados de sinais vitais como frequência cardíaca, respiração e pressão arterial, permitindo uma análise visual através do display OLED.

Botão A: Alterna exibição entre diferentes parâmetros.

Botão B: Reseta os valores exibidos.

Modo 3 - Animação visual: Exibe animações interativas na matriz de LED, como olhos que seguem a direção do joystick, proporcionando uma experiência visual dinâmica.

Botão A: Alterna entre expressões faciais.

Botão B: Liga ou desliga a iluminação dos LEDs.

Modo 4 - Diagnóstico do sistema: Apresenta informações sobre os LEDs, botões, sensores analógicos e periféricos conectados, facilitando a depuração e verificação do hardware.

Botão A: Testa todos os LEDs e buzzers.

Botão B: Alterna a exibição entre diferentes sensores.

Como Compilar e Executar

Requisitos:

SDK do Raspberry Pi Pico configurado

Compilador GCC para ARM (arm-none-eabi-gcc)

Ferramenta CMake

Compilação:

mkdir build
cd build
cmake ..
make

Upload para o Pico:

Conecte o Pico ao PC segurando o botão BOOTSEL

Copie o arquivo .uf2 gerado para a unidade do Pico

Uso

O sistema inicia exibindo informações ambientais.

Botões permitem alternar entre diferentes telas e modos.

O sistema pode operar automaticamente ou ser controlado manualmente.

Autor

Valter Machado Silva Filho

link GIthub
https://github.com/valtermsf90/projeto_final_valter/
