//  
//
//  LOGICAL ASSIGMENTS
//

struct b_log
{
    LEX_T info = {};
    b_log* left ;
    b_log* right;
};


b_log* New_tree_branch(LEX_T* get , b_log* left , b_log* right)
{
    assert(get);

    b_log* tree =(b_log*)calloc(1 , sizeof(*tree));

    tree->info  = *get;
    tree->right = right;
    tree->left  = left;

    return tree;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

b_log* Get_Condition(LEX_T* lexes , int* lex_num);
b_log* Get_And      (LEX_T* lexes , int* lex_num);
b_log* Get_OR       (LEX_T* lexes , int* lex_num);
b_log* Get_Var(LEX_T* lexes , int* lex_num);

int Tree_Dump(b_log* tree);
int Tree_Print(b_log* tree);

//---------------------------------------------------------------------------------------------------------------------------------------------------------

b_log* Get_Condition(LEX_T lexes[] , int * lex_num)
{
    assert(lexes);
    assert(lex_num);

    assert(lexes[*lex_num].op_flag == OPEN);
    (*lex_num)++;

    b_log* condition_tree = NULL;

    if(lexes[*lex_num].op_flag == OPEN)
    {
        condition_tree = Get_OR(lexes , lex_num);
    }

    else
    {

        b_log* left    = Get_Var(lexes , lex_num);
        condition_tree = New_tree_branch(&lexes[*lex_num] , NULL , NULL);
        (*lex_num)++;
        b_log* right   = Get_Var  (lexes , lex_num);

        condition_tree->left   = left;
        condition_tree->right  = right;

    }

    assert(lexes[*lex_num].op_flag == CLOSE);
    (*lex_num)++;

    return condition_tree;
    
}


b_log* Get_OR (LEX_T lexes[] , int* lex_num)
{
    assert(lexes);
    assert(lex_num);
    
    b_log* Con = Get_And(lexes , lex_num);

    while(lexes[*lex_num].op_flag == OR)
    {

        LEX_T INFO = lexes[*lex_num];
        (*lex_num)++;

        Con = New_tree_branch(&INFO , Con , Get_And(lexes , lex_num));

    }

    return Con;

}

b_log* Get_And(LEX_T lexes[] , int* lex_num)
{
    assert(lexes);
    assert(lex_num);

    b_log* And = Get_Condition(lexes , lex_num);

    while(lexes[*lex_num].op_flag == AND)
    {

        LEX_T INFO = lexes[*lex_num];
        (*lex_num)++;

        And = New_tree_branch(&INFO , And , Get_Condition(lexes , lex_num));

    }
    
    return And;
}

b_log* Get_Var(LEX_T* lexes , int* lex_num)
{
    assert(lex_num);
    assert(lexes);
    
    b_log* Var = New_tree_branch(&lexes[*lex_num] , NULL , NULL);
    (*lex_num)++;
    
    return Var;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

int Tree_Print(b_log* tree)
{
    Tree_Dump(tree);
    printf("\n\n");
    
    return 1;
}

int Tree_Dump(b_log* tree)
{
    assert(tree);
    
    printf("(");
    
    if     (tree->info.op_flag == MORE)                printf(" > "  );
    else if(tree->info.op_flag == LESS)                printf(" < "  );
    else if(tree->info.op_flag == OR)                  printf(" OR " );
    else if(tree->info.op_flag == AND)                 printf(" AND ");
    else if(tree->info.op_flag == VAR)                 printf(" var ");   
    else if(tree->info.op_flag == NUMBER)              printf(" %d" , tree->info.value);                               
    else
    {
        assert(!"ERROR uncorrected info");
    }
    
    if(tree->left)    Tree_Dump(tree->left );
    if(tree->right)   Tree_Dump(tree->right);
    
    printf(")");
    
    return 1;
}



















