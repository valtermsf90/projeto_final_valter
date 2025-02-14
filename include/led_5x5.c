uint8_t _intensidade_ = 255;
int tamanho_matriz = 5;

uint matrizint[5][5] = {
		{24, 23, 22, 21, 20},
		{15, 16, 17, 18, 19},
		{14, 13, 12, 11, 10},
		{5, 6, 7, 8, 9},
		{4, 3, 2, 1, 0}};

void desenhar(char desenho[5][5])
{
	_intensidade_ = 255;
	for (int x = 0; x < tamanho_matriz; x++)
	{
		for (int y = 0; y < tamanho_matriz; y++)
		{
			if (desenho[x][y] == 'R')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 0, 0, _intensidade_);
			}
			if (desenho[x][y] == 'G')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 255, 0, _intensidade_);
			}
			if (desenho[x][y] == 'B')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 0, 255, _intensidade_);
			}
			if (desenho[x][y] == 'Y')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 255, 0, _intensidade_);
			}
			if (desenho[x][y] == 'P')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 0, 255, _intensidade_);
			}
			if (desenho[x][y] == 'C')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 255, 255, _intensidade_);
			}
			if (desenho[x][y] == 'W')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 255, 255, _intensidade_);
			}
			if (desenho[x][y] == '*')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 0, 0, _intensidade_);
			}
		}
		// Atualiza a matriz de LEDs e espera um tempo antes de mudar a cor
		escrever_no_buffer();
		sleep_ms(10); // 300ms entre as mudanças de cor
	}
}
