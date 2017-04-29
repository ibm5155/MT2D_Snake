#ifndef NO_H
#define NO_H

struct No{
	No *Proximo;
	char X;
	char Y;
	int Sleep = false;
};

No *Cria_No(unsigned char X, unsigned char Y);
void Destroi_Nos(No *Cabeca);
#endif