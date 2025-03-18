#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int V = 4;

typedef struct s{
    struct s* prox;
    int adj;
    int custo;
}NO;


typedef struct {
    NO* inicio;
    int flag;
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
//se falg=2 quer dizer que esse elemento eh conectado com i
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

/*
void profMatricial(Matriz m[V][V], int i, flag[V], bool ciclo){
    flag[i] = 1;
    for(int j = 0; j <= V; i++){
        if(flag[j] == 1) ciclo = true;
        if(m[i][j] == 1 && flag[j] == 0) prof(m, j, flag, ciclo);
    }
    flag[i] = 2;
}
*/

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

void largura(Vertice* g, int i){
    zerarFlags(g);
    Fila* f;
    inicializarFila(&f);
    EntrarFila(&f, i);
    g[i].flag = 1; //discovered
    while(f){
        i = SairFila(&f);
        g[i].flag = 2;
        NO* p = g[i].inicio;
        while(p){
            if(g[p->adj].flag == 0){
                g[p->adj].flag = 1;
                EntrarFila(&f, p->adj);
            }
            p = p->prox;
        }
    }
}


//Crie um grafo h movendo as arestas que incidem sobre i, DICA: nao use malloc
//fazer uma copia transposta de g em um novo grafo h

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