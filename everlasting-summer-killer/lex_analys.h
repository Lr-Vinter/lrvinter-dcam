//
// LEXICAL ANALYS
//
// 
//=========================================================================================================================================================


const int LESS    = 1;
const int MORE    = 2;
const int EQUAL   = 3;
const int OPEN    = 5;
const int CLOSE   = 4;
const int OR      = 6;
const int AND     = 7;

const int VAR     = 8;
const int NUMBER  = 9;

//=========================================================================================================================================================

int Get_Lex       (char logical_cond[] , LEX_T lexes[] );

// TYPES

int logic_symbol  (char logical_cond[] , LEX_T lexes[] , int * pointer , int * lex_step);
int number_analys (char logical_cond[] , LEX_T lexes[] , int * pointer , int * lex_step);
int var_analys    (char logical_cond[] , LEX_T lexes[] , int * pointer , int * lex_step);



int find_num      (LIB_T* lib , char var[]);
int Print_lex     (LEX_T lexes[]);


//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Get_Lex(char logical_cond[] , LEX_T lexes[] )
{
    assert(logical_cond);
    assert(lexes);
    
    int lex_step = 0;
    int pointer  = 0;
    
    int a = 0;
    while(logical_cond[pointer])
    {
        
        if       (isspace(logical_cond[pointer]))                                { pointer++;                                                       }               
        else if  ('a' <= logical_cond[pointer] && logical_cond[pointer] <= 'z')  { var_analys   (logical_cond , lexes , &pointer , &lex_step    );  }
        else if  ('0' <= logical_cond[pointer] && logical_cond[pointer] <= '9')  { number_analys(logical_cond , lexes , &pointer , &lex_step    );  }
        else                                                                     { logic_symbol (logical_cond , lexes , &pointer , &lex_step    );  }
        
    }
    
    return 1;
}

int logic_symbol  (char logical_cond[] , LEX_T lexes[] , int * pointer , int * lex_step)
{
    //printf("'%s' pointer %d\n", logical_cond, *pointer);
    assert(logical_cond);
    assert(lexes);
    
    if       (logical_cond[*pointer] == '|')               { lexes[*lex_step].op_flag = OR;    }
    else if  (logical_cond[*pointer] == '&')               { lexes[*lex_step].op_flag = AND;   }
    else if  (logical_cond[*pointer] == '>')               { lexes[*lex_step].op_flag = MORE;  }
    else if  (logical_cond[*pointer] == '<')               { lexes[*lex_step].op_flag = LESS;  }
    else if  (logical_cond[*pointer] == '=')               { lexes[*lex_step].op_flag = EQUAL; }
    else if  (logical_cond[*pointer] == '(')               { lexes[*lex_step].op_flag = OPEN;  }
    else if  (logical_cond[*pointer] == ')')               { lexes[*lex_step].op_flag = CLOSE; }
    else 
    {
        printf("symbol '%c' code %d\n" , logical_cond[*pointer], (unsigned int)logical_cond[*pointer]);
        assert(!"ERROR uncorrect symbol");
    }
    
    (*lex_step)++;
    (*pointer )++;
    
    return 1;
}

int number_analys (char logical_cond[] , LEX_T lexes[] , int * pointer , int * lex_step)
{
    assert(logical_cond);
    assert(lexes);
    
    int value = 0;
    while('0' <= logical_cond[*pointer] && logical_cond[*pointer] <= '9')
    {
        value  = value*10 + logical_cond[*pointer] - '0';
        (*pointer)++;
    }
            
    lexes[*lex_step].op_flag = NUMBER;
    lexes[*lex_step].value   = value ;
    
    (*lex_step)++;
    
    return 1;
}

int var_analys   (char logical_cond[] , LEX_T lexes[] , int * pointer , int * lex_step )
{
    assert(logical_cond);
    assert(lexes);
    
    char var[MAX_NAME_SIZE] = {};
    
    int step = 0;
    while('a' <= logical_cond[*pointer] && logical_cond[*pointer] <= 'z') 
    {
        var[step] = logical_cond[*pointer];
        (*pointer)++;
        step      ++;
    }
    
    lexes[*lex_step].op_flag = VAR;
    strcpy(lexes[*lex_step].var_name , var);
        
    // lexes[*lex_step].var_num = find_num(lib , var);   // ATTENTION
    
    (*lex_step)++;
    
    return 1;
}

int find_num       (LIB_T* lib , char var[])
{
    assert(lib);
    assert(var);
    
    int number = lib->active_cnt;
    int found  = 0;
    
    for(int i = 0; i < number; i++)
    {
        if(strcmp(lib->block.vars[i].name , var) == 0)  return i;
        if(i == ( number - 1 )                 ) printf("ERROR , you use undefined variable");      assert(0);                 
    }
}

// END OF LOGIC
//=========================================================================================================================================================
//=========================================================================================================================================================

char* Get_cond_text(FILE* in , LIBRARY_T* lib) // считывает | включительно 
{
    assert(lib);
    assert(in);
    
    char* buf = (char*)calloc(MAX_COMMAND_SIZE , sizeof(char));
    
    int step = 0;
    for(;;)
    {
        char c = fgetc(in);
        
        if(c == ' ')    continue;
        if(c == '|')    break;
        
        buf[step] = c;
        step++;
        
    }
    
    return buf;
}


//=========================================================================================================================================================

int Print_lex (LEX_T lexes[])
{
    assert(lexes);
    int step = 0;
    
    printf("==============================\n");
    printf("======This is lex dump========\n");
    printf("==============================\n");
    
    while(true)
    {
        if(lexes[step].op_flag == 0)                                      break;
        
        if(LESS <= lexes[step].op_flag && lexes[step].op_flag <= AND)     printf(" operation %d \n"     , lexes[step].op_flag );      
        else if(lexes[step].op_flag == VAR)                               printf(" var %s       \n"     , lexes[step].var_name);   
        else if(lexes[step].op_flag == NUMBER)                            printf(" number , value %d\n" , lexes[step].value);                               
        else
        {
            assert(!"ERROR");
        }
        
        step ++;
    }
    
    return 1;
}







