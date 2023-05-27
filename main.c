// Trabalho GB - Evelyn Noro e Mateus Tizotti

#include <stdio.h>
#include <math.h>

typedef union {
	float f;
	struct {
	    unsigned int mantissa: 23;
		unsigned int expoente: 8;
		unsigned int sinal: 1;
	} field;
} unionfloat;

void printNumeroReconstruido(float num) {
    printf("\nNumero reconstruido: %f \n\n", num);
}

void binario(int n, int i) {
    int k;
    for (i--; i >= 0; i--) {
      k = n >> i;
      if (k & 1)
        printf("1");
      else
        printf("0");
    }
    printf(" ");
}

printBinario(int sinal, int expoente, int mantissa) {
    printf("Num binario: %d ", sinal);
    binario(expoente, 8);
    binario(mantissa, 23);
}

float converterBinarioFloat(int sinal, int expoente, int mantissa) {
    return (pow(-1, sinal) * (mantissa / pow(2,23)) * pow(2, (expoente - 127)));
}

void soma(unionfloat var, unionfloat var2) {
    int exp = var.field.expoente - 127;
    int exp2 = var2.field.expoente - 127;

    int desloca;
    unsigned int soma_mantissa;

    unsigned int mantissa_aux = var.field.mantissa + 0x800000;
    unsigned int mantissa_aux2 = var2.field.mantissa + 0x800000;
    if (exp != exp2) {
        if (exp > exp2) {
            desloca = exp - exp2;
            mantissa_aux2 = mantissa_aux2 >> desloca;
            soma_mantissa = mantissa_aux2 + mantissa_aux;
            printf("valor mantissa 1: ");
            binario(soma_mantissa,23);
            printBinario(var.field.sinal, var.field.expoente, soma_mantissa);
            printNumeroReconstruido(converterBinarioFloat(var.field.sinal, var.field.expoente, soma_mantissa));
        } else {
            desloca = exp2 - exp;
            mantissa_aux = mantissa_aux >> desloca;
            soma_mantissa = mantissa_aux + mantissa_aux2;
            printf("valor mantissa 2: ");
            binario(soma_mantissa,23);
            printBinario(var2.field.sinal, var2.field.expoente, soma_mantissa);
            printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, soma_mantissa));
        }
    } else {
        soma_mantissa = mantissa_aux + mantissa_aux2;
        printf("valor mantissa 3: ");
        binario(soma_mantissa, 23);
        printBinario(var2.field.sinal, var2.field.expoente, soma_mantissa);
        printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, soma_mantissa));
    }
}

int main() {
    unionfloat var;
    unionfloat var2;
    char op = 'o';
    while (op != '!' ) {
        printf("Digite 's' para soma.\nDigite 'b' para subtracao.\nOpcao: ");
        scanf("%s", &op);
        printf("\n\nEntre com um ponto flutuante 1: ");
        scanf("%f",&var.f);
        
        printf("Entre com um ponto flutuante 2: ");
        scanf("%f",&var2.f);
        
        printf("Binario num 1: %d ",var.field.sinal);
        binario(var.field.expoente, 8);
        binario(var.field.mantissa, 23);
        printNumeroReconstruido(converterBinarioFloat(var.field.sinal, var.field.expoente, var.field.mantissa + 0x800000));

        printf("Binario num 2: %d ",var2.field.sinal);
        binario(var2.field.expoente, 8);
        binario(var2.field.mantissa, 23);
        printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, var2.field.mantissa + 0x800000));

        soma(var, var2);
    }

    return 0;
}