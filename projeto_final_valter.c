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
void config_display(); // Configura o display
static void interrupcao(uint gpio, uint32_t events);
void tela(int modo);
void desenhar(char desenho[5][5], int potencia);
void sysIrricacao();
void config_sysIrr();
void config_valvulas();
void monitor();

// VARIAVEL GLOBAL
int cont = 0;     // CONTADOR
bool cor = true;  // COR DO RETANGULO
bool st_bz_A = 0; // status buzzer A
bool st_bz_B = 0; // status buzzer B

bool led_ON = false; // status led
bool status = false;
bool status2 = true;
int seletor = 0;          // SELETOR de opções
int quadro = 1;           // tela de inicio
int tx_atualizacao = 100; // tempo de atualização

static volatile uint32_t last_time = 0; // tempo do ultimo evento debounce

// VARIAVEIS QUADRO 01
bool B1 = false;
bool A1 = true;
int temp = 18;
int umidadeSoloMax = 80;
int umidadeSoloMin = 30;
int umidadeSolo = 32;
int radiacao = 0;
int nv_tanque = 48;
int nv_tanqueMin = 10;
bool abastecimento = false;
bool irrigacao = false;
bool sys_auto = false;
bool power_sys = false;

// VARIAVEIS QUADRO 02
bool B2 = false;
bool A2 = true;
bool obito = false;
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

// VARIAVEIS QUADRO 03
bool A3 = true;
bool B3 = false;
int setor;
bool v1 = true;
bool v2 = true;
bool v3 = true;
int n_valvulas = 3;

// VARIAVEIS QUADRO 04
bool A4 = true;
bool B4 = false;

// VARIAVEIS QUADRO 02
bool A11 = true;
bool B11 = false;
char str_umidadeMax[5];
char str_umidadeMin[5];
char str_nv_tanqueMin[5];
int selecao = 0;

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

        printf("VARIAVEIS DA SISTEAMA\n");
        printf("cont: %d\n", cont);
        printf("valvulas abertas: %d\n", n_valvulas);
        printf("NvMinimo: %d\n", nv_tanqueMin);
        printf("nivel1: %d\n", nv_tanque);
        printf("Umidade MAX: %d\n", umidadeSoloMax);
        printf("Umidade MiN: %d\n", umidadeSoloMin);
        printf("umidade: %d\n", umidadeSolo);
        printf("temperatura: %d\n", temp);
        printf("Raios UV: %d\n", radiacao);
        printf("Irrigação: %d\n", irrigacao);
        printf("abastecimento: %d\n", abastecimento);
        printf("sysAuto: %d\n", sys_auto);
        printf("PowerSys: %d\n", power_sys);
        printf("status: %d\n", status);
        printf("\n\n");
        // Exibe os valores dos eixos e perifericos no terminal para depuração

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
        printf("BOTÃO A: %d\n", gpio_get(BT_A));
        printf("BOTÃO B: %d\n", gpio_get(BT_B));
        printf("BOTAO J: %d\n", gpio_get(BT_J));
        printf("\nINFORMÇÕES DA EXCUÇÃO\n");
        printf("TIMER: %d\n", tx_atualizacao);
        printf("TELA %d\n", quadro);
        printf("STATUS: %d\n", status);
        printf("status_2: %d\n", status2);
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
            cont = 0;
            quadro++;

            if (quadro == 4)
            {
                quadro = 1;
                gpio_init(LED_G);
                gpio_set_dir(LED_G, GPIO_OUT);
                gpio_init(LED_R);
                gpio_set_dir(LED_R, GPIO_OUT);
                gpio_init(LED_B);
                gpio_set_dir(LED_B, GPIO_OUT);
            }
            if (quadro == 6)
            {
                quadro = 1;
                gpio_init(LED_G);
                gpio_set_dir(LED_G, GPIO_OUT);
                gpio_init(LED_R);
                gpio_set_dir(LED_R, GPIO_OUT);
                gpio_init(LED_B);
                gpio_set_dir(LED_B, GPIO_OUT);
            }
        }

        // Verifica se o botão BT_B foi pressionado e reinicia no modo bootloader
        if (gpio == BT_B)
        {

            if (quadro == 1)
            {
                B1 = !B1;
            }
            if (quadro == 2)
            {
                B2 = !B2;
                
            }
            if (quadro == 3)
            {
                B3 = !B3; // Alterna entre ligado e desligado
                quadro = 5;
                reset_usb_boot(0, 0); // BOOTSEL
            }
            if (quadro == 5)
            {
                B4 = !B4; // Alterna entre ligado e desligado
                
            }
        }

        // Verifica se o botão BT_A foi pressionado e alterna o status do PWM
        if (gpio == BT_A)
        {
            if (quadro == 1)
            {
                A1 = !A1;
            }
            if (quadro == 2)
            {
                A2 = !A2;
            }
            if (quadro == 3)
            {
                A3 = !A3; // Alterna entre ligado e desligado
            }
            if (quadro == 4)
            {
                A4 = !A4; // Alterna entre ligado e desligado
            }          
        }
    }
}
void tela(int modo)
{
    if (modo == 1)
    {

        sysIrricacao();
    }

    if (modo == 2)
    {
        // ECG();
        config_sysIrr();
    }
    if (modo == 3) // OLHOS MOVENDO  ok
    {
        config_valvulas();
    }
    if (modo == 5) // INFORAMÇÕES DOS LEDS E BOTÕES( EIXOS E MIC E BUTTON)
    {
        monitor();
    }
    
}

void sysIrricacao()
{
    // LIMPA O BUFFER
    limpar_o_buffer();
    desenhar(matriz_1, 64);
    escrever_no_buffer();
    // VARIAVEIS
    
    power_sys = A1;
    tx_atualizacao = 10;

    radiacao = temp * 2;
    char str_nv_tanque[5];
    char str_umidadeSolo[5];
    char str_radiacao[5];
    char str_temp[5];
    // Converte o inteiro em string
    sprintf(str_nv_tanque, "%d", nv_tanque);
    sprintf(str_temp, "%d", temp);
    sprintf(str_radiacao, "%d", radiacao);
    sprintf(str_umidadeSolo, "%d", umidadeSolo);

    //  LAYOUT-----------------------------------------------------------
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
    ssd1306_rect(&ssd, 2, WIDTH - 10, 8, 8, cor, power_sys);
    ssd1306_rect(&ssd, 12, WIDTH - 10, 8, 8, cor, irrigacao);
    ssd1306_rect(&ssd, 22, WIDTH - 10, 8, 8, cor, abastecimento);

    ssd1306_hline(&ssd, 0, WIDTH, HEIGHT - 33, cor);
    ssd1306_draw_string(&ssd, "VALV", 2, HEIGHT - 31);
    ssd1306_rect(&ssd, HEIGHT - 31, 60, 8, 8, cor, v3);
    ssd1306_rect(&ssd, HEIGHT - 31, 40, 8, 8, cor, v1);
    ssd1306_rect(&ssd, HEIGHT - 31, 50, 8, 8, cor, v2);
    ssd1306_draw_string(&ssd, "Auto", 80, HEIGHT - 31);
    ssd1306_rect(&ssd, HEIGHT - 31, WIDTH - 10, 8, 8, cor, sys_auto);
    ssd1306_hline(&ssd, 0, WIDTH, HEIGHT - 22, cor);
    ssd1306_draw_string(&ssd, "NIVEL TANQUE", 12, HEIGHT - 20);
    ssd1306_draw_char(&ssd, '0', 1, HEIGHT - 10);
    ssd1306_rect(&ssd, HEIGHT - 10, 9, nv_tanque, 8, cor, cor);
    ssd1306_draw_string(&ssd, str_nv_tanque, 12 + nv_tanque, HEIGHT - 10);
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, cor, !cor);
    // FIM LAYOUT-----------------------------------------------------------
    if (power_sys == false) // se o sistema estiver desligado
    {

        apagado(0);
        sys_auto = power_sys;
        irrigacao = false;
        abastecimento = false;
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
            // FIM SIMULADOR-------------------------------------------
        }
    }
    else // se o sistema estiver ligado
    {
        amarelo(0);
        piscar(cont, 7);
        sys_auto = B1;
        if ((irrigacao == false)) // se a irrigação estiver desligada
        {
            if ((cont % 50 == 0) && (irrigacao == false))
            {
                temp++;
                radiacao = radiacao + (temp % 2);
            }
            if ((cont % 30 == 0) && (irrigacao == false))
            {
                umidadeSolo = umidadeSolo - (temp / 10);
            }
        }
        if ((sys_auto)) // se o sistema estiver em modo automatico
        {
            if (n_valvulas < 1)
            {
                vermelho(0);
                piscar(cont, 5);
    }
            else // se o sistema estiver em modo manual
            {
                verde(0);
                piscar(cont, 7);

                if ((umidadeSolo < umidadeSoloMin) && (abastecimento == false))

                {
                    irrigacao = true;
                    abastecimento = false;
                }
                if (irrigacao)
                {
                    ciano(0);
                    piscar(cont, 7);
                    if (nv_tanque % 4 == 0)
                    {
                        temp--;
                    }
                    if (temp < 12)
                    {
                        temp = 12;
                    }
                    umidadeSolo++;
                    if (umidadeSolo >= umidadeSoloMax)
                    {
                        irrigacao = false;
                    }
                    nv_tanque -= n_valvulas;
                    if (nv_tanque < 0)
                    {
                        nv_tanque = 0;
                    }
                    if (nv_tanque <= nv_tanqueMin)
                    { // se o tanque estiver vazio
                        irrigacao = false;
                        abastecimento = true;
                    }
                }
                if (abastecimento)
                { // se o tanque estiver vazio
                    azul(0);
                    piscar(cont, 7);
                    nv_tanque++;
                    if (nv_tanque >= 99)
                    {
                        nv_tanque = 99;
                        abastecimento = false;
                    }
                }
            }
        }

        cont++;
        if (cont == 99)
        {
            cont = 0;
        }
        printf("VARIAVEIS DA TELA\n");
        printf("cont: %d\n", cont);
        printf("valvulas abertas: %d\n", n_valvulas);
        printf("NvMinimo: %d\n", nv_tanqueMin);
        printf("nivel1: %d\n", nv_tanque);
        printf("Umidade MAX: %d\n", umidadeSoloMax);
        printf("Umidade MiN: %d\n", umidadeSoloMin);
        printf("umidade: %d\n", umidadeSolo);
        printf("temperatura: %d\n", temp);
        printf("Raios UV: %d\n", radiacao);
        printf("Irrigação: %d\n", irrigacao);
        printf("abastecimento: %d\n", abastecimento);
        printf("sysAuto: %d\n", sys_auto);
        printf("PowerSys: %d\n", power_sys);
        printf("status: %d\n", status);
    }
}

void config_sysIrr()
{
    if (quadro == 2)
    {
        // Limpa o display com a cor inversa
        power_sys = false;
        rosa(0);
        seletor = 18;

        // conovertendo os valores inteiros em string
        sprintf(str_umidadeMax, "%d", umidadeSoloMax);
        sprintf(str_umidadeMin, "%d", umidadeSoloMin);
        sprintf(str_nv_tanqueMin, "%d", nv_tanqueMin);

        // seleciona a posição do seletor
        ssd1306_draw_bitmap(&ssd, seletor + (selecao * 41), 47, baixo);
        ssd1306_draw_bitmap(&ssd, seletor + (selecao * 41), 28, cima);

        ssd1306_rect(&ssd, 0, 0, 127, 63, cor, !cor);
        ssd1306_rect(&ssd, 10, 0, 127, 11, cor, !cor);
        ssd1306_vline(&ssd, 41, 10, 63, true);
        ssd1306_vline(&ssd, 82, 0, 63, true);

        ssd1306_draw_string(&ssd, "UMIDADE", 2, 2);
        ssd1306_draw_string(&ssd, "AGUA", 92, 2);
        ssd1306_draw_string(&ssd, "MIN", 6, 12);
        ssd1306_draw_string(&ssd, "MAX", 50, 12);
        ssd1306_draw_string(&ssd, "MIN", 94, 12);

        ssd1306_draw_string(&ssd, str_umidadeMin, 14, 38);
        ssd1306_draw_string(&ssd, str_umidadeMax, 55, 38);
        ssd1306_draw_string(&ssd, str_nv_tanqueMin, 96, 38);
        // finaliza a tela

        // verifica analogico eixo X e altera os valores
        if (eixo_x_valor < 1000)
        {
            selecao--;
            if (selecao < 0)
            {
                selecao = 0;
            }
        }
        if (eixo_x_valor > 3000)
        {
            selecao++;
            if (selecao > 2)
            {
                selecao = 2;
            }
        }

        // verifica analogico eixo Y e altera os valores
        if (eixo_y_valor < 1000)
        {
            if (selecao == 0)
            {
                umidadeSoloMin--;
                if (umidadeSoloMin < 1)
                {
                    umidadeSoloMin = 0;
                }
            }

            if (selecao == 1)
            {
                umidadeSoloMax--;
                if (umidadeSoloMax == umidadeSoloMin)
                {
                    umidadeSoloMax = umidadeSoloMin + 1;
                }
            }

            if (selecao == 2)
            {
                nv_tanqueMin--;
                if (nv_tanqueMin < 1)
                {
                    nv_tanqueMin = 1;
                }
            }
        }

        if (eixo_y_valor > 3000)
        {

            if (selecao == 0)
            {
                umidadeSoloMin++;
                if (umidadeSoloMin == umidadeSoloMax)
                {
                    umidadeSoloMin = umidadeSoloMax - 1;
                }
            }
            if (selecao == 1)
            {
                umidadeSoloMax++;
                if (umidadeSoloMax > 99)
                {
                    umidadeSoloMax = 99;
                }
            }
            if (selecao == 2)
            {
                nv_tanqueMin++;
                if (nv_tanqueMin > 99)
                {
                    nv_tanqueMin = 99;
                }
            }
        }
        // imprime as variaveis para depuração
        printf("selecao: %d\n", selecao);
        printf("cont: %d\n", cont);
        printf("NvMinimo: %d\n", nv_tanqueMin);
        printf("nivel1: %d\n", nv_tanque);
        printf("Umidade MAX: %d\n", umidadeSoloMax);
        printf("Umidade MiN: %d\n", umidadeSoloMin);
        printf("umidade: %d\n", umidadeSolo);
        printf("temperatura: %d\n", temp);
        printf("Raios UV: %d\n", radiacao);
        printf("Irrigação: %d\n", irrigacao);
        printf("abastecimento: %d\n", abastecimento);
        printf("sysAuto: %d\n", sys_auto);
        printf("PowerSys: %d\n", power_sys);
        printf("status: %d\n", status);
    }
}
// TELA 03 - VALVULAS SETORES
void config_valvulas()
{
    // LIMPA O BUFFER
    branco(0);
    power_sys = false;
    n_valvulas = (v1 + v2 + v3); // numero de valvulas abertas

    // LAYOUT-----------------------------------------------------------
    if (quadro == 3)
    {
        tx_atualizacao = 100;
        limpar_o_buffer();
        desenhar(matriz_3, 24);
        escrever_no_buffer();

        seletor = 18;

        ssd1306_draw_bitmap(&ssd, seletor + (setor * 41), 21, baixo);
        ssd1306_draw_bitmap(&ssd, seletor + (setor * 41), 55, cima);

        ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, cor, !cor);
        ssd1306_rect(&ssd, 10, 0, 127, 11, cor, !cor);
        ssd1306_vline(&ssd, 41, 10, 63, true);
        ssd1306_vline(&ssd, 82, 0, 63, true);

        ssd1306_draw_string(&ssd, "VALVULAS", 5, 2);
        ssd1306_draw_string(&ssd, "ON", 84, 2);
        ssd1306_draw_string(&ssd, "OFF", 103, 2);
        ssd1306_draw_string(&ssd, "1", 19, 12);
        ssd1306_draw_string(&ssd, "2", 63, 12);
        ssd1306_draw_string(&ssd, "3", 104, 12);

        ssd1306_rect(&ssd, 32, 12, 20, 20, cor, v1);
        ssd1306_rect(&ssd, 32, 53, 20, 20, cor, v2);
        ssd1306_rect(&ssd, 32, 94, 20, 20, cor, v3);
        //-------------------------------------------------------------------
        // verifica analogico eixo X e altera os valores
        if (eixo_x_valor < 1000)
        {
            setor--;
            if (setor < 0)
            {
                setor = 0;
            }
        }
        if (eixo_x_valor > 3000)
        {
            setor++;
            if (setor > 2)
            {
                setor = 2;
            }
        }
        // verifica o botao BT_A e altera o status das valvulas
        if (gpio_get(BT_A) == 0)
        {
            if (setor == 0)
            {
                v1 = !v1;
            }
            if (setor == 1)
            {
                v2 = !v2;
            }
            if (setor == 2)
            {
                v3 = !v3;
            }

            n_valvulas = (v1 + v2 + v3);
        }
        // imprime as variaveis para depuração
        printf("VARIAVEIS DA TELA\n");
        printf("cont: %d\n", cont);
        printf("valvulas abertas: %d\n", n_valvulas);
        printf("NvMinimo: %d\n", nv_tanqueMin);
        printf("nivel1: %d\n", nv_tanque);
        printf("Umidade MAX: %d\n", umidadeSoloMax);
        printf("Umidade MiN: %d\n", umidadeSoloMin);
        printf("umidade: %d\n", umidadeSolo);
        printf("temperatura: %d\n", temp);
        printf("Raios UV: %d\n", radiacao);
        printf("Irrigação: %d\n", irrigacao);
        printf("abastecimento: %d\n", abastecimento);
        printf("sysAuto: %d\n", sys_auto);
        printf("PowerSys: %d\n", power_sys);
        printf("status: %d\n", status);
    }
}
void monitor()
{

    limpar_o_buffer();
    desenhar(matriz_4, 64);
    escrever_no_buffer();
    gpio_init(LED_R);
    config_pwm(LED_B, A4);
    config_pwm(LED_R, A4);
    config_pwm_beep(BUZZER_A, B4, 50000000);
    config_pwm_beep(BUZZER_B, B4, 20000000);
    gpio_put(LED_G, B4);
    st_led_G = B4;

    // Controla o brilho do LED vermelho com base no eixo X
    if ((eixo_x_valor < 1500) || (eixo_x_valor > 2200))
    {
        pwm_set_gpio_level(LED_R, eixo_x_valor);
        st_led_R = A4;
        pwm_set_gpio_level(BUZZER_A, eixo_x_valor);
        st_bz_A = B4;
    }
    else
    {
        pwm_set_gpio_level(LED_R, 0);
        st_led_R = 0;
        pwm_set_gpio_level(BUZZER_A, 0);
        st_bz_A = 0;
    }

    // Controla o brilho do LED azul com base no eixo Y
    if ((eixo_y_valor < 1500) || (eixo_y_valor > 2200))
    {
        pwm_set_gpio_level(LED_B, eixo_y_valor);
        st_led_B = A4;
        pwm_set_gpio_level(BUZZER_B, eixo_y_valor);
        st_bz_B = B4;
    }
    else
    {
        pwm_set_gpio_level(LED_B, 0);
        st_led_B = 0;
        pwm_set_gpio_level(BUZZER_B, 0);
        st_bz_B = 0;
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
    ssd1306_draw_string(&ssd, "B", 93, 23);
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
    ssd1306_draw_string(&ssd, "B", 98, 53);
    ssd1306_rect(&ssd, 53, 110, 8, 8, cor, !gpio_get(BT_B));
}
