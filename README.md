# Sistema de Monitoramento de irrigaçao automatizada e Monittoramento ECG Controle com Matriz de LED e Sensores

## 1. Escopo do Projeto

### Apresentação do Projeto
Este projeto implementa um **sistema embarcado** utilizando o **Raspberry Pi Pico W** para monitoramento e controle de variáveis ambientais e fisiológicas. Ele inclui sensores para **temperatura, umidade do solo, radiação UV e sinais vitais**, além de controle de **LEDs RGB e matriz de LED** para exibição de informações.

### Tecnologias e Bibliotecas Utilizadas
- **Plataforma:** Raspberry Pi Pico W
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
✅ Exibição de **sinais vitais** (frequência cardíaca, respiração, pressão arterial)
✅ Controle de **LEDs RGB e buzzer** com base nos sensores
✅ Exibição gráfica de dados no **display OLED**
✅ Modo automático de **controle de irrigação e abastecimento de tanque**

### Modos de Operação

#### 🔹 **Modo 1 - Monitoramento Ambiental**
Exibe informações sobre temperatura, umidade do solo e radiação UV. Permite ativação do modo automático para controle da irrigação e abastecimento do tanque.
- **Botão A:** Liga/Desliga sistema de irrigação.
- **Botão B:** Alterna o status do sistema para automatica ou manual.

#### 🔹 **Modo 2 - Monitoramento Fisiológico**
Exibe dados de sinais vitais como **frequência cardíaca, respiração e pressão arterial**, permitindo uma análise visual no display OLED.
- **Botão A:** Alterna exibição entre diferentes parâmetros.
- **Botão B:** Entra no modo BOOTSEL.

#### 🔹 **Modo 3 - Animação Visual**
Exibe **animações interativas** na matriz de LED, como olhos que seguem a direção do joystick.
- **Botão A:** Alterna entre expressões faciais.
- **Botão B:** Liga ou desliga a iluminação dos LEDs.

#### 🔹 **Modo 4 - Diagnóstico do Sistema**
Apresenta informações sobre **LEDs, botões, sensores analógicos e periféricos conectados**, facilitando a depuração do hardware.
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
```

### **Upload para o Pico:**
1. Conecte o Pico ao PC segurando o botão **BOOTSEL**.
2. Copie o arquivo **.uf2** gerado para a unidade do Pico.

### **Uso:**
- O sistema inicia exibindo **informações ambientais**.
- Os **botões permitem alternar** entre diferentes telas e modos.
- O sistema pode operar **automaticamente ou manualmente**.

---

## 3. Execução do Projeto

### Metodologia
1. Definição do escopo e funcionalidades.
2. Pesquisa de componentes e bibliotecas.
3. Desenvolvimento modular do firmware.
4. Testes e depuração.

### Testes de Validação
✅ Teste dos sensores analógicos.
✅ Exibição correta das informações no display.
✅ Funcionamento dos LEDs e buzzers.

### Discussão dos Resultados
Os testes demonstraram que o sistema embarcado funciona conforme esperado, alternando corretamente entre os modos e exibindo informações em tempo real.

---

## 4. Referências
- **Raspberry Pi Pico W Datasheet**: [https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf]
- **Código-fonte no GitHub**: [https://github.com/valtermsf90/projeto_final_valter/]
- **Simulação no Wokwi**: [https://wokwi.com/projects/423361855703845889]

---

## 5. Autor
**Valtemar Machado Silva Filho**

