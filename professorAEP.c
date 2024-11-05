#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 20

void limpaEntrada();
void criptografarASCII(const char *input, char *output);
void descriptografarASCII(const char *input, char *output);
void arquivo(int tp);
void inserir();
void alterar();
void excluir();
void listar(int tipo);
void menu();
int buscaUsuario(const char *email);
int senhaValida(const char *senha);

int totuser = 0;
char emailArmazenado[MAX_USERS][50];
char senhaArmazenada[MAX_USERS][150];

int main() {
    arquivo(0);
    menu();
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
        sprintf(&output[i * 3], "%03d", (unsigned char)input[i]);
    }
    output[i * 3] = '\0';
}

void descriptografarASCII(const char *input, char *output) {
    int i;
    for (i = 0; i < strlen(input); i += 3) {
        char buffer[4];
        strncpy(buffer, &input[i], 3);
        buffer[3] = '\0';
        output[i / 3] = (char)atoi(buffer);
    }
    output[i / 3] = '\0';
}

void arquivo(int tp) {
    char linha[150];
    int x;
    FILE *file = fopen("emailesenha.txt", tp == 0 ? "r" : "w");
    if (file != NULL && tp == 0) {
        totuser = 0;
        x = 0;
        while (fgets(linha, sizeof(linha), file) != NULL) {
            linha[strlen(linha) - 1] = '\0';
            if (x % 2 == 0) {
                strcpy(emailArmazenado[totuser], linha);
            } else {
                strcpy(senhaArmazenada[totuser], linha);
                totuser++;
            }
            x++;
        }
    } else if (file != NULL && tp == 1) {
        for (x = 0; x < totuser; x++) {
            fprintf(file, "%s\n%s\n", emailArmazenado[x], senhaArmazenada[x]);
        }
    }
    fclose(file);
}

void inserir() {
    char email[50], senha[50], senhaConf[50], senhaCript[150];
    int index;

    limpaEntrada();
    printf("Digite o email do usuário: ");
    scanf("%s", email);

    if (buscaUsuario(email) != -1) {
        printf("Usuário já cadastrado.\n");
        return;
    }

    do {
        limpaEntrada();
        printf("Digite a senha: ");
        scanf("%s", senha);
        printf("Confirme a senha: ");
        scanf("%s", senhaConf);
    } while (strcmp(senha, senhaConf) != 0 || !senhaValida(senha));

    criptografarASCII(senha, senhaCript);
    strcpy(emailArmazenado[totuser], email);
    strcpy(senhaArmazenada[totuser], senhaCript);
    totuser++;

    arquivo(1);
    printf("Usuário cadastrado com sucesso!\n");
}

void alterar() {
    char email[50], novaSenha[50], senhaConf[50], senhaCript[150];
    int index;

    limpaEntrada();
    printf("Digite o email do usuário a alterar: ");
    scanf("%s", email);

    index = buscaUsuario(email);
    if (index == -1) {
        printf("Usuário não encontrado.\n");
        return;
    }

    do {
        limpaEntrada();
        printf("Digite a nova senha: ");
        scanf("%s", novaSenha);
        printf("Confirme a nova senha: ");
        scanf("%s", senhaConf);
    } while (strcmp(novaSenha, senhaConf) != 0 || !senhaValida(novaSenha));

    criptografarASCII(novaSenha, senhaCript);
    strcpy(senhaArmazenada[index], senhaCript);

    arquivo(1);
    printf("Senha alterada com sucesso!\n");
}

void excluir() {
    char email[50];
    int index;

    limpaEntrada();
    printf("Digite o email do usuário a excluir: ");
    scanf("%s", email);

    index = buscaUsuario(email);
    if (index == -1) {
        printf("Usuário não encontrado.\n");
        return;
    }

    for (int i = index; i < totuser - 1; i++) {
        strcpy(emailArmazenado[i], emailArmazenado[i + 1]);
        strcpy(senhaArmazenada[i], senhaArmazenada[i + 1]);
    }
    totuser--;

    arquivo(1);
    printf("Usuário excluído com sucesso!\n");
}

void listar(int tipo) {
    printf("---------------------------------------------------------\n");
    printf("Usuário\t\tSenha\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < totuser; i++) {
        if (tipo == 0) {
            printf("%s\n", emailArmazenado[i]);
        } else if (tipo == 1) {
            char senhaDescript[50];
            descriptografarASCII(senhaArmazenada[i], senhaDescript);
            printf("%s\t%s\n", emailArmazenado[i], senhaDescript);
        } else if (tipo == 2) {
            printf("%s\t%s\n", emailArmazenado[i], senhaArmazenada[i]);
        }
    }
    printf("---------------------------------------------------------\n");
}

void menu() {
    char op;
    do {
        limpaEntrada();
        printf("\nMENU\n");
        printf("1 - Inserir\n");
        printf("2 - Alterar\n");
        printf("3 - Excluir\n");
        printf("4 - Listar Usuários\n");
        printf("5 - Listar com Senhas Visíveis\n");
        printf("6 - Listar com Senhas Criptografadas\n");
        printf("7 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%c", &op);

        switch (op) {
            case '1':
                inserir();
                break;
            case '2':
                alterar();
                break;
            case '3':
                excluir();
                break;
            case '4':
                listar(0);
                break;
            case '5':
                listar(1);
                break;
            case '6':
                listar(2);
                break;
        }
    } while (op != '7');
}

int buscaUsuario(const char *email) {
    for (int i = 0; i < totuser; i++) {
        if (strcmp(emailArmazenado[i], email) == 0) {
            return i;
        }
    }
    return -1;
}

int senhaValida(const char *senha) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    if (strlen(senha) < 8 || strlen(senha) > 20) return 0;

    for (int i = 0; senha[i]; i++) {
        if (isupper(senha[i])) hasUpper = 1;
        else if (islower(senha[i])) hasLower = 1;
        else if (isdigit(senha[i])) hasDigit = 1;
        else hasSpecial = 1;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}
