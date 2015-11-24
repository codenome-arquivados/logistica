#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MATERIAIS 1000
#define MAX_CLIENTES 1000
#define MAX_PEDIDOS 1000

#define ENTREGUE 0
#define AGUARDANDO_ENTREGA 1

//-----------------------ESTRUTURAS----------------------------
typedef struct cliente {
    char    primeiroNome[20];
    char       sobrenome[20];
    char         empresa[40];
    char            cnpj[15];
    char       endereco[100];
} cliente;

typedef struct material {
    char            produto[40];
    int              quantidade;
    int                  codigo;
    char pontosDeEntrega[5][20];
} material;

typedef struct pedido {
    char          cnpj[15];
    int       cod_material;
    int             status;
    int             codigo;
} pedido;

//------------------------GLOBAL-------------------------------
cliente clientes[MAX_CLIENTES];
int qtd_clientes = 0;

material materiais[MAX_MATERIAIS];
int qtd_materiais = 0;

pedido pedidos[MAX_PEDIDOS];
int qtd_pedidos = 0;

int indice_cod_materiais = 1;
int indice_pedidos = 1;

//-------------------------FUNÇÕES-----------------------------
void                              bemVindo();
int                          menuPrincipal();
void                       cadastraCliente();
void                         listaClientes();
void                        listaMateriais();
void                          listaPedidos();
void                      cadastraMaterial();
cliente*      encontraCliente(char cnpj[15]);
material*              encontraMaterial(int);
void                            geraPedido();
void       diminuiQuantidade(material*, int);
void                       registraEntrega();
void                      entregaPedido(int);                 
//-------------------------PRINCIPAL---------------------------

int main() {
    //Iniciando o programa
    
    int sequencia = 1;
    while(sequencia != 0) {
        system("clear");
        sequencia = menuPrincipal();
    }
    exit(0);

    printf("\n\n\n");
}

//-------------------------FUNÇÕES-----------------------------
void bemVindo() {
    printf("--------------------SEJA BEM VINDO!---------------------\n");
    printf("---------------------CAMPCOMP 1.0!----------------------\n\n\n");
}

int menuPrincipal() {
    int escolha;
    int sequencia = 1;
    bemVindo();
    printf("[1] Cadastrar cliente\n");
    printf("[2] Cadastrar materiais\n");
    printf("[3] Realizar um pedido\n");
    printf("[4] Relatório de entregas\n");
    printf("[5] Listar clientes\n");
    printf("[6] Listar materiais\n");
    printf("[7] Registrar entrega\n");
    printf("\n\n\n[0] SAIR\n");
    printf("Digite uma opção: ");
    scanf("%d", &escolha);
    system("clear");

    switch(escolha) {
        case 1:        
        cadastraCliente();
        break;

        case 2:
        cadastraMaterial();
        break;

        case 3:
        geraPedido();
        break;

        case 4:
        listaPedidos();
        break;

        case 5:
        listaClientes();
        break;

        case 6:
        listaMateriais();
        break;

        case 7:
        registraEntrega();
        break;

        case 0:
        printf("Sair\n");
        return 0;

        default:
        printf("Opção inválida! Tente novamente . . .\n");
    }
        
    return 1;
}

void cadastraCliente() {
    cliente cadastro;
    printf("-------------ÁREA DE CADASTRO DE CLIENTES---------------\n");
    printf("---------------------CAMPCOMP 1.0!----------------------\n\n\n");
    printf("Primeiro nome: ");
    scanf("%s", cadastro.primeiroNome);
    printf("Sobrenome: ");
    scanf("%s", cadastro.sobrenome);
    printf("Empresa: ");
    scanf("%s", cadastro.empresa);
    printf("CNPJ: ");
    scanf("%s", cadastro.cnpj);
        __fpurge(stdin); //Limpa Buffer da string abaixo
    printf("Endereço: ");
    fgets(cadastro.endereco, 100, stdin);
    clientes[qtd_clientes] = cadastro;
    qtd_clientes++;
}

void listaClientes() {
    printf("=============================================================\n");
    printf("| %-17s|", "CNPJ");
    printf("%-30s", "Nome");
    printf("\n=============================================================\n");

    int i;

    for(i=0; i<qtd_clientes; i++) {
        printf("| %-17s|", clientes[i].cnpj);
        printf("%s %s\n", clientes[i].primeiroNome, clientes[i].sobrenome);
    }

    printf("=============================================================\n");
    printf("\nPressione qualquer tecla para retornar ao menu.");
    __fpurge(stdin);
    getchar();
}

void listaMateriais() {
    printf("=============================================================\n");
    printf("| %-17s|", "Codigo");
    printf(" %-30s", "Nome");
    printf("| %s", "Qtd.");
    printf("\n=============================================================\n");
    
    int i;

    for(i=0; i<qtd_materiais; i++) {
        printf("| %-17d|", materiais[i].codigo);
        printf(" %-30s", materiais[i].produto);
        printf("| %d\n", materiais[i].quantidade);
    }

    printf("=============================================================\n");
    printf("\nPressione qualquer tecla para retornar ao menu.");
    __fpurge(stdin);
    getchar();
}

void listaPedidos() {
    printf("=========================================================================\n");
    printf("| %-8s|", "Codigo");
    printf(" %-18s|", "CNPJ");
    printf(" %-20s|", "Cod do material");
    printf(" %s", "Status");
    printf("\n=========================================================================\n");

    int i;

    for(i=0; i<qtd_pedidos; i++) {
        printf("| %-8d|", pedidos[i].codigo);
        printf(" %-18s|", pedidos[i].cnpj);
        printf(" %-20d", pedidos[i].cod_material);

        switch(pedidos[i].status) {
            case ENTREGUE:
                printf("| ENTREGUE\n");
                break;
            case AGUARDANDO_ENTREGA:
                printf("| AGUARDANDO ENTREGA\n");
                break;
        }
    }

    printf("=========================================================================\n");
    printf("\nPressione qualquer tecla para retornar ao menu.");
    __fpurge(stdin);
    getchar();
}

void cadastraMaterial() {
    printf("-------------ÁREA DE CADASTRO DE MATERIAIS--------------\n");
    printf("---------------------CAMPCOMP 1.0!----------------------\n\n\n");

    material cadastro;

    printf("Produto: ");
    scanf("%s", cadastro.produto);
    printf("Quantidade: ");
    scanf("%d", &cadastro.quantidade);    

    cadastro.codigo = indice_cod_materiais;

    int i;
    char flag_ponto_de_entrega;

    for(i=0; i<5; i++) {
        printf("Ponto de entrega %d: ", i+1);
        scanf("%s", cadastro.pontosDeEntrega[i]);

        __fpurge(stdin);

        if(i != 4) {
            printf("Deseja adicionar outro ponto de entrega? [s/n]");

            scanf("%c", &flag_ponto_de_entrega);
        }

        if(flag_ponto_de_entrega != 's' && flag_ponto_de_entrega != 'S') {
            break;
        }
    }
    //Criar uma função para validar a quandidade de mateirias.

    materiais[qtd_materiais] = cadastro;    
    qtd_materiais++;

    printf("\nCadastro efetuado com sucesso!\n");
    printf("\nO codigo do material e %d", indice_cod_materiais);
    printf("\nPressione qualquer tecla para retornar ao menu.");

    __fpurge(stdin);
    getchar();

    indice_cod_materiais++;    
}

cliente* encontraCliente(char cnpj[15]) {
    int i;

    for(i=0; i<qtd_clientes; i++) {
        if(strcmp(clientes[i].cnpj, cnpj) == 0) {
            return &clientes[i];
        }
    }

    return 0;
}

material* encontraMaterial(int codigo) {
    int i;

    for(i=0; i<qtd_materiais; i++) {
        if(materiais[i].codigo == codigo) {
            return &materiais[i];
        }
    }

    return 0;
}

void geraPedido() {
    printf("---------------ÁREA DE GERAÇÃO DE PEDIDOS-----------------\n");
    printf("---------------------CAMPCOMP 1.0!----------------------\n");

    char cnpj[15];
    int cod_material, qtd;
    cliente* _cliente;
    material* _material;
    pedido _pedido;

    printf("\nInsira o cnpj do cliente: ");
    scanf("%s", cnpj);

    _cliente = encontraCliente(cnpj);

    if(_cliente == NULL) {
        printf("\n!! Erro: cliente nao encontrado!");
        printf("\nPressione qualquer tecla para retornar ao menu.");
        __fpurge(stdin);
            getchar();
            return;
        }

    printf("\nInsira o codigo do material: ");
    scanf("%d", &cod_material);

    _material = encontraMaterial(cod_material);

    if(_material == NULL) {
        printf("\n!! Erro: material nao encontrado!");
        printf("\nPressione qualquer tecla para retornar ao menu.");
        __fpurge(stdin);
            getchar();
            return;
    }
    
    printf("\nQuantas unidades deseja? ");
    scanf("%d", &qtd);

    diminuiQuantidade(_material, qtd);

    strcpy(_pedido.cnpj, _cliente->cnpj);
    _pedido.cod_material = _material->codigo;
    _pedido.status = AGUARDANDO_ENTREGA;
    _pedido.codigo = indice_pedidos;    

    pedidos[qtd_pedidos] = _pedido;

    printf("\nPedido efetuado com sucesso!\n");
    printf("\nO codigo do pedido e %d", indice_pedidos);
    printf("\nPressione qualquer tecla para retornar ao menu.");

    __fpurge(stdin);
    getchar();

    qtd_pedidos++;
    indice_pedidos++;
}

void diminuiQuantidade(material *_material, int qtd) {
    int i;

    for(i=0; i<qtd_materiais; i++) {
        if(materiais[i].codigo == _material->codigo) {
            materiais[i].quantidade -= qtd;
        }
    }
}

void registraEntrega() {
    int cod_pedido;
    pedido _pedido;

    printf("\nInsira o codigo do pedido: ");
    scanf("%d", &cod_pedido);

    entregaPedido(cod_pedido);

    printf("\nRegistro efetuado com sucesso!\n");
    printf("\nPressione qualquer tecla para retornar ao menu.");

    __fpurge(stdin);
    getchar();
}

void entregaPedido(int cod_pedido) {
    int i;

    for(i=0; i<qtd_pedidos; i++) {
        if(pedidos[i].codigo == cod_pedido) {
            pedidos[i].status = ENTREGUE;
        }
    }
}
