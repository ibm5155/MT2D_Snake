#include <MT2D/MT2D_System_Calls.h>
#include <MT2D/MT2D_Keyboard.h>
#include <MT2D/MT2D_Display.h>
#include <MT2D/Tools/Video/MT2D_Generic_Line.h>
#include <MT2D/MT2D.h>
#include "No.h"

#include <stdio.h>
#include <stdlib.h>

#define METADE_DA_TELA MAX_HOR/2

enum {
	CIMA, BAIXO, ESQUERDA, DIREITA
};


int G_PONTOS = 0;
unsigned char G_Direcao = CIMA;
No *G_Snake = 0;
No Item;


bool Colidiu_Cobra_Com_Corpo()
{
	bool Saida = false;
	No *Atual = G_Snake->Proximo;
	while (Saida == false && Atual)
	{
		if (G_Snake->X == Atual->X)
		{
			if (G_Snake->Y == Atual->Y)
			{
				Saida = true;
			}
			else
			{
				Atual = Atual->Proximo;
			}
		}
		else
		{
			Atual = Atual->Proximo;
		}
	}
	return Saida;
}

bool Colidiu_Cobra_Com_Mapa()
{
	bool Saida = false;
	if (G_Snake->X == -1 || G_Snake->X == (MAX_HOR/2) || G_Snake->Y == -1 || G_Snake->Y == MAX_VER)
	{
		Saida = true;
	}
	return Saida;
}

bool Colidiu_Cobra_Posicao(unsigned char X, unsigned char Y)
{
	bool Saida = false;
	No *Atual = G_Snake;
	while (Saida == false && Atual)
	{
		if (X == Atual->X)
		{
			if (Y == Atual->Y)
			{
				Saida = true;
			}
			else
			{
				Atual = Atual->Proximo;
			}
		}
		else
		{
			Atual = Atual->Proximo;
		}
	}
	return Saida;
}

void Adiciona_Cauda(char Sleep)
{
	No *Anterior_Cauda = G_Snake;
	No *Cauda = NULL;
	while (Anterior_Cauda->Proximo)
	{
		Anterior_Cauda = Anterior_Cauda->Proximo;
	}
	Cauda = Cria_No(Anterior_Cauda->X, Anterior_Cauda->Y);
	Anterior_Cauda->Proximo = Cauda;
	Cauda->Sleep = Sleep;

}

void Move_Cobra()
{
	No *Atual = G_Snake->Proximo;
	No *Anterior = G_Snake;
	char AtualX;
	char AtualY;
	char OldX = G_Snake->X;
	char OldY = G_Snake->Y;
	while (Atual)
	{
		if (Atual->Sleep)
		{
			Atual->Sleep--;
		}
		else
		{
			AtualX = Atual->X;
			AtualY = Atual->Y;
			Atual->X = OldX;
			Atual->Y = OldY;
			OldX = AtualX;
			OldY = AtualY;

		}
		Atual = Atual->Proximo;
		Anterior = Anterior->Proximo;
	}
	switch (G_Direcao)
	{
	case CIMA:
		G_Snake->Y--;
		break;
	case BAIXO:
		G_Snake->Y++;
		break;
	case ESQUERDA:
		G_Snake->X--;
		break;
	case DIREITA:
		G_Snake->X++;
		break;
	}
}
void Desenha_Cobra()
{
	No *Atual = G_Snake->Proximo;
	WINDOW1[G_Snake->Y][G_Snake->X] = 178;

	while(Atual)
	{
		WINDOW1[Atual->Y][Atual->X] = 177;
		Atual = Atual->Proximo;
	}
}

void Cria_Item()
{
	do {
		Item.X = rand() % (METADE_DA_TELA);
		Item.Y = rand() % MAX_VER;
	} while (Colidiu_Cobra_Posicao(Item.X, Item.Y) == true);
	Item.Proximo = (No*)1; // existe
}

void Jogo()
{
#pragma region INICIALIZAÇÃO
	unsigned char tecla_lida = NULL;

	G_PONTOS = 0;
	G_Snake = Cria_No(MAX_HOR/4, MAX_VER/2);
	G_Direcao = CIMA;
	Cria_Item();
#pragma endregion
	while (1)
	{
#pragma region RENDERIZA
		MT2D_Clear_Main_Window();
		MT2D_Line_Draw_Horizontal(METADE_DA_TELA, 186);
		Desenha_Cobra();
		if (Item.Proximo != NULL) {
			WINDOW1[Item.Y][Item.X] = 'o'; // desenha item
		}
		else
		{
			Cria_Item();
		}

		insert_string_on_display("SNAKE", 0, METADE_DA_TELA + 1, DISPLAY_WINDOW1);
		insert_string_on_display("PONTOS:", 2, METADE_DA_TELA + 3, DISPLAY_WINDOW1);
		insert_number_on_display(G_PONTOS, 2, METADE_DA_TELA + 11, DISPLAY_WINDOW1);
		MT2D_Draw_Window(DISPLAY_WINDOW1);
#pragma endregion e recria item caso deletado
#pragma region LE TECLADO
		if (MT2D_Keyboard_touched())
		{
			tecla_lida = MT2D_Keyboard_keytouched();
			if (tecla_lida == arrow_key_pressed)
			{
				tecla_lida = MT2D_Keyboard_keytouched();
				if (tecla_lida == arrow_up_pressed)
				{
					G_Direcao = CIMA;
				}
				else if (tecla_lida == arrow_down_pressed)
				{
					G_Direcao = BAIXO;
				}
				else if (tecla_lida == arrow_left_pressed)
				{
					G_Direcao = ESQUERDA;
				}
				else if (tecla_lida == arrow_right_pressed)
				{
					G_Direcao = DIREITA;
				}
			}
		}
#pragma endregion
#pragma region ESPEREA
		MT2D_System_Delay(200 - G_PONTOS);
#pragma endregion
#pragma region MOVE COBRA
		Move_Cobra();
#pragma endregion
#pragma region DETECTA COLISÕES
		if (Colidiu_Cobra_Com_Corpo())
		{
			return;
		}
		if (Colidiu_Cobra_Com_Mapa())
		{
			return;
		}
		if (Colidiu_Cobra_Posicao(Item.X, Item.Y))
		{
			switch (rand() % 5)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				G_PONTOS++;
				Adiciona_Cauda(1);
				break;
			case 4:
				G_PONTOS+=2;
				Adiciona_Cauda(1);
				Adiciona_Cauda(2);
				break;
			case 5:
				G_PONTOS += 3;
				Adiciona_Cauda(1);
				Adiciona_Cauda(2);
				Adiciona_Cauda(3);
				break;

			}
			Item.Proximo = NULL;
		}
#pragma endregion
#pragma region AÇÃO
#pragma endregion
	}
	Destroi_Nos(G_Snake);
}

/*
	ZERA DADOS

	RENDERIZA

	LE TECLADO

	ESPERA

	DETECTA COLISÕES
	
	AÇÃO

*/