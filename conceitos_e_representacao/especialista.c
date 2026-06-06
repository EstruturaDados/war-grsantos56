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
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void liberarMemoria(Territorio* mapa, char* missao);

int main() {
    srand(time(NULL));

    // Pool de missões estratégicas pré-definidas
    const char* poolMissoes[] = {
        "Conquistar 3 territorios",
        "Conquistar 4 territorios",
        "Eliminar todas as tropas inimigas",
        "Dominar o mapa completo",
        "Alcancas um total de 10 tropas"
    };
    int totalMissoes = 5;

    int qtdTerritorios = 0;
    Territorio* mapa = alocarEInteragir(&qtdTerritorios);

    if (mapa == NULL) {
        printf("Erro ao inicializar o mapa.\n");
        return 1;
    }

    // Alocação dinâmica para a missão do jogador (passagem por referência na atribuição)
    char* missaoJogador = (char*) malloc(100 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memória para a missão.\n");
        free(mapa);
        return 1;
    }

    // Define a cor do jogador humano com base no primeiro território cadastrado para fins de validação da missão
    char corJogador[10];
    strcpy(corJogador, mapa[0].cor);

    // Sorteio e atribuição da missão
    atribuirMissao(missaoJogador, poolMissoes, totalMissoes);

    printf("\n========================================\n");
    printf("SUA MISSÃO SECRETA: %s\n", missaoJogador);
    printf("========================================\n");

    int turno = 1;
    int jogoAtivo = 1;

    while (jogoAtivo) {
        printf("\n========================================\n");
        printf("         SITUAÇÃO DO MAPA - TURNO %d     \n", turno);
        printf("========================================\n");
        exibirMapa(mapa, qtdTerritorios);

        printf("\nDeseja realizar um ataque? (1 - Sim / 0 - Passar Turno/Sair): ");
        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) {
            realizarAtaque(mapa, qtdTerritorios);
        } else {
            printf("Turno finalizado.\n");
        }

        // Verificação silenciosa da condição de vitória ao final do turno
        if (verificarMissao(missaoJogador, mapa, qtdTerritorios, corJogador)) {
            printf("\n========================================\n");
            printf("🎉 PARABÉNS! Você cumpriu sua missão: '%s' e VENCEU O JOGO! 🎉\n", missaoJogador);
            printf("========================================\n");
            jogoAtivo = 0;
        } else if (opcao == 0) {
            // Se escolheu sair e não ganhou, encerra o loop
            jogoAtivo = 0;
        }
        
        turno++;
    }

    // Liberação de toda a memória alocada dinamicamente
    liberarMemoria(mapa, missaoJogador);
    printf("\nMemória liberada. Jogo encerrado!\n");

    return 0;
}

// Alocação dinâmica do mapa e cadastro
Territorio* alocarEInteragir(int* quantidade) {
    printf("Digite a quantidade de territórios para o jogo: ");
    scanf("%d", quantidade);

    if (*quantidade <= 0) return NULL;

    Territorio* mapa = (Territorio*) calloc(*quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    printf("\n=== CADASTRO DOS TERRITÓRIOS ===\n");
    for (int i = 0; i < *quantidade; i++) {
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

// Exibição do mapa utilizando ponteiros
void exibirMapa(Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] %s | Exército: %s | Tropas: %d\n", 
               i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
    }
}

// Lógica de ataque e manipulação de dados por ponteiros
void realizarAtaque(Territorio* mapa, int quantidade) {
    int idxAtacante, idxDefensor;

    printf("\nEscolha o número do território ATACANTE (1 a %d): ", quantidade);
    scanf("%d", &idxAtacante);
    printf("Escolha o número do território DEFENSOR (1 a %d): ", quantidade);
    scanf("%d", &idxDefensor);

    idxAtacante--;
    idxDefensor--;

    if (idxAtacante < 0 || idxAtacante >= quantidade || idxDefensor < 0 || idxDefensor >= quantidade) {
        printf("\n[ERRO] Índices inválidos!\n");
        return;
    }

    Territorio* atacante = mapa + idxAtacante;
    Territorio* defensor = mapa + idxDefensor;

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Ataque inválido! Você não pode atacar a sua própria cor.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("\n[ERRO] O território atacante precisa de pelo menos 2 tropas para atacar.\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- CONFRONTO ---\n");
    printf("%s rola o dado: %d\n", atacante->nome, dadoAtacante);
    printf("%s rola o dado: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n** Vitória do ATACANTE (%s)! **\n", atacante->nome);
        
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;

        printf("O território %s agora é controlado pela cor %s com %d tropas!\n", 
               defensor->nome, defensor->cor, defensor->tropas);
    } else {
        printf("\n** Vitória do DEFENSOR (%s)! Defesa bem-sucedida. **\n", defensor->nome);
        atacante->tropas--;
    }
}

// Atribui uma missão aleatória copiando para o ponteiro de destino (passagem por referência)
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

// Analisa os objetivos com base na situação do mapa e cor do jogador
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    int territoriosControlados = 0;
    int totalTropasJogador = 0;
    int tropasInimigas = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp((mapa + i)->cor, corJogador) == 0) {
            territoriosControlados++;
            totalTropasJogador += (mapa + i)->tropas;
        } else {
            tropasInimigas += (mapa + i)->tropas;
        }
    }

    // Lógicas para validação dinâmica de cada string de missão
    if (strcmp(missao, "Conquistar 3 territorios") == 0 && territoriosControlados >= 3) return 1;
    if (strcmp(missao, "Conquistar 4 territorios") == 0 && territoriosControlados >= 4) return 1;
    if (strcmp(missao, "Eliminar todas as tropas inimigas") == 0 && tropasInimigas == 0) return 1;
    if (strcmp(missao, "Dominar o mapa completo") == 0 && territoriosControlados == tamanho) return 1;
    if (strcmp(missao, "Alcancas um total de 10 tropas") == 0 && totalTropasJogador >= 10) return 1;

    return 0; // Condição de vitória ainda não alcançada
}

// Liberação segura de todas as alocações do sistema
void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa != NULL) free(mapa);
    if (missao != NULL) free(missao);
}