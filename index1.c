#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

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
    fwrite(&p.primeironome, sizeof(char), 20, dados);

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
}

FILE * criarIndicePri(FILE *dados)
{
  FILE *indicepri;
  int i=0;
  int pos=0;

  indicepri = fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.txt", "w");

  fprintf(indicepri, "%d", quant);

  for(i=0;i<quant;i++)
  {
    pos=i*130;
    fseek(dados, pos, SEEK_SET);
    fread(&p.chave, sizeof(char), 4, dados);

    //RRN
    fwrite(&i, sizeof(int), 1, indicepri); //fprintf?
    fwrite(&p.chave, sizeof(char), 4, indicepri);
  }
  fclose(indicepri);

  return indicepri;
}

FILE * criarIndiceSec(FILE *dados)
{
  FILE *indicesec;
  int i=0;
  int pos=0;

  indicesec = fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.txt", "w");

  fwrite(&quant, sizeof(int), 1, indicesec);//here

  for(i=0;i<quant;i++)
  {
    pos=i*130;
    fseek(dados, pos, SEEK_SET);
    fread(&p.chave, sizeof(char), 4, dados);

    pos=pos+64;
    fseek(dados, pos, SEEK_SET);
    fread(&p.cidade, sizeof(char), 20, dados);

    fwrite(&p.cidade, sizeof(char), 20, indicesec);//here
    fwrite(&p.chave, sizeof(char), 4, indicesec);//here
  }
  fclose(indicesec);

  return indicesec;
}

//new code
char * loadIndicePri(FILE *indicepri, int *nroRec)
{
  nroRec=0;
  int i=0;
  int pos=0;
  
  fseek(indicepri, 0, SEEK_SET);
  fscanf(indicepri, "%d", nroRec);
  
  char *Index1 = (char*) malloc(nroRec * (sizeof(int) + 4 * sizeof(char)));
  
  for(i=0;i<nroRec;i++)
  {
    pos=i*8;
    fread(Index1[pos], sizeof(int), 1, indicepri);
    pos=pos+4;
    fread(Index1[pos], sizeof(char), 4, indicepri);
  }
  
  return Index1;
}

char * loadIndiceSec(FILE *indicesec, int *nroRec)
{
  nroRec=0;
  int i=0;
  int pos=0;
  
  fseek(indicesec, 0, SEEK_SET);
  fscanf(indicesec, "%d", nroRec);
  
  char *Index2 = (char*) malloc(nroRec * (20 * sizeof(char) + 4 * sizeof(char)));
  
  for(i=0;i<nroRec;i++)
  {
    pos=i*24;
    fread(Index2[pos], sizeof(char), 20, indicesec);
    pos=pos+20;
    fread(Index2[pos], sizeof(char), 4, indicesec);
  }
  
  return Index2;
}

int buscaChavePri(char *Index1, int nroRec, char chaveBus[4])
{
  int i=0;
  int pos=0;
  char chaveEnc[4];
  
  for(i=0;i<nroRec;i++)
  {
    pos=8;
    chaveEnc=Index1[pos];
    if(chaveEnc==chaveBus)
      return true;
  }
  
  return false;
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
    // Carregar arquivos de indice em memoria primaria
    
    char chave[4];
    char *Index1;
    char *Index2;
    char escolha;
    int nroRec1;
    int nroRec2;
    int boolean;
  
    nroRec1=0;
    nroRec2=0;
    boolean = false;
    escolha="s";
    
    indicepri=fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.txt", "r+");
    indicesec=fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.txt", "r+");
    
    Index1=loadIndicePri(indicepri, &nroRec1);
    Index2=loadIndiceSec(indeicesec, &nroRec2);
    
    printf("Indice primario: %s\n", Index1); 
    printf("Indice secundario: %s\n", Index2); 
    
    // 4) Busca por valor do campo chave
    
    while(escolha=="s")
    printf("Buscar chave? (s/n)\n");
    scanf("%c", &escolha);
    
    printf("Digite um valor existente: ");
    scanf("%4s", &chave);
    
    boolean=buscaChavePri(Index1, nroRec1, chave);
    
    if(boolean==0)
      printf("Chave inexistente\n");
    if(boolean==1)
      printf("Chave existente\n");

  
  }
  return 0;
}
