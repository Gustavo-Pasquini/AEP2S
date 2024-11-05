#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void limpaEntrada();
void criptografarASCII(const char *input, char *output);
void descriptografarASCII(const char *input, char *output);
void arquivo(int tp);

int totuser=0;
char emailArmazenado[15][50];
char senhaArmazenada[15][150];


int main() {

    arquivo(0);

    printf("%s", emailArmazenado[0]);

    char acao;
    char email[50];
    char senha[50];
    char senhaConf[50];
    char senhaCript[150]; // 3x o tamanho da senha original
    char senhaDescript[15][50];
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
            for (int i = 0; i < totuser; i++){
                if (contaCriada && strcmp(email, emailArmazenado[i]) == 0) {
                    printf("O email digitado já está cadastrado no sistema!\n");
                    continue; 
                }
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

void arquivo(int tp){ //0 - leitura / 1 - gravacao
  char linha[15],tmp[15]; 
  int x;
  FILE *file; //associa vari�vel file em tipo FILE (palavra reservada que caracteriza fim de string)
    file = fopen("emailesenha.txt", "r"); //Associa file em "emailesenha.txt" e tenta abrir arquivo para leitura
    if (file != NULL && tp == 0) { //se n�o deu erro na abertura e o TIPO for igual a 0
        totuser=0;
        x=0;
        while (fgets(linha, sizeof(linha), file) != NULL && strlen(linha)>0) { 
          linha[strlen(linha)-1]='\0'; //coloca caracter null no fim da string (caracteriza fim)
            if (x%2==1){ //linha impar
              totuser++; //incrementa o total de usuarios no vetor
              //aplicar o procedimento ou func�o que descritografa se quiser
              strcpy(emailArmazenado[totuser-1],linha); //copia linha em nome[]
              }
            else //linha par
              //aplicar o procedimento ou func�o que descritografa se quiser
              strcpy(senhaArmazenada[totuser-1],linha); //copia linha em senha[]
              x++; //incrementa x. Cada vez que passar pela leitura da linha, ser� lido ou nome ou senha (linha impar / linha par)
        }
    }
    else{ //caso nao tenha aberto o arquivo para leitura ou o tipo seja diferente de zero
      file = fopen("NomeArq.txt", "w"); //Associa file em "NomeArq.txt" e cria o arquivo (mesmo que ele j� exista - sobrescrevendo-o)
        for (x=0;x<totuser;x++){ //la�o de 0 at� total de usuarios no Vetor-1
          strcpy(tmp,emailArmazenado[x]); //copia o nome[x] em tmp
          //aplicar a criptografa tmp (nome) se quiser
          fprintf(file, "%s\n", tmp); //grava no arquivo
          strcpy(tmp,senhaArmazenada[x]); //copia o nome[x] em tmp 
          //aplicar a criptografa tmp (senha) se quiser
          fprintf(file, "%s\n", tmp); //grava no arquivo o conte�do de tmp
        } 
   }
    fclose(file); //fecha arquivo
}