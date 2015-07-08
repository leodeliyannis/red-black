#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>

#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_BG_BLACK      "\x1b[100m"
#define ANSI_BG_RED        "\x1b[101m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include <sys/time.h>
#include <algorithm>
#include <string>

#define INFTO 1123456789
#define isvalid(X) (((X) != NIL) && ((X) != NULL))
#define RB_BALANCE

#define ERR_SYNTAX() fprintf(stderr, "ERROR: Invalid syntax\n")
#define ERR_NO_NODE() fprintf(stderr, "ERROR: Tried to remove inexistent node\n")
#define ERR_DUP_NODE() fprintf(stderr, "ERROR: No duplicate nodes allowed\n")
#define ERR_ALLOC() { fprintf(stderr, "FATAL: Could not allocate memory\n"); exit(1); }

using namespace std;

enum color {BLACK, RED};
struct nodo{
	nodo *p, *s[2];
	int k;
	color c;
};
typedef nodo* node;

struct Tree{
	node root, NIL;
	
	node menor(node T){ return (T->s[0] != NIL ? menor(T->s[0]) : T); }
	node maior(node T){ return (T->s[1] != NIL ? maior(T->s[1]) : T); }
	
	node procura(node T, int v){
		if(T == NIL) return NULL;
		if(v == T->k) return T;
		return procura(T->s[v > T->k], v);
	}
	
	node aloca(int v) {
		node N = new (nothrow) nodo;
		if(N == NULL)
			ERR_ALLOC();
		N->p = NIL;
		N->s[0] = N->s[1] = NIL;
		N->k = v;
		N->c = RED;
		return N;
	}
	
	void leftRotate(node *T, node y){
		node x = y->s[1];
		y->s[1] = x->s[0];
		if (x->s[0] != NIL)
			x->s[0]->p = y;
		x->p = y->p;
		if (x->p == NIL)
			(*T) = x;
		else if (y == y->p->s[1])
			y->p->s[1] = x;
		else y->p->s[0] = x;
		x->s[0] = y;
		y->p = x;
	}
	
	void rightRotate(node *T, node y){
		node x = y->s[0];
		y->s[0] = x->s[1];
		if (x->s[1] != NIL)
			x->s[1]->p = y;
		x->p = y->p;
		if (x->p == NIL)
			(*T) = x;
		else if (y == y->p->s[0])
			y->p->s[0] = x;
		else y->p->s[1] = x;
		x->s[1] = y;
		y->p = x;
	}
	
#ifdef RB_BALANCE
	void insert_fixup(node *T, node z){
		// iterate until z is not the T and z's p c is red
		while (z != *T && z->p->c == RED){
			node y;
			// Find uncle and store uncle in y
			if (z->p == z->p->p->s[0])
				y = z->p->p->s[1];
			else
				y = z->p->p->s[0];
	 
			// If uncle is RED, do following
			// (i)  Change c of p and uncle as BLACK
			// (ii) Change c of grandparent as RED
			// (iii) Move z to grandparent
			if (y->c == RED){
				y->c = BLACK;
				z->p->c = BLACK;
				z->p->p->c = RED;
				z = z->p->p;
			}
	 
			// Uncle is BLACK, there are four cases (LL, LR, RL and RR)
			else{// Left-Left (LL) case, do following
				// (i)  Swap c of p and grandparent
				// (ii) Right Rotate Grandparent
				if (z->p == z->p->p->s[0] && z == z->p->s[0]){
					color ch = z->p->c ;
					z->p->c = z->p->p->c;
					z->p->p->c = ch;
					rightRotate(T,z->p->p);
				}
	 
				// Left-Right (LR) case, do following
				// (i)  Swap c of current node  and grandparent
				// (ii) Left Rotate Parent
				// (iii) Right Rotate Grand Parent
				if (z->p == z->p->p->s[0] && z == z->p->s[1]){
					color ch = z->c ;
					z->c = z->p->p->c;
					z->p->p->c = ch;
					leftRotate(T,z->p);
					rightRotate(T,z->p->p);
				}
				
				// Right-Right (RR) case, do following
				// (i)  Swap c of p and grandparent
				// (ii) Left Rotate Grandparent
				if (z->p == z->p->p->s[1] && z == z->p->s[1]){
					color ch = z->p->c ;
					z->p->c = z->p->p->c;
					z->p->p->c = ch;
					leftRotate(T,z->p->p);
				}
	 
				// Right-Left (RL) case, do following
				// (i)  Swap c of current node  and grandparent
				// (ii) Right Rotate Parent
				// (iii) Left Rotate Grand Parent
				if (z->p == z->p->p->s[1] && z == z->p->s[0]){
					color ch = z->c ;
					z->c = z->p->p->c;
					z->p->p->c = ch;
					rightRotate(T,z->p);
					leftRotate(T,z->p->p);
				}
			}
		}
		(*T)->c = BLACK; //keep T always black
	}
#endif // RB_BALANCE;

	node insere (node T, int v) {
		if(T == NIL){
			T = aloca(v);
			T->c = BLACK;
		}else if(T->k != v){
			if(T->s[v > T->k] != NIL)
				insere(T->s[v > T->k], v);
			else{
				T->s[v > T->k] = aloca(v);
				T->s[v > T->k]->p = T;
			}
		}
		return T;
	}
	
	void prefixo(node T) {
		if(T != NIL){
			printf("%d", T->k);
			if(T->s[0] != NIL){ printf(" "); prefixo(T->s[0]); }
			if(T->s[1] != NIL){ printf(" "); prefixo(T->s[1]); }
		}
	}
	  
	void infixo(node T) {
		if(T != NIL){
			if(T->s[0] != NIL){ infixo(T->s[0]); printf(" "); }
			printf("%d", T->k);
			if(T->s[1] != NIL){ printf(" "); infixo(T->s[1]); }
		}
	}
	 
	void posfixo(node T) {
		if(T != NIL){
			if(T->s[0] != NIL){ posfixo(T->s[0]); printf(" "); }
			if(T->s[1] != NIL){ posfixo(T->s[1]); printf(" "); }
			printf("%d", T->k);
		}
	}

	node retira(node T, int v) {
		if(T == NIL) return T;
		else if(v < T->k) T->s[0] = retira(T->s[0], v);
		else if(v > T->k) T->s[1] = retira(T->s[1], v);
		else{
			if(T->s[0] == NIL && T->s[1] == NIL){ // no child
				delete T;
				T = NIL;
			}
			else if(!(T->s[0] != NIL && T->s[1] != NIL)){ // one child
				node aux = T;
				T = (T->s[0] != NIL) ? T->s[0] : T->s[1];
				delete aux; 
			}
			else{
				node aux = maior(T->s[0]);
				T->k = aux->k;
				T->s[0] = retira(T->s[0], aux->k);
			}
		}
		return T;
	}

	void libera(node T){
		if(T != NIL){
			if(T->s[0] != NIL) libera(T->s[0]);
			if(T->s[1] != NIL) libera(T->s[1]);
			delete T;
		}
		//return T;
	}

	void initialize(){ 
		NIL = aloca(-INFTO);
		NIL->p = NIL->s[0] = NIL->s[1] = NULL;
		NIL->c = BLACK;
		root = NIL;
	}
	
	void preorder(){ prefixo(root); }
	void inorder(){ infixo(root); }
	void postorder(){ posfixo(root); }
	
	node search(int v){ return procura(root, v); }
	
	void push(int v){ 
		root = insere(root, v);	
#ifdef RB_BALANCE		
		node z = search(v);
		insert_fixup(&root, z);
#endif
	}
	
	void pop(int v){ 
#ifdef RB_BALANCE	
		node aux = search(v);
		if(aux == NULL){
			ERR_NO_NODE();
			return;
		}
#endif	
		root = retira(root, v);
#ifdef RB_BALANCE
		//insert_fixup(&root, aux->p);
#endif
	}
	void clear(){ libera(root); delete NIL; }
	
	// aux display and verification routines, helpful but not essential
	struct galho{
		galho *ant;
		string s;
	};
	 
	void exibe_galhos(galho *p){
		if (!p) return;
		exibe_galhos(p->ant);
		printf("%s", p->s.c_str());
	}
	 
	// this is very haphazzard
	void mostra(nodo *T, galho *ant, int esq){
		if (T == NIL) return;
	 
		galho este = { ant, "    " };
		string anterior = este.s;
		mostra(T->s[0], &este, 1);
	 
		if (!ant)
			este.s = "---";
		else if (esq) {
			este.s = ".--";
			anterior = "   |";
		} else {
			este.s = "`--";
			ant->s = anterior;
		}
	 
		exibe_galhos(&este);
		printf("%s%d%s\n", (T->c ? ANSI_BG_RED : ANSI_BG_BLACK), T->k, ANSI_COLOR_RESET);
	 
		if (ant) ant->s = anterior;
		este.s = "   |";
	 
		mostra(T->s[1], &este, 0);
		if (!ant) puts("");
	}
	
	void print_info_util(node n){
		if(n == NULL){
			printf("NULL");
			return;
		}
		printf((n == NIL) 
			? ANSI_BG_BLACK "NIL" 
			: "%s%d", (n->c ? ANSI_BG_RED : ANSI_BG_BLACK), n->k);
		puts(ANSI_COLOR_RESET);
	}
	
	void print_info(node n){
		if(n == NULL){
			puts("Node does not exist.");
			return;
		}
		printf("Parent: ");
		print_info_util(n->p);
		printf("Key: ");
		print_info_util(n);
		printf("Left: ");
		print_info_util(n->s[0]);
		printf("Right: ");
		print_info_util(n->s[1]);
	}
};

void strcaps(char *s){
	int i;
	for(i=0; s[i]; i++)
		s[i] = toupper(s[i]);
}

// FUNÇÕES DE INTERAÇÃO COM O USUÁRIO
//#define GUI
#ifdef GUI

void espere() {  
    printf("Press enter to continue...\n");
    getchar();
    getchar();
}

void menu( ) {
	system("clear");
	puts("I x - insere nodo de valor X\n"
		 "P x - informa se o nodo de valor X existe\n"
		 "R x - remove o nodo de valor X"
		 "PREFIXA - percorre a árvore na ordem: nodo, esq, dir\n"
		 "INFIXA - percorre a árvore na ordem: esq, nodo, dir\n"
		 "POSFIXA - percorre a árvore na ordem: esq, dir, nodo\n"
		 "M - mostra o estado atual da árvore\n")
	return;
}
#endif

// FIM DAS FUNÇÕES DE INTERAÇÃO COM O USUÁRIO

#define eq(a,b) (strcmp(a,b)==0)

int main(void){
	Tree T;
	T.initialize();
	int v, rt;
	char op[8];
#ifdef GUI
	menu();
#endif
	while(scanf("%s", op)!=EOF){
		strcaps(op);
		if(eq(op,"I")){
			scanf("%d", &v);
			T.push(v);
		}else
		if(eq(op,"RAND")){
			//int b, e;
			scanf("%d", &v);
			//scanf("%d %d", &b, &e);
			for(int it = 0; it < v; it++){
				T.push(rand() % INFTO);
			}
		}else
		
		if(eq(op,"INFIXA")) { T.inorder(); printf("\n"); }else
		if(eq(op,"PREFIXA")){ T.preorder(); printf("\n"); }else
		if(eq(op,"POSFIXA")){ T.postorder(); printf("\n"); }else
		
		if(eq(op,"P")){
			scanf("%d", &v);
			printf("%d%s existe\n", v, T.search(v) ? "" : " nao");
		}else
		if(eq(op,"R")){
			scanf("%d", &v);
			T.pop(v);
		}else
		if(eq(op,"ROTATE")){
			scanf("%d %d", &v, &rt);
			node aux = T.search(v);
			if(aux){
				rt ? T.rightRotate(&(T.root), aux)
				: T.leftRotate(&(T.root), aux);
			}
		}else
		if(eq(op,"M")){
			T.mostra(T.root, 0, 0);
		}else
		if(eq(op,"INFO")){
			scanf("%d", &v);
			T.print_info(T.search(v));
			putchar('\n');
		}else{
			ERR_SYNTAX();
		}
		
#ifdef GUI
		menu();
#endif
	}
	T.clear();
	printf(ANSI_COLOR_RESET);
	return 0;
}
