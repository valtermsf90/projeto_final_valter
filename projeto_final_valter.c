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
void desenhar(char desenho[5][5], int potencia);// VARIAVEIS

bool cor = true;
bool st_bz_A = 0;
bool st_bz_B = 0;
bool st_led_R = false;
bool st_led_B = false;
bool st_led_G = false;
bool led_ON = false;
bool status = false;
bool status2 = true;
int quadro = 3; //tela de inicio
int tx_atualizacao = 1000;
static volatile uint32_t last_time = 0;
// VARIAVEIS QUADRO 3
int temp = 46;
int umidadeSolo=75;
int radiacao = 0;   
int nv_tanque = 20;
bool abastecimento = false;
bool irrigacao = false;
bool sys_auto = false;
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
        printf("VALORES PARA DEPURAÇÃO\n");
        printf("SINAIS ANALOGICOS\n");
        printf("EIXO X: %d\n", eixo_x_valor);
        printf("EIXO Y: %d\n", eixo_y_valor);
        printf("MIC: %d\n", mic);
        printf("\nSTATUS PERIFERICOS\n");
        printf("LED R: %d\n", st_led_R);
        printf("LED B: %d\n", st_led_B);
        printf("LED G: %d\n", st_led_G);
        printf("BUZZER A: %d\n", st_bz_A);
        printf("BUZZER B: %d\n", st_bz_B);
        printf("\nINFORMÇÕES DA EXCUÇÃO\n");
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
            
            quadro++;
            if (quadro > 4)
            {
                quadro = 1;
            }
        }

        // Verifica se o botão BT_B foi pressionado e reinicia no modo bootloader
        if (gpio == BT_B)
        {
            
            if(quadro == 1){
                status2 = !status2;  
                
            }
            if(quadro == 2){
                status2 = !status2;  
                
            }
            if(quadro == 3){
            status2 = !status2;               // Alterna entre ligado e desligado
             
            }// Ativa/desativa o PWM
            if(quadro == 4){
            }
                         
            
        }

        // Verifica se o botão BT_A foi pressionado e alterna o status do PWM
        if (gpio == BT_A)
        {
            if(quadro == 1){
                status = !status;  
                
            }
            if(quadro == 2){
                status = !status;  
                
            }
            if(quadro == 3){
            status = !status;               // Alterna entre ligado e desligado
            
            }// Ativa/desativa o PWM
            if(quadro == 4){
                          // Alterna entre ligado e desligado
            pwm_set_enabled(LED_R, false); 
            }// Ativa/desativa o PWM
        }
    }
}
void tela(int modo)
{
    if (modo == 1) // OLHOS MOVENDO
    {
        gpio_put(LED_G, 0);st_led_G= 0;
                gpio_put(LED_B, 0);st_led_B= 0;
                gpio_put(LED_R, 0);st_led_R= 0;
        limpar_o_buffer();
        desenhar(matriz_1,64);
        escrever_no_buffer();
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
            config_pwm(LED_B, status);
            config_pwm(LED_R, status);

                // Controla o brilho do LED vermelho com base no eixo X
            if ((eixo_x_valor < 1500) || (eixo_x_valor > 2200))
            {
                pwm_set_gpio_level(LED_R, eixo_x_valor);st_led_R= 1;
            }
            else
            {
                pwm_set_gpio_level(LED_R, 0);st_led_R= 0;
            }

            // Controla o brilho do LED azul com base no eixo Y
            if ((eixo_y_valor < 1500) || (eixo_y_valor > 2200))
            {
                pwm_set_gpio_level(LED_B, eixo_y_valor);st_led_B= 1;
            }
            else
            {
                pwm_set_gpio_level(LED_B, 0);st_led_B= 0;
            }


            }
            else
            {
                // BOCA TRISTE
                ssd1306_rect(&ssd, 60, 55, 5, 5, cor, cor);
                ssd1306_rect(&ssd, 60, 70, 5, 5, cor, cor);
                ssd1306_rect(&ssd, 55, 60, 10, 5, cor, cor);
            }
        // IRIS MOVEL
        ssd1306_rect(&ssd, olho_esq_y, olho_esq_x, 15, 15, cor, status);
        ssd1306_rect(&ssd, olho_esq_y, olho_esq_x + 66, 15, 15, cor, status);

         // EXIBIR VALORES PARA DEPURAÇÃO----------------------------------------------------------------------
         printf("VARIAVEIS DA TELA\n");
         printf("SORRISO: %d\n", status); 
         printf("PWM_LED R e B: %d\n", status); 
    }
    
    if (modo == 2) // INFORAMÇÕES DOS LEDS E BOTÕES EIXOS E MIC
    {
        limpar_o_buffer();
        desenhar(matriz_2,64);
        escrever_no_buffer();
        gpio_init(LED_R);
            config_pwm(LED_B, status);
            config_pwm(LED_R, status);
            config_pwm_beep(BUZZER_A,status, 5000);
            config_pwm_beep(BUZZER_B,status, 2000);
            gpio_put(LED_G, status); st_led_G = status;

                // Controla o brilho do LED vermelho com base no eixo X
            if ((eixo_x_valor < 1500) || (eixo_x_valor > 2200))
            {
                pwm_set_gpio_level(LED_R, eixo_x_valor);st_led_R= status;
                pwm_set_gpio_level(BUZZER_A, eixo_x_valor);st_bz_A = status;
            }
            else
            {
                pwm_set_gpio_level(LED_R, 0);st_led_R= 0;
                pwm_set_gpio_level(BUZZER_A, 0);st_bz_A = 0;
            }
            
            // Controla o brilho do LED azul com base no eixo Y
            if ((eixo_y_valor < 1500) || (eixo_y_valor > 2200))
            {
                pwm_set_gpio_level(LED_B, eixo_y_valor);st_led_B= status;
                pwm_set_gpio_level(BUZZER_B, eixo_y_valor);st_bz_B = status;
            }
            else
            {
                pwm_set_gpio_level(LED_B, 0);st_led_B= 0;
                pwm_set_gpio_level(BUZZER_B, 0);st_bz_B = 0;
            }

        tx_atualizacao = 200;

        ssd1306_rect(&ssd, 0, 0, 127, 63, cor, !cor);

        ssd1306_draw_string(&ssd, "LEDS", 3, 3);
        ssd1306_draw_string(&ssd, "R", 63, 3);
        ssd1306_draw_string(&ssd, "G", 83, 3);
        ssd1306_draw_string(&ssd, "B", 103, 3);
        ssd1306_draw_string(&ssd, "STATUS", 3, 13);
        ssd1306_draw_string(&ssd, "BUZZERS", 3, 23);
        ssd1306_rect(&ssd, 13, 63, 8, 8, cor, st_led_R);
        ssd1306_rect(&ssd, 13, 83, 8, 8, cor, gpio_get(LED_G));
        ssd1306_rect(&ssd, 13, 103, 8, 8, cor, st_led_B);
        ssd1306_draw_string(&ssd, "A", 63, 23);
        ssd1306_rect(&ssd, 23, 78, 8, 8, cor, st_bz_A);
        ssd1306_draw_string(&ssd, "A", 93, 23);
        ssd1306_rect(&ssd, 23, 108, 8, 8, cor, st_bz_B);

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
        limpar_o_buffer();
        desenhar(matriz_3, 64);
        escrever_no_buffer();
        // VARIAVEIS
        const int TEMP_MAX = 40;
        const int UMID_MIN = 50;
        const int UV = 70;

        tx_atualizacao = 5;
        sys_auto = status;
        radiacao = temp * 2;
        char str_nv_tanque[5];
        char str_umidadeSolo[5];
        char str_radiacao[5];
        char str_temp[5];
        sprintf(str_nv_tanque, "%d", nv_tanque); // Converte o inteiro em string
        sprintf(str_temp, "%d", temp);
        sprintf(str_radiacao, "%d", radiacao);
        sprintf(str_umidadeSolo, "%d", umidadeSolo);

        // LAYOUT TELA 3 ------------------------------------------
        ssd1306_draw_string(&ssd, "UMIDADE", 2, 2);
        ssd1306_draw_string(&ssd, str_umidadeSolo, 70, 2);
        ssd1306_draw_string(&ssd, "TEMP C", 2, 12);
        ssd1306_draw_string(&ssd, str_temp, 70, 12);
        ssd1306_draw_string(&ssd, "RAIO UV", 2, 22);
        ssd1306_draw_string(&ssd, str_radiacao, 70, 22);
        ssd1306_vline(&ssd, 87, 0, HEIGHT - 33, cor);

        ssd1306_draw_string(&ssd, "PWR", 89, 2);
        ssd1306_draw_string(&ssd, "IRR", 89, 12);
        ssd1306_draw_string(&ssd, "ABS", 89, 22);
        ssd1306_rect(&ssd, 2, WIDTH - 10, 8, 8, cor, status2);
        ssd1306_rect(&ssd, 12, WIDTH - 10, 8, 8, cor, irrigacao);
        ssd1306_rect(&ssd, 22, WIDTH - 10, 8, 8, cor, abastecimento);

        ssd1306_hline(&ssd, 0, WIDTH, HEIGHT - 33, cor);
        ssd1306_draw_string(&ssd, "System Auto", 2, HEIGHT - 31);
        ssd1306_rect(&ssd, HEIGHT - 31, WIDTH - 10, 8, 8, cor, sys_auto); // alterar botao A
        ssd1306_hline(&ssd, 0, WIDTH, HEIGHT - 22, cor);
        ssd1306_draw_string(&ssd, "NIVEL TANQUE", 12, HEIGHT - 20);
        ssd1306_draw_char(&ssd, '0', 1, HEIGHT - 10);
        ssd1306_rect(&ssd, HEIGHT - 10, 9, nv_tanque, 8, cor, cor);
        ssd1306_draw_string(&ssd, str_nv_tanque, 12 + nv_tanque, HEIGHT - 10);
        ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, cor, !cor);
        // FIM LAYOUT-----------------------------------------------------------
        if(status2 == false){
            irrigacao = false;
        }else {
        // SIMULAR TEMPERATURA JOYSTICK----------------------------
        if (sys_auto == false)
        {
            if (eixo_x_valor < 1000)
            {
                umidadeSolo--;
            }
            if (eixo_x_valor > 3000)
            {
                umidadeSolo++;
                temp = temp - (umidadeSolo % 2);
            }

            // Controla o brilho do LED azul com base no eixo Y
            if (eixo_y_valor < 1000)
            {
                temp--;
            }
            if (eixo_y_valor > 2200)
            {
                temp++;
                umidadeSolo--;
            }
        }
        // FIM SIMULADOR-------------------------------------------
    
        
        if ((temp > 47) || (umidadeSolo < 60))
        {
            irrigacao = true;
            abastecimento = false;
            sys_auto = false;
           
        }
        if(irrigacao == true){
            nv_tanque--;
            temp = temp - (nv_tanque % 3);
            if(temp < 6){
                temp = 6;
            }
            umidadeSolo = umidadeSolo + (temp % 3);
            if(umidadeSolo > 95){
                umidadeSolo = 95;
            }
             abastecimento= false;
            if (nv_tanque < 99)
            {
                if ((nv_tanque < 5) && (irrigacao == true))
                {
                    irrigacao = false;
                    sys_auto= true;
                    abastecimento= true;
                }
            }

        }   
        if(abastecimento == true){
            nv_tanque++;
           
            sys_auto=true;
            if(nv_tanque==99){
                abastecimento = false;
            }
            
        }     
        if ((temp == 18) || (umidadeSolo == 90))
        {
            irrigacao = false;
            sys_auto = true;
        }
    }
        if ((sys_auto == true) && (irrigacao == false))
        {
            if ((cont % 50 == 0) && (irrigacao == false))
            {
                temp++;
                umidadeSolo = umidadeSolo - (temp % 3);
                radiacao = radiacao + (temp % 2);
            }
        }
        cont++;
        if (cont == 99)
        {
            cont = 0;
        }

        printf("VARIAVEIS DA TELA\n");
        printf("cont: %d\n", cont);
        printf("nivel1: %d\n", nv_tanque);
        printf("umidade: %d\n", umidadeSolo);
        printf("temperatura: %d\n", temp);
        printf("Raios UV: %d\n", radiacao);
        printf("Irrigação: %d\n", irrigacao);
        printf("abastecimento: %d\n", abastecimento);
        printf("sysAuto: %d\n", sys_auto);
        printf("status2: %d\n", status2);
        printf("status: %d\n", status);
    }
    if (modo == 4)
    {
        limpar_o_buffer();
        desenhar(matriz_4,64);
        escrever_no_buffer();
        adc_config();
       
       
        // VARIAVEIS
        // obtendo dados analogicos
        tx_atualizacao = 120;
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
            }
            else
            {
                bpm = (mic * 160) / 4098;
            }
            resp = pA * 3;
            temp_C = (pA2*84)/ 14;
            sprintf(str_resp, "%d", resp);
            sprintf(str_temp_C, "%d", temp_C);
            sprintf(str_pA, "%d", pA);   // Converte o inteiro em string
            sprintf(str_pA2, "%d", pA2); // Converte o inteiro em string
            sprintf(str_bpm, "%d", bpm);
            config_pwm_beep(BUZZER_B, 1, 2000);st_bz_B = 1;
            config_pwm_beep(BUZZER_A, 1, 2000);st_bz_A = 1;
        }
        if ((cont % 6 == 0) && (obito == false))
        {
            gpio_put(LED_G, 0);st_led_G = 0;
            gpio_put(LED_R, 0);st_led_R = 0;
            sleep_ms(100);
        }
        config_pwm_beep(BUZZER_B, 0, 2000);st_bz_B = 0;
        config_pwm_beep(BUZZER_A, 0, 2000);st_bz_A = 0;

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
        ssd1306_draw_string(&ssd, "TEMP", 2, 23);                    // BPM          |
        ssd1306_draw_string(&ssd, str_temp_C, 37, 23);
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
            gpio_put(LED_R, 1);st_led_R = 1;
            gpio_put(LED_G, 0);st_led_G = 0;
            config_pwm_beep(BUZZER_A, 1, 2000);st_bz_A = 1;
            config_pwm_beep(BUZZER_B, 1, 2000);st_bz_B = 1;
            obito = true;

        } //-------------------------------------------------------------------------------------------------
        else // NAO OBTIO------------------------------------------------------------------------------------
        {

            obito = false;
            gpio_put(LED_R, 0);st_led_R= 0;
            gpio_put(LED_G, 1);st_led_G = 1;

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
        printf("VARIAVEIS DA TELA\n");
        printf("cont: %d\n", cont);        
        printf("pA: %d\n", pA);
        printf("pA2: %d\n", pA2);
        printf("Bpm: %d\n", bpm);
        printf("Resp: %d\n", resp);
        printf("C°: %d\n", temp_C);
        printf("\nX: %d\n", x);
        printf("Y: %d\n", y);
        printf("Y INVERT: %d\n", y_invert);     

        // CONTADOR PARA REINICIAR AO CHEGAR NO FINAL----------------------------------------------------------
        cont = cont + 1;
        if (cont > 29)
        {
            cont = 0;
        }
    }
}
