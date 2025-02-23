# Sistema de Monitoramento de Irrigação Automatizada e Monitoramento ECG com Controle de Matriz de LED e Sensores

## 1. Escopo do Projeto

### Apresentação do Projeto
Este projeto implementa um **sistema embarcado** utilizando o **Raspberry Pi Pico** para monitoramento e controle de variáveis ambientais e fisiológicas. Ele inclui sensores para **temperatura, umidade do solo, radiação UV e sinais vitais**, além de controle de **LEDs RGB e matriz de LED** para exibição de informações. O sistema também permite o controle de irrigação automática com base nas condições do solo e do ambiente.

### Tecnologias e Bibliotecas Utilizadas
- **Plataforma:** Raspberry Pi Pico
- **Linguagem:** C
- **Bibliotecas:**
  - `pico/stdlib.h` (para controle geral)
  - `hardware/adc.h` (para leitura de sensores analógicos)
  - `hardware/pwm.h` (para controle de LEDs e buzzers)
  - `hardware/i2c.h` (para comunicação com display OLED)
  - `hardware/timer.h` (para temporização)
- **Outros Arquivos Incluídos:**
  - `led_5x5.c`, `frames.c`, `led_RGB_cores.c`, `funcoes.c`, `pwm.c`, `adc.c`

### Funcionalidades
✅ Monitoramento de **temperatura, umidade do solo e radiação UV**  
✅ Controle de **irrigação automática** com base na umidade do solo  
✅ Monitoramento de **nível de água no tanque** e abastecimento automático  
✅ Exibição gráfica de dados no **display OLED**  
✅ Controle de **LEDs RGB e buzzer** com base nos sensores  
✅ Modo manual para controle de válvulas de irrigação  

### Modos de Operação

#### 🔹 **Modo 1 - Monitoramento Ambiental e Irrigação**
Exibe informações sobre temperatura, umidade do solo, radiação UV e nível de água no tanque. Permite ativação do modo automático para controle da irrigação e abastecimento do tanque.
- **Botão A:** Liga/Desliga o sistema de irrigação.
- **Botão B:** Alterna o status do sistema para automático ou manual.
- **Botão Joystick** Ir para Modo 2 - Configuração do Sistema.

#### 🔹 **Modo 2 - Configuração do Sistema**
Permite configurar parâmetros como umidade mínima e máxima do solo, nível mínimo de água no tanque e outros.
- **joystick esquerda/direta:** Seleciona o parâmetro a ser ajustado.
- **joystick cima/baixo:** Aumenta ou diminui o valor do parâmetro selecionado.
 **Botão Joystick** Ir para Modo 3 - Controle Manual de Válvulas.

#### 🔹 **Modo 3 - Controle Manual de Válvulas**
Permite o controle manual das válvulas de irrigação.
- **Botão A:** Alterna o estado da válvula selecionada (ligar/desligar).
-**Botão B:** Ir para Modo 4 - Monitoramento de Sensores e Atuadores.
- **joystick esquerda/direta:** Seleciona a válvula (1, 2 ou 3).
 **Botão Joysttick** Ir para Modo 1.

#### 🔹 **Modo 4 - Monitoramento de Sensores e Atuadores**
Exibe o status dos LEDs, buzzers e valores dos sensores (eixo X, eixo Y, microfone).
- **Botão A:** Testa todos os LEDs e buzzers.
- **Botão B:** Alterna a exibição entre diferentes sensores.

---

## 2. Como Compilar e Executar

### **Requisitos:**
- SDK do Raspberry Pi Pico configurado
- Compilador GCC para ARM (`arm-none-eabi-gcc`)
- Ferramenta **CMake**

### **Compilação:**
```sh
mkdir build
cd build
cmake ..
make