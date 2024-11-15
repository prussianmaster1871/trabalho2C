#include "graph.h"
#include <stdio.h>

int main() {
    Graph* grafo = initGrafo();

    // Ler dados dos arquivos
    printf("Carregando nós...\n");
    loadNodes(grafo, "../pontos.txt");

    printf("Carregando arestas...\n");
    loadEdges(grafo, "../vizinhos.txt");

    // Verificar se o grafo foi carregado corretamente
    printf("Estrutura do grafo carregada:\n");
    Node* node = grafo->list->head;
    while (node) {
        printf("Nó: %c, x: %.2f, y: %.2f, Ruas: %s, %s\n",
               node->vertex, node->x, node->y, node->rua1, node->rua2);
        if (node->norte) printf("  Norte -> %c (%s)\n", node->norte->destino, node->norte->street);
        if (node->sul) printf("  Sul -> %c (%s)\n", node->sul->destino, node->sul->street);
        if (node->leste) printf("  Leste -> %c (%s)\n", node->leste->destino, node->leste->street);
        if (node->oeste) printf("  Oeste -> %c (%s)\n", node->oeste->destino, node->oeste->street);
        node = node->next;
    }

    // Receber pontos de origem e destino
    char origem, destino;
    printf("Digite o ponto de origem: ");
    scanf(" %c", &origem);
    printf("Digite o ponto de destino: ");
    scanf(" %c", &destino);

    // Validar os pontos de origem e destino
    if (!getVertex(grafo, origem)) {
        printf("Erro: O ponto de origem '%c' não existe.\n", origem);
        free_graph(grafo);
        return 1;
    }

    if (!getVertex(grafo, destino)) {
        printf("Erro: O ponto de destino '%c' não existe.\n", destino);
        free_graph(grafo);
        return 1;
    }

    // Executar Dijkstra
    printf("Executando Dijkstra...\n");
    dijkstra(grafo, origem, destino);

    // Liberar o grafo
    free_graph(grafo);

    return 0;
}
