#include <stdio.h>
#include <vector>
#include <assert.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

const int MAX_TREE_BRANCHES = 20;
const int MAX_TEXT_SIZE     = 300;


//    all rights
//    Lord Vinter     (c)
// 
// 

using namespace std;
const int MAX_COMMAND_SIZE = 100;
const int MAX_NODE_NAME    = 30;
const int MAX_NAME_SIZE    = 20;
const int MAX_NODE_SIZE    = 500;

const int CHOOSE_HERE      = 1;
const int CHOOSE_NO        = 0;

const int MAX_VARS         = 50;
const int MAX_LEX          = 20;
const int MAX_LOGIC        = 3;

struct THIS_VT
{
    int   value               = 0;
    char  name[MAX_NAME_SIZE] = {};
};

struct VAR_T
{
    THIS_VT  vars[MAX_VARS] = {};
    int      vr_ptr = 0;
};


//=========================================================================================================================================================
//=========================================================================================================================================================


struct tree_t
{
    char text[MAX_TEXT_SIZE];
    
//    CHOOSE_T choise[MAX_TREE_BRANCHES] = {};
//    BRANCH_T branches[MAX_TEXT_SIZE] = {};
    
};

struct LIBRARY_T
{
    tree_t* tree;
    
    char  node_name[MAX_NODE_NAME] = {};
    char  main_root[MAX_NODE_NAME] = {};
    
};

struct LIB_T
{
    VAR_T     block                  = {};
    LIBRARY_T library[MAX_NODE_SIZE] = {};
    int active_cnt = 0;
};

#include "lex_analys.h"
#include "create_tree.h"

//=========================================================================================================================================================
//=========================================================================================================================================================

int   First_read      (char* file_name ,LIB_T* library);
int   Get_Vars        (FILE* in , LIB_T*     lib);
// for one node 

int   Get_one_node    (FILE* in , LIBRARY_T* library);
int   Get_info        (FILE* in , LIBRARY_T* library);
int   Get_info_text   (FILE* in , LIBRARY_T* library);
int   Get_branches    (FILE* in , LIBRARY_T* library);

int   Get_choose(FILE* in , LIBRARY_T* library);
int   Get_one_choose  (FILE* in , LIBRARY_T* library , int* step);

int   choose_advance (FILE* in , LIBRARY_T* library);
char*   Get_cond_text(FILE* in , LIBRARY_T* lib); // считывает | включительно 

// LEX ANALYSIS !!!!!!!!
// 



// HELP - functions, which allow us to work with text.

int   Get_Back_ifnot  (FILE* in , char* string);
int   Space_pass      (FILE* in , char  next);
int   Check_end       (FILE* in , char  next);
char* Get_text        (FILE* in , char  stop_symbol);



int   Library_memory(LIBRARY_T library[]);
int   Vars_Dump     (LIB_T* lib);

int main()
{

    LIB_T lib = {};
    
    //First_read("temp_in.txt", &lib);
    
    
    char  input[MAX_COMMAND_SIZE] = {};
    LEX_T lexes[MAX_COMMAND_SIZE] = {};
    
    gets(input);
    Get_Lex(input , lexes);
    
    int lex_num = 0;
    b_log* tree = Get_Condition(lexes , &lex_num);
    Tree_Print(tree);
    
    Print_lex(lexes);

    return 0;
}

int First_read(char* file_name , LIB_T* lib)
{
    
    assert(file_name);
    assert(lib);
    
    FILE* in = fopen(file_name , "r");
    assert(in);

    fseek(in , 0 , SEEK_SET );
    Get_Vars(in , lib);
    
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

int Get_Vars (FILE* in , LIB_T* lib)
{
    assert(in);
    assert(lib);
    
    char opString[MAX_NAME_SIZE] = {};
    
    fscanf(in , "%s" , opString);
    assert(strcmp(opString , "VARS" ) == 0 );
    
    Space_pass(in , '{');
    int step = 0;
    for(;;)
    {
        if(Get_Back_ifnot(in , "}") == 1 ) { break; }
        fscanf(in , "%s" , opString);
        
        strcpy(lib->block.vars[step].name , opString);
        step++;
        
    }
    
    lib->active_cnt = step;
    printf("step %d\n" , lib->active_cnt);
    return 1;
}

//
// MAIN GET FUNCTION , BITCHES , ATTENTION
//
//

int Get_info(FILE* in , LIBRARY_T* library)  // считывает все части скрипта(выбор, ветки , текст) для данного узла
{
    assert(in);
    assert(library);
    
    Get_info_text  (in , library);      
    Get_choose     (in , library);      
    // Get_branches   (in , library);   // ATTENTION
    
    return 1;
}

int Get_choose(FILE* in , LIBRARY_T* library)  
{
    assert(in);
    assert(library);
    
    char opString[MAX_COMMAND_SIZE] = {};
    
    fscanf(in , "%s" , opString            );
    assert(strcmp(opString , "choose") == 0);
    
    Space_pass(in , '{');
    
    int step = 0;
    for(;;)
    {
        if(Get_Back_ifnot(in , "}") == 1) break;
        Get_one_choose(in , library , &step);
    }

    return 1;
}

int Get_one_choose(FILE* in , LIBRARY_T* library , int* step)
{
    assert(in);
    assert(library);
    assert(step);
    
    Space_pass(in , '(');
    
    char*    logical_cond = Get_cond_text(in , library);
    
//    tree_t*  logical_tree = Get_tree(lex);
    
    
    Space_pass(in , ')');
    
    return 1;
}

//=======================================================================================================================================================
//========================================================================================================================================================

char* Get_text(FILE* in , char stop_symbol)  // считывает текст типа "я такой унылый битард блаблабла" из скри
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



int Vars_Dump    (LIB_T* lib)
{
    assert(lib);
    int number = lib->active_cnt;
    
    for(int i = 0; i < number ; i++ )
    {
        printf("variable name %s , value %d\n\n" , lib->block.vars[i].name , lib->block.vars[i].value);
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





