#include <stdio.h>
#include <stdlib.h>
#include "No.h"


No *Cria_No(unsigned char X, unsigned char Y)
{
	No *N = (No*)malloc(sizeof(No));
	N->Proximo = NULL;
	N->X = X;
	N->Y = Y;
	return N;
}

void Destroi_Nos(No *Cabeca)
{
	No *A = Cabeca;
	if (A) {
		while (A)
		{
			A = Cabeca;
			if (A->Proximo == NULL)
			{
				free(Cabeca);
				A = NULL;
			}
			else
			{
				while (A->Proximo != NULL)
				{
					A = A->Proximo;
				}
				free(A);
				A = Cabeca;
			}
		}
	}
}