#include <stdio.h>
#include <string.h>

// Definição da struct para representar um território
// Agrupa informações relacionadas: nome, cor do exército e quantidade de tropas
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Declaração de um vetor de structs com capacidade para 5 territórios
    Territorio territorios[5];
    
    printf("=== CADASTRO DE TERRITÓRIOS ===\n\n");

    // Laço de repetição para entrada de dados dos 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("--- Território %d ---\n", i + 1);
        
        printf("Digite o nome do território: ");
        scanf("%29s", territorios[i].nome); // Limita a leitura para evitar estouro de buffer

        printf("Digite a cor do exército: ");
        scanf("%9s", territorios[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        
        printf("\n");
    }

    printf("=== DADOS DOS TERRITÓRIOS CADASTRADOS ===\n\n");

    // Laço de repetição para exibição imediata dos dados cadastrados
    for (int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exército: %s\n", territorios[i].cor);
        printf("  Quantidade de Tropas: %d\n", territorios[i].tropas);
        printf("----------------------------------------\n");
    }

    return 0;
}