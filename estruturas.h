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
