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
} Carrinho;

Produto produtos[MAX_PRODUTOS];
Carrinho carrinho[MAX_CARRINHO];
int numProdutos = 0;
int numCarrinho = 0;

void menu();
void cadastrarProduto();
void listarProdutos();
void comprarProduto();
void visualizarCarrinho();
void fecharPedido();
int temNoCarrinho(int codigo);
Produto pegarProdutoPorCodigo(int codigo);
void infoProduto(Produto prod);
void atualizarQuantidadeCarrinho(int index, int quantidade);

int main() {
    menu();
    return 0;
}

void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Supermercado ---\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Comprar Produto\n");
        printf("4. Visualizar Carrinho\n");
        printf("5. Fechar Pedido\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();
        
        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: comprarProduto(); break;
            case 4: visualizarCarrinho(); break;
            case 5: fecharPedido(); break;
            case 6: printf("Saindo do sistema...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 6);
}

void cadastrarProduto() {
    if (numProdutos >= MAX_PRODUTOS) {
        printf("Nao e possivel cadastrar mais produtos. Capacidade maxima atingida.\n");
        return;
    }
    
    Produto p;
    printf("Digite o codigo do produto: ");
    scanf("%d", &p.codigo);
    getchar();  // Limpa o buffer do teclado
    
    printf("Digite o nome do produto: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';  // Remove o newline no final da string
    
    printf("Digite o preço do produto: ");
    scanf("%f", &p.preco);
    
    produtos[numProdutos++] = p;
    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {
    if (numProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    
    printf("\n--- Lista de Produtos ---\n");
    for (int i = 0; i < numProdutos; i++) {
        infoProduto(produtos[i]);
    }
}

void comprarProduto() {
    int codigo, quantidade;
    
    listarProdutos();
    
    printf("Digite o codigo do produto que deseja comprar: ");
    scanf("%d", &codigo);
    
    Produto p = pegarProdutoPorCodigo(codigo);
    
    if (p.codigo == -1) {
        printf("Produto não encontrado.\n");
        return;
    }
    
    printf("Digite a quantidade: ");
    scanf("%d", &quantidade);
    
    if (quantidade <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    
    if (temNoCarrinho(codigo) != -1) {
        atualizarQuantidadeCarrinho(temNoCarrinho(codigo), quantidade);
    } else {
        if (numCarrinho >= MAX_CARRINHO) {
            printf("Carrinho cheio. Nao e possivel adicionar mais produtos.\n");
            return;
        }
        
        Carrinho c;
        c.produto = p;
        c.quantidade = quantidade;
        carrinho[numCarrinho++] = c;
    }
    
    printf("Produto adicionado ao carrinho.\n");
}

void visualizarCarrinho() {
    if (numCarrinho == 0) {
        printf("Carrinho vazio.\n");
        return;
    }
    
    printf("\n--- Carrinho de Compras ---\n");
    float total = 0.0;
    for (int i = 0; i < numCarrinho; i++) {
        Carrinho c = carrinho[i];
        printf("Codigo: %d | Nome: %s | Preco: %.2f | Quantidade: %d | Total: %.2f\n",
               c.produto.codigo, c.produto.nome, c.produto.preco,
               c.quantidade, c.produto.preco * c.quantidade);
        total += c.produto.preco * c.quantidade;
    }
    printf("Valor total: %.2f\n", total);
}

void fecharPedido() {
    if (numCarrinho == 0) {
        printf("Carrinho vazio. Nao ha pedidos para fechar.\n");
        return;
    }
    
    visualizarCarrinho();
    printf("Fechando o pedido...\n");
    
    numCarrinho = 0;
    printf("Pedido fechado com sucesso! O carrinho foi esvaziado.\n");
}

int temNoCarrinho(int codigo) {
    for (int i = 0; i < numCarrinho; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            return i;
        }
    }
    return -1;
}

Produto pegarProdutoPorCodigo(int codigo) {
    Produto p;
    p.codigo = -1;
    
    for (int i = 0; i < numProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            p = produtos[i];
            break;
        }
    }
    
    return p;
}

void infoProduto(Produto prod) {
    printf("Codigo: %d | Nome: %s | Preco: %.2f\n", prod.codigo, prod.nome, prod.preco);
}

void atualizarQuantidadeCarrinho(int index, int quantidade) {
    carrinho[index].quantidade += quantidade;
}