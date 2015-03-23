#include <stdio.h>
#include <cstdlib>
#include <iostream>

const int MAX_TREE_BRANCHES = 20;
const int MAX_TEXT_SIZE     = 300;

struct tree_t
{
    char text[MAX_TEXT_SIZE];
    
    tree_t* branches[MAX_TREE_BRANCHES];
    char  * choise_text[MAX_TREE_BRANCHES];
    
    int CHOOSE = 0;
};

tree_t* New_tree_branch();
int Dump_tree(tree_t* tree);

tree_t* New_tree_branch()
{

    tree_t* tree = (tree_t*)calloc( 1 , sizeof(*tree) );

    return tree;
}

int Dump_tree(tree_t* tree)
{


    return 1;
}
