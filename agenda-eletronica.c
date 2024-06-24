#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura do contato
typedef struct {
    char nome[50];
    char telefone[13];
} Contato;

// Função para exibir o menu
int exibirMenu() {
    int opcao;
    printf("\nEscolha uma opcao:\n");
    printf("1. Consultar Contato\n");
    printf("2. Adicionar Contato\n");
    printf("3. Excluir Contato\n");
    printf("4. Sair\n");
    scanf("%d", &opcao);
    return opcao;
}

// Função para consultar um contato
void consultarContato() {
    char nomeConsulta[50];
    Contato contato;
    FILE *arquivo;

    printf("Digite o nome do contato para consultar: ");
    scanf("%s", nomeConsulta);

    arquivo = fopen("contatos.bin", "rb");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo de contatos.\n");
        return;
    }

  // Procura o contato pelo nome dentro do arquivo
    while (fread(&contato, sizeof(Contato), 1, arquivo)) {
        if (strcmp(contato.nome, nomeConsulta) == 0) {
            printf("Nome: %s\nTelefone: %s\n\n", contato.nome, contato.telefone);
            fclose(arquivo);
            return;
        }
    }
    
    fclose(arquivo);
    printf("\nCONTATO NAO ENCONTRADO.\n");
}

// Função para adicionar um contato
void adicionarContato() {
    Contato novoContato;
    FILE *arquivo;

    arquivo = fopen("contatos.bin", "ab");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo de contatos.\n");
        return;
    }

    printf("Nome: ");
    scanf("%s", novoContato.nome);

    printf("Telefone: ");
    scanf("%s", novoContato.telefone);

  // Escreve o novo contato no arquivo
    fwrite(&novoContato, sizeof(Contato), 1, arquivo);
    fclose(arquivo);

    printf("\nCONTATO ADICIONADO COM SUCESSO.\n");
}

// Função para excluir um contato
void excluirContato() {
    char nomeExcluir[50];
    Contato contato;
    FILE *arquivo, *temporario;

    printf("Digite o nome do contato que deseja excluir: ");
    scanf("%s", nomeExcluir);

    arquivo = fopen("contatos.bin", "rb");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo de contatos.\n");
        return;
    }

    temporario = fopen("temp.bin", "wb");
    if (temporario == NULL) {
        printf("ERRO: Nao foi possivel criar arquivo temporario.\n");
        fclose(arquivo);
        return;
    }

    int encontrado = 0;
    while (fread(&contato, sizeof(Contato), 1, arquivo)) {
        if (strcmp(contato.nome, nomeExcluir) == 0) {
            encontrado = 1;
            printf("Nome: %s\nTelefone: %s\n", contato.nome, contato.telefone);
            int confirmacao;
            printf("Tem certeza que deseja excluir esse contato?\n 1. Sim\n 2. Nao\n");
            scanf("%d", &confirmacao);
            if (confirmacao == 1) {
                printf("\nCONTATO EXCLUIDO.\n");
            } else {
                printf("\nOPERACAO CANCELADA.\n");
                fwrite(&contato, sizeof(Contato), 1, temporario);
            }
        } else {
            fwrite(&contato, sizeof(Contato), 1, temporario);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrado) {
        printf(" \nCONTATO NAO ENCONTRADO.\n");
    }

    remove("contatos.bin"); // Remove o arquivo original
    rename("temp.bin", "contatos.bin"); // Renomeia o arquivo temporário para o nome original
}

// Função principal
int main() {
    int escolha;
// Looping que exibe o menu
    do {
        escolha = exibirMenu();
        switch (escolha) {
            case 1:
                consultarContato();
                break;
            case 2:
                adicionarContato();
                break;
            case 3:
                excluirContato();
                break;
            case 4:
                printf("\nSAINDO...\n");
                break;
            default:
                printf("\nOPCAO INVALIDA.\n");
                break;
        }
    } while (escolha != 4);

    return 0;
}
