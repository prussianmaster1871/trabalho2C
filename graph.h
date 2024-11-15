#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30
#define INFINITY_VALUE 9999


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

typedef struct {
    Node* node;
    float distance;
} PriorityQueue;


Graph* criarGrafo();
Node* addNode(Graph* grafo, char vertex, float x, float y, const char* street_1, const char* street_2);
Edge* addEdge(Node* origem, char direcao, char dst, float peso, const char* rua);
Node *getVertex(Graph* grafo, char vertex);
void dijkstra(Graph* grafo, char origem, char destino);
void free_lista_node(AdjNodeList *list);
void free_graph(Graph *grafo);
void loadEdges(Graph* grafo, const char* filename);
void loadNodes(Graph* grafo, const char* filename);
Graph* initGrafo();

#endif
