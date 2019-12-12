#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int TAM = 100;

typedef struct veiculo{
  char placaNc[5];
  char placaC[3];
  char placa[9];
  int  placaN;
  char modelo[25];
  char nome[40];
  char rua[40];
  int numero;
  char cidade[40];
  char bairro[40];
  char estado[3];
  int id;

}TVeiculo;

typedef struct tdata{
  int dia;
  int mes;
  int ano;
} TData;

typedef struct thora{
  int hora;
  int min;
  int seg;
}THora;

typedef struct multa{
  int id;
  char idC[4];
  char placaNc[5];
  char placaC[4];
  char placa[9];
  int  placaN;
  char dat[5];
  char hor[4];
  TData data;
  THora hora;
  char rodovia[20];
  int km;
  int velocidade;
  float multa;
  int excesso;
} TMulta;

typedef struct moduloVeiculo{
  int indice;
  TVeiculo vetor[500];
}TModuloVeiculo;

typedef struct moduloMulta{
  int indice;
  TMulta vetor[500];
  int ordem;
}TModuloMulta;

void iniciarVeiculo(TModuloVeiculo *moduloVeiculo){
  moduloVeiculo->indice=0;
}

void iniciarMulta(TModuloMulta *moduloMulta){
  moduloMulta->indice=0;
}

int calcByte (FILE *arq){
  int byte2 = 0;
  int x = 255;
  char byte[1];

  fread(byte, 1, 1, arq);
  if(byte[0] == ' '){
    fread(byte, 1, 1, arq);
  }
  byte2 = x & byte[0];
  return byte2;
}

void ImprimirData(TData data){
  if(data.mes <=9 && data.dia  > 9)
  printf(" %d/0%d/%d", data.dia, data.mes, data.ano);
  if(data.dia <=9 && data.mes > 9)
  printf(" 0%d/%d/%d", data.dia, data.mes, data.ano);
  if(data.dia <=9 && data.mes <=9)
  printf(" 0%d/0%d/%d", data.dia, data.mes, data.ano);
  if(data.dia > 9 && data.mes > 9)
  printf(" %d/%d/%d", data.dia, data.mes, data.ano);
}

void ImprimirHora(THora hora){

  if(hora.hora<10 )
  printf("0%d:", hora.hora);
  else
  printf("%d:", hora.hora);

  if(hora.min<10)
  printf("0%d:", hora.min);
  else
  printf("%d:", hora.min);

  if(hora.seg<10)
  printf("0%d", hora.seg);
  else
  printf("%d", hora.seg);
}

void imprimirMulta(TMulta mult, int i){
  int x;
  printf("\n==========================================\n  Multa %d\n", i);

  printf("\n\tID:              %d", mult.id);
  printf("\n\tPlaca:           %s", mult.placa);
  printf("\n\tData:           ");
  ImprimirData(mult.data);
  printf("\n\tHora:            ");
  ImprimirHora(mult.hora);
  printf("\n\tRodovia          %s", mult.rodovia);
  printf("\n\tKm:              %d\n", mult.km);
  printf("\tVelocidade:      %d\n", mult.velocidade);
  printf("\tExcesso:         %d\n", mult.excesso);
  printf("\tValor da multa:  %.2f\n", mult.multa);

  printf("==========================================\n");
}

void imprimirTodosM(TModuloMulta moduloMulta){
  for(int x=0; x<moduloMulta.indice; x++)
  imprimirMulta(moduloMulta.vetor[x],x);
}

void calculoMulta(TMulta *mult){

  if(mult->excesso <= 5)
  mult->multa = 80;
  else if ( mult->excesso > 5 && mult->excesso <= 10)
  mult->multa = 180;
  else if ( mult->excesso > 10 && mult->excesso <= 20)
  mult->multa = 360;
  else if ( mult->excesso > 20  && mult->excesso <= 40)
  mult->multa = 1080;
  else if ( mult->excesso > 40)
  mult->multa = 3800;
}

void LerHora(THora *hora, char hor[3]){
  int z = 255;
  hora->hora = z & hor[0];
  hora->min = z & hor[1];
  hora->seg = z & hor[2];
}

void LerData (TData *data, char dat[4]){
  int z = 255;

  data->dia = z & dat[0];
  data->mes = z & dat[1];
  data->ano = z & dat[2];
  data->ano = data->ano << 8;
  data->ano = data->ano + (z & dat[3]);
}

void LerMulta(FILE *arq, TMulta *mult){

  char byte[1],placaAux[4];
  int z = 255, byte2 = 0;

  //LER ID
  fread(mult->idC,1,4,arq);
  mult->id = 0;
  mult->id = z & mult->idC[0];
  mult->id=mult->id << 8;
  mult->id = mult->id + (z & mult->idC[1]);
  mult->id = mult->id << 8;
  mult->id = mult->id + (z & mult->idC[2]);
  mult->id = mult->id << 8;
  mult->id = mult->id + (z & mult->idC[3]);

  byte2 = calcByte(arq);

  fseek(arq,-1,1);
  //PLACA
  fread(mult->placaNc,sizeof(char),5,arq);
  {
    //LI TODOS OS BITS EM UM VETOR DE CHAR[5] E SALVEI NAS OUTRAS VARIAVEIS PRA TRABALHAR AQUI E FICAR ORGANIZADO
    mult->placaC[0] = mult->placaNc[0];
    mult->placaC[1] = mult->placaNc[1];
    mult->placaC[2] = mult->placaNc[2];

    mult->placa[0] = mult->placaNc[0];
    mult->placa[1] = mult->placaNc[1];
    mult->placa[2] = mult->placaNc[2];


    mult->placaN = 0;
    mult->placaN = z & mult->placaNc[3];
    //SHIFT � TIPO PRA MULTIPLICAR POR 1000, SO QUE EM BINARIO, E NO CASO MULTIPLICA POR 100000000, COM 8 "ZEROS"
    mult->placaN  = mult->placaN << 8;
    mult->placaN = mult->placaN + (z & mult->placaNc[4]);

    //UMAS MANIPULA�OES PARA QUANDO O NUMERO INTEIRO COME�AR COM ZERO NA ESQUERDA, E NA PLACA APARECER OS ZEROS
    if(mult->placaN<1){
      sprintf(placaAux, "%d", mult->placaN);
      mult->placa[3] = '-';
      mult->placa[4] = '0';
      mult->placa[5] = '0';
      mult->placa[6] = '0';
      mult->placa[7] = '0';
    }else if(mult->placaN<10){
      sprintf(placaAux, "%d", mult->placaN);
      mult->placa[3] = '-';
      mult->placa[4] = '0';
      mult->placa[5] = '0';
      mult->placa[6] = '0';
      mult->placa[7] = placaAux[0];
    }else if(mult->placaN<100){
      sprintf(placaAux, "%d", mult->placaN);
      mult->placa[3] = '-';
      mult->placa[4] = '0';
      mult->placa[5] = '0';
      mult->placa[6] = placaAux[0];
      mult->placa[7] = placaAux[1];
    } else if(mult->placaN < 1000 && mult->placaN > 100){
      sprintf(placaAux, "%d", mult->placaN);
      mult->placa[3] = '-';
      mult->placa[4] = '0';
      mult->placa[5] = placaAux[0];
      mult->placa[6] = placaAux[1];
      mult->placa[7] = placaAux[2];
    }else {
      sprintf(placaAux, "%d", mult->placaN);
      mult->placa[3] = '-';
      mult->placa[4] = placaAux[0];
      mult->placa[5] = placaAux[1];
      mult->placa[6] = placaAux[2];
      mult->placa[7] = placaAux[3];
    }
  }

  fread(mult->dat,sizeof(char),4,arq);
  LerData(&mult->data, mult->dat);

  //LER HORA
  fread(mult->hor,sizeof(char),3,arq);
  LerHora(&mult->hora, mult->hor);
  byte2 = calcByte(arq);

  //LER RODOVIA
  fread(mult->rodovia,sizeof(char),byte2 + 2,arq);
  byte2 = calcByte(arq);

  // LER KM
  fread(byte,sizeof(char),1,arq);
  mult->km = z & byte[0];

  //LER VELOCIDADE
  fseek(arq, 3, SEEK_CUR);
  fread(byte,sizeof(char),1,arq);

  mult->velocidade = z & byte[0];

  //LER EXCESSO
  fseek(arq, 3, SEEK_CUR);
  fread(byte,sizeof(char),1,arq);

  mult->excesso = z & byte[0];
  calculoMulta(mult);
}

void LerVeiculo(FILE *arq, TVeiculo *vet){

  char byte[1],placaAux[4];
  int z = 255, byte2 = 0;

  //PLACA
  fread(vet->placaNc,sizeof(char),5,arq);
  //LENDO TODOS OS BITS EM UM VETOR DE CHAR[5] E SALVEANDO NAS OUTRAS VARIAVEIS PRA FICAR ORGANIZADO
  vet->placaC[0] = vet->placaNc[0];
  vet->placaC[1] = vet->placaNc[1];
  vet->placaC[2] = vet->placaNc[2];

  vet->placa[0] = vet->placaNc[0];
  vet->placa[1] = vet->placaNc[1];
  vet->placa[2] = vet->placaNc[2];


  vet->placaN = 0;
  vet->placaN = z & vet->placaNc[3];
  vet->placaN  = vet->placaN << 8;
  vet->placaN = vet->placaN + (z & vet->placaNc[4]);

  //MANIPULA�OES PARA QUANDO O NUMERO INTEIRO COME�AR COM ZERO NA ESQUERDA, E NA PLACA APARECER OS ZEROS EX: 0123, APARECERIA 123 POIS � UM int
  if(vet->placaN<1){
    sprintf(placaAux, "%d", vet->placaN);
    vet->placa[3] = '-';
    vet->placa[4] = '0';
    vet->placa[5] = '0';
    vet->placa[6] = '0';
    vet->placa[7] = '0';
  }else if(vet->placaN<10){
    sprintf(placaAux, "%d", vet->placaN);
    vet->placa[3] = '-';
    vet->placa[4] = '0';
    vet->placa[5] = '0';
    vet->placa[6] = '0';
    vet->placa[7] = placaAux[0];
  }else if(vet->placaN<100){
    sprintf(placaAux, "%d", vet->placaN);
    vet->placa[3] = '-';
    vet->placa[4] = '0';
    vet->placa[5] = '0';
    vet->placa[6] = placaAux[0];
    vet->placa[7] = placaAux[1];
  } else if(vet->placaN < 1000 && vet->placaN > 100){
    sprintf(placaAux, "%d", vet->placaN);
    vet->placa[3] = '-';
    vet->placa[4] = '0';
    vet->placa[5] = placaAux[0];
    vet->placa[6] = placaAux[1];
    vet->placa[7] = placaAux[2];
  }else {
    sprintf(placaAux, "%d", vet->placaN);
    vet->placa[3] = '-';
    vet->placa[4] = placaAux[0];
    vet->placa[5] = placaAux[1];
    vet->placa[6] = placaAux[2];
    vet->placa[7] = placaAux[3];
  }

  fseek(arq, 1, SEEK_CUR);
  byte2 = calcByte(arq);

  //LER MODELO
  fread(vet->modelo,sizeof(char),byte2 + 1,arq);
  byte2 = calcByte(arq);

  //LER NOME
  fread(vet->nome,sizeof(char),byte2 + 1,arq);
  byte2 = calcByte(arq);

  //LER RUA
  fread(vet->rua,sizeof(char),byte2 + 1,arq);
  byte2 = calcByte(arq);

  // LER NUMERO
  fseek(arq, 1, SEEK_CUR);
  fread(byte,sizeof(char),1,arq);
  vet->numero = z & byte[0];

  fseek(arq, 1, SEEK_CUR);
  byte2 = calcByte(arq);

  //LER CIDADE
  fread(vet->cidade,sizeof(char),byte2 + 1,arq);
  byte2 = calcByte(arq);

  //LER BAIRRO
  fread(vet->bairro,sizeof(char),byte2 + 1,arq);
  byte2 = calcByte(arq);

  //LER ESTADO
  fread(vet->estado,sizeof(char),2,arq);
  byte2 = calcByte(arq);


  fseek(arq, -1, SEEK_CUR);
}

void imprimirVeiculo(TVeiculo vet, int j){
  int x;
  printf("\n==========================================\n  Veiculo %d\n", j);
  printf("\n\tPlaca:   %s    id: %d", vet.placa, vet.id);
  printf("\n\tModelo:  %s", vet.modelo);
  printf("\n\tNome:    %s", vet.nome);
  printf("\n\tRua:     %s", vet.rua);
  printf("\n\tNumero:  %d", vet.numero);
  printf("\n\tCidade:  %s", vet.cidade);
  printf("\n\tBairro:  %s", vet.bairro);
  printf("\n\tEstado:  %c%c\n", vet.estado[0], vet.estado[1]);

  printf("==========================================\n");
}

void imprimirTodosV(TModuloVeiculo moduloVeiculo){
  for(int x=0; x<moduloVeiculo.indice; x++)
  imprimirVeiculo(moduloVeiculo.vetor[x],x);
}

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

  imprimirTodosM(moduloMulta);

  return 0;
}
