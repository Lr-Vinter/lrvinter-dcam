#include "list.h"
#include <string.h>

//   HASH - TABLE
//

const int MAX_HASH_SIZE = 100;
const int MAX_HASH_WORD_SIZE = 30;


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int Hash_function_transform(int (*func)(char*) , char* word);

void analysis_input(FILE* input_f , header_t* hash_table , int (*func)(char*));

void Fill_hash_table(header_t* hash_table , char* word , int (*func)(char*));

void Hash_fill(header_t* hash_table , FILE* out_f);


int Hash_one(char* word);

int Hash_two(char* word);

int Hash_three(char* word);

int Hash_four(char* word);

int Hash_five(char* word);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{

    header_t* hash_table = (header_t*)calloc(MAX_HASH_SIZE , sizeof(*hash_table));

    FILE* input_f = fopen("input.txt" , "r");                      assert(input_f);
    FILE* out_f   = fopen("out.csv" , "w");

    
    
    analysis_input(input_f , hash_table , Hash_five);

    

    for(int i = 0; i < MAX_HASH_SIZE; i++)
    {
        printf("value %d\n" , hash_table[i].counter);
    }

    Hash_fill(hash_table , out_f);

    free(hash_table); hash_table = NULL;

    return 0;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void analysis_input(FILE* input_f , header_t* hash_table , int (*func)(char*) )
{

    assert(input_f);
    assert(func);
    assert(hash_table);

    char word[MAX_HASH_WORD_SIZE] = {};
    while(true)
    {

        int OK = fscanf(input_f , "%s" , word);
        if(OK != 1) { break ;}

     //   printf("it is going scanfing word '%s'\n" , word); !!!!!!

        Fill_hash_table(hash_table , word , func);

    }

}


void Fill_hash_table(header_t* hash_table , char* word , int (*func)(char*))
{

    assert(hash_table);
    assert(func);
    assert(func);
    assert(word);

    int number = (unsigned) func (word) % MAX_HASH_SIZE;

    List_add_element( (hash_table + number) , 1 , LEFT ,  word  );

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int Hash_one(char* word)
{

    assert(word);

    //printf("CODE %d" , word[0]);
    return word[0];

}

int Hash_two(char* word)
{

    assert(word);

    return 1;

}

int Hash_three(char* word)
{

    assert(word);

    return strlen(word);

}

int Hash_four(char * word)
{

    assert(word);

    int length = strlen(word);
    int sum = 0;

    for(int i = 0; i < length; i++)
    {
        sum = sum + word[i];
    }

    return sum;

}

int Hash_five(char* word)
{

    assert(word);

    int length = strlen(word);
    int value = 0;

    for(int i = 1; i < length ; i++)
    {
        value = (value<<1)^( word[i] );
    }

    return value;

}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Hash_fill(header_t* hash_table , FILE* out_f)
{

    assert(hash_table);
    assert(out_f);

    for(int i = 0; i < MAX_HASH_SIZE; i++)
    {

        fprintf(out_f , "%d\n" , hash_table[i].counter);

    }

    fprintf(out_f, "\n");

}







