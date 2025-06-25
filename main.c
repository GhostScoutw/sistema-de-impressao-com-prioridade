
// Autor: Bruno Kissen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../usuarios/usuarios.h"
#include "../impressao/impressao.h"
#include "../fila/fila.h"
#include "../historico/historico.h"

int main() {
    NoUsuario* listaUsuarios = iniListaUsuario();
    NoFila* primeiroFila = iniNoFila();
    NoFila* ultimoFila = iniNoFila();
    NoHistorico* historico = iniNoHistorico();

    int opcao;
    do {
        printf("\n--- MENU SISTEMA DE IMPRESSAO ---\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Cadastrar solicitação de impressão\n");
        printf("3. Executar impressão\n");
        printf("4. Mostrar fila de espera\n");
        printf("5. Mostrar histórico de impressões\n");
        printf("6. Estatísticas\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {   //cadastrar usuario
                char nome[100];
                int cpf, tipo;

                printf("Nome: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("CPF: ");
                scanf("%d", &cpf);

                printf("Tipo (1=Estudante, 2=Professor, 3=Administração/Direção): ");
                scanf("%d", &tipo);
                getchar();

                if (addUsuario(listaUsuarios, nome, cpf, (tipoUsuario)tipo))
                    printf("Usuário adicionado com sucesso.\n");
                else
                    printf("Erro ao adicionar usuário.\n");
                break;
            }

            case 2: {   //cadastrar solicitacao de impressao
                int cpf, paginas;
                printf("CPF do usuário: ");
                scanf("%d", &cpf);
                getchar();

                NoUsuario* usuario = getUsuarioCpf(listaUsuarios, cpf);
                if (!usuario) {
                    printf("Usuário não encontrado.\n");
                    break;
                }

                printf("Número de páginas: ");
                scanf("%d", &paginas);

                Impressao* nova = criarImpressao(usuario, paginas);
                colocarEmFila(nova, primeiroFila, ultimoFila);
                printf("Solicitação adicionada à fila.\n");
                break;
            }

            case 3: { //executar impressao
                realizarImpressao(historico, primeiroFila, ultimoFila);
                break;
            }
                
            case 4: { //mostrar fila de espera
                printFila(primeiroFila, ultimoFila);
                break;
            }
                
            case 5: { //mostrar historico
                printHistorico(historico);
                break;
            }
                
            case 6: {   //mostrar estatísticas
                int totalEstudante = 0, 
                totalProfessor = 0, 
                totalAdmin = 0,

                paginasEstudante = 0, 
                paginasProfessor = 0, 
                paginasAdmin = 0,

                countEstudante = 0, 
                countProfessor = 0, 
                countAdmin = 0;

                NoHistorico* atual = historico->proximo;
                while (atual != NULL) {
                    Impressao* imp = atual->impressao;
                    if (imp && imp->usuario) {

                        switch (imp->usuario->tipoUsuario) {


                            case ESTUDANTE:
                                totalEstudante++;
                                paginasEstudante += imp->numPaginas;
                                countEstudante++;
                                break;

                            case PROFESSOR:
                                totalProfessor++;
                                paginasProfessor += imp->numPaginas;
                                countProfessor++;
                                break;

                            case ADMINISTRACAO:
                                totalAdmin++;
                                paginasAdmin += imp->numPaginas;
                                countAdmin++;
                                break;
                        }
                    }
                    atual = atual->proximo;
                }

                printf("\n--- Estatísticas ---\n");
                printf("Total de impressões:\n");
                printf("Estudantes: %d\n", totalEstudante);
                printf("Professores: %d\n", totalProfessor);
                printf("Administração/Direção: %d\n", totalAdmin);

                printf("\nNúmero total de páginas:\n");
                printf("Estudantes: %d\n", paginasEstudante);
                printf("Professores: %d\n", paginasProfessor);
                printf("Administração/Direção: %d\n", paginasAdmin);

                printf("\nTempo médio estimado por prioridade:\n");
                if (countEstudante > 0)
                    printf("Estudantes: %.2f s\n", (paginasEstudante * 5.0) / countEstudante);
                else
                    printf("Estudantes: N/A\n");
                if (countProfessor > 0)
                    printf("Professores: %.2f s\n", (paginasProfessor * 5.0) / countProfessor);
                else
                    printf("Professores: N/A\n");
                if (countAdmin > 0)
                    printf("Administração/Direção: %.2f s\n", (paginasAdmin * 5.0) / countAdmin);
                else
                    printf("Administração/Direção: N/A\n");

                break;
            }

            case 7:
                printf("Encerrando o sistema...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 7);

    freeListaUsuario(listaUsuarios);
    freeFila(primeiroFila);
    freeFila(ultimoFila);
    freeHistorico(historico);

    return 0;
}
