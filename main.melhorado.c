#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 50
#define MAX_CARRINHO 50

typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

typedef struct {
    Produto produto;
    int quantidade;
} CarrinhoItem;

typedef struct {
    Produto produtos[MAX_PRODUTOS];
    CarrinhoItem carrinho[MAX_CARRINHO];
    int numProdutos;
    int numCarrinho;
} Supermercado;

void menu(Supermercado *super);
void cadastrarProduto(Supermercado *super);
void listarProdutos(Supermercado *super);
void comprarProduto(Supermercado *super);
void visualizarCarrinho(Supermercado *super);
void fecharPedido(Supermercado *super);

int pegarEntradaInt();
float pegarEntradaFloat();
int temNoCarrinho(Supermercado *super, int codigo);
Produto *pegarProdutoPorCodigo(Supermercado *super, int codigo);
void atualizarQuantidadeCarrinho(Supermercado *super, int index, int quantidade);
void infoProduto(Produto *prod);

int main() {
    Supermercado super = { .numProdutos = 0, .numCarrinho = 0 };
    menu(&super);
    return 0;
}

void menu(Supermercado *super) {
    int opcao;
    do {
        printf("\n--- Sistema de Supermercado ---\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Comprar Produto\n");
        printf("4. Visualizar Carrinho\n");
        printf("5. Fechar Pedido\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        opcao = pegarEntradaInt();

        switch (opcao) {
            case 1: cadastrarProduto(super); break;
            case 2: listarProdutos(super); break;
            case 3: comprarProduto(super); break;
            case 4: visualizarCarrinho(super); break;
            case 5: fecharPedido(super); break;
            case 6: printf("Saindo do sistema...\n"); break;
            default: printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);
}

void cadastrarProduto(Supermercado *super) {
    if (super->numProdutos >= MAX_PRODUTOS) {
        printf("Capacidade máxima de produtos atingida.\n");
        return;
    }

    Produto p;
    printf("Digite o código do produto: ");
    p.codigo = pegarEntradaInt();
    printf("Digite o nome do produto: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("Digite o preço do produto: ");
    p.preco = pegarEntradaFloat();

    super->produtos[super->numProdutos++] = p;
    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos(Supermercado *super) {
    if (super->numProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    
    printf("\n--- Lista de Produtos ---\n");
    for (int i = 0; i < super->numProdutos; i++) {
        infoProduto(&super->produtos[i]);
    }
}

void comprarProduto(Supermercado *super) {
    int codigo, quantidade;

    listarProdutos(super);

    printf("Digite o código do produto que deseja comprar: ");
    codigo = pegarEntradaInt();

    Produto *p = pegarProdutoPorCodigo(super, codigo);
    if (p == NULL) {
        printf("Produto não encontrado.\n");
        return;
    }

    printf("Digite a quantidade: ");
    quantidade = pegarEntradaInt();
    if (quantidade <= 0) {
        printf("Quantidade inválida.\n");
        return;
    }

    int index = temNoCarrinho(super, codigo);
    if (index != -1) {
        atualizarQuantidadeCarrinho(super, index, quantidade);
    } else {
        if (super->numCarrinho >= MAX_CARRINHO) {
            printf("Carrinho cheio.\n");
            return;
        }
        super->carrinho[super->numCarrinho].produto = *p;
        super->carrinho[super->numCarrinho++].quantidade = quantidade;
    }
    printf("Produto adicionado ao carrinho.\n");
}

void visualizarCarrinho(Supermercado *super) {
    if (super->numCarrinho == 0) {
        printf("Carrinho vazio.\n");
        return;
    }
    
    printf("\n--- Carrinho de Compras ---\n");
    float total = 0.0;
    for (int i = 0; i < super->numCarrinho; i++) {
        CarrinhoItem c = super->carrinho[i];
        printf("Código: %d | Nome: %s | Preço: %.2f | Quantidade: %d | Total: %.2f\n",
               c.produto.codigo, c.produto.nome, c.produto.preco,
               c.quantidade, c.produto.preco * c.quantidade);
        total += c.produto.preco * c.quantidade;
    }
    printf("Valor total: %.2f\n", total);
}

void fecharPedido(Supermercado *super) {
    if (super->numCarrinho == 0) {
        printf("Carrinho vazio. Não há pedidos para fechar.\n");
        return;
    }

    visualizarCarrinho(super);
    printf("Fechando o pedido...\n");

    super->numCarrinho = 0;
    printf("Pedido fechado com sucesso! O carrinho foi esvaziado.\n");
}


int temNoCarrinho(Supermercado *super, int codigo) {
    for (int i = 0; i < super->numCarrinho; i++) {
        if (super->carrinho[i].produto.codigo == codigo) {
            return i;
        }
    }
    return -1;
}

Produto *pegarProdutoPorCodigo(Supermercado *super, int codigo) {
    for (int i = 0; i < super->numProdutos; i++) {
        if (super->produtos[i].codigo == codigo) {
            return &super->produtos[i];
        }
    }
    return NULL;
}

void atualizarQuantidadeCarrinho(Supermercado *super, int index, int quantidade) {
    super->carrinho[index].quantidade += quantidade;
}

void infoProduto(Produto *prod) {
    printf("Código: %d | Nome: %s | Preço: %.2f\n", prod->codigo, prod->nome, prod->preco);
}

int pegarEntradaInt() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida. Digite um número: ");
        while (getchar() != '\n');
    }
    getchar();
    return valor;
}

float pegarEntradaFloat() {
    float valor;
    while (scanf("%f", &valor) != 1) {
        printf("Entrada inválida. Digite um número: ");
        while (getchar() != '\n');
    }
    getchar();
    return valor;
}
