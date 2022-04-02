#include "din_lib.h"

#define FAILURE 0
#define SUCCESS 1
// common object; mutex is initialized statically
data_t data = {PTHREAD_MUTEX_INITIALIZER, 0};

void *thread_routine(void *arg) {
    pthread_mutex_t *mutex = &data.mutex;
    int errflag;

    args *ptr = (args *)arg;
    printf("In function thread id = %d\n", ptr->i);
    int i = 0;
    int length = 1;
    while (ptr->var_pred->predikat->logic[length - 1] != 3) {
        length++;
    }
    while (i * (ptr->n) + (ptr->i) < ptr->var_pred->count_var) {
        if (check_vars(ptr->var_pred->predikat,
                       ptr->var_pred->vars[i * (ptr->n) + ptr->i], length) == 1) {
            // lock mutex before value change
            errflag = pthread_mutex_lock(mutex);
            // check if pthread_mutex_lock call was successful
            if (errflag != 0) {
                printf("Caught error: %d\n", errflag);
            }
            data.value += 1; // change value
            // unlock mutex after value change
            errflag = pthread_mutex_unlock(mutex);
            // check if pthread_mutex_unlock call was successful
            if (errflag != 0) {
                printf("Error: %d\n", errflag);
            }
        }
        i++;
    }
    return NULL;
}

//  считываем предикат
char *read_predicate(FILE *file) {
    if (file == NULL) {
        return FAILURE;
    }
    char *str;
    char buff;
    size_t length = 1;
    str = malloc(sizeof(char));
    str[0] = '\0';
    buff = (char)fgetc(file);

    char *tmp;
    while (buff != '\n' && buff != EOF) {
        tmp = realloc(str, sizeof(char) * (++length));
        if (!tmp) {
            free(str);
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
int check(char *str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    size_t i = 0;
    while (i < length) {
        if (str[i] == '(') {
            i++;
            if ((str[i] == 'X') && (str[i + 1] == ' ')) {
                i += 2;
                if (((str[i] == '>') || (str[i] == '<') || (str[i] == '=')) &&
                    (str[i + 1] == ' ')) {
                    i += 2;
                } else {
                    if (((str[i] == '>') || (str[i] == '<')) && (str[i + 1] == '=') &&
                        (str[i + 2] == ' ')) {
                        i += 3;

                    } else {
                        printf("not >,< etc. or _ after that");
                        return FAILURE;
                    }
                }

            } else {
                printf("not _ after X or incorrect register");
                return FAILURE;
            }
            while (str[i] >= '0' && str[i] <= '9') {
                i++;
            }
            if (str[i] == ')') {
                if (str[i + 1] == ' ') {
                    i += 2;
                    if ((((str[i] == '&') && (str[i + 1] == '&')) ||
                         ((str[i] == '|') && (str[i + 1] == '|'))) &&
                        (str[i + 2] == ' '))
                        i += 3;
                    else
                        return FAILURE;
                } else {
                    if (str[i + 1] == '\0') {
                        i++;
                    } else {
                        printf("not _ after )");
                        return FAILURE;
                    }
                }
            } else {
                printf("not ) after count");
                return FAILURE;
            }
        } else {
            printf("not (");
            return FAILURE;
        }
    }
    return SUCCESS;
}
//  заполняем дерево предиката
int fill_predikate(char *str, predikate *predikat) {
    size_t count = count_var(str);
    if (count == 0) {
        return FAILURE;
    }
    predikat->comparison = malloc(sizeof(char) * count);
    predikat->value = malloc(sizeof(int) * count);
    predikat->logic = malloc(sizeof(char) * count);
    size_t i = 0;
    // printf("predicate:");
    for (size_t j = 0; j < count; j++) {
        size_t length = 0;
        while (str[i] != ')') {
            i++;
            length++;
        }
        char *pred = malloc(sizeof(char) * (length + 2));
        for (size_t k = 0; k < length + 1; k++) {
            pred[k] = str[i - length + k];
        }
        pred[length + 1] = '\0';
        create_predikate(pred, predikat, (int)j);
        i++;
        if (str[i] != '\0') {
            i++;
            if (str[i] == '&') {
                // printf(" && ");
                predikat->logic[j] = 1;
            } else {
                // printf(" || ");
                predikat->logic[j] = 2;
            }
        }
        i += 3;
    }
    // printf("\n");
    predikat->logic[count - 1] = 3;
    return SUCCESS;
}
int create_predikate(char *str, predikate *node, int count) {
    size_t i = 3;
    if ((str[i] == '<') && (str[i + 1] == ' ')) {
        // printf("(X < ");
        node->comparison[count] = 1;
        i += 2;
    } else {
        if ((str[i] == '>') && (str[i + 1] == ' ')) {
            // printf("(X > ");
            node->comparison[count] = 2;
            i += 2;
        } else {
            if ((str[i] == '=') && (str[i + 1] == ' ')) {
                // printf("(X = ");
                node->comparison[count] = 3;
                i += 2;
            } else {
                if ((str[i] == '<') && (str[i + 1] == '=')) {
                    // printf("(X <= ");
                    node->comparison[count] = 4;
                    i += 3;
                } else {
                    if ((str[i] == '>') && (str[i + 1] == '=')) {
                        // printf("(X >= ");
                        node->comparison[count] = 5;
                        i += 3;
                    }
                }
            }
        }
    }
    size_t length = 0;
    while (str[i + length] != ')') {
        length++;
    }
    char *buff = malloc(sizeof(char) * (length + 1));
    for (size_t j = 0; j < length; j++) {
        buff[j] = str[i + j];
    }
    buff[length] = '\0';
    node->value[count] = atoi(buff);
    // printf("%d)",node->value[count]);
    return SUCCESS;
}
//  считаем количество x
size_t count_var(char *str) {
    size_t count = 0;
    size_t i = 0;
    while (str[i] != '\0') {
        if (str[i] == '(') {
            count++;
        }
        // printf("Counting... str[i]:%c count:%d \n",str[i],count);
        i++;
    }

    // printf("Counted! count:%d \n",count);
    return count;
}
//  считываем числа
int read_var(FILE *file, arges *arg) {
    if (file == NULL) {
        return FAILURE;
    }
    if (feof(file)) {
        return FAILURE;
    }
    // printf("file open in rv\n");
    char buff = 't';
    char *tmp;
    arg->vars = malloc(sizeof(int));
    arg->count_var = 0;
    while (buff != EOF) {
        // printf("read next\n");
        size_t length;
        length = 1;
        char *str = malloc(sizeof(char));
        str[0] = '\0';
        buff = (char)fgetc(file);

        while (buff != ' ' && buff != EOF) {
            tmp = realloc(str, sizeof(char) * (++length));
            if (!tmp) {
                free(str);
                return FAILURE;
            }
            str = tmp;
            str[length - 2] = buff;
            str[length - 1] = '\0';
            buff = (char)fgetc(file);
        }
        int var = atoi(str);
        // printf("var %d ",var);
        int *tmp2;
        tmp2 = realloc(arg->vars, sizeof(int) * (arg->count_var + 1));
        if (!tmp2) {
            free(str);
            return FAILURE;
        }
        arg->vars = tmp2;
        arg->vars[arg->count_var] = var;
        arg->count_var++;
        // printf("readed\n");
        free(str);
    }
    // printf("readed\n");
    return SUCCESS;
}

//  проверяем подходит ли число
int check_vars(predikate *predikat, int var, int count) {
    // printf("count in cheking %d\n",count);
    int prev = check_var(predikat, var, 0);
    for (int i = 1; i < count; i++) {
        int curr = check_var(predikat, var, i);
        // 1 - &&
        // 2 - ||
        if (predikat->logic[i - 1] == 1) {
            // printf("%d && %d?\n",prev,curr);
            if (prev != 1 || curr != 1) {
                return 0;
            }
        }
        if (predikat->logic[i - 1] == 2) {
            // printf("%d || %d?\n",prev,curr);
            if (prev == 0 && curr == 0) {
                return 0;
            }
        }
        prev = 1;
    }
    return 1;
}
int check_var(predikate *predikat, int var, int count) {
    // 1 - <
    // 2 - >
    // 3 - =
    // 4 - <=
    // 5 - >=
    if (predikat->comparison[count] == 1) {
        // printf("%d < %d?\n",var,predikat->value[count]);
        if (var < predikat->value[count]) {
            return 1;
        } else {
            return 0;
        }
    }
    if (predikat->comparison[count] == 2) {
        // printf("%d > %d?\n",var,predikat->value[count]);
        if (var > predikat->value[count]) {
            return 1;
        } else {
            return 0;
        }
    }
    if (predikat->comparison[count] == 3) {
        // printf("%d = %d?\n",var,predikat->value[count]);
        if (var == predikat->value[count]) {
            return 1;
        } else {
            return 0;
        }
    }
    if (predikat->comparison[count] == 4) {
        // printf("%d <= %d?\n",var,predikat->value[count]);
        if (var <= predikat->value[count]) {
            return 1;
        } else {
            return 0;
        }
    }
    if (predikat->comparison[count] == 5) {
        // printf("%d >= %d?\n",var,predikat->value[count]);
        if (var >= predikat->value[count]) {
            return 1;
        } else {
            return 0;
        }
    }
    return FAILURE;
}
//  стартуем
int start(char *predicat_file, char *variables_file) {
    FILE *predicat = fopen(predicat_file, "r");
    if (!predicat) {
        printf("dont open predicat");
        return FAILURE;
    }
    printf("predicat.txt opend\n");
    char *str = read_predicate(predicat);
    if (str == FAILURE) {
        free(str);
        fclose(predicat);
        return FAILURE;
    }
    printf("curr predicat: %s\n", str);
    if (!check(str)) {
        free(str);
        fclose(predicat);
        return FAILURE;
    }
    printf("check success\n");
    predikate *predikat = malloc(sizeof(predikate));
    fill_predikate(str, predikat);
    printf("predicate created\n");
    FILE *file = fopen(variables_file, "r");
    if (!file) {
        free(str);
        free(predikat);
        fclose(predicat);
        printf("dont open variables");
        return FAILURE;
    }
    printf("variables.txt opend\n");
    arges *arg = malloc(sizeof(arges));
    if (read_var(file, arg) == FAILURE) {
        printf("fail\n");
        free(arg);
        free(str);
        free(predikat);
        fclose(predicat);
        fclose(file);
        return FAILURE;
    }
    arg->predikat = predikat;
    int N = 4; // потом исправить кол-во потоков
    args ptr[N];
    pthread_t threadIds[N];
    for (int j = 0; j < N; j++) {
        ptr[j].var_pred = arg;
        ptr[j].n = N;
        ptr[j].i = j;
        int errflag =
                pthread_create(&threadIds[j], NULL, thread_routine, (void *)&ptr[j]);
        if (errflag != 0) {
            printf("Caught error when create: %d\n", errflag);
        }
        // sleep(2);
    }
    for (int i = 0; i < N; i++) {
        int errflag = pthread_join(threadIds[i], NULL);
        if (errflag != 0) {
            printf("Caught error when join: %d\n", errflag);
        }
    }
    printf("result count: %d\n", data.value);
    // pthread_exit(NULL);
    return data.value;
}
