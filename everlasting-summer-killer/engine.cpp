#include "tree_class.h"
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <string.h>

// 
// 
// 
// 

using namespace std;
const int MAX_COMMAND_SIZE = 100;
const int MAX_NODE_NAME    = 30;
const int MAX_NAME_SIZE    = 20;
const int MAX_NODE_SIZE    = 500;

struct LIBRARY_T
{
    tree_t* tree;
    
    char  node_name[MAX_NODE_NAME] = {};
    char* branch_names[MAX_TREE_BRANCHES] = {};
};

struct LIB_T
{
    LIBRARY_T library[MAX_NODE_SIZE] = {};
    int active_cnt = 0;
};

//

int   First_read      (char* file_name ,LIB_T* library);

// for one node 

int   Get_one_node    (FILE* in , LIBRARY_T* library);
int   Get_info        (FILE* in , LIBRARY_T* library);
int   Get_info_text   (FILE* in , LIBRARY_T* library);
int   Get_branches    (FILE* in , LIBRARY_T* library);
int   Get_choose      (FILE* in , LIBRARY_T* library);

// HELP - functions, which allow us to work with text.

int   Get_Back_ifnot  (FILE* in , char* string);
int   Space_pass      (FILE* in , char  next);
int   Check_end       (FILE* in , char  next);
char* Get_text        (FILE* in , char  stop_symbol);

int   Library_memory(LIBRARY_T library[]);

// tree-making

int   Build_tree    (LIB_T* library);
int   Pointer_build (LIB_T* library , int node_number);

int main()
{

    LIB_T lib = {};
    
    First_read("in.txt", &lib);
    Build_tree(&lib);


    return 0;
}

int First_read(char* file_name , LIB_T* lib)
{
    
    assert(file_name);
    assert(lib);
    
    FILE* in = fopen(file_name , "r");
    assert(in);

    fseek(in , 0 , SEEK_SET );
    
    char command[MAX_COMMAND_SIZE] = {};
    int step = 0;

    while(true)
    {
        
        lib->library[step].tree = (tree_t*)calloc(1 , sizeof(tree_t) );
        Get_one_node(in , &(lib->library[step]));
        step++;
        
        int end = Get_Back_ifnot(in , "END_ALL");
        if     (end == 1) { break; }
        
    }
    
    lib->active_cnt = step;
    fclose(in);
    return 1;
}


int Get_one_node(FILE* in , LIBRARY_T* library) //one module
{
    assert(in);
    assert(library);
    
    char opString[MAX_COMMAND_SIZE] = {};
    
    fscanf(in , "%s" , opString);
    assert(strcmp(opString , "NODE") == 0);
    
    fscanf(in , "%s" , opString);
    strcpy( library->node_name , opString);        

    Get_info(in , library);
    
    fscanf(in , "%s" , opString);
    assert(strcmp(opString , "END_NODE") == 0);
    
    return 1;
}

int Get_info(FILE* in , LIBRARY_T* library)  // считывает все части скрипта(выбор, ветки , текст) для данного узла
{
    assert(in);
    assert(library);
    
    Get_info_text  (in , library);
    Get_choose     (in , library);
    Get_branches   (in , library);
    
    return 1;
}

int Get_branches(FILE* in , LIBRARY_T* library)  // считывает ветки выбора
{
    assert(in);
    assert(library);
    char opString[MAX_COMMAND_SIZE] = {};
    
    fscanf(in , "%s" , opString);
    assert(strcmp(opString , "branches") == 0);
    
    Space_pass(in , '{');
    
    for(;;)
    {
        
        if(Get_Back_ifnot(in , "}") == 1)  { break; }
        
        char symbol;
        int  number_branch = 0;
        
        fscanf(in , "%d" , &number_branch);
        assert(0 <= number_branch && number_branch <= MAX_TREE_BRANCHES);
        Space_pass(in , ':');
        
        library->branch_names[number_branch] = (char*)calloc(MAX_NAME_SIZE , sizeof(char));
        fscanf(in , "%s" , library->branch_names[number_branch]);
        
    }
    
    return 1;
}


char* Get_text(FILE* in , char stop_symbol)  // считывает текст типа "я такой унылый битард блаблабла" из скрипта 
{
    assert(in);
    
    char* text = (char*) calloc(MAX_TEXT_SIZE , sizeof(char) );
    
    for(int i = 0;;i++)
    {
        
        if( i > MAX_TEXT_SIZE)
        {
            printf("ERROR : TOO MUCH TEXT, IDITE NAXUI");
        }
        
        int offset_pointer  = ftell(in);
        char symbol         = fgetc(in);
        
        if(symbol == stop_symbol) { fseek(in , offset_pointer , SEEK_SET); break; }
        
        text[i] = symbol;
    }
    
    return text;
}

int Get_choose(FILE* in , LIBRARY_T* library)
{
    assert(in);
    assert(library);
    char opString[MAX_COMMAND_SIZE] = {};
    
    fscanf(in , "%s" , opString);
    assert(strcmp(opString , "choose") == 0);
    
    Space_pass(in , '{');
    
    for(;;)
    {
        
        if(Get_Back_ifnot(in , "}") == 1)   { break; }
        Space_pass(in , '<');
        
        int branch_number = 0;
        
        char* string_ptr = Get_text(in , '>');
        Space_pass(in , '>');
        Space_pass(in , '=');
        
        fscanf(in , "%d" , &branch_number);
        library->tree->choise_text[branch_number] = (char*)calloc(MAX_TEXT_SIZE , sizeof(char) );
        strcpy(library->tree->choise_text[branch_number] , string_ptr);   // ЗАКАТИТ ЛИ ?
        
    }
    
    return 1;
}

int Get_info_text(FILE* in , LIBRARY_T* library)
{
    assert(in);
    assert(library);
    char opString[MAX_COMMAND_SIZE] = {};
    
    fscanf(in , "%s" , opString);
    assert(strcmp(opString , "text") == 0);
    
    Space_pass(in , '{');
    
    char*  string_ptr = Get_text(in , '}');
    strcpy(library->tree->text , string_ptr);
    
    Space_pass(in , '}');
    return 1;
}

//================================================================================================================

int Space_pass(FILE* in , char next)
{
    assert(in);
    for(;;)
    {
        char c = fgetc(in);
        if(!isspace(c))
        {
            if (c != next)
                printf("expected: '%c', found: '%c'\n", next, c);
            
            assert(c == next); 
            break;
        }
    }
    return 1;
}

int Check_end(FILE* in , char next)
{
    assert(in);
    for(;;)
    {
        char c = fgetc(in);
        if(c != ' ')
        {
            if(c == next) return 1;
            else return 0;
        }
        
    }
}

int Get_Back_ifnot(FILE* in , char* string)
{
    assert(in);
    char tested[MAX_COMMAND_SIZE] = {};
    int offset_pointer            = ftell(in);
    
    fscanf(in , "%s" , tested);
        
    if(strcmp(tested , string) == 0)   {                                        return 1; }
    else                               { fseek(in , offset_pointer , SEEK_SET); return 0;}
        
    return 1;
}

//================================================================================================================

int Build_tree   (LIB_T* lib)
{
    assert(lib);
    
    int lib_size    = lib->active_cnt;
    int node_number = 0;
    
    for(int node_number = 0; node_number < lib_size; node_number++)
    {
        Pointer_build(lib , node_number);
    }
    
    return 1;
}

int Pointer_build (LIB_T* lib , int node_number)
{
    assert(lib);
    int lib_size = lib->active_cnt;
    
    for(int i = 0; i < MAX_TREE_BRANCHES; i++)
    {
        for(int temp = 0; temp < lib_size; temp++)
        {
            
            if(lib->library[node_number].branch_names[i] == NULL)  continue; 
                
            if(strcmp(lib->library[node_number].branch_names[i] , lib->library[temp].node_name) == 0)
            {
                lib->library[node_number].tree->branches[i] = lib->library[temp].tree;
                break;
            }
            
        }
    }
    
    return 1;
}

/*

int Tree_Dump    (tree_t* tree , int main_node)
{
    assert(lib);
    int step    = 0;
    int length  = lib->active_cnt;
    
    printf("NODE NAME      :      %s" , lib->library[main_node]->node_name);
    printf("TEXT   IN NODE : \n   %s" , lib->library[main_node]->tree->text);
    
    for(int i = 0; i < MAX_TREE_BRANCHES; i++)
    {
        if(lib->library[main_node]->tree->branches[i] != NULL)
        {
            printf("BRANCH %d CHOOSE TEXT: \n  %s \n\n  " , i , lib->library[main_node]->branch_names[i] );
            printf("NEXT   ");
            //Tree_Dump(lib , 
        }
    }

    
    return 1;
}

*/
//================================================================================================================










