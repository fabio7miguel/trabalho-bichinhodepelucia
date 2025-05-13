
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FELICIDADE 10
#define MAX_LIMPEZA 10
#define MAX_IDADE 7
#define MAX_FOME 10

typedef struct {
    char nome[50];
    int idade_dias;
    int horas_vivas;
    int felicidade;
    int limpeza;
    int fome;
    int doente;
    int horas_doente;
    int vivo;
} Pet;

void mostrar_status(Pet *pet) {
    printf("\n=== STATUS DE %s ===\n", pet->nome);
    printf("Idade: %d dias\n", pet->idade_dias);
    printf("Felicidade: %d\n", pet->felicidade);
    printf("Limpeza: %d\n", pet->limpeza);
    printf("Fome: %d\n", pet->fome);
    printf("Doente: %s\n", pet->doente ? "Sim" : "Não");
    printf("===========================\n\n");
}

void verificar_estado(Pet *pet) {
    if (pet->fome >= MAX_FOME) {
        printf("\n%s morreu de fome! \n", pet->nome);
        pet->vivo = 0;
    } else if (pet->felicidade <= 0) {
        printf("\n%s morreu de tristeza! \n", pet->nome);
        pet->vivo = 0;
    } else if (pet->limpeza <= 0) {
        printf("\n%s morreu por sujeira! \n", pet->nome);
        pet->vivo = 0;
    } else if (pet->idade_dias >= MAX_IDADE) {
        printf("\n%s viveu até o fim muito bem e com felicidade!  Você venceu o jogo!\n", pet->nome);
        pet->vivo = 0;
    }
}

void avancar_tempo(Pet *pet) {
    pet->horas_vivas += 8;
    pet->fome += 2;
    if (pet->fome > MAX_FOME) pet->fome = MAX_FOME;
    pet->limpeza -= 2;
    if (pet->limpeza < 0) pet->limpeza = 0;

    if (pet->horas_vivas % 24 == 0) {
        pet->idade_dias++;
    }

    // Doença aleatória
    if (!pet->doente && rand() % 10 < 2) {
        pet->doente = 1;
        pet->horas_doente = 0;
        printf("\n⚠️ %s ficou doente!\n", pet->nome);
    }

    // Tentativa de cura
    if (pet->doente) {
        pet->horas_doente += 8;
        float chance = 0.0;
        if (pet->horas_doente >= 24) {
            chance = 1.0;
        } else if (pet->horas_doente >= 16) {
            chance = 0.666;
        } else if (pet->horas_doente >= 8) {
            chance = 0.333;
        }

        if ((float)rand() / RAND_MAX < chance) {
            printf("\n✨ %s se curou da doença!\n", pet->nome);
            pet->doente = 0;
            pet->horas_doente = 0;
        }
    }

    verificar_estado(pet);
}

void alimentar(Pet *pet) {
    if (pet->fome == 0) {
        printf("\n%s não está com fome e ficou triste por ser forçado a comer.\n", pet->nome);
        pet->felicidade -= 2;
        if (pet->felicidade < 0) pet->felicidade = 0;
    } else {
        printf("\nVocê alimentou %s.\n", pet->nome);
        pet->fome -= 4;
        if (pet->fome < 0) pet->fome = 0;
        pet->felicidade++;
        if (pet->felicidade > MAX_FELICIDADE) pet->felicidade = MAX_FELICIDADE;
    }
    verificar_estado(pet);
}

void jogar(Pet *pet) {
    int escolha_usuario, escolha_pet;
    printf("\nVamos jogar pedra (0), papel (1) ou tesoura (2): ");
    scanf("%d", &escolha_usuario);
    escolha_pet = rand() % 3;

    printf("%s escolheu: %d | Você escolheu: %d\n", pet->nome, escolha_pet, escolha_usuario);

    if (escolha_usuario == escolha_pet) {
        printf("Empate!\n");
        pet->felicidade += 3;
    } else if ((escolha_usuario == 0 && escolha_pet == 2) ||
               (escolha_usuario == 1 && escolha_pet == 0) ||
               (escolha_usuario == 2 && escolha_pet == 1)) {
        printf("Você venceu!\n");
        pet->felicidade += 3;
    } else {
        printf("%s venceu!\n", pet->nome);
        pet->felicidade += 5;
    }

    if (pet->felicidade > MAX_FELICIDADE) pet->felicidade = MAX_FELICIDADE;
}

void dar_banho(Pet *pet) {
    if (pet->limpeza == MAX_LIMPEZA) {
        printf("\n%s já está limpo e ficou triste por tomar banho à toa.\n", pet->nome);
        pet->felicidade -= 6;
        if (pet->felicidade < 0) pet->felicidade = 0;
    } else {
        printf("\nVocê deu banho em %s. Agora ele está limpinho!\n", pet->nome);
        pet->limpeza = MAX_LIMPEZA;
        pet->felicidade++;
        if (pet->felicidade > MAX_FELICIDADE) pet->felicidade = MAX_FELICIDADE;
    }
    verificar_estado(pet);
}

int main() {
    srand(time(NULL));

    Pet pet;
    pet.idade_dias = 0;
    pet.horas_vivas = 0;
    pet.felicidade = 5;
    pet.limpeza = 10;
    pet.fome = 0;
    pet.doente = 0;
    pet.horas_doente = 0;
    pet.vivo = 1;

    printf("Dê um nome ao seu bichinho virtual: ");
    fgets(pet.nome, 50, stdin);
    pet.nome[strcspn(pet.nome, "\n")] = 0;

    int opcao;
    while (pet.vivo) {
        printf("\n=== Menu Tamagotchi (%s) ===\n", pet.nome);
        printf("1. Avançar o tempo (8h)\n");
        printf("2. Alimentar\n");
        printf("3. Jogar\n");
        printf("4. Dar banho\n");
        printf("5. Ver status\n");
        printf("6. Desligar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: avancar_tempo(&pet); break;
            case 2: alimentar(&pet); break;
            case 3: jogar(&pet); break;
            case 4: dar_banho(&pet); break;
            case 5: mostrar_status(&pet); break;
            case 6: pet.vivo = 0; break;
            default: printf("Opção inválida!\n");
        }

        // ALERTAS
        if (pet.vivo) {
            if (pet.fome >= 8) printf("⚠️ %s está com muita fome!\n", pet.nome);
            if (pet.limpeza <= 2) printf("⚠️ %s está muito sujo!\n", pet.nome);
            if (pet.felicidade <= 2) printf("⚠️ %s está muito triste!\n", pet.nome);
        }
    }

    printf("\nJogo encerrado. %s viveu %d dias.\n", pet.nome, pet.idade_dias);
    return 0;
}
