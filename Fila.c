#include <stdio.h>
#include <stdlib.h>

// Estrutura para um nó da Fila
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Estrutura para a Fila
typedef struct {
    Node* front; // Aponta para o primeiro elemento
    Node* rear;  // Aponta para o último elemento
} Fila;

// Função para criar uma Fila vazia
Fila* inicializarFila() {
    Fila* q = (Fila*)malloc(sizeof(Fila));
    q->front = q->rear = NULL;
    return q;
}

// Função para verificar se a Fila está vazia
int isEmpty(Fila* q) {
    return q->front == NULL;
}

// Função para inserir um elemento na Fila (EntrarFila)
void EntrarFila(Fila* q, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro de alocaçao de memoria!\n");
        return;
    }
    newNode->data = value;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        // Se a Fila está vazia, o novo nó será tanto front quanto rear
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// Função para remover um elemento da Fila (SairFila)
int SairFila(Fila* q) {
    if (isEmpty(q)) {
        printf("Fila vazia! Nao eh possivel desenfileirar.\n");
        return -1;
    }
    
    Node* temp = q->front;
    int value = temp->data;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL; // Se a Fila ficar vazia, rear também deve ser NULL
    }
    
    free(temp);
    return value;
}

// Função para consultar o primeiro elemento da Fila (peek)
int peek(Fila* q) {
    if (isEmpty(q)) {
        printf("Fila vazia!\n");
        return -1;
    }
    return q->front->data;
}

// Função para exibir os elementos da Fila
void displayFila(Fila* q) {
    if (isEmpty(q)) {
        printf("Fila vazia!\n");
        return;
    }
    
    Node* temp = q->front;
    printf("Fila: ");
    while (temp) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Função para liberar toda a memória da Fila
void freeFila(Fila* q) {
    while (!isEmpty(q)) {
        SairFila(q);
    }
    free(q);
    printf("Fila liberada da memoria.\n");
}
