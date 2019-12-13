#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "estruturas.h"
#include "operacoes.h"
int main(int argc, char const *argv[]) {

  setlocale(LC_ALL,"Portuguese");
  FILE *arq;
  FILE *arq2;

  TModuloVeiculo moduloVeiculo;
  TModuloMulta moduloMulta;

  iniciarMulta(&moduloMulta);
  iniciarVeiculo(&moduloVeiculo);

  TVeiculo veiculo;
  TMulta multa;
  char byte[1], vetor[5000], vetor2[5000];
  int  k = 0;
  int sair = 0, opcao = 0;
  int indice;

  arq = fopen("/home/legend/aeds/veiculos.dat", "rb");
  if(arq == NULL ){
    printf("\nError ao abrir veiculos\n");
    getchar();
    exit(1);
  }

  arq2 = fopen("/home/legend/aeds/multas.dat", "rb");
  if(arq2 == NULL ){
    printf("\nError ao abrir multas\n");
    getchar();
    exit(1);
  }

  rewind(arq);
  rewind(arq2);

  while(k<TAM){
    LerMulta(arq2,&moduloMulta.vetor[k]);
    moduloMulta.indice++;
    k++;
  }

  k = 0;

  while(k < TAM){
    LerVeiculo(arq,&moduloVeiculo.vetor[k]);
    moduloVeiculo.vetor[k].id = k;
    moduloVeiculo.indice++;
    k++;
  }

  do
  {

      MSG_MENU();
      fflush(stdin);
      scanf("%d", &opcao);

      switch (opcao)
      {
      case 1:
          submenuVeiculo(&moduloVeiculo, veiculo);
          break;

      case 2:
          submenuMulta(&moduloMulta, multa,&moduloVeiculo);
          break;

      case 3:
          printf("\n\t>>>>>Sair<<<<<<\n");
          getchar();
          break;

      default:
          printf("\nOpcao invalida.");
          getchar();

      }
  }
  while(opcao != 3);



  fclose(arq);
  fclose(arq2);
  return 0;

}
