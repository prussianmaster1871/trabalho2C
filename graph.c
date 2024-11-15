#include "graph.h"

Graph* initGrafo() {
    Graph* grafo = (Graph*)malloc(sizeof(Graph));
    grafo->list = (AdjNodeList*)malloc(sizeof(AdjNodeList));
    grafo->list->head = NULL;
    grafo->list->size = 0;
    return grafo;
}

Node* addNode(Graph* grafo, char vertex, float x, float y, const char* rua1, const char* rua2) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->x = x;
    newNode->y = y;
    newNode->norte = NULL;
    newNode->leste = NULL;
    newNode->oeste = NULL;
    newNode->sul = NULL;
    newNode->next = grafo->list->head;

    grafo->list->head = newNode;
    grafo->list->size++;
    return newNode;
}

Edge* addEdge(Node* origem, char direcao, char dst, float peso, const char* rua) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destino = dst;
    newEdge->weight = peso;
    strcpy(newEdge->direction, direcao == 'N' ? "North" : direcao == 'S' ? "South" : direcao == 'E' ? "East" : "West");
    strcpy(newEdge->street, rua);

    switch (direcao) {
        case 'N': origem->norte = newEdge; break;
        case 'S': origem->sul = newEdge; break;
        case 'E': origem->leste = newEdge; break;
        case 'W': origem->oeste = newEdge; break;
        default:
            fprintf(stderr, "Direção inválida: %c\n", direcao);
        free(newEdge);
}