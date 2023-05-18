#include <stdio.h>
#include <math.h>

int desloca;
int mantissa_aux;
int mantissa_aux2;
int soma_mantissa;

typedef union {
	float f;
	struct field {
		unsigned int mantissa : 23;
		unsigned int expoente : 8;
		unsigned int sinal : 1;
	} field;
} unionfloat;

void binario(int n, int i)
{
    int k;
    for (i--; i >= 0; i--) {
      k = n >> i;
      if (k & 1)
          printf("1");
      else
         printf("0");
    }
}

void printNumeroReconstruido(struct field valor, int mantissa) {
    printf("\n\nnumero reconstituido: %f \n", pow(-1,(valor.sinal)) * ( mantissa / pow(2,23)) * pow(2,(valor.expoente - 127)));
}

void soma(unionfloat var, unionfloat var2) {
    int exp = var.field.expoente - 127;
    int exp2 = var2.field.expoente - 127;

    int mantissa_aux = var.field.mantissa + 0x800000;
    int mantissa_aux2 = var2.field.mantissa + 0x800000;
    if (exp != exp2) {
        if (exp > exp2) {
            int desloca = exp-exp2;
            mantissa_aux2 = mantissa_aux2 >> desloca;
            int soma_mantissa = mantissa_aux2 + mantissa_aux;
            binario(soma_mantissa, 23);
            printNumeroReconstruido(var.field, soma_mantissa);
        } else {
            desloca = exp2-exp;
            mantissa_aux = mantissa_aux >> desloca;
            soma_mantissa = mantissa_aux + mantissa_aux2;
            binario(soma_mantissa, 23);
            printNumeroReconstruido(var2.field, soma_mantissa);
        }
    } else {
        soma_mantissa = mantissa_aux + mantissa_aux2;
        binario(soma_mantissa, 23);
        printNumeroReconstruido(var2.field, soma_mantissa);
    }
}

int main() {
    unionfloat var;
    unionfloat var2;

    printf("Entre com um ponto flutuante 1: ");
    scanf("%f",&var.f);
    
    printf("Entre com um ponto flutuante 2: ");
    scanf("%f",&var2.f);
    
    printf("%d ",var.field.sinal);
    binario(var.field.expoente, 8);
    printf(" ");
    binario(var.field.mantissa, 23);
    printNumeroReconstruido(var.field, var.field.mantissa);

    printf("\n%d ",var2.field.sinal);
    binario(var2.field.expoente, 8);
    printf(" ");
    binario(var2.field.mantissa, 23);
    printNumeroReconstruido(var2.field, var2.field.mantissa);

    soma(var, var2);

    return 0;
}