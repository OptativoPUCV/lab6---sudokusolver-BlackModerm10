#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int filas[9][10] = {0};
    int columnas[9][10] = {0};
    int subMatriz[9][10] = {0};
    for(int i = 0; i < 9; i++) {
      for(int j = 0; j < 9; j++) {
        int num = n->sudo[i][j];
        if(num != 0) {
          if(filas[i][num] || columnas[j][num] || subMatriz[i/3*3 + j/3][num]) {
            return 0;
          }
          filas[i][num] = 1;
          columnas[j][num] = 1;
          subMatriz[i/3*3 + j/3][num] = 1;
        }
      }
    }
  
    return 1;
}


List* get_adj_nodes(Node* n){
    List* list=createList();
    int i, j;

    for(i = 0; i < 9; i++) {
      for(j = 0; j < 9; j++) {
        if(n->sudo[i][j] == 0) {
          for(int num = 1; num <= 9; num++) {
            Node* newNode = copy(n);
            newNode->sudo[i][j] = num;
            
            if(is_valid(newNode)) { 
              pushBack(list, newNode);
            } else {
              free(newNode);
            }
          }
          return list; //No puedo creer que este return para detener la busqueda apenas encuentre la casilla era la solucion :c
        }
      }
    }
    return list;
}


int is_final(Node* n){
  int i, j;
  
  for(i = 0; i < 9; i++) {
    for(j = 0; j < 9; j++) {
      if(n->sudo[i][j] == 0) {
        return 0;
      }
    }
  }
   return 1; 
}

Node* DFS(Node* initial, int* cont){
  Stack* stack = createStack();
  push(stack, initial);
  
  while (!is_empty(stack)) {
    Node* current = top(stack);
    pop(stack);
    (*cont)++;

    if (is_final(current)) {
      return current;
    }
    
    int min_row = -1;
    int min_col = -1;
    int min_options = 10;
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (current->sudo[i][j] == 0) {
          int options = 0;
          for (int num = 1; num <= 9; num++) {
            if (is_valid(current)) {
              options++;
            }
            current->sudo[i][j] = 0;
          }
          if (options < min_options) {
              min_row = i;
              min_col = j;
              min_options = options;
            }
          }
        }
      }
    for (int num = 1; num <= 9; num++) {
      current->sudo[min_row][min_col] = num;
      if (is_valid(current)) {
        Node* adj_node = copy(current);
        push(stack, adj_node);
      }
    }
  }
    
  return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/