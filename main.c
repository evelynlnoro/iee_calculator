// Trabalho GB - Evelyn Noro e Mateus Tizotti

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>

typedef union {
	float f;
	struct {
	    unsigned int mantissa: 23;
		unsigned int expoente: 8;
		unsigned int sinal: 1;
	} field;
} unionfloat;

unionfloat multiplicacao(unionfloat n1, unionfloat n2) {
    unionfloat result;

    // Obtém os sinais dos números
    result.field.sinal = n1.field.sinal ^ n2.field.sinal;

    // Soma os expoentes e subtrai o bias
    int exponent = n1.field.expoente + n2.field.expoente - 127;

    // Obtém as mantissas dos números e adiciona o bit implícito
    u_int32_t mantissa1 = n1.field.mantissa | 0x800000;
    u_int32_t mantissa2 = n2.field.mantissa | 0x800000;

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

    return result;
}


/// <summary>
/// Funcao para imprimir na tela os bits de um numero
/// </summary>
/// <param name="n">valor decimal qualquer</param>
/// <param name="i">numero de bits que sera implementado o numero</param>
/// <returns></returns>
void binario(int n, int i)
{
	int k;
	for (i--; i >= 0; i--)
	{
		k = n >> i; // valor de n deslocado em i
		if (k & 1) // 1 e uma mascara, numero 1 em binario 00000001
			printf("1");
		else
			printf("0");	
	}
}

void binario_s(int n, int i, char *s) {
	printf("\n%s\t", s);
	binario(n, i);
}

// Converte o numero no padrao ieee754 para o numero com ponto flutuante normal
float reconstruirNumero(unionfloat numero) {
	return pow(-1, (numero.field.sinal)) * (1.0 + numero.field.mantissa / pow(2, 23)) * pow(2, (numero.field.expoente - 127));
}

// Normalizar o numero a partir do sinal, expoente e mantissa, colocando o primeiro bit como 1
unionfloat normalizaNumero(int sinal, int exp, int mantissa) {
	unionfloat n;
	n.field.sinal = sinal;
	int desloc_exp = 0;
	
	// Se nao for zero (nao faz sentido procurar)
	if (mantissa != 0) {
		// Procura pelo primeiro 1 da mantissa, da esquerda (MSB) para direita (LSB)
		int mascara = 0x80000000;
		for (int i = 31; i >= 0; i--) {
			if ((mantissa & mascara) != 0) {
				desloc_exp = i;
				break;
			}
			mascara = mascara >> 1;
		}
		// Calcula deslocamento em relacao a posicao padrao (23)
		desloc_exp = desloc_exp - 23;
		if (desloc_exp < 0)
			mantissa = mantissa << abs(desloc_exp);
		else 
			mantissa = mantissa >> desloc_exp;
		n.field.expoente = exp + desloc_exp + 127;
	} else {
		n.field.expoente = 0;
	}

	n.field.mantissa = mantissa;

	return n;
}

// Realiza o complemento
void ajustaNegativo(int * mantissa, int * exp) {
	*mantissa = ~(*mantissa) + 1;
}

unionfloat add(unionfloat a, unionfloat b) {
	int exp;
	int exp2;
	int desloca;
	int mantissa_aux;
	int mantissa_aux2;
	int soma_mantissa;

	exp = a.field.expoente - 127; // recupera o expoente limpo (valora armazenado na variavel expoente - bias(127))
	exp2 = b.field.expoente - 127;

	mantissa_aux = a.field.mantissa + 0x800000; // passando para variavel auxiliar e somando 1. (1.mantissa implicito que deve ser adicionado
	mantissa_aux2 = b.field.mantissa + 0x800000; // passando para variavel auxiliar e somando 1.

	// Se for um numero negativo
	if (a.field.sinal == 1) {
		ajustaNegativo(&mantissa_aux, &exp);
	}
	// Se for um numero negativo
	if (b.field.sinal == 1) {
		ajustaNegativo(&mantissa_aux2, &exp2);
	}

	if (exp != exp2) {
		// se os expoentes forem diferentes, tem que pegar o menor e igualar ao maior
		if (exp > exp2) {  // iguala o expoente dos numeros para poder somar (so podemos somar as mantissas se tiverem expoentes iguais) nesse caso exp do numero 1 e maior que o do numero 2
			desloca = exp - exp2; // quantas vezes tem que deslocar essa mantissa
			exp2 = exp;
			mantissa_aux2 = mantissa_aux2 >> desloca; // entao deslocamos a virgula do numero menor (2)
		} else { // iguala o expoente dos numeros para poder somar (so podemos somar as mantissas se tiverem expoentes iguais) nesse caso exp do numero 2 e maior que o do numero 1
			desloca = exp2 - exp; // quantas vezes tem que deslocar essa mantissa
			exp = exp2;
			mantissa_aux = mantissa_aux >> desloca;  // entao deslocamos a virgula do numero menor (1)
		}
	} 

	soma_mantissa = mantissa_aux + mantissa_aux2;

	int sinal = 0;
	// So faz o complemento se o numero for negativo
	if (soma_mantissa < 0) {
		soma_mantissa = ~soma_mantissa;
		sinal = 1;
	}
	
	return normalizaNumero(sinal, exp, soma_mantissa);
}

void exibeNumero(unionfloat n, char * descricao) {
	printf("\n");
	printf("%d ", n.field.sinal);
	binario(n.field.expoente, 8);
	printf(" ");
	binario(n.field.mantissa, 23);
	printf("\n");
	printf("%s reconstituido:  \t%f\n", descricao, reconstruirNumero(n));
	printf("\n");
}

int main() {
    unionfloat var;
    unionfloat var2;
    int op = -1;
    do {
        printf("Digite:\n'1' para soma.\n'2' para subtracao.\n'3' para multiplicacao.\n'0' para sair.\nOpcao: ");
        scanf("%d", &op);
       
        if (op) {
            unionfloat numeroResultadoOperacao;
            printf("\nEntre com um ponto flutuante 1: ");
            scanf("%f",&var.f);
            
            printf("Entre com um ponto flutuante 2: ");
            scanf("%f",&var2.f);
       
            if (op == 1 || op == 2) {
                if (op == 2) 
                    var2.f = - var2.f;
                numeroResultadoOperacao = add(var, var2);
            } else if (op == 3) {
                numeroResultadoOperacao = multiplicacao(var, var2);
            }
            exibeNumero(numeroResultadoOperacao, "Resultado (simulado)");
        }
    } while (op != 0);

    return 0;
}