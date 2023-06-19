// Trabalho GB - Evelyn Noro e Mateus Tizotti

#include <stdio.h>
#include <stdint.h>
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

int binaryProduct(int binNum1, int binNum2) {
    int i = 0;
    int rem = 0;
    int product = 0;
    int sum[20] = { 0 };

    while (binNum1 != 0 || binNum2 != 0) {

        sum[i] = (binNum1 % 10 + binNum2 % 10 + rem) % 2;
        rem = (binNum1 % 10 + binNum2 % 10 + rem) / 2;

        binNum1 = binNum1 / 10;
        binNum2 = binNum2 / 10;

        i = i + 1;
    }

    if (rem != 0)
        sum[i] = rem;

    while (i >= 0) {
        product = product * 10 + sum[i];
        i = i - 1;
    }

    return product;
}

void printBinario(int sinal, int expoente, unsigned int mantissa, char msg[]) {
    printf("\nValor %s em binario: %d ", msg, sinal);
    binario(expoente, 8);
    binario(mantissa, 23);
}

float converterBinarioFloat(int sinal, int expoente, unsigned int mantissa) {
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
            printBinario(var.field.sinal, var.field.expoente, soma_mantissa, "soma");
            printNumeroReconstruido(converterBinarioFloat(var.field.sinal, var.field.expoente, soma_mantissa));
        } else {
            desloca = exp2 - exp;
            mantissa_aux = mantissa_aux >> desloca;
            soma_mantissa = mantissa_aux + mantissa_aux2;
            printBinario(var2.field.sinal, var2.field.expoente, soma_mantissa, "soma");
            printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, soma_mantissa));
        }
    } else {
        soma_mantissa = mantissa_aux + mantissa_aux2;
        printBinario(var2.field.sinal, var2.field.expoente, soma_mantissa, "soma");
        printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, soma_mantissa));
    }
}

void subtracao(unionfloat var, unionfloat var2) {
    int exp = var.field.expoente - 127;
    int exp2 = var2.field.expoente - 127;
   
    int desloca;
    unsigned int sub_mantissa;

    unsigned int mantissa_aux = var.field.mantissa + 0x800000;
    unsigned int mantissa_aux2 = var2.field.mantissa + 0x800000;
    if (exp != exp2) {
        if (exp > exp2) {
            desloca = exp - exp2;
            mantissa_aux2 = mantissa_aux2 >> desloca;
            sub_mantissa = mantissa_aux2 - mantissa_aux;
            printBinario(var.field.sinal, var.field.expoente, sub_mantissa, "subtração");
            printNumeroReconstruido(converterBinarioFloat(var.field.sinal, var.field.expoente, sub_mantissa));
        } else {
            desloca = exp2 - exp;
            mantissa_aux = mantissa_aux >> desloca;
            sub_mantissa = mantissa_aux - mantissa_aux2;
            printBinario(var2.field.sinal, var2.field.expoente, sub_mantissa, "subtração");
            printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, sub_mantissa));
        }
    } else {
        sub_mantissa = mantissa_aux - mantissa_aux2;
        printBinario(var2.field.sinal, var2.field.expoente, sub_mantissa, "subtração");
        printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, sub_mantissa));
    }
}

void multiplicacao(unionfloat f1, unionfloat f2) {
    unionfloat result;

    // Obtém os sinais dos números
    result.field.sinal = f1.field.sinal ^ f2.field.sinal;

    // Soma os expoentes e subtrai o bias
    int exponent = f1.field.expoente + f2.field.expoente - 127;

    // Obtém as mantissas dos números e adiciona o bit implícito era uint32_t
    u_int32_t mantissa1 = f1.field.mantissa | 0x800000;
    u_int32_t mantissa2 = f2.field.mantissa | 0x800000;

    // Multiplica as mantissas
    uint64_t mantissa_result = (uint64_t)mantissa1 * (uint64_t)mantissa2;

    // Normaliza a mantissa resultante
    if (mantissa_result & 0x8000000000000000) {
        mantissa_result >>= 1;
        exponent++;
    }

    // Ajusta o expoente do resultado e remove o bit implícito
    result.field.expoente = exponent;
    result.field.mantissa = (mantissa_result >> 23) & 0x7FFFFF;

    printf("Resultado: %f\n", result.f);
}

int main() {
    unionfloat var;
    unionfloat var2;
    int op = -1;
    do {
        printf("Digite:\n'1' para soma.\n'2' para subtracao.\n'3' para multiplicacao.\n'0' para sair.\nOpcao: ");
        scanf("%d", &op);
       
        if (op) {
            printf("\nEntre com um ponto flutuante 1: ");
            scanf("%f",&var.f);
            
            printf("Entre com um ponto flutuante 2: ");
            scanf("%f",&var2.f);
            
            printBinario(var.field.sinal, var.field.expoente, var.field.mantissa, "1");
            printNumeroReconstruido(converterBinarioFloat(var.field.sinal, var.field.expoente, var.field.mantissa + 0x800000));

            printBinario(var2.field.sinal, var2.field.expoente, var2.field.mantissa, "2");
            printNumeroReconstruido(converterBinarioFloat(var2.field.sinal, var2.field.expoente, var2.field.mantissa + 0x800000));

            if (op == 1) {
                soma(var, var2);
            } else if (op == 2) {
                subtracao(var, var2);
            } else if (op == 3) {
                multiplicacao(var, var2);
            }
        }
    } while (op != 0);

    return 0;
}