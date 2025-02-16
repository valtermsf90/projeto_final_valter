#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "include/led_5x5.c"
#include "include/frames.c"
#include "include/led_RGB_cores.c"
#include "include/funcoes.c"
#include "include/pwm.c"
#include "include/adc.c"
#include "include/display.c"
//#include "include/ssd1306.h"
#include "pico/bootrom.h"

// PROTOTIPOS
void config_display();
static void interrupcao(uint gpio, uint32_t events);

// VARIAVEIS
static volatile uint32_t last_time = 0;

bool cor = true;
bool st_led_R = false;
bool st_led_B = false;






bool led_ON = false;
bool status = true;
int quadro = 1;
// PROTOTIPOS

void tela(int modo)
{
    if (modo == 1) // modo camera
    {

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

        // boca
        ssd1306_rect(&ssd, 55, 55, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 55, 70, 5, 5, cor, cor);
        ssd1306_rect(&ssd, 60, 60, 10, 5, cor, cor);

        // olho
        ssd1306_rect(&ssd, olho_esq_y, olho_esq_x, 15, 15, cor, cor);
        ssd1306_rect(&ssd, olho_esq_y, olho_esq_x + 66, 15, 15, cor, cor);
    }
    if (modo == 2) // retangulo triplo
    {
        

        ssd1306_rect(&ssd, 0, 0, 127, 63, cor, !cor);

        ssd1306_draw_string(&ssd, "LEDS", 3, 3); ssd1306_draw_string(&ssd, "R", 63, 3);ssd1306_draw_string(&ssd, "G", 83, 3);ssd1306_draw_string(&ssd, "B", 103, 3);
        ssd1306_draw_string(&ssd, "STATUS", 3, 13);
                ssd1306_rect(&ssd, 13, 63, 8, 8, cor, st_led_R); ssd1306_rect(&ssd, 13, 83, 8, 8, cor, gpio_get(LED_G));ssd1306_rect(&ssd, 13, 103, 8, 8, cor, st_led_B);


        ssd1306_rect(&ssd, 31, 1, 96, 32, cor, !cor);
        ssd1306_draw_string(&ssd, "MIC", 3, 33); ssd1306_draw_string(&ssd, str_mic, 63, 33);
        ssd1306_draw_string(&ssd, "EIXO X", 3, 43); ssd1306_draw_string(&ssd, str_x, 63, 43);
        ssd1306_draw_string(&ssd, "EIXO Y", 3, 53); ssd1306_draw_string(&ssd, str_y, 63, 53);

        ssd1306_rect(&ssd, 31, 96, 31, 32, cor, !cor);  
        ssd1306_draw_string(&ssd, " BT", 97, 33); 
        ssd1306_draw_string(&ssd, "A", 99, 43); ssd1306_rect(&ssd, 43, 110, 8, 8, cor, !gpio_get(BT_A));
        ssd1306_draw_string(&ssd, "J", 98, 53); ssd1306_rect(&ssd, 53, 110, 8, 8, cor, !gpio_get(BT_J));
     
    }
}
int main()
{
    // Configuração das interrupções
    gpio_set_irq_enabled_with_callback(BT_J, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
    gpio_set_irq_enabled_with_callback(BT_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
    gpio_set_irq_enabled_with_callback(BT_B, GPIO_IRQ_EDGE_FALL, true, &interrupcao);

    // INCIALIZACAO DO ADC, GIO, PWM
    stdio_init_all();
    adc_init();
    inicializacao_gpio();
    inicializacao_maquina_pio(PINO_MATRIZ_LED);
    config_display();
    adc_config();

    config_pwm(LED_B, status);
    config_pwm(LED_R, status);

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

        // Controla o brilho do LED vermelho com base no eixo X
        if ((eixo_x_valor < 1500) || (eixo_x_valor > 2200))
        {
            pwm_set_gpio_level(LED_R, eixo_x_valor);
            st_led_R = st_led_R + status;
           
        }
        else
        {
            pwm_set_gpio_level(LED_R, 0);
            st_led_R = false;
        }

        // Controla o brilho do LED azul com base no eixo Y
        if ((eixo_y_valor < 1500) || (eixo_y_valor > 2200))
        {
            pwm_set_gpio_level(LED_B, eixo_y_valor);
            st_led_B = st_led_B + status;

        }
        else
        {
            pwm_set_gpio_level(LED_B, 0);
            st_led_B = false;
        }

        // Exibe os valores dos eixos no terminal para depuração
        printf("EIXO X: %d\n", eixo_x_valor);
        printf("EIXO Y: %d\n", eixo_y_valor);
        printf("MIC: %d\n", mic);
        printf("LED R: %d\n", st_led_R);
        printf("LED B: %d\n", st_led_B);
        printf("LED G: %d\n", led_ON);
        sleep_ms(500);
        limpar_tela_serial();
    }
}

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
            if (quadro > 2)
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
