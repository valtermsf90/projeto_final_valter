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
#include "include/ssd1306.h"

// PROTOTIPOS
void config_display();
static void interrupcao(uint gpio, uint32_t events);

// VARIAVEIS
uint8_t luz = 0;
int num = 0;
bool flag = true;
bool cor = true;
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define WIDTH 128
#define HEIGHT 64

ssd1306_t ssd;
char strx[5];
char stry[5];
char strnum[5];
char cores[10];

void config_display()
{
    // Configuração do I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do display OLED
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    // Limpa o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

int main()
{
    // I/O, PINOS, MATRIZ LED 5X5, ADC E PWM------+
    stdio_init_all();                           //|
    inicializacao_gpio();                       //|
    inicializacao_maquina_pio(PINO_MATRIZ_LED); //|
    adc_init();                                 //|
                                                //|
                                                //|
    config_display();                           //|
    //--------------------------------------------+
    int PIN = LED_G;
    adc_config();
    gpio_set_irq_enabled_with_callback(BT_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
    gpio_set_irq_enabled_with_callback(BT_B, GPIO_IRQ_EDGE_FALL, true, &interrupcao);

    while (true)
    {

        desenhar(matriz[num], 64);
        escrever_no_buffer();
        
        adc_config();
        uint8_t luz = 255 / 4096 * eixo_y_valor;
        sprintf(strx, "%d", eixo_x_valor); // Converte o inteiro em string
        sprintf(stry, "%d", eixo_y_valor);
        sprintf(strnum, "%d", num);
        ssd1306_fill(&ssd, !cor);                           // Limpa o displayKT
        ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 10, 0); // Desenha uma string
        ssd1306_draw_string(&ssd, "Led RGB Ativo", 18, 16); // Desenha uma string
        ssd1306_draw_string(&ssd, cores, 20, 28);

        ssd1306_draw_string(&ssd, "X    Y    PB", 20, 41);
        ssd1306_line(&ssd, 44, 37, 44, 60, cor); // Desenha uma linha vertical
        ssd1306_draw_string(&ssd, strx, 8, 52);  // Desenha uma string
        ssd1306_line(&ssd, 84, 37, 84, 60, cor);
        ssd1306_draw_string(&ssd, stry, 49, 52);
        ssd1306_draw_string(&ssd, strnum, 99, 52);  // Desenha uma string
        ssd1306_send_data(&ssd); // Atualiza o display

        config_pwm(PIN);
        if (eixo_x_valor < 1500)
        {
            PIN = LED_B;

            strcpy(cores, "AZUL");
            printf("LED esta %s\n", cores);
        }
        if (eixo_x_valor >= 1500)
        {
            PIN = LED_G;
            strcpy(cores, "VERDE");
            printf("LED esta %s\n", cores);
        }
        if (eixo_x_valor > 3000)
        {
            PIN = LED_R;
            strcpy(cores, "VERMELHO");
            printf("LED esta %s\n", cores);
        }
        pwm_set_gpio_level(PIN, eixo_y_valor);

        printf("eixo x:%d\n", eixo_x_valor);
        printf("eixo y:%d\n", eixo_y_valor);

        sleep_ms(100);
    }
}

void interrupcao(uint gpio, uint32_t events)
{

    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 500000) // 500 ms de debouncing
    {
        last_time = current_time; // Atualiza o tempo do último evento
      
        if (gpio == BT_A)
        {
            if (num == 9)
            {
                num = 9;
            }
            else
            {
                num++;
            }
            printf(" O numero da matriz é %d\n", num);
        }
        if (gpio == BT_B)
        {
            if (num == 0)
            {
                num = 0;
            }
            else
            {
                num--;
            }
            
        }
    }
}
