#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Fila.c"

int V = 4;

typedef struct s{
    struct s* prox;
    int adj;
    int custo;
}NO;


typedef struct {
    NO* inicio;
    int flag;
    int tipo;
    int nivel;
    int via;
}Vertice;

Vertice* inicializarVertice(int V){
   Vertice* g = (Vertice*)malloc((V+1)*sizeof(Vertice));
    for(int i = 0; i <= V ;i++){
        g[i].inicio = NULL;
    }
    return g;
}

NO* arestaExiste(Vertice* g, int i, int j, NO** ant){
    *ant = NULL;
    NO* p = g[i].inicio;
    while(p){
        if(p->adj == j) return p;
        *ant = p;
        p = p->prox;
    }
    return NULL; //aresta nao existe
}

bool inserirAresta(Vertice* g, int i, int j){
    NO* ant;
    NO* atual = arestaExiste(g, i, j, &ant);
    if(atual) return false; //Ja existe
    NO* novo = (NO*)malloc(sizeof(NO));
    //inicio da insercao
    novo->adj = j;
    novo->prox = g[i].inicio;
    g[i].inicio = novo;
    return true;
}

bool excluirAresta(Vertice* g, int i, int j){
    NO* ant;
    NO* atual = arestaExiste(g, i, j, &ant);
    if(!atual) return false;
    if(ant) {
        ant->prox = atual->prox;
        }
    else {
        g[i].inicio = atual->prox;
        }
    free(atual);
    return true;
}

int grauDeSaida(Vertice* g, int i){
    int resp = 0;
    NO* p = g[i].inicio;
    while(p){
        resp++;
        p = p->prox;
    }
    return resp;
}

int grauDeEntrada(Vertice* g, int i){
    int resp = 0;
    //V is a constant that marks the array's size
    for(int j = 0; j <= V; j++){
        NO* p = g[j].inicio;
        while(p){
            if(p->adj = i) resp++;
            p = p->prox;
        }
    }
    return resp;
}

void exibirGrafo(Vertice* g, int V){
    //V is a constant that marks the array's size
    for(int j = 0; j <= V; j++){
        NO* p = g[j].inicio;
        printf("%d|", j);
        while(p){
            printf(" -> %d", p->adj);
            p = p->prox;
        }
        printf("\n");
    }
}

Vertice* copiarGrafo(Vertice* g){
    Vertice* resp = (Vertice*)malloc(sizeof(Vertice)*(V+1));
    //inicializarVertice(resp);
    for(int i = 0; i <= V; i++){
        NO* p = g[i].inicio;
        resp[i].inicio = NULL;
        while(p){
            NO* novo = (NO*)malloc(sizeof(NO));
            novo->adj = p->adj;
            novo->prox = resp[i].inicio;
            resp[i].inicio = novo;
            p = p->prox;
        }
    }
    return resp;
}

int zerarFlags(Vertice* g){
    for(int i = 0; i <= V; i++){
        g[i].flag = 0;
    }
}
//se flag=2 quer dizer que esse elemento eh conectado com i
void prof(Vertice* g, int i, bool* ciclo,int* count){
    g[i].flag = 1; //descoberto
    NO* p = g[i].inicio;
    while(p){
        if(g[p->adj].flag == 0){
            (*count)++;
            prof(g, p->adj, ciclo, count);
        } else if (g[p->adj].flag == 1) *ciclo = true;
        p = p->prox;
    }
    g[i].flag = 2;
}




//copia o valor do NO no inicio do novo grafo e libera a memoria ocupada no grafo original, no fim coloca o inicio daquela linha como nulo
Vertice moverVertice(Vertice* g, int i){
    Vertice copia;
    copia.inicio = g[i].inicio;
    g[i].inicio = NULL;
    return copia;
}

//Verifica em quais o vetor incide, quando descobre um, coloca esse vetor incidindo sobre o vetor original (que incedia no grafo original) fazendo a transposta
Vertice* copiaTransposta(Vertice* g, int V){
    Vertice* copia = inicializarVertice(V);
    NO* atual = NULL;
    for(int i = 0; i <= V; i++){
        atual = g[i].inicio;
        while(atual){
            NO* novo = (NO*)malloc(sizeof(NO));
            novo->adj = i;
            novo->prox = copia[atual->adj].inicio;
            copia[atual->adj].inicio = novo;
            atual = atual->prox;
        }
    }
    return copia;
}

int ZeroFlags(Vertice* g){
	int i = 0;
    int maior = 0;
	int j;
	for(j = 0; j <= V; j++){
		if(g[j].flag == 0){
			i = j;
			break;
	    }
	}
	if(i == 0) return 0;
    bool ciclo;
    int count;
	prof(g, i, &ciclo, &count);
    if(count > maior){
        maior = count;
    }
	printf("\n"); 
    return maior;
}

//busca em largura
//necessita do campo flag do Vertice
void largura(Vertice* g, int i){
    zerarFlags(g);
    Fila* f = inicializarFila();
    EntrarFila(f, i);
    g[i].flag = 1; //discovered
    while(f){
        i = SairFila(f);
        g[i].flag = 2;
        NO* p = g[i].inicio;
        while(p){
            if(g[p->adj].flag == 0){
                g[p->adj].flag = 1;
                EntrarFila(f, p->adj);
            }
            p = p->prox;
        }
    }
}

//Partindo do nivel i, quero todos os vertices a uma distancia de no maximo N de i que tem o tipo 1
//precisa dos campos nivel, tipo e flag do Vertice
void exibirEmPrio(Vertice* g, int i, int N){
    zerarFlags(g);
    for(int j = 0; j < V; j++){
        g[j].nivel = -1;
    }
    g[i].nivel = 0;
    g[i].flag = 1;
    Fila* f = inicializarFila();
    EntrarFila(f, i);
    while(f){
        i = SairFila(f);
        g[i].flag = 2;
        NO* p = g[i].inicio;
        while(p){
            if(g[p->adj].flag == 0){
                g[p->adj].flag = 1;
                EntrarFila(f, p->adj);
                g[p->adj].nivel = g[i].nivel + 1;
                if(g[p->adj].nivel <= N && g[p->adj].tipo == 1){
                    printf("%d ", p->adj);
                } else {
                    while(f) SairFila(f);
                    return;
                }
            }//flag 0
            p = p->prox;
        }
    }
    while(f) SairFila(f);
}



//Busca e exibe o menor caminho de i ate j
//necessita 
void exibirVia(Vertice* g, int i, int d){
    zerarFlags(g);
    for(int j = 0; j < V; j++){
        g[j].via = -1;
    }
    g[i].via= 0;
    g[i].flag = 1;
    Fila* f = inicializarFila();
    EntrarFila(f, i);
    while(f){
        i = SairFila(f);
        g[i].flag = 2;
        NO* p = g[i].inicio;
        while(p){
            if(g[p->adj].flag == 0){
                g[p->adj].flag = 1;
                EntrarFila(f, p->adj);
                g[p->adj].via = i;
            }//flag 0
            if(p->adj == d){
                Vertice* pr = &g[p->adj];
                while(pr){
                    printf("%d ", p->adj);
                    pr = &g[pr->via];
                }
                return;
            }
            p = p->prox;
        }
    }
}

//Busca de profundidade matricial
/*void profMatricial(int m[V][V], int i,int flag[V], bool ciclo){
    flag[i] = 1;
    for(int j = 0; j <= V; i++){
        if(flag[j] == 1) ciclo = true;
        if(m[i][j] == 1 && flag[j] == 0) prof(m, j, flag, ciclo);
    }
    flag[i] = 2;
}*/

//Zerar flags em matriz
void zerarFlagsMatriz(int flag[V+1]){
    for(int i = 0; i <= V; i++){
        flag[i] = 0;
    }
}
//Busca em largura na forma matricial
void BuscaLargura(int m[V][V], int i, int destino, int flag[V]){
    Fila* f;
    inicializarFila(f);
    EntrarFila(f, i);
    int aux[V];
    zerarFlagsMatriz(aux);
    aux[i] = 1;
    while(f){
        i = SairFila(f);
        aux[i] = 2;
        int j;
        for(j = 0; j <= V; j++){
            if(m[i][j] == 1 && aux[j] == 0){
                if(j == destino) break;
                aux[j] = 1;
                EntrarFila(f, j);
            }
        }
    }
    if(aux[destino] == 0) return;
    for(int j = 0; j <= V; j++){
        if(aux[j] == 2) flag[j] = 2;
    }
}

//Exibir todos os vértices de onde é possível alcançar o destino(Versao Matricial)
//Zerar flags antes de utilizar:
void exibirOrigens(int m[V][V], int dest, int flag[V]){
    int i;
    for(i = 0; i <= V; i++){
        if(i == dest || flag[i]>0) continue;
        BuscaLargura(m, i, dest, flag);
    }
    for(i = 0; i <= V; i++){
        if(flag[i] > 0) printf("%d ", i);
    }

}

bool arestaExistente(Vertice* g, int i, int dest){
    NO* atual = g[i].inicio;
    while(atual){
        if(atual->adj == dest) return true;
        atual = atual->prox;
    }
    return false;
}

bool caminhoExiste(Vertice* g, int tamanho ,int c[tamanho]){
    if(tamanho < 2) return false;
    for(int j; j < tamanho-1; j++){
            if(!arestaExistente(g, c[j], c[j+1]))return false;
    }
    return true;
}

bool subgrafo(int m[V][V], Vertice* g){
    for(int i = 0; i <= V; i++){
        for(int j = 0; j <= V; j++){
            if(m[i][j] == 1){
                if(!arestaExistente(g, i, j)) return false;
            }
        }
    }
    return true;
}


/*int maiorCaminho(Vertice*g, int i){
    int[V] resposta;
    int j;
    for(j = 0; j <= V; j++){
        if(j == i) continue;
        resposta[j] = menorCaminhoLargura(g, j, i);
    }
    int response = 0;
    for(j = 0; j < V; j++){
        if(response <  resposta[j]) response = resposta[j];
    }
    return response;
}*/



void main(){
    Vertice* h = inicializarVertice(V);
    inserirAresta(h, 0, 1);
    inserirAresta(h, 1, 4);
    inserirAresta(h, 2, 3);
    inserirAresta(h, 3, 0);
    inserirAresta(h, 2, 4);
    inserirAresta(h, 0, 3);
    inserirAresta(h, 4, 2);
    inserirAresta(h, 1, 0);
    inserirAresta(h, 3, 4);

    exibirGrafo(h, V);

    Vertice g = moverVertice(h, 1);
    printf("\n");
    exibirGrafo(&g, 0);

    printf("\n");
    exibirGrafo(h, V);

    Vertice* copia = copiaTransposta(h, V);

    printf("\n");
    exibirGrafo(copia, V);
}