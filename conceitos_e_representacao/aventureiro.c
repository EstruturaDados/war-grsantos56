#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos das funções
Territorio* alocarEInteragir(int* quantidade);
void exibirMapa(Territorio* mapa, int quantidade);
void realizarAtaque(Territorio* mapa, int quantidade);
void liberarMemoria(Territorio* mapa);

int main() {
    // Inicializa o gerador de números aleatórios com base no tempo atual
    srand(time(NULL));

    int qtdTerritorios = 0;
    // Alocação dinâmica e cadastro via ponteiros
    Territorio* mapa = alocarEInteragir(&qtdTerritorios);

    if (mapa == NULL) {
        printf("Erro ao inicializar o mapa.\n");
        return 1;
    }

    int continuar = 1;
    while (continuar) {
        printf("\n========================================\n");
        printf("         SITUAÇÃO ATUAL DO MAPA         \n");
        printf("========================================\n");
        exibirMapa(mapa, qtdTerritorios);

        printf("\nDeseja realizar um ataque? (1 - Sim / 0 - Sair): ");
        scanf("%d", &continuar);

        if (continuar == 1) {
            realizarAtaque(mapa, qtdTerritorios);
        }
    }

    // Liberação da memória alocada dinamicamente
    liberarMemoria(mapa);
    printf("\nMemória liberada. Jogo encerrado!\n");

    return 0;
}

// Função para alocar o vetor dinamicamente e cadastrar os territórios
Territorio* alocarEInteragir(int* quantidade) {
    printf("Digite a quantidade de territórios para o jogo: ");
    scanf("%d", quantidade);

    if (*quantidade <= 0) return NULL;

    // Utiliza calloc para alocar e já limpar a memória com zeros
    Territorio* mapa = (Territorio*) calloc(*quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    printf("\n=== CADASTRO DOS TERRITÓRIOS ===\n");
    for (int i = 0; i < *quantidade; i++) {
        // Acesso aos elementos utilizando aritmética de ponteiros e desreferenciação
        printf("\nTerritório [%d]:\n", i + 1);
        printf("Nome: ");
        scanf("%29s", (mapa + i)->nome);
        printf("Cor do Exército: ");
        scanf("%9s", (mapa + i)->cor);
        printf("Quantidade de Tropas: ");
        scanf("%d", &(mapa + i)->tropas);
    }
    return mapa;
}

// Função para exibir o estado atual de todos os territórios cadastrados
void exibirMapa(Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] %s | Exército: %s | Tropas: %d\n", 
               i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
    }
}

// Função que gerencia a escolha dos territórios e executa a lógica de batalha
void realizarAtaque(Territorio* mapa, int quantidade) {
    int idxAtacante, idxDefensor;

    printf("\nEscolha o número do território ATACANTE (1 a %d): ", quantidade);
    scanf("%d", &idxAtacante);
    printf("Escolha o número do território DEFENSOR (1 a %d): ", quantidade);
    scanf("%d", &idxDefensor);

    // Ajusta para o índice do vetor (0 a N-1)
    idxAtacante--;
    idxDefensor--;

    // Validações de limites do vetor
    if (idxAtacante < 0 || idxAtacante >= quantidade || idxDefensor < 0 || idxDefensor >= quantidade) {
        printf("\n[ERRO] Índices inválidos!\n");
        return;
    }

    // Criação de ponteiros diretos para os territórios selecionados
    Territorio* atacante = mapa + idxAtacante;
    Territorio* defensor = mapa + idxDefensor;

    // Regra de negócio: Não pode atacar a si mesmo ou aliados da mesma cor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Ataque inválido! Você não pode atacar a sua própria cor.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("\n[ERRO] O território atacante precisa de pelo menos 2 tropas para atacar.\n");
        return;
    }

    // Simulação dos dados de batalha (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- CONFRONTO ---\n");
    printf("%s rola o dado: %d\n", atacante->nome, dadoAtacante);
    printf("%s rola o dado: %d\n", defensor->nome, dadoDefensor);

    // Resolução da batalha utilizando ponteiros para alterar os dados originais
    if (dadoAtacante > dadoDefensor) {
        printf("\n** Vitória do ATACANTE (%s)! **\n", atacante->nome);
        
        // O defensor muda de dono (recebe a cor do atacante)
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas do atacante para o novo território conquistado
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;

        printf("O território %s agora é controlado pela cor %s com %d tropas!\n", 
               defensor->nome, defensor->cor, defensor->tropas);
    } else {
        printf("\n** Vitória do DEFENSOR (%s)! Defesa bem-sucedida. **\n", defensor->nome);
        // Se perder, o atacante perde uma tropa
        atacante->tropas--;
    }
}

// Função responsável por limpar a memória alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}