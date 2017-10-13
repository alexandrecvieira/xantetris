/****************************************************************************************************************************************/
/* Alexandre - Graduado em Sistemas de Informação - Jogo Tetris, movimentado pelas setas do teclado                                     */
/*                                                     Código Versão 1-4 Linux                                                          */
/* Para compilar é necessário a biblioteca pdcurses e utilizar o comando: i586-mingw32msvc-gcc tetris_v1.4.1w.c -o tetris.exe -lpdcurses*/
/****************************************************************************************************************************************/
/**************************************************************************************************************************************************/
/* O código consiste de quatro funções, a função <monta_peca> recebe como parâmetros a matriz peca, o número (npeca) da peça e a		  		  */
/* posição (pospeca) da peça, as peças podem ser 1, 2 ou 3 e as	posições podem ser 1, 2, 3 ou 4, a peça número 1 só tem a posição 1.              */
/*                                                                                                                                  			  */
/* A função <inicializa> zera todas as posições da matriz placar, a matriz placar tem a função de armazenar as peças a medida que chegam ao	      */
/* final da tela. A função <mostra_tela> imprime a tela no terminal recebendo como parâmetros a matriz peca, a matriz placar,			  		  */
/* a posição atual da linha (pi_atual), a posição atual da coluna (pj_atual) e os pontos.                                                         */
/*                                                                                      														  */
/* A função <imprime_peca> imprime a peça centralizada no rodapé da tela.                                                                         */
/*                                                                            																	  */
/* As matrizes peca e placar armazenam 0 (zero) em todas as posições, as posições ocupadas pelas peças armazenam 1 (um), então quando a função    */
/* <mostra_tela> encontra uma posição com 1 (um) armazenado ela imprime o caractere <addch(ACS_CKBOARD)> (na tabela ascii extendida seria o 	  */
/* caractere 177)                                                                                                                                 */
/*                    				                                                                                                        	  */
/* A constante <TAMPECA> não pode ser alterada.                                                                                                   */
/*                                                  																							  */
/* A constante <TAMTELA> pode ser mudada de acordo com os tamanhos (de tela) testados, 20 (notebook), 26 (notebook), 32, 38 e 44		  		  */
/**************************************************************************************************************************************************/

#include <stdlib.h> /* Biblioteca incluida para utilização das funções rand e srand */
#include <locale.h>
#include <time.h> /* Biblioteca incluida para utilização da função time */
#ifdef __WINDOWS__
#include <curses.h> /* Biblioteca principal utilizada para todas as funções de IO */
#include <windows.h> /* Biblioteca incluida para utilização da função Sleep */
#else
#ifdef __LINUX__
#include <unistd.h> /* Biblioteca incluida para utilização da função sleep */
#include <ncurses.h> /* Biblioteca principal utilizada para todas as funções de IO */
#else
#error "Deve-se especificar __LINUX__ ou __WINDOWS__ na compilação"
#endif
#endif
#define TAMPECA 3 /* Determina o tamanho da peça 3x3 */
#define TAMTELA 32  /* Determina o tamanho da tela do jogo, tamanhos testados: 20, 26, 32, 38 e 44 */
#define CENTROV COLS / 2 /* Determina o centro vertical da tela */
#define BORDAJ (COLS - TAMTELA) / 2 /* Determina o valor das bordas verticais da área do jogo */

/* Início do bloco de funções ************************************************************************************************ */
	void monta_peca(int peca[][TAMPECA], int npeca, int pospeca)
	{
		int i, j;
		for(i = 0; i < 3; i++)
			{
				for(j = 0; j < 3; j++)
					{
						peca[i][j] = 0;
						if(npeca == 1 && pospeca == 1) peca[i][j] = 1;
						/*if(npeca == 1 && pospeca == 2 && j == 0) peca[i][j] = 1;*/ /* Suprimido durante o desenvolvimento */
						if((npeca == 2 && pospeca == 1) && (i == 2 || j == 1)) peca[i][j] = 1;
						if((npeca == 2 && pospeca == 2) && (i == 1 || j == 2)) peca[i][j] = 1;
						if((npeca == 2 && pospeca == 3) && (i == 0 || j == 1)) peca[i][j] = 1;
						if((npeca == 2 && pospeca == 4) && (i == 1 || j == 0)) peca[i][j] = 1;
						if((npeca == 3 && pospeca == 1) && (i == 2 || j == 2)) peca[i][j] = 1;
						if((npeca == 3 && pospeca == 2) && (i == 0 || j == 2)) peca[i][j] = 1;
						if((npeca == 3 && pospeca == 3) && (i == 0 || j == 0)) peca[i][j] = 1;
						if((npeca == 3 && pospeca == 4) && (i == 2 || j == 0)) peca[i][j] = 1;
					}
			}
	}

void inicializa(int placar[TAMTELA - 7][TAMTELA - 2])
	{
		int i, j;
		for(i = 0; i < TAMTELA - 7; i++)
			{
				for(j = 0; j < TAMTELA - 2; j++)
					{
						placar[i][j] = 0;
					}
			}
	}

void mostra_tela(int peca[][TAMPECA], int placar[TAMTELA - 7][TAMTELA - 2], int pi_atual, int pj_atual, int pontos, int status)
	{
		int i, j, l, m;
		mvprintw(0, CENTROV - 22, "|------------------------------------------|");
		mvprintw(1, CENTROV - 22, "|      Tetris por Alexandre C Vieira       |");
		mvprintw(2, CENTROV - 22, "|[ <F2> Zera o jogo ][ <F3> Inicia o jogo ]|");
		mvprintw(3, CENTROV - 22, "|   [ <F4> Fecha ] [ <Setas> Movimenta ]   |");
		mvprintw(4, CENTROV - 22, "|              V1 - Junho 2012             |");
		mvprintw(5, CENTROV - 22, "|------------------------------------------|");
		for(i = 6; i < TAMTELA; i++)
			{
				for(j = BORDAJ; j < TAMTELA + BORDAJ; j++)
					{
						if(i == 6)mvprintw(i, j, "-");
						if(j == BORDAJ && i > 6)mvprintw(i, j, "|");
						if(status == 1 && j < TAMTELA + BORDAJ - 1 && placar[i - 6][j - BORDAJ] == 1) /* Bloco que imprime as peças armazenadas em placar na tela */
							{
								move(i, j + 1);
								addch(ACS_CKBOARD);
							}
						if(i == pi_atual && j == pj_atual) /* Bloco que imprime a peça em movimento na tela */
							{
								for(l = 0; l < 3; l++)
									{
										for(m = 0; m < 3; m++)
											{
												if(peca[l][m] == 1)
													{
														move(i + l, j + m);
														addch(ACS_CKBOARD);
													}
											}
									}
							}
						if(status == 0)
							{
								mvprintw(10, CENTROV - 10, "|-------------------|");
								mvprintw(11, CENTROV - 10, "|  [ <a> Level 1 ]  |");
								mvprintw(12, CENTROV - 10, "|  [ <b> Level 2 ]  |");
								mvprintw(13, CENTROV - 10, "|  [ <c> Level 3 ]  |");
								mvprintw(14, CENTROV - 10, "|  [ <d> Level 4 ]  |");
								mvprintw(15, CENTROV - 10, "|-------------------|");
							}
						if(i == TAMTELA - 1)mvprintw(i, j, "-");
						if(j == TAMTELA + BORDAJ - 1 && (i > 6 && i < TAMTELA - 1))mvprintw(i, j, "|");
					}
			}
		mvprintw(TAMTELA + 1, (COLS / 2) - 22, "|------------------------------------------|");
		mvprintw(TAMTELA + 2, (COLS / 2) - 22, "|   Linha %2d   Coluna %2d     Pontos %4d   |", pi_atual - 6, pj_atual - BORDAJ, pontos);
		mvprintw(TAMTELA + 3, (COLS / 2) - 22, "|------------------------------------------|");
		refresh();
	}

	void imprime_peca(int peca[][TAMPECA])
	{
		int i, j, l = TAMTELA + 4, m = TAMTELA / 2 - 1;
		for(i = 0; i < 3; i++)
			{
				for(j = 0; j < 3; j++)
					{
						if(peca[i][j] == 1)
							{
								move(l + i, m + j + BORDAJ);
								addch(ACS_CKBOARD);
							}
					}
			}
	}
/* Final do bloco de funções *************************************************************************************************** */

int main()
{
	initscr(); /* Inicializa a biblioteca */
	noecho(); /* Faz com que o caracter digitado não seja ecoado na tela */
	keypad(stdscr, TRUE); /* Habilita o uso das teclas especiais (Setas, F1, F2, etc) */
	curs_set(0); /* Faz o cursor ficar invisível (0) ou visível (1) */
	/* cbreak(); */ /* Disponibiliza imediatamente o caracter digitado ao programa, usado durante o desenvolvimento */
	halfdelay(5); /* Espera alguns décimos de segundo (5) o usuário digitar alguma posição, retorna ERR em caso de timeout */

	int i, j, p = KEY_F(2), pi_atual = 7, pi_final = TAMTELA - 1, pj_atual = CENTROV - 1, peca[TAMPECA][TAMPECA];
	int	placar[TAMTELA - 7][TAMTELA - 2], npeca = 1, pospeca = 1, pontos = 0, status = 0, aux, zerai;

	setlocale(LC_ALL, "pt_BR.utf-8");
	resize_term(TAMTELA + 12, TAMTELA + 20);
	srand(time(NULL)); /* Gera a semente para a geração das peças e posições aleatórias */
	monta_peca(peca, npeca, pospeca); /* Monta a peça inicial */

	while(p != KEY_F(4))
		{
			if(p == KEY_F(2))
				{
					status = 0; /* status = 0 Desabilita as teclas de movimentação */
					pontos = 0; /* Zera os pontos */
					inicializa(placar); /* Inicializa a matriz placar */
					npeca = 1 + (int)(3.0 * rand() / (RAND_MAX + 1.0)); /* Sorteia a peça randomicamente */
					pospeca = 1; /* Seta a posição da peça em 1 para ser usada a peça 1 */
					if(npeca > 1) /* Se a peça sorteada for diferente da 1 gera a posição randomicamente */
						{
							pospeca = 1 + (int)(5.0 * rand() / (RAND_MAX + 1.0)); /* Sorteia a posição da peça randomicamente */
							if(pospeca > 4)pospeca = 1; /* Força a posição 1 se pospeca for incrementado acima de 4 */
						}
					pi_atual = 7; /* Seta a linha inicial */
					pi_final = TAMTELA - 1; /* Seta o final da movimentação */
					pj_atual = CENTROV - 1; /* Seta a coluna inicial */
					monta_peca(peca, npeca, pospeca);
				}
			if(p == KEY_F(3)) /* F3 inicia o jogo */
				{
					status = 1; /* Habilita as teclas de movimentação */
					pontos = 0;
					inicializa(placar);
					npeca = 1 + (int)(3.0 * rand() / (RAND_MAX + 1.0)); /* Sorteia a peça randomicamente */
					pospeca = 1;
					if(npeca > 1)
						{
							pospeca = 1 + (int)(5.0 * rand() / (RAND_MAX + 1.0)); /* Sorteia a posição da peça randomicamente */
							if(pospeca > 4)pospeca = 1; /* Força a posição 1 se pospeca for incrementado acima de 4 */
						}
					pi_atual = 7; /* Seta a linha inicial */
					pi_final = TAMTELA - 1;
					pj_atual = CENTROV - 1; /* Seta a coluna inicial */
					monta_peca(peca, npeca, pospeca);
				}
			if(p == KEY_UP && status == 1) /* Gira a peça */
				{
					if(npeca > 1) /* Altera posição da peça se a peça for diferente da 1 */
						{
							pospeca++;
							if(pospeca > 4)pospeca = 1;
							monta_peca(peca, npeca, pospeca);
						}
				}
			if(p == KEY_RIGHT && pj_atual < TAMTELA + BORDAJ - 4 && status == 1) /* Movimenta a peça para a direita */
				{
					if(placar[pi_atual - 4][pj_atual + 2] != 1 || placar[pi_atual - 5][pj_atual + 2] != 1 || placar[pi_atual - 6][pj_atual + 2] != 1)pj_atual++;
				}
			if(p == KEY_LEFT && pj_atual > BORDAJ + 1 && status == 1) /* Movimenta a peça para a esquerda */
				{
					if(placar[pi_atual - 4][pj_atual - 2] != 1 || placar[pi_atual - 5][pj_atual - 2] != 1 || placar[pi_atual - 6][pj_atual - 2] != 1)pj_atual--;
				}
			if(p == KEY_DOWN && pi_atual <= pi_final && status == 1) /* Movimenta a peça para baixo */
				{
					pi_atual++;
					zerai = 0;
					pi_final = TAMTELA - 1;
					if(placar[3][pj_atual - BORDAJ] == 1)status = 2; /* Se a matriz placar estiver preenchida em qualquer posição no início, seta status = 2 (game over) */

					/* Início do bloco que verifica as posições das peças em placar e seta os limites da movimentação *****************************************************/
					if((placar[pi_atual - 3][pj_atual - BORDAJ - 1] == 1 && peca[2][0] == 1) || (placar[pi_atual - 3][pj_atual - BORDAJ] == 1 && peca[2][1] == 1) || (placar[pi_atual - 3][pj_atual - BORDAJ + 1] == 1 && peca[2][2] == 1))
						{
							pi_final = pi_atual;
						}
					if((placar[pi_atual - 4][pj_atual - BORDAJ - 1] == 1 && peca[1][0] == 1) || (placar[pi_atual - 4][pj_atual - BORDAJ] == 1 && peca[1][1] == 1) || (placar[pi_atual - 4][pj_atual - BORDAJ + 1] == 1 && peca[1][2] == 1))
						{
							pi_final = pi_atual;
						}
					if((placar[pi_atual - 5][pj_atual - BORDAJ - 1] == 1 && peca[0][0] == 1) || (placar[pi_atual - 5][pj_atual - BORDAJ] == 1 && peca[0][1] == 1) || (placar[pi_atual - 5][pj_atual - BORDAJ + 1] == 1 && peca[0][2] == 1))
						{
							pi_final = pi_atual;
						}
					/* Final do bloco que verifica as posições das peças em placar e seta os limites da movimentação */

					/* Início do bloco que armazena as peças na matriz placar e reinicia a movimentação do início da tela */
					if(pi_atual == TAMTELA - 4 || pi_atual == pi_final)
						{
							for(i = 0; i < 3; i++)
								{
									for(j = 0; j < 3; j++)
										{
											if(peca[i][j] == 1)
												{
													placar[pi_atual - 6 + i][pj_atual - BORDAJ - 1 + j] = 1;
													pontos++; /* Incrementa a pontuação por peça */
												}
										}
								}
							npeca = 1 + (int)(3.0 * rand() / (RAND_MAX + 1.0)); /* Sorteia a peça randomicamente */
							pospeca = 1;
							if(npeca > 1)
								{
									pospeca = 1 + (int)(5.0 * rand() / (RAND_MAX + 1.0)); /* Sorteia a posição da peça randomicamente */
									if(pospeca > 4)pospeca = 1; /* Força a posição 1 se pospeca for incrementado acima de 4 */
								}
							monta_peca(peca, npeca, pospeca);
							pi_atual = 7;
						}
					/* Final do bloco que armazena as peças na matriz placar e reinicia a movimentação do início da tela */

					/* Início do bloco que apaga as linhas completadas */
					for(i = 0; i < TAMTELA - 7; i++)
							{
								aux = 0;
								for(j = 0; j < TAMTELA - 2; j++)
									{
										if(placar[i][j] == 1)
											{
												aux++;
												if(aux == TAMTELA - 2) zerai = i;
											}
									}
							}
					if(zerai > 0)
						{
							for(i = zerai; i > 0; i--)
								{
									for(j = 0; j < TAMTELA - 2; j++)
										{
											placar[i][j] = placar[i - 1][j];
										}
								}
							pontos += 20; /* Incrementa a pontuação por linha */
						}
					/* Final do bloco que apaga as linhas completadas */
				}
			mostra_tela(peca, placar, pi_atual, pj_atual, pontos, status); /* Imprime a tela final no terminal */
			imprime_peca(peca); /* Imprime a peça no rodapé da tela */
			p = 0;
			p = getch();
			if(p == ERR) p = KEY_DOWN;
			if(p == 'a')
				{
					halfdelay(6);
				}
			if(p == 'b')
				{
					halfdelay(4);
				}
			if(p == 'c')
				{
					halfdelay(2);
				}
			if(p == 'd')
				{
					halfdelay(1);
				}
			if(status == 2) /* Mensagem de game over */
				{
					mvprintw(TAMTELA + 7, CENTROV - 22, "****GAME OVER******************GAME OVER****");
					mvprintw(TAMTELA + 8, CENTROV - 22, "*****************GAME OVER******************");
					mvprintw(TAMTELA + 9, CENTROV - 22, "****GAME OVER******************GAME OVER****");
					refresh();
#ifdef __WINDOWS__
					Sleep(5000); /* Paralisa o jogo por 5 segundos */
#else
#ifdef __LINUX__
					sleep(5000);
#endif
#endif
					p = KEY_F(2); /* Reinicia a tela zerada */
				}
			clear();
			refresh();
		}

	endwin(); /* Finaliza a biblioteca e libera o terminal, obrigatório antes de finalizar o programa */
	return 0;
}

