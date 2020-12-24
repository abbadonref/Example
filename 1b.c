#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define ONE_SENT 64
#define NUMS_SENT 8


int read_text(char ***text);
void free_memory(char **text, int size);
int is_dubl(char **text, int size, char *str);
char *replace_word(char *sentence);

int count_commas(char *sentence);
int compare(const void *a, const void *b);
char **sort_by_commas_count(char **text, int size);


int read_text(char ***text){
    int sent_count = NUMS_SENT;
    int cur_sent =0;
    int i = 0, is_new = 1;
    int max_simb = ONE_SENT;
    char c;
    char *sent, *tmp, **tmp2;
    *text = (char**)malloc(sent_count*sizeof(char*));
    if (*text == NULL){
        printf("No access to memory");
        return -1;
    }
    while((c= getchar()) != '\n'){
        if(is_new){
            is_new = 0;
            sent = (char*)malloc(max_simb*sizeof(char));
            if (sent == NULL){
                printf("No access to memory");
                free_memory(*text, cur_sent);
                return -1;
            }

            if(cur_sent ==sent_count){
                sent_count +=NUMS_SENT;
                tmp2 = (char**)realloc(*text, sent_count*sizeof(char *));
                if (tmp2 == NULL){
                    printf("No access to memory");
                    free_memory(*text, cur_sent);
                    free(sent);
                    return -1;
                }
                *text = tmp2;
            }


        }
        if (i == max_simb - 1){
            max_simb += ONE_SENT;
            tmp = realloc(sent, max_simb*sizeof(char));
            if (tmp == NULL){
                printf("No access to memory");
                free_memory(*text, cur_sent);
                free(sent);
                return -1;
            }
            sent = tmp;
        }

        sent[i] = c;
        i++;
        if(c == '.'){
            sent[i] = '\0';
            i = 0;
            if (!is_dubl(*text, cur_sent, sent)){
                (*text)[cur_sent] = sent;
                cur_sent++;
                is_new = 1;
                max_simb = ONE_SENT;
            }
        }
    }
    return cur_sent;
}


int is_dubl(char **text, int size, char *str)
{
    for(int i = 0; i < size; i++){
        if (!strcasecmp(text[i], str))
            return 1;
    }
    return 0;
}

char *replace_word(char *sentence)
{
	int len = strlen(sentence);
	char *new = calloc(len + 1, sizeof(char)); // Измененное предложение
	char *p = sentence; // Указатель на исходное предложения для поиска подстроки
	char *prev = p;
	char find[] = "Robin"; // Подстрока поиска
	char replace[] = "I am Batman"; // Строка замены
	int find_len = strlen(find), replace_len = strlen(replace);

	while ((p = strstr(p, find)) != NULL) // Поиск подстроки
	{
		len += replace_len - find_len; // Разница размеров заменяемого и искомого слова прибавляется к длине предложения
		new = realloc(new, (len + 1) * sizeof(char));
		strncat(new, prev, p-prev); // Копирование части предложения до заменяемого слова
		strncat(new, replace, replace_len); // Копирование строки замены
		p += find_len; // Переход к следующей части поиска слова
		prev = p;
	}
	p = &sentence[strlen(sentence)]; // Указатель на конец строки
	strncat(new, prev, p-prev); // Копирование остатка строки
	free(sentence);
	sentence = new; // Переприсваивание строки
	return sentence;
}


int count_commas(char *sentence)
{
    int commas = 0;
    for (int i = 0; sentence[i] != '\0'; i++) // посимвольный перебор предложения
        if (sentence[i] == ',')
            commas++;
    return commas;
}

int compare(const void *a, const void *b) // функция сравнения двух предложений
{
    int c1 = count_commas(*(char **)a);
    int c2 = count_commas(*(char **)b);

    if (c1 > c2)
        return 1;
    if (c1 < c2)
        return -1;
    return 0;
}

char **sort_by_commas_count(char **text, int size)
{
    qsort(text, size, sizeof(char *), compare); // функция сортировки
    return text;
}



void print_text(char **text, int size){
    for (int i =0; i < size; i++ )
        printf("%s", text[i]);
    printf("\n");
}

void free_memory(char **text, int size){
    for (int i =0; i <size; i++)
        free(text[i]);
    free(text);
}


int main(){
    setlocale(LC_ALL, "Rus");
    char user_choice, **text;
    int size;
    printf("Введите текст: ");
    size = read_text(&text);
    if (size == -1){
        return -1;
    }

    printf("Выберите действие:\n"
           "1 - Заменить в тексте подстроки “Robin” (могут быть внутри слова) ”"
           "на подстроку “I am Batman””.\n"
           "2 - Отсортировать предложения по увеличению количества запятых в предложении.\n"
           "3 - Удалить все предложения в которых сумма цифр в предложении равняется 13.\n"
           "4 - Вывести все предложения в которых первое слово начинается с гласной буквы или цифры.\n"
           "0 - Выйти из программы.\n"
           "Введите команду: ");
    scanf(" %c", &user_choice);
    while (user_choice != '0'){
        switch (user_choice){
            case '1':
            	for (int i = 0; i < size; i++)
            		text[i] = replace_word(text[i]);
            	print_text(text, size);
                break;
            case '2':
                text = sort_by_commas_count(text, size);
            	print_text(text, size);
                break;
            
            /*case '3':

                break;
            case '4':

                break;
*/
            default:
                printf("Неверная команда, выберите правильное действие\n");

        }
        printf("Введите команду: ");
        scanf(" %c", &user_choice);
    }
    free_memory(text, size);
    return 0;
}
