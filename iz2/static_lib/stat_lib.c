#include "stat_lib.h"

#define FAILURE 0
#define SUCCESS 1
//  считываем предикат
char* read_predicate(FILE* file){
    if(file == NULL){
        return FAILURE;
    }
    char *str;
    char buff;
    size_t length = 1;
    str = malloc( sizeof(char));
    str[0] = '\0';
    buff = (char)fgetc(file);

    char *tmp;
    while (buff != '\n' && buff!=EOF) {
        tmp = realloc(str, sizeof(char) * (++length));
        if (!tmp) {
            return FAILURE;
        }
        str = tmp;
        str[length - 2] = buff;
        str[length - 1] = '\0';
        buff = (char)fgetc(file);
    }

    return str;
}
//  проверяем корректность
int check (char* str) {

    size_t length = 0;
    while (str[length]!='\0'){
        length++;
    }
    size_t i = 0;
    while (i < length){
        if (str[i] == '(') {
            i++;
            if((str[i] == 'X') && (str[i+1] == ' ')){
                i+=2;
                if (((str[i] == '>') || (str[i] == '<') || (str[i] == '=')) && (str[i+1] == ' ')){
                    i+=2;
                }
                else{
                    if (((str[i] == '>') || (str[i] == '<')) && (str[i+1] == '=') && (str[i+2] == ' '))
                    {
                        i+=3;

                    }
                    else{
                        printf("not >,< etc. or _ after that");
                        return FAILURE;
                    }
                }

            }
            else{
                printf("not _ after X or incorrect register");
                return FAILURE;
            }
            while (str[i] >= '0' && str[i] <= '9'){
                i++;
            }
            if (str[i] == ')')
            {
                if  (str[i+1] == ' '){
                    i+=2;
                    if ((((str[i] == '&') && (str[i+1] == '&')) || ((str[i] == '|') && (str[i+1] == '|'))) && (str[i+2] == ' '))
                        i+=3;
                    else return FAILURE;
                }
                else{
                    if (str[i+1] == '\0') {
                        i++;
                    }
                    else {
                        printf("not _ after )");
                        return FAILURE;
                    }
                }
            }
            else {
                printf("not ) after count");
                return FAILURE;
            }
        }
        else{
            printf("not (");
            return FAILURE;
        }
    }
    return SUCCESS;
}
//  заполняем дерево предиката
int fill_predikate(char *str,predikate* predikat){
    size_t count = count_var(str);
    predikat->comparison = malloc(sizeof(char) * count);
    predikat->value = malloc(sizeof(int ) *count);
    predikat->logic = malloc(sizeof(char) * count);
    size_t i =0;
    printf("predicate:");
    for (size_t j = 0;j<count;j++){
        size_t length=0;
        while (str[i]!=')'){
            i++;
            length++;
        }
        char* pred = malloc(sizeof(char) * (length+2));
        for (size_t k = 0;k<length+1;k++){
            pred[k]=str[i-length+k];
        }
        pred[length+1]='\0';
        create_predikate(pred,predikat,(int)j);
        i++;
        if(str[i]!='\0') {
            i++;
            if (str[i] == '&') {
                printf(" && ");
                predikat->logic[j] = 1;
            } else {
                printf(" || ");
                predikat->logic[j] = 2;
            }
        }
        i+=3;
    }
    printf("\n");
    predikat->logic[count-1] = 3;
    return  SUCCESS;
}
int create_predikate(char *str,predikate* node,int count){
    size_t i =3;
    if ((str[i] == '<') && (str[i + 1] == ' ')) {
        printf("(X < ");
        node->comparison[count]=1;
        i += 2;
    } else {
        if ((str[i] == '>') && (str[i + 1] == ' ')) {
            printf("(X > ");
            node->comparison[count]=2;
            i += 2;
        } else {
            if ((str[i] == '=') && (str[i + 1] == ' ')) {
                printf("(X = ");
                node->comparison[count]=3;
                i += 2;
            } else {
                if ((str[i] == '<') && (str[i + 1] == '=')) {
                    printf("(X <= ");
                    node->comparison[count]=4;
                    i += 3;
                } else {
                    if ((str[i] == '>') && (str[i + 1] == '=')) {
                        printf("(X >= ");
                        node->comparison[count]=5;
                        i += 3;
                    }
                }
            }
        }
    }
    size_t length = 0;
    while (str[i+length]!=')'){
        length++;
    }
    char* buff= malloc(sizeof(char) * (length+1));
    for(size_t j = 0;j<length;j++){
        buff[j]=str[i+j];
    }
    buff[length]='\0';
    node->value[count] = atoi(buff);
    printf("%d)",node->value[count]);
    return  SUCCESS;
}
//  считаем количество x
size_t count_var(char* str){
    size_t count = 0;
    size_t i = 0;
    while (str[i]!='\0'){
        if (str[i]=='('){
            count++;
        }
//        printf("Counting... str[i]:%c count:%d \n",str[i],count);
        i++;
    }

//    printf("Counted! count:%d \n",count);
    return count;
}
//  считываем числа
int read_var(FILE* file){
    if (file == NULL){
        return FAILURE;
    }
    if (feof(file)){
        return  FAILURE;
    }
    char *str;
    char buff;
    size_t length = 1;
    str = malloc( sizeof(char));
    str[0] = '\0';
    buff = (char)fgetc(file);

    char *tmp;
    while (buff != ' ' && buff != EOF) {
        tmp = realloc(str, sizeof(char) * (++length));
        if (!tmp) {
            return FAILURE;
        }
        str = tmp;
        str[length - 2] = buff;
        str[length - 1] = '\0';
        buff = (char)fgetc(file);
    }
    int var = atoi(str);
    return var;
}

//  проверяем подходит ли число
int check_vars(predikate* predikat,int var,int count){
//    printf("count in cheking %d\n",count);
    int prev = check_var(predikat,var,0);
    for (int i = 1;i<count;i++){
        int curr = check_var(predikat,var,i);
        // 1 - &&
        // 2 - ||
        if (predikat->logic[i-1]== 1){
//            printf("%d && %d?\n",prev,curr);
            if (prev!=1 || curr!=1){
                return 0;
            }
        }
        if (predikat->logic[i-1]== 2){
//            printf("%d || %d?\n",prev,curr);
            if (prev==0 && curr==0){
                return 0;
            }
        }
        prev = 1;
    }
    return 1;
}
int check_var(predikate* predikat,int var,int count){
    // 1 - <
// 2 - >
// 3 - =
// 4 - <=
// 5 - >=
    if (predikat->comparison[count] == 1){
//        printf("%d < %d?\n",var,predikat->value[count]);
        if (var < predikat->value[count]){
            return 1;
        }
        else return 0;
    }
    if (predikat->comparison[count] == 2){
//        printf("%d > %d?\n",var,predikat->value[count]);
        if (var > predikat->value[count]){
            return 1;
        }
        else return 0;
    }
    if (predikat->comparison[count] == 3){

//        printf("%d = %d?\n",var,predikat->value[count]);
        if (var == predikat->value[count]){
            return 1;
        }

        else return 0;
    }
    if (predikat->comparison[count] == 4){
//        printf("%d <= %d?\n",var,predikat->value[count]);
        if (var <= predikat->value[count]){
            return 1;
        }
        else return 0;
    }
    if (predikat->comparison[count] == 5){
//        printf("%d >= %d?\n",var,predikat->value[count]);
        if (var >= predikat->value[count]){
            return 1;
        }
        else return 0;
    }
    return  FAILURE;
}
int start(char* predicat_file,char* variables_file){
    FILE* predicat = fopen(predicat_file,"r");
    if(!predicat){
        printf("dont open predicat");
        return FAILURE;
    }
    printf("predicat.txt opend\n");
    char* str = read_predicate(predicat);
    if (str == FAILURE){
        fclose(predicat);
        return FAILURE;
    }
    printf("curr predicat: %s\n",str);
    if(!check(str)){
        fclose(predicat);
        return FAILURE;
    }
    printf("check success\n");
    predikate* predikat = malloc(sizeof(predikate));
    fill_predikate(str,predikat);
    printf("predicate created\n");
    int var;
    FILE* file = fopen(variables_file, "r");
    if(!file){
        printf("dont open variables");
        return FAILURE;
    }
    printf("variables.txt opend\n");
    var = read_var(file);
    int length=1;
    while(predikat->logic[length-1]!=3){
        //printf("%d",predikat->logic[length-1]);
        length++;
    }
    int count=0;
    while (var!=FAILURE) {
        count +=check_vars(predikat,var,length);
        printf("check var %d, so count %d\n",var,count);
        var = read_var(file);
    }
    free(predikat->comparison);
    free(predikat->value);
    free(predikat->logic);
    free(predikat);
    free(str);
    printf("final count: %d\n",count);
    fclose(predicat);
    fclose(file);
    return count;
}