
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
int fill_predicate(char *str,predicate* tree){
    int count = count_var(str);
    if (count == FAILURE){
        return FAILURE;
    }
    int flag;
    printf("in fp count=%d \n",count);
    flag = create_predicate(str,tree,count);
    printf("Current predicat: ");
    print_tree(tree,count);
    printf("\n");
    return flag;
}
int create_predicate(char *str,predicate* node,int count){
    printf("create predicate with str: -%s- count %d\n",str,count);
    if (count < 1){
        return FAILURE;
    }
    size_t i =0;
    if (count == 1) {
        while (str[i] != ' ') {
            i++;
        }
        i++;
        if ((str[i] == '<') && (str[i + 1] == ' ')) {
            node->data = malloc(sizeof(char) * 2);
            node->data[0] = '<';
            node->data[1] = '\0';
            i += 2;
        } else {
            if ((str[i] == '>') && (str[i + 1] == ' ')) {
                node->data = malloc(sizeof(char) * 2);
                node->data[0] = '>';
                node->data[1] = '\0';
                i += 2;
            } else {
                if ((str[i] == '=') && (str[i + 1] == ' ')) {
                    node->data = malloc(sizeof(char) * 2);
                    node->data[0] = '=';
                    node->data[1] = '\0';
                    i += 2;
                } else {
                    if ((str[i] == '<') && (str[i + 1] == '=')) {
                        node->data = malloc(sizeof(char) * 3);
                        node->data[0] = '<';
                        node->data[1] = '=';
                        node->data[2] = '\0';
                        i += 3;
                    } else {
                        if ((str[i] == '>') && (str[i + 1] == '=')) {
                            node->data = malloc(sizeof(char) * 3);
                            node->data[0] = '>';
                            node->data[1] = '=';
                            node->data[2] = '\0';
                            i += 3;
                        }
                    }
                }
            }
        }
        predicate* left = malloc(sizeof(predicate)) ;
        left->left=NULL;
        left->right=NULL;
        left->data = malloc(sizeof(char)* 2);
        left->data[0]='X';
        left->data[1]='\0';
        node->left = left;
        predicate* right = malloc(sizeof(predicate));
        right->left=NULL;
        right->right=NULL;
        size_t tmp = 0;
        while (str[i+tmp] != ')'){
            tmp++;
        }
        right->data = malloc(sizeof(char)*(tmp+1));
        for (size_t j=0;j<tmp;j++){
            right->data[j]=str[i+j];
        }
        right->data[tmp]='\0';
        node->right= right;
        return SUCCESS;

    }
    while (str[i] != '\0'){
        i++;
    }
    size_t length = i;
    while(str[i] != '('){
        i--;
    }
    if (str[i-2] == '&'){
        node->data = malloc(sizeof(char)* 3);
        node->data[0]='&';
        node->data[1]='&';
        node->data[2]='\0';
    }
    if (str[i-2] == '|'){
        node->data = malloc(sizeof(char)* 3);
        node->data[0]='|';
        node->data[1]='|';
        node->data[2]='\0';
    }
    predicate* right = malloc(sizeof(predicate));
    predicate* left = malloc(sizeof(predicate));
    node->right=right;
    node->left =left;
    char* right_str= malloc(sizeof(char)*(length-i+2));
    for(size_t j=i;j<length+2;j++){
        right_str[j-i]=str[j];
    }
    i-=4;
    char* left_str = malloc(sizeof(char)* (i+1));
    for(size_t j=0;j<i;j++){
        left_str[j]=str[j];
    }
    left_str[i]='\0';
    create_predicate(left_str,left,count - 1);
    create_predicate(right_str,right,1);
    return SUCCESS;
}
void print_tree(predicate* tree,int count){
    if (count == 1){
        printf("(%s %s %s)",tree->left->data,tree->data,tree->right->data);
    }else{
        print_tree(tree->left,count-1);
        printf(" %s ",tree->data);
        print_tree(tree->right,1);
    }

}
//  считаем количество x
int count_var(char* str){
    int count = 0;
    size_t i = 0;
    while (str[i]!='\0'){
        if (str[i]=='('){
            count++;
        }
        printf("Counting... str[i]:%c count:%d \n",str[i],count);
        i++;
    }
    
    printf("Counted! count:%d \n",count);
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
    /*free(str);
    free(tmp);*/
    return var;
}

//  проверяем подходит ли число
int check_vars(predicate* tree, int var){
    predicate* copy = malloc(sizeof(predicate));
    copy_tree(copy,tree);
    return check_var(copy,var);
}
int copy_tree(predicate* node,predicate* tree){
    if (tree->left!=NULL){
        predicate* left = malloc(sizeof(predicate));
        node->left =left;
        copy_tree(node->left,tree->left);
    }
    else{
        node->left=NULL;
    }
    if (tree->right!=NULL){
        predicate* right = malloc(sizeof(predicate));
        node->right=right;
        copy_tree(node->right,tree->right);
    }
    else{
        node->right=NULL;
    }
    size_t i =0;
    while(tree->data[i]!='\0'){
        i++;
    }
    node->data = malloc(sizeof(char) * (i+1));
    for (size_t j=0;j< i+1;j++){
        node->data[j]=tree->data[j];
    }
    return SUCCESS;
}
int check_var(predicate* tree,int var){
    if(tree->right->right!=NULL){
        check_var(tree->right,var);
    }

        int tmp = atoi(tree->right->data);
        if (tree->data[0]=='>' && tree->data[1]=='\0'){
            //printf("\n%d > %d\n",var, tmp);
            if (var > tmp){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
        if (tree->data[0]=='<' && tree->data[1]=='\0'){
            //printf("\n%d < %d\n",var, tmp);
            if (var < tmp){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
        if (tree->data[0]=='=' && tree->data[1]=='\0'){
            //printf("\n%d = %d\n",var, tmp);
            if (var == tmp){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
        if (tree->data[0]=='>' && tree->data[1]=='='){
            //printf("\n%d >= %d\n",var, tmp);
            if (var >= tmp){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
        if (tree->data[0]=='<' && tree->data[1]=='='){
            //printf("\n%d <= %d\n",var, tmp);
            if (var <= tmp){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
        if (tree->data[0]=='&'){
            check_var(tree->left,var);
            //printf("\n%c && %c\n",tree->left->data[0], tree->right->data[0]);
            if (tree->left->data[0]=='1' && tree->right->data[0]=='1'){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
        if (tree->data[0]=='|'){
            check_var(tree->left,var);
            //printf("\n%c || %c\n",tree->left->data[0], tree->right->data[0]);
            if (tree->left->data[0]=='1' || tree->right->data[0]=='1'){
                tree->data[0]='1';
                tree->data[1]='\0';
            }
            else{
                tree->data[0]='0';
                tree->data[1]='\0';
            }
        }
    return atoi(tree->data);
}
int free_memor(predicate* tree){
    if(tree->left!=NULL){
        free_memor(tree->left);
    }
    if(tree->right!=NULL){
        free_memor(tree->right);
    }
    free(tree->data);
    free(tree);
    return SUCCESS;
}
int start(){
    FILE* predicat = fopen("../iz2/predicat.txt","r");
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
    predicate* tree = malloc(sizeof(predicate));
    if(!fill_predicate(str,tree)){
        printf("don't crete predicate's tree\n");
        fclose(predicat);
        return FAILURE;
    }
    printf("predicate's  tree created\n");
    int var;
    FILE* file = fopen("../iz2/variables.txt", "r");
    if(!file){
        printf("dont open variables");
        return FAILURE;
    }
    printf("variables.txt opend\n");
    var = read_var(file);

    int count=0;
    while (var!=FAILURE) {
        count +=check_vars(tree,var);
        printf("check var %d, so count %d\n",var,count);
        var = read_var(file);
    }
    free(tree);
    free(str);
    printf("final count: %d\n",count);
    fclose(predicat);
    fclose(file);
    return count;
}