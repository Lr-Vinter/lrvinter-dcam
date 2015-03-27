//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//      ATTENTION 
//  Я аккуратно писал этот хедер , он компелируется , но я его не дебагал , поэтому функцию Tree_Complile не вызывал в main.
//  

int Tree_compile     (LIB_T* lib , int start);

tree_t* Get_next_node       (LIB_T* lib , tree_t* current_tree);
tree_t* find_tree_by_name   (LIB_T* lib , char name[]);
tree_t* Analys_branches     (LIB_T* lib , tree_t* current_tree);

int     Make_increment      (LIB_T* lib , tree_t* current_tree);
int     Analys_choise       (LIB_T* lib , tree_t* current_tree);
int 	Tree_compile        (LIB_T* lib , int start);
int 	Analys_cond         (LIB_T* lib , b_log* logical);
int  	Get_var_or_number   (LIB_T* lib , b_log* tree);
int 	Get_var_value       (LIB_T* lib , b_log* tree);



int Tree_compile     (LIB_T* lib , int start)
{
    assert(lib);
    
    tree_t* current_tree     = lib->library[start].tree;
    while(true)
    {
        current_tree = Get_next_node(lib , current_tree);
        if(current_tree == NULL)  break;
    }
    
    return 1;
}

tree_t* Get_next_node (LIB_T* lib , tree_t* current_tree)
{
	assert(lib);
	assert(current_tree);
	
	Analys_choise(lib , current_tree);
	
	return Analys_branches(lib , current_tree);
}

tree_t* Analys_branches(LIB_T* lib , tree_t* current_tree)
{
	assert(lib);
	assert(current_tree);
	
	tree_t* found = NULL;
	int step = 0;
	for(;;)
	{
		int check_cond = Analys_cond(lib , current_tree->branches[step].condition);
		if (check_cond == 1)
		{
			found = find_tree_by_name(lib , current_tree->branches[step].next_name);
			if   (found == NULL) assert(!"ERROR : you use undefined node");
			else  break;
		}
		if(step > MAX_NODE_SIZE) assert(!"ERROR : There are no variants to be confirmed");
		step++;
	}
	
	Make_increment(lib , current_tree);
	
	return found;
}


int Make_increment       (LIB_T* lib , tree_t* current_tree)
{
	assert(lib);
	assert(current_tree);
	
	for(int i = 0; i < MAX_VARS; i++)
	{
		if(strcmp(lib->block.vars[i].name , current_tree->branches[i].instruction.var_name) == 0)
		{
			int incr_number = current_tree->branches[i].instruction.number;
			
			if(current_tree->branches[i].instruction.op == PLUS)   (lib->block.vars[i].value)+=incr_number;
			if(current_tree->branches[i].instruction.op == MIN )   (lib->block.vars[i].value)+=incr_number;
		}
	}
	
	return 1;
}

tree_t* find_tree_by_name(LIB_T* lib , char name[])
{
	assert(lib);
	assert(name);
	
	for(int i = 0; i < MAX_NODE_SIZE; i++)
	{
		if(strcmp(lib->library[i].node_name , name) == 0)
		{
			return lib->library[i].tree;
		}
	}
	
	return NULL;
}

int Analys_choise    (LIB_T* lib , tree_t* current_tree)
{
	assert(lib);
	assert(current_tree);
	
	int step     = 0;
	int user_ch  = 0;
	for(;;)                         //можно было сделать самый обычный for , но я привык for(;;)
	{
		int visible = Analys_cond(lib , current_tree->choise[step].cond_visible);
		if (visible == 1)  
		{
			printf("Enter %d to choose : %s \n" , current_tree->choise[step].choose , current_tree->choise[step].text);
			scanf ("%d" , &user_ch);
			current_tree->user_choose = user_ch;
		}
		else
		{
			assert(visible == 0);
		}
		step++;
		if (step > MAX_TREE_BRANCHES) break ;  // CHECK AFTER !
	}
	
	return 1;
}

int Analys_cond (LIB_T* lib , b_log* logical)
{
	assert(logical);
	assert(lib);
	
	int check = ( (logical->info.op_flag == OR) || (logical->info.op_flag == AND) ); 
	if(check)
	{
		int log_left   = Analys_cond(lib , logical->left);
		int log_right  = Analys_cond(lib , logical->right);
		
		if(logical->info.op_flag == OR ) return log_left + log_right;
		if(logical->info.op_flag == AND) return log_left*log_right  ;
	}
	
	else
	{
		
		int left_value  = Get_var_or_number(lib , logical->left);
		int right_value = Get_var_or_number(lib , logical->right);
		
		if(logical->info.op_flag == MORE) 
		{	
			if(left_value > right_value) return 1;
			else                         return 0;
		}
		
		if(logical->info.op_flag == LESS)
		{
			if(left_value < right_value) return 1;
			else                         return 0;
		}
		
		if(logical->info.op_flag == EQUAL) 
		{	
			if(left_value > right_value) return 1;
			else                         return 0;
		}
		
	}

	return 1;
}

int Get_var_or_number(LIB_T* lib , b_log* tree)
{
	assert(tree);
	assert(lib);
	
	if(tree->left->info.op_flag == VAR) 
	{
		return Get_var_value(lib , tree);
	}
	else
	{
		assert(tree->info.op_flag == NUMBER);
		return tree->info.value;
	}
		
}

int Get_var_value (LIB_T* lib , b_log* tree)
{
	assert(lib);
	assert(tree);
	
	for(int i = 0; i < MAX_VARS; i++)
	{
		if(strcmp(lib->block.vars[i].name , tree->info.var_name) == 0)
			return lib->block.vars[i].value;
	}
	
	assert(!"ERROR : There are no such variable");
	return 1;
}




