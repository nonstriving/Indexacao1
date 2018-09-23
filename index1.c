#include <stdio.h>
#include <stdlib.h>

int quant=0;

typedef struct
{
  char chave[4];
  char ultimonome[20];
  char primeironome[20];
  char endereco[20];
  char cidade[20];
  char estado[20];
  char cep[11];
  char telefone[15];
} pessoa;

pessoa p = {.chave[0]='\0',
            .ultimonome[0]='\0',
            .primeironome[0]='\0',
            .endereco[0]='\0',
            .cidade[0]='\0',
            .estado[0]='\0',
            .cep[0]='\0',
            .telefone[0]='\0'};

void lerdados(FILE* dados)
{
  printf("Nro de registros: ");
  scanf("%d", &quant);

  int i;
  char n = '\0';
  for(i = 0; i < quant; i++)
  {
    printf("Pessoa # %d \n \n", i + 1);

    int c;
    do
    {
      c = getchar();
    }
    while(c != EOF && c != '\n');

    printf("Nro de identificacao: ");
    fgets(p.chave, 4, stdin);
    fwrite(&p.chave, sizeof(char), 4, dados);

    printf("Sobrenome: ");
    fgets(p.ultimonome, 20, stdin);
    fwrite(&p.ultimonome, sizeof(char), 20, dados);

    printf("Primeiro nome: ");
    fgets(p.primeironome, 20, stdin);
    fwrite(&p.primeironome, sizeof(p.primeironome), 20, dados);

    printf("Endereco: ");
    fgets(p.endereco, 20, stdin);
    fwrite(&p.endereco, sizeof(char), 20, dados);

    printf("Cidade: ");
    fgets(p.cidade, 20, stdin);
    fwrite(&p.cidade, sizeof(char), 20, dados);

    printf("Estado: ");
    fgets(p.estado, 20, stdin);
    fwrite(&p.estado, sizeof(char), 20, dados);

    printf("CEP: ");
    fgets(p.cep, 11, stdin);
    fwrite(&p.cep, sizeof(char), 11, dados);

    printf("Telefone: ");
    fgets(p.telefone, 15, stdin);
    fwrite(&p.telefone, sizeof(char), 15, dados);

    printf("\n");
  }
  fwrite(&n, sizeof(char), 1, dados);
}

FILE * criarIndicePri(FILE *dados)
{
  FILE *indicepri;
  int i=0;
  int pos=0;
  int read;

  indicepri = fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.txt", "w");

  fwrite(&quant, sizeof(int), 1, indicepri);
  while((read=fgetc(dados)) != EOF)
  {
    pos=i*130;
    fseek(dados, pos, SEEK_SET);
    fread(&p.chave, sizeof(char), 4, dados);

    //RRN
    fwrite(&i, sizeof(int), 1, indicepri);
    fwrite(&p.chave, sizeof(char), 4, indicepri);
    i++;
  }

  return indicepri;
}

FILE * criarIndiceSec(FILE *dados)
{
  FILE *indicesec;
  int i=0;
  int pos=0;
  int read;

  indicesec = fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.txt", "r+");

  fwrite(&quant, sizeof(int), 1, indicesec);

  while((read=fgetc(dados)) != EOF)
  {
    pos=i*130;
    fseek(dados, pos, SEEK_SET);
    fread(&p.chave, sizeof(char), 4, dados);

    pos=pos+64;
    fseek(dados, pos, SEEK_SET);
    fread(&p.cidade, sizeof(char), 20, dados);

    fwrite(&p.cidade, sizeof(char), 20, indicesec);
    fwrite(&p.chave, sizeof(char), 4, indicesec);

    i++;
  }

  return indicesec;
}

int main()
{

  FILE *dados;
  FILE *indicepri;
  FILE *indicesec;

  if (fopen("/Users/samara/Documents/ORI/Indexacao1/dados.txt", "r+") == NULL ||
      fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.txt", "r+") == NULL ||
      fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.txt", "r+") == NULL)
  {
    dados = fopen("/Users/samara/Documents/ORI/Indexacao1/dados.txt", "w");

    // Questao 1
    lerdados(dados);

    // Questao 2

    // Criar indice primario
    indicepri = criarIndicePri(dados);

    // Criar indice secundario
    indicesec = criarIndiceSec(dados);

    fclose(dados);
    fclose(indicepri);
    fclose(indicesec);
  }
  else
  {
    // Arquivos de indice ja existem
    // Carregar arquivo de indice primario em memoria primaria


    // Arquivo de indice primario
    indicepri = fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.txt", "r+");
    char* indice1 = (char*) malloc(quant * (sizeof(p.chave) + 2 * sizeof(char)));

    char c;

    int i = 0;

    while(1) {
      c = fgetc(indicepri);
      if( feof(indicepri) ) {
        break ;
      }
      indice1[i++] = c;
      printf("%c", c);
    }

    i = i - 1;
    indice1[i] = '\0';

    fclose(indicepri);

    // Arquivo de indice secundario
    indicesec = fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.txt", "r+");
    char* indice2 = (char*) malloc(quant * (sizeof(p.cidade) + sizeof(p.chave)));

    i = 0;

    while(1) {
      c = fgetc(indicesec);
      if( feof(indicesec) ) {
        break ;
      }
      indice2[i++] = c;
      printf("%c", c);
    }

    i = i - 1;
    indice2[i] = '\0';

    fclose(indicesec);

    // 4) Busca pelo valor existente do campo pessoa.chave
    // 5) Busca pelo campo pessoa.chave com valor nao existente

    while(1)
    {
      int valor;
      int valor2;

      printf("Busca por valor do campo pessoa.chave:\n");
      scanf("%d", &valor);

      // Busca no vetor do arquivo de indice primario

      int existe = 0;

      for(int i = 0; i < quant; i++)
      {
        p.chave[0] = indice1[i * 6];
        p.chave[1] = indice1[i * 6 + 1];
        p.chave[2] = indice1[i * 6 + 2];
        p.chave[3] = indice1[i * 6 + 3];

        valor2 = p.chave[0] - '0';

        if(valor == valor2)
        {
          valor = i;
          existe = 1;
        }
      }

      if(existe == 0)
      {
        printf("Valor nao existe\n");
      }
      else
      {
        dados = fopen("/Users/samara/Documents/ORI/Indexacao1/dados.txt", "r");
        fseek(dados, valor * 130, SEEK_SET);
        fread(&p.chave, sizeof(p.chave), 1, dados);
        fread(&p.ultimonome, sizeof(p.ultimonome), 1, dados);
        fread(&p.primeironome, sizeof(p.primeironome), 1, dados);
        fread(&p.endereco, sizeof(p.endereco), 1, dados);
        fread(&p.cidade, sizeof(p.cidade), 1, dados);
        fread(&p.estado, sizeof(p.estado), 1, dados);
        fread(&p.cep, sizeof(p.cep), 1, dados);
        fread(&p.telefone, sizeof(p.telefone), 1, dados);

        printf("%4s", p.chave);
        printf("%20s", p.ultimonome);
        printf("%20s", p.primeironome);
        printf("%20s", p.endereco);
        printf("%20s", p.cidade);
        printf("%20s", p.estado);
        printf("%11s", p.cep);
        printf("%15s", p.telefone);
      }
    }
  }
  return 0;
}
