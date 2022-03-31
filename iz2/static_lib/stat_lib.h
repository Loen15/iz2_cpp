#ifndef IZ2_STAT_LIB_H_
#define IZ2_STAT_LIB_H_
#include <stdio.h>
#include <stdlib.h>
typedef struct predikate predikate;
struct  predikate{
    char* comparison;
// 1 - <
// 2 - >
// 3 - =
// 4 - <=
// 5 - >=
    int* value;
    char*  logic;
// 1 - &&
// 2 - ||
// 3 - end
};
//  считываем предикат
char* read_predicate(FILE* file);
//  проверяем корректность
int check (char* str);
//  заполняем дерево предиката
int fill_predikate(char *str,predikate* predikat);
int create_predikate(char *str,predikate* node,int count);
//  считаем количество x
size_t count_var(char* str);
//  считываем числа
int read_var(FILE* file);
//  проверяем подходит ли число
int check_vars(predikate* predikat,int var,int count);
int check_var(predikate* predikat,int var,int count);
int start(char* predicat,char* variables);
#endif  //IZ2_STAT_LIB_H_