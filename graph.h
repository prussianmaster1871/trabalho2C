#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30


typedef struct Edge {
    char destino;
    float weight;
    char direction[MAX];
    char street[MAX];
} Edge;


typedef struct Node {
    char vertex;
    float x, y;
    char rua1[MAX];
    char rua2[MAX];
    Edge *norte;
    Edge *sul;
    Edge *leste;
    Edge *oeste;
    struct Node *next;
} Node;


typedef struct {
    Node *head;
    int size;
} AdjNodeList;


typedef struct {
    AdjNodeList *list;
} Graph;


Graph* criarGrafo();
Node* addNode(Graph* grafo, char vertex, float x, float y, const char* street_1, const char* street_2);
Edge* addEdge(Node* origem, char direcao, char dst, float peso, const char* rua);
Node* buscarNo(Graph* grafo, char vertex);
void liberarGrafo(Graph* grafo);

#endif
