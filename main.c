#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void limpaEntrada();
void criptografarASCII(const char *input, char *output);
void descriptografarASCII(const char *input, char *output);

int main() {
    char acao;
    char email[50];
    char senha[50];
    char senhaConf[50];
    char senhaCript[150]; // 3x o tamanho da senha original
    char senhaArmazenada[150];
    char senhaDescript[50];
    char emailArmazenado[50];
    int conferencia;
    int contaCriada = 0; 

    do {
        // Visualiza qual ação o usuário deseja executar
        do {
            limpaEntrada();
            printf("Escolha uma ação entre:\nCriar Conta - C\nEntrar - E\nSair - S\n");
            scanf("%c", &acao);
        } while ((toupper(acao) != 'C') && (toupper(acao) != 'E') && (toupper(acao) != 'S'));

        if (toupper(acao) == 'C')  {
            // Pede o email e confere se já está presente no "BD"
            limpaEntrada();
            printf("Digite o seu email: ");
            scanf("%s", email);
            if (contaCriada && strcmp(email, emailArmazenado) == 0) {
                printf("O email digitado já está cadastrado no sistema!\n");
                continue; 
            }
            
            // Pede para o usuário criar a senha duas vezes para conferência
            limpaEntrada();
            printf("Digite a sua senha: ");
            scanf("%s", senha);

            limpaEntrada();
            printf("Digite a sua senha novamente: ");
            scanf("%s", senhaConf);
            conferencia = strcmp(senha, senhaConf);
            if (conferencia != 0) {
                printf("Senha digitada incorretamente!\n");
                continue;
            } else {
                // Faz a criptografia da senha
                criptografarASCII(senha, senhaCript);

                // Armazena email e senha criptografada
                strcpy(emailArmazenado, email);
                strcpy(senhaArmazenada, senhaCript);
                contaCriada = 1;

                printf("Cadastro criado com sucesso!!!\n");
                printf("Senha criptografada = %s\n", senhaCript);
                printf("Digite 'E' se deseja acessar sua conta.\n");
            }
        }

        if (toupper(acao) == 'E') {
            if (!contaCriada) {
                printf("Nenhuma conta foi criada ainda. Crie uma conta primeiro.\n");
                continue;
            }

            limpaEntrada();
            printf("Digite o seu email: ");
            scanf("%s", email);
            limpaEntrada();
            printf("Digite a sua senha: ");
            scanf("%s", senha);

            // Criptografa a senha para comparar com a armazenada
            criptografarASCII(senha, senhaCript);

            if (strcmp(email, emailArmazenado) == 0 && strcmp(senhaCript, senhaArmazenada) == 0) {
                printf("Login realizado com sucesso!\n");

                // Descriptografa a senha armazenada para mostrar (se necessário)
                descriptografarASCII(senhaArmazenada, senhaDescript);
                printf("Senha armazenada (descriptografada) = %s\n", senhaDescript);
            } else {
                printf("Email ou senha incorretos.\n");
            }
        }

    } while (toupper(acao) != 'S');

    printf("Programa encerrado.\n");

    return 0;
}

void limpaEntrada() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void criptografarASCII(const char *input, char *output) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        sprintf(&output[i * 3], "%03d", (unsigned char)input[i]); // Converte cada caractere para 3 dígitos ASCII
    }
    output[i * 3] = '\0'; // Finaliza a string
}

void descriptografarASCII(const char *input, char *output) {
    int i;
    for (i = 0; i < strlen(input); i += 3) {
        char buffer[4];
        strncpy(buffer, &input[i], 3);
        buffer[3] = '\0';
        output[i / 3] = (char)atoi(buffer); // Converte de volta para caractere
    }
    output[i / 3] = '\0'; // Finaliza a string
}
