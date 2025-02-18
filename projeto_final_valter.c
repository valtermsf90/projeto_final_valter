#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/bootrom.h"
#include "include/led_5x5.c"
#include "include/frames.c"
#include "include/led_RGB_cores.c"
#include "include/funcoes.c"
#include "include/pwm.c"
#include "include/adc.c"

// PROTOTIPOS
void config_display();
static void interrupcao(uint gpio, uint32_t events);
void tela(int modo);

// VARIAVEIS
bool cor = true;
bool st_led_R = true;
bool st_led_B = false;
bool led_ON = false;
bool status = false;
int quadro = 4;
int tx_atualizacao = 1000;
static volatile uint32_t last_time = 0;

// VARIAVEIS QUADRO 4
bool obito = false;
int cont = 0;
int y = 0;
int linha = 40;
int coluna = 55;
int x;
int y_invert;
int y2;
int pA = 0;
int pA2 = 0;
char str_pA[5];
char str_pA2[5];
int bpm = 0;
char str_bpm[5];
int resp;
char str_resp[5];
int temp_C;
char str_temp_C[5];
//===================
bool pulso(struct repeating_timer *t)
{
    gpio_put(LED_R, 1);
    sleep_ms(60 / bpm * 500);
    gpio_put(LED_R, 0);
}

// inicio
int main()
{
    // Configuração das interrupções
    gpio_set_irq_enabled_with_callback(BT_J, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
    gpio_set_irq_enabled_with_callback(BT_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
    gpio_set_irq_enabled_with_callback(BT_B, GPIO_IRQ_EDGE_FALL, true, &interrupcao);

    // INCIALIZACAO DO ADC, GIO, PWM, DISPLAY| CONGIURAR ADC E PWM
    stdio_init_all();
    adc_init();
    inicializacao_gpio();
    inicializacao_maquina_pio(PINO_MATRIZ_LED);
    config_display();
    adc_config();

    // LOOP
    while (true)
    {
        // muda o estado do o LED verde
        gpio_put(LED_G, led_ON);

        // Le os valores dos analogicos eixo x, eixo y e microfone e atualiza as variaveis
        adc_config();

        // Limpa o display com a cor inversa
        ssd1306_fill(&ssd, !cor);

        // mostra tela de acordo com o modo
        tela(quadro);

        // Atualiza o display
        ssd1306_send_data(&ssd);

        // Exibe os valores dos eixos e perifericos no terminal para depuração
        printf("\n\n");
        printf("X: %d\n", x);
        printf("Y: %d\n", y);
        printf("Y2: %d\n", y2);
        printf("Y INVERT: %d\n", y_invert);
        printf("EIXO X: %d\n", eixo_x_valor);
        printf("EIXO Y: %d\n", eixo_y_valor);
        printf("MIC: %d\n", mic);
        printf("LED R: %d\n", st_led_R);
        printf("LED B: %d\n", st_led_B);
        printf("LED G: %d\n", led_ON);
        printf("TIMER: %d\n", tx_atualizacao);
        printf("TELA %d\n", quadro);
        printf("STATUS: %d\n", status);
        sleep_ms(tx_atualizacao);
        limpar_tela_serial();
    }
}

// interrupções e  temporizadores
void interrupcao(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Implementação de debounce: só executa se passaram pelo menos 500ms desde o último evento
    if (current_time - last_time > 500000)
    {
        last_time = current_time; // Atualiza o tempo do último acionamento

        // Verifica se o botão BT_J foi pressionado
        if (gpio == BT_J)
        {
            // Alterna o estado da variável led_ON
            led_ON = !led_ON;
            quadro++;
            if (quadro > 4)
            {
                quadro = 1;
            }
        }

        // Verifica se o botão BT_B foi pressionado e reinicia no modo bootloader
        if (gpio == BT_B)
        {
            reset_usb_boot(0, 0);
        }

        // Verifica se o botão BT_A foi pressionado e alterna o status do PWM
        if (gpio == BT_A)
        {
            status = !status;               // Alterna entre ligado e desligado
            pwm_set_enabled(slice, status); // Ativa/desativa o PWM
        }
    }
}
void tela(int modo)
{
    if (modo == 1) // OLHOS MOVENDO
    {
        // VARIAVEIS
        tx_atualizacao = 10;
        int olho_esq_x;
        signed int olho_esq_y;

        // Calcula a posição X a partir do valor do eixo X
        olho_esq_x = (eixo_x_valor / 31.2) - 32;

        // Calcula a posição Y invertendo o eixo (porque a tela cresce para baixo)
        olho_esq_y = (60 - (eixo_y_valor / 70.2) - 8);

        if (olho_esq_x > 37) // Limita a posição máxima em X
        {
            olho_esq_x = 37;
        }
        if (olho_esq_x < 13) // Limita a posição mínima em X
        {
            olho_esq_x = 13;
        }

        if (olho_esq_y > 37) // Limita a posição máxima em Y
        {
            olho_esq_y = 37;
        }
        if (olho_esq_y < 13) // Limita a posição mínima em Y
        {
            olho_esq_y = 13;
        }
        // olho esquerdo
        ssd1306_rect(&ssd, 5, 20, 25, 5, cor, cor);
        ssd1306_rect(&ssd, 10, 15, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 10, 45, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 15, 10, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 15, 50, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 20, 5, 5, 25, cor, cor);
        ssd1306_rect(&ssd, 20, 55, 5, 25, cor, cor);
        ssd1306_rect(&ssd, 45, 10, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 45, 50, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 50, 15, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 50, 45, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 55, 20, 25, 5, cor, cor);

        // olho direito
        ssd1306_rect(&ssd, 5, 85, 25, 5, cor, cor);
        ssd1306_rect(&ssd, 10, 80, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 10, 110, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 45, 75, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 45, 115, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 20, 70, 5, 25, cor, cor);
        ssd1306_rect(&ssd, 20, 120, 5, 25, cor, cor);
        ssd1306_rect(&ssd, 15, 75, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 15, 115, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 50, 80, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 50, 110, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 55, 85, 25, 5, cor, cor);
        if (status == true)
        {
            // BOCA SORRINDO
            ssd1306_rect(&ssd, 55, 55, 5, 5, cor, cor);
            ssd1306_rect(&ssd, 55, 70, 5, 5, cor, cor);
            ssd1306_rect(&ssd, 60, 60, 10, 5, cor, cor);
        }
        else
        {
            // BOCA TRISTE
            ssd1306_rect(&ssd, 60, 55, 5, 5, cor, cor);
            ssd1306_rect(&ssd, 60, 70, 5, 5, cor, cor);
            ssd1306_rect(&ssd, 55, 60, 10, 5, cor, cor);
        }
        // IRIS MOVEL
        ssd1306_rect(&ssd, olho_esq_y, olho_esq_x, 15, 15, cor, cor);
        ssd1306_rect(&ssd, olho_esq_y, olho_esq_x + 66, 15, 15, cor, cor);
    }
    if (modo == 2) // INFORAMÇÕES DOS LEDS E BOTÕES EIXOS E MIC
    {
        led_ON = true;
        tx_atualizacao = 200;

        ssd1306_rect(&ssd, 0, 0, 127, 63, cor, !cor);

        ssd1306_draw_string(&ssd, "LEDS", 3, 3);
        ssd1306_draw_string(&ssd, "R", 63, 3);
        ssd1306_draw_string(&ssd, "G", 83, 3);
        ssd1306_draw_string(&ssd, "B", 103, 3);
        ssd1306_draw_string(&ssd, "STATUS", 3, 13);
        ssd1306_rect(&ssd, 13, 63, 8, 8, cor, st_led_R);
        ssd1306_rect(&ssd, 13, 83, 8, 8, cor, gpio_get(LED_G));
        ssd1306_rect(&ssd, 13, 103, 8, 8, cor, st_led_B);

        ssd1306_rect(&ssd, 31, 1, 96, 32, cor, !cor);
        ssd1306_draw_string(&ssd, "MIC", 3, 33);
        ssd1306_draw_string(&ssd, str_mic, 63, 33);
        ssd1306_draw_string(&ssd, "EIXO X", 3, 43);
        ssd1306_draw_string(&ssd, str_x, 63, 43);
        ssd1306_draw_string(&ssd, "EIXO Y", 3, 53);
        ssd1306_draw_string(&ssd, str_y, 63, 53);

        ssd1306_rect(&ssd, 31, 96, 31, 32, cor, !cor);
        ssd1306_draw_string(&ssd, " BT", 97, 33);
        ssd1306_draw_string(&ssd, "A", 99, 43);
        ssd1306_rect(&ssd, 43, 110, 8, 8, cor, !gpio_get(BT_A));
        ssd1306_draw_string(&ssd, "J", 98, 53);
        ssd1306_rect(&ssd, 53, 110, 8, 8, cor, !gpio_get(BT_J));
    }
    if (modo == 3)
    {
        // VARIAVEIS
        tx_atualizacao = 500;
        int coluna = 60;
        int x = cont % 67 + coluna;
        int y_invert = (mic) % 43;
        y = 63 - y_invert;
        int pA = (y_invert * 190) / 43;
        char str_pA[5];
        sprintf(str_pA, "%d", pA); // Converte o inteiro em string

        // DESENHO--------
        ssd1306_rect(&ssd, 0, 60, 127 - 60, 18, cor, !cor);       // caixa menor
        ssd1306_draw_string(&ssd, "pA", 64, 4);                   // pA
        ssd1306_draw_string(&ssd, str_pA, 100, 4);                // VARIAVEL pA
        ssd1306_rect(&ssd, 18, 60, 127 - 60, 63 - 18, cor, !cor); // caixa maior

        ssd1306_rect(&ssd, 60, coluna, cont, 2, cor, cor); // LINHA FIXA ANTERIOR
        // ESCALA CRESCENTE
        ssd1306_rect(&ssd, y + ((61 - y) / 3) * 2, x, 2, (62 - y) / 3, cor, cor);
        ssd1306_rect(&ssd, y + ((61 - y) / 3), x + 2, 2, (62 - y) / 3, cor, cor);
        ssd1306_rect(&ssd, y, x + 4, 2, (62 - y) / 3, cor, cor);

        ssd1306_rect(&ssd, y_invert, x + 4, 1, 1, cor, cor); // PONTO MOVEL

        // ESCALA DECRESCENTE
        ssd1306_rect(&ssd, y, x + 4, 2, (62 - y) / 3, cor, cor);
        ssd1306_rect(&ssd, y + ((61 - y) / 3), x + 6, 2, (62 - y) / 3, cor, cor);
        ssd1306_rect(&ssd, y + ((61 - y) / 3) * 2, x + 8, 2, (62 - y) / 3, cor, cor);

        ssd1306_rect(&ssd, 60, x + 8, 127 - x, 2, cor, cor); // LINHA FIXA POSTERIOR

        // EXIBIR VALORES PARA DEPURAÇÃO
        printf("X: %d\n", x);
        printf("Y: %d\n", y);
        printf("Y INVERT: %d\n", y_invert);
        printf("cont: %d\n", cont);

        // beep(BUZZER_A, tx_atualizacao/2);

        // CONTADOR PARA REINICIAR AO CHEGAR NO FINAL
        cont++;
        if (cont > 66)
        {
            cont = 0;
        }
    }
    if (modo == 4)
    {
        adc_config();

        // VARIAVEIS
        // obtendo dados analogicos
        tx_atualizacao = 150;
        y_invert = (eixo_x_valor * 43) / 4000;
        x = cont % 67 + coluna;
        y = 63 - y_invert;
        y2 = 63 - (eixo_y_valor * 43) / 4000;

        if (cont % 6 == 0)
        {
            pA = (eixo_x_valor * 22) / 4000;
            pA2 = (eixo_y_valor * 14) / 4000;
            if ((pA == 0) || (pA2 == 0))
            {
                bpm = 0;
                
                ;
            }
            else
            {
                bpm = (mic * 160) / 4098;
            }
            resp = pA * 3;
            temp_C = (pA2*56)/ 14;
            sprintf(str_resp, "%d", resp);
            sprintf(str_temp_C, "%d", temp_C);
            sprintf(str_pA, "%d", pA);   // Converte o inteiro em string
            sprintf(str_pA2, "%d", pA2); // Converte o inteiro em string
            sprintf(str_bpm, "%d", bpm);
            config_pwm_beep(BUZZER_A, 1, 2000);
            config_pwm_beep(BUZZER_B, 1, 2000);
        }
        if ((cont % 6 == 0) && (obito == false))
        {
            gpio_put(LED_G, 0);
            gpio_put(LED_R, 0);
            sleep_ms(100);
        }
        config_pwm_beep(BUZZER_B, 0, 2000);
        config_pwm_beep(BUZZER_A, 0, 2000);

        // BPM----------------------------------------------------------------------+
        ssd1306_rect(&ssd, 0, 0, coluna, 21, cor, !cor);           // caixa menor   |
        ssd1306_draw_string(&ssd, "BPM", 2, 2);                    // BPM           |
        ssd1306_draw_string(&ssd, str_bpm, 37, 2);                 //
        ssd1306_line(&ssd, 0, 18, cont, 18, cor);
        ssd1306_line(&ssd, 2 + cont, 18, 10 + cont,18-((bpm * 10)/160), cor);          //|
        ssd1306_line(&ssd, 10 + cont, 18-((bpm * 10)/160), 18 + cont,18, cor); 
        ssd1306_line(&ssd, 18+cont, 18, coluna, 18, cor);         //|
        //--------------------------------------------------------------------------+
        // BPM----------------------------------------------------------------------+
        ssd1306_rect(&ssd, 21, 0, coluna, 21, cor, !cor);           // caixa menor  |
        ssd1306_draw_string(&ssd, "TEM", 2, 23);                    // BPM          |
        ssd1306_draw_string(&ssd, str_temp_C, 30, 23);
        ssd1306_line(&ssd, 0, 39, cont, 39, cor);
        ssd1306_line(&ssd, cont, 39, 14 + cont,39-((temp_C * 10)/46), cor);          //|
        ssd1306_line(&ssd, 14 + cont, 39-((temp_C * 10)/46), 28 + cont,39, cor); 
        ssd1306_line(&ssd, 28+cont, 39, coluna, 39, cor);                   //               |
        //--------------------------------------------------------------------------+
        // RESP---------------------------------------------------------------------+
        ssd1306_rect(&ssd, 42, 0, coluna,21, cor, !cor);           // caixa menor   |
        ssd1306_draw_string(&ssd, "RES", 2, 44);                    // REPSIRAÇÃO   |
        ssd1306_draw_string(&ssd, str_resp, 37, 44);   
        ssd1306_line(&ssd, 0, 60, cont, 60, cor);
        ssd1306_line(&ssd, cont, 60, 12 + cont,60-((resp * 10)/66), cor);          //|
        ssd1306_line(&ssd, 12 + cont, 60-((resp * 10)/66), 24 + cont,60, cor); 
        ssd1306_line(&ssd, 24 + cont, 60, coluna, 60, cor);
        //--------------------------------------------------------------------------+
       
        // pA-----------------------------------------------------------------------+
        ssd1306_rect(&ssd, 0, coluna, WIDTH - coluna, 18, cor, !cor);// caixa menor |
        ssd1306_draw_string(&ssd, "PA", coluna + 3, 4);              // pA          |
        ssd1306_draw_string(&ssd, str_pA, coluna + 30, 4);           //             |
        ssd1306_line(&ssd, coluna + 46, 14, coluna + 50, 4, cor);    //             |
        ssd1306_draw_string(&ssd, str_pA2, 109, 4); // VARIAVEL pA                  |
        //--------------------------------------------------------------------------+
        //CAIXA MAIOR pA-----------------------------------------------------------------------+
        // LINHA FIXA ANTERIOR                                                      
        ssd1306_rect(&ssd, 18, coluna, WIDTH - coluna, 63 - 18, cor, !cor);                 
        ssd1306_rect(&ssd, linha, coluna, cont, 1, cor, cor);                       
        //  SE OBITO ---------------------------------------------------------------------------                                                        
        if ((pA == 0) || (pA2 == 0) || (bpm == 0))                                 
        {

            // MEDIÇÃO 01
            ssd1306_line(&ssd, coluna + cont, linha, coluna + cont + 5, linha, cor);
            ssd1306_line(&ssd, coluna + cont + 5, linha, coluna + cont + 10, linha, cor);
            ssd1306_line(&ssd, coluna + cont + 10, linha, coluna + cont + 15, linha, cor);

            // MEDIÇAÕ 02
            ssd1306_line(&ssd, coluna + cont + 27, linha, coluna + cont + 32, linha, cor);
            ssd1306_line(&ssd, coluna + cont + 32, linha, coluna + cont + 37, linha, cor);
            ssd1306_line(&ssd, coluna + cont + 37, linha, coluna + cont + 42, linha, cor);
            gpio_put(LED_R, 1);
            gpio_put(LED_G, 0);
            config_pwm_beep(BUZZER_A, 1, 2000);
            config_pwm_beep(BUZZER_B, 1, 2000);
            obito = true;

        } //-------------------------------------------------------------------------------------------------
        else // NAO OBTIO------------------------------------------------------------------------------------
        {

            obito = false;
            gpio_put(LED_R, 0);
            gpio_put(LED_G, 1);

            // MEDIÇÃO 01
            ssd1306_line(&ssd, coluna + cont, linha, coluna + cont + 5, y, cor);
            ssd1306_line(&ssd, coluna + cont + 5, y, coluna + cont + 10, (linha - y) + linha, cor);
            ssd1306_line(&ssd, coluna + cont + 10, (linha - y) + linha, coluna + cont + 15, linha, cor);
            // MEDIÇAÕ 02
            ssd1306_line(&ssd, coluna + cont + 27, linha, coluna + cont + 32, y2, cor);
            ssd1306_line(&ssd, coluna + cont + 32, y2, coluna + cont + 37, (linha - y2) + linha, cor);
            ssd1306_line(&ssd, coluna + cont + 37, (linha - y2) + linha, coluna + cont + 42, linha, cor);
        }
        // LINHA FIXA MEIO
        ssd1306_rect(&ssd, linha, coluna + cont + 15, 12, 1, cor, cor);

        // LINHA FINAL
        ssd1306_rect(&ssd, linha, coluna + cont + 42, 127 - (coluna + cont + 42), 1, cor, cor);
        //----------------------------------------------------------------------------------------------------
        // EXIBIR VALORES PARA DEPURAÇÃO----------------------------------------------------------------------
        printf("cont: %d\n", cont);
        
        printf("pA: %d\n", pA);
        printf("pA2: %d\n", pA2);
        printf("Bpm: %d\n", bpm);
        printf("Resp: %d\n", resp);
        printf("C° : %d\n", temp_C);     

        // CONTADOR PARA REINICIAR AO CHEGAR NO FINAL----------------------------------------------------------
        cont = cont + 1;
        if (cont > 29)
        {
            cont = 0;
        }
    }
}
