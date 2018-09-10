#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *dados;
  FILE *indicepri;
  FILE *indicesec;

  char chave[4];
  char ultimonome[20];
  char primeironome[20];
  char endereco[20];
  char cidade[20];
  char estado[20];
  char cep[11];
  char telefone[15];

  int quant = 2;

  if (fopen("/Users/samara/Documents/ORI/Indexacao1/dados.bin", "rb+") == NULL ||
      fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.bin", "rb+") == NULL ||
      fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.bin", "rb+") == NULL)
  {
    dados = fopen("/Users/samara/Documents/ORI/Indexacao1/dados.bin", "wb");

    indicepri = fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.bin", "wb+");
    indicesec = fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.bin", "wb+");

    if (dados == NULL || indicepri == NULL || indicesec == NULL)
    {
      printf("Erro");
      return 0;
    }

    printf("Nro de registros: ");

    int i;
    for(i = 0; i < quant; i++)
    {
      printf("Pessoa # %d \n \n", i + 1);

      printf("%ld", ftell(dados));

      printf("Nro de identificacao: ");
      //fflush(stdout);
      //getchar();
      fgets(chave, 4, stdin);
      //printf("%s\n",chave);
      fwrite(&chave, sizeof(chave), 1, dados);

      printf("Sobrenome: ");
      fgets(ultimonome, 20, stdin);
      fwrite(&ultimonome, sizeof(ultimonome), 1, dados);

      printf("Primeiro nome: ");
      fgets(primeironome, 20, stdin);
      fwrite(&primeironome, sizeof(primeironome), 1, dados);

      printf("Endereco: ");
      fgets(endereco, 20, stdin);
      fwrite(&endereco, sizeof(endereco), 1, dados);

      printf("Cidade: ");
      fgets(cidade, 20, stdin);
      fwrite(&cidade, sizeof(cidade), 1, dados);

      printf("Estado: ");
      fgets(estado, 20, stdin);
      fwrite(&estado, sizeof(estado), 1, dados);

      printf("CEP: ");
      fgets(cep, 11, stdin);
      fwrite(&cep, sizeof(cep), 1, dados);

      printf("Telefone: ");
      fgets(telefone, 15, stdin);
      fwrite(&telefone, sizeof(telefone), 1, dados);

      printf("\n");
    }

    fclose(dados);

    // Criar arquivo de indice primario (campo chave)

    dados = fopen("/Users/samara/Documents/ORI/Indexacao1/dados.bin", "rb");
    indicepri = fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.bin", "wb");
    int pos;

    for(i = 0; i < quant; i++)
    {
      char character;
      pos = i * 130;
      fseek(dados, pos, SEEK_SET);
      printf("pointer position: %ld\n", ftell(dados));
      fread(&character, sizeof(character), 1, dados);
      fwrite(&character, sizeof(character), 1, indicepri);
      fwrite(&pos, sizeof(pos), 1, indicepri);
      printf("quant: %d\n", quant);
      printf("character: %c\n", character);
      printf("pos: %d\n", pos);
    }

    fclose(dados);
    fclose(indicepri);

    // Criar arquivo de indice secundario (campo cidade)

    dados = fopen("/Users/samara/Documents/ORI/Indexacao1/dados.bin", "rb");
    indicesec = fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.bin", "wb");

    for(i = 0; i < quant; i++)
    {
      pos = i * 130;
      fseek(dados, pos, SEEK_SET);
      fread(&chave, sizeof(chave), 1, dados);
      fseek(dados, pos + 64, SEEK_SET);
      fread(&cidade, sizeof(cidade), 1, dados);

      fwrite(&cidade, sizeof(cidade), 1, indicesec);
      fwrite(&chave, sizeof(chave), 1, indicesec);
    }

    fclose(dados);
    fclose(indicesec);
  }

  else
  {
    // Arquivos de indice ja existem
    // Carregar em memoria primaria

    // Arquivo de indice primario
    indicepri = fopen("/Users/samara/Documents/ORI/Indexacao1/indicepri.bin", "rb+");
    fseek(indicepri, 0, SEEK_END);
    int tamanhopri = ftell(indicepri);

    char* indice1 = (char*) malloc(tamanhopri);
    fseek(indicepri, 0, SEEK_SET);
    fread(indice1, tamanhopri, 10, indicepri);
    printf("%s\n", indice1);
    fclose(indicepri);

    // Arquivo de indice secundario
    indicesec = fopen("/Users/samara/Documents/ORI/Indexacao1/indicesec.bin", "rb+");
    fseek(indicesec, 0, SEEK_END);
    int tamanhosec = ftell(indicesec);

    char* indice2 = (char*) malloc(tamanhosec);
    fseek(indicesec, 0, SEEK_SET);
    fread(indice2, tamanhosec, 1, indicesec);
    printf("%s", indice2);
    fclose(indicesec);

    fseek(indicepri, 0, SEEK_SET);
    fseek(indicesec, 0, SEEK_SET);

    /*
    char ch;
    while ( (ch = fgetc(indicepri)) != EOF )
    {
      fscanf(indicepri, "%c", &ch);
      fprintf(stdout, "%c", ch);
      fprintf(stdout, "\n");
    }

    while ( (ch = fgetc(indicesec)) != EOF )
    {
      fscanf(indicesec, "%c", &ch);
      fprintf(stdout, "%c", ch);
    }
    */

  }

  return 0;
}
