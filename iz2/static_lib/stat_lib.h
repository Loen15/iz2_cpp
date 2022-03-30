
#ifndef IZ2_STAT_LIB_H_
#define IZ2_STAT_LIB_H_
#include <stdio.h>
#include <stdlib.h>
typedef struct predicate predicate;
struct  predicate{
char* data;
predicate* left;
predicate* right;
};
//  считываем предикат
char* read_predicate(FILE* file);
//  проверяем корректность
int check (char* str);
//  заполняем дерево предиката
int fill_predicate(char *str,predicate* tree);
int create_predicate(char *str,predicate* node,int count);
void print_tree(predicate* tree,int count);
//  считаем количество x
int count_var(char* str);
//  считываем числа
int read_var(FILE* file);
//  проверяем подходит ли число
int check_vars(predicate* tree, int var);
int copy_tree(predicate* node,predicate* tree);
int check_var(predicate* tree, int var);
int free_memor(predicate* tree);
int start();
#endif  //IZ2_STAT_LIB_H_
