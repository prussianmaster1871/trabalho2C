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
}

Node *getVertex(Graph* grafo, char vertex) {
    for(Node* node = grafo->list->head; node != NULL; node = node->next) {
        if (node->vertex == vertex) {
            return node;
        }
    }
    return NULL;
}

void freeNodes(Node *head)
{
    if (head != NULL)
    {
        freeNodes(head->next);

        if (head->norte != NULL)
            free(head->norte);

        if (head->sul != NULL)
            free(head->sul);

        if (head->leste != NULL)
            free(head->leste);

        if (head->oeste != NULL)
            free(head->oeste);

        free(head);
    }
}

void free_lista_node(AdjNodeList *list)
{
    freeNodes(list->head);
    free(list);
}

void free_graph(Graph *grafo)
{
    free_lista_node(grafo->list);
    free(grafo);
}

void loadNodes(Graph* grafo, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de nós");
        return;
    }

    char vertex;
    float x, y;
    char rua1[MAX], rua2[MAX];

    while (fscanf(file, " %c\n%f %f\n%[^\n]\n%[^\n]\n", &vertex, &x, &y, rua1, rua2) == 5) {
        addNode(grafo, vertex, x, y, rua1, rua2);
    }

    fclose(file);
}

void loadEdges(Graph* grafo, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de arestas");
        return;
    }

    char origem, destino;
    char rua[MAX];

    while (fscanf(file, " %c\n%c\n%[^\n]\n", &origem, &destino, rua) == 3) {
        Node* origemNode = getVertex(grafo, origem);
        Node* destinoNode = getVertex(grafo, destino);

        if (origemNode && destinoNode) {
            // Direção será determinada pela lógica (exemplo: A -> B é Norte)
            if (origemNode->x == destinoNode->x && origemNode->y < destinoNode->y) {
                addEdge(origemNode, 'N', destino, 1.0, rua);
                addEdge(destinoNode, 'S', origem, 1.0, rua);
            } else if (origemNode->x == destinoNode->x && origemNode->y > destinoNode->y) {
                addEdge(origemNode, 'S', destino, 1.0, rua);
                addEdge(destinoNode, 'N', origem, 1.0, rua);
            } else if (origemNode->x < destinoNode->x && origemNode->y == destinoNode->y) {
                addEdge(origemNode, 'E', destino, 1.0, rua);
                addEdge(destinoNode, 'W', origem, 1.0, rua);
            } else if (origemNode->x > destinoNode->x && origemNode->y == destinoNode->y) {
                addEdge(origemNode, 'W', destino, 1.0, rua);
                addEdge(destinoNode, 'E', origem, 1.0, rua);
            } else {
                fprintf(stderr, "Erro: Conexão inválida entre '%c' e '%c'.\n", origem, destino);
            }
        } else {
            fprintf(stderr, "Erro: Um dos nós '%c' ou '%c' não existe.\n", origem, destino);
        }
    }

    fclose(file);
}

void dijkstra(Graph* grafo, char origem, char destino) {
    float dist[MAX];
    int prev[MAX];
    int visited[MAX] = {0};
    Node* vertices[MAX];
    int vertexCount = 0;

    // Inicializa as estruturas
    for (Node* current = grafo->list->head; current != NULL; current = current->next) {
        dist[vertexCount] = INFINITY_VALUE;
        prev[vertexCount] = -1;  // Nenhum predecessor
        vertices[vertexCount] = current;
        if (current->vertex == origem) {
            dist[vertexCount] = 0.0f;
        }
        vertexCount++;
    }

    for (int i = 0; i < vertexCount; i++) {
        // Encontra o nó com a menor distância ainda não visitado
        int minIndex = -1;
        for (int j = 0; j < vertexCount; j++) {
            if (!visited[j] && (minIndex == -1 || dist[j] < dist[minIndex])) {
                minIndex = j;
            }
        }

        if (minIndex == -1) break;

        visited[minIndex] = 1;
        Node* current = vertices[minIndex];

        // Atualiza as distâncias para os vizinhos
        Edge* edges[] = {current->norte, current->sul, current->leste, current->oeste};
        for (int k = 0; k < 4; k++) {
            Edge* edge = edges[k];
            if (edge) {
                for (int j = 0; j < vertexCount; j++) {
                    if (vertices[j]->vertex == edge->destino) {
                        float newDist = dist[minIndex] + edge->weight;
                        if (newDist < dist[j]) {
                            dist[j] = newDist;
                            prev[j] = minIndex;
                        }
                    }
                }
            }
        }
    }

    // Reconstruir o caminho
    int destinoIndex = -1;
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i]->vertex == destino) {
            destinoIndex = i;
            break;
        }
    }

    if (destinoIndex == -1 || dist[destinoIndex] == INFINITY_VALUE) {
        printf("Não há caminho entre '%c' e '%c'.\n", origem, destino);
        return;
    }

    printf("Caminho: ");
    int caminho[MAX];
    int caminhoTamanho = 0;
    for (int at = destinoIndex; at != -1; at = prev[at]) {
        caminho[caminhoTamanho++] = at;
    }
    for (int i = caminhoTamanho - 1; i >= 0; i--) {
        printf("%c", vertices[caminho[i]]->vertex);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}




