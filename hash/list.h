#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


//---------------------------------------------------------------------------------------------------------------------------------------------------

const int MAX_WORD_SIZE = 30;

const int RIGHT =  1;
const int LEFT  = -1;
const int START =  0;

//---------------------------------------------------------------------------------------------------------------------------------------------------

enum ERROR_LIST_t
{

    WRONG_CONNECTION ,
    WRONG_LAST_ELEM_RIGHT_POINTER ,
    WRONG_FIRST_ELEM_LEFT_POINTER,
    WRONG_FIRST_ELEM_RIGHT_POINTER,
    WRONG_OPERATION_POSITION,
    WRONG_HEADER_COUNTER,
    LIST_OK

};

ERROR_LIST_t ERROR_LIST = LIST_OK;

//---------------------------------------------------------------------------------------------------------------------------------------------------


#define VERIFIE if( List_OK(header) ) {List_Dump(header , 1); printf("ERROR in line %d " , __LINE__ );}


struct list_elem_t
{
    char* word;

    list_elem_t* left_pointer;
    list_elem_t* right_pointer;
};

struct header_t
{
    list_elem_t* last_pointer;
    list_elem_t* first_pointer;

    int counter;
};

//---------------------------------------------------------------------------------------------------------------------------------------------------
//

void List_add_element(header_t* header , int position , int orient , char* value);

void List_Ctor(header_t* header);
void List_Dtor(header_t* header);

void Element_Exterminate(list_elem_t* element);
void List_delete_element(header_t* header , int position );

int List_OK(header_t* header);
void List_Dump(header_t* header , int print_visible);
void position_control(header_t* header , int position);

void m_printERROR(char* str);
char* m_strerror(ERROR_LIST_t ERROR_LIST);

void List_Print(header_t* header , int elements_number , int step);

//---------------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------------

void List_Ctor(header_t* header)
{
    header_t other = {};
    *header = other;
}

void Element_Exterminate(list_elem_t* element)
{
    assert(element);

    element->word          = NULL;
    element->left_pointer  = NULL;
    element->right_pointer = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void List_Dump(header_t* header , int print_visible)
{
    assert(header);

    list_elem_t* current_element = header->first_pointer;
    int elements_number = 1;

    while((current_element->right_pointer) != NULL)
    {
        current_element = current_element->right_pointer;
        elements_number++;
    }

    current_element = header->first_pointer;

    int step = 0;
    int BLA = 0;

    switch(List_OK(header))
    {

        case 1:

            m_printERROR("WTF : ");
            printf("LIST(Something went left)\n\n");
            List_Print(header , elements_number , step);

            break;

        case 0:

            if(print_visible == 1)
            {

                printf("LIST(OK)\n\n");
                List_Print(header , elements_number , step);

                break;

            }

        default: printf("ERROR");

    }

}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void List_Print(header_t* header , int elements_number , int step)
{
    assert(header);

    printf("counter(header information) : %d\n" , header->counter);
    printf("counter(count information)  : %d\n\n" , elements_number);

    list_elem_t* current_element = header->first_pointer;

    printf("Elements:\n\n");

    while(current_element != NULL)

        {

            if(step < (header->counter)) { printf("in header : "); }
            printf("Elem number[%d] , value :%s\n" , step , current_element->word);
            current_element = current_element->right_pointer;

            step++;

        }

}

//---------------------------------------------------------------------------------------------------------------------------------------------------

int List_OK(header_t* header)
{

    assert(header);
    int list_length = header->counter;


    //----------------------------------------------------------------------------------------------------------------------------
    // first element check

    list_elem_t* current_element = header->first_pointer;
    list_elem_t* second_element  = current_element->right_pointer;

    if((current_element->left_pointer)  != NULL)             { ERROR_LIST = WRONG_FIRST_ELEM_LEFT_POINTER; return 1;}
    if( second_element->left_pointer != current_element)  { ERROR_LIST = WRONG_FIRST_ELEM_RIGHT_POINTER;return 1;}

    current_element = current_element->right_pointer;

    //----------------------------------------------------------------------------------------------------------------------------
    // main elements check


    for(int current = 2; current < list_length - 1; current++)
    {

        list_elem_t* back_elem           = current_element->left_pointer;
        list_elem_t* back_from_back      = back_elem->right_pointer;

        list_elem_t* forward_elem        = current_element->right_pointer;
        list_elem_t* back_from_forward   = forward_elem->left_pointer;

        if(current_element != back_from_back)
        {
            ERROR_LIST = WRONG_CONNECTION;
            return 1;
        }

        if(current_element != back_from_forward)
        {
            ERROR_LIST = WRONG_CONNECTION;
            return 1;
        }

        current_element = current_element->right_pointer;

    }

    //----------------------------------------------------------------------------------------------------------------------------
    // last element check

    list_elem_t* last_elem = current_element->right_pointer;

    if(last_elem->right_pointer != NULL) {ERROR_LIST = WRONG_LAST_ELEM_RIGHT_POINTER; return 1;}

    //----------------------------------------------------------------------------------------------------------------------------
    // list elements number check

    current_element = header->first_pointer;
    int elements_number = 1;

    while(current_element->right_pointer != NULL)
    {
        current_element = current_element->right_pointer;
        elements_number++;
    }

    if(elements_number != header->counter) { ERROR_LIST = WRONG_HEADER_COUNTER; return 1;}

    return 0;

}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void List_delete_element(header_t* header , int position )
{
    assert(header);
    assert(position <= (header->counter));

    list_elem_t* current_element = header->first_pointer;

    int current = 1;
    while(current < position - 1)
    {
        current_element = current_element->right_pointer;
        current++;
    }

    list_elem_t* next = current_element->right_pointer;
    if(position != 1)
    {

        if(position < (header->counter))
        {
            (next->right_pointer)->left_pointer = current_element;
        }

        if(position != 1)
        {
            current_element->right_pointer = next->right_pointer;
        }

        Element_Exterminate(next);
        free(next);   next = NULL;

    }

    else
    {
        header->first_pointer = next;
        next->left_pointer    = NULL;
        Element_Exterminate(current_element);
        free(current_element);  current_element = NULL;
    }

    (header->counter)--;

}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void List_Dtor(header_t* header)
{

    assert(header);

    list_elem_t* current_element = header->first_pointer;
    list_elem_t* next_element = NULL;

    while(current_element!= NULL)
    {
        next_element = current_element->right_pointer;
        Element_Exterminate(current_element);

        free(current_element); current_element = NULL;

        current_element = next_element;
    }

    header->first_pointer = NULL;
    header->last_pointer  = NULL;
    header->counter       = 0;

    header = NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void List_add_element(header_t* header , int position , int orient , char* value)
{

    assert(header);
    assert(value);

    position_control(header , position);

    list_elem_t* current_element = header->first_pointer;
    int current = 1;

    list_elem_t* new_element = (list_elem_t*)calloc(1 , sizeof(*new_element));
    new_element->word = value;

    if((header->first_pointer) != NULL)
    {

            while(current < position)
            {
                current_element = current_element->right_pointer;
                current++;
            }                       // позиция position

            assert(current == position);

            switch(orient)
            {


                case  RIGHT:

                                if(position < (header->counter))
                                {
                                    new_element->right_pointer = current_element->right_pointer;
                                    (current_element->right_pointer)->left_pointer = new_element;
                                }
                                current_element->right_pointer = new_element;
                                new_element->left_pointer = current_element;

                                if(position == (header->counter))
                                {
                                    new_element->right_pointer = NULL;
                                }

                                break;

                case   LEFT:
                                new_element->right_pointer = current_element;
                                current_element->left_pointer = new_element;

                                if(position != 1)
                                {
                                    new_element->left_pointer = current_element->left_pointer;
                                    (current_element->left_pointer)->right_pointer = new_element;
                                }

                                if(position == 1)
                                {
                                    header->first_pointer = new_element;
                                }

                                break;

                default:       printf("There are no such position");

            }

            (header->counter)++;
    }

    else // первый элемент
    {
        header->first_pointer = new_element;
        (header->counter)++;
    }

}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------


void m_printERROR(char* str)
{
    char* error = m_strerror(ERROR_LIST);
    printf("%s%s" , str , error);
}


char* m_strerror(ERROR_LIST_t ERROR_LIST)
{
    switch(ERROR_LIST)
    {

    case WRONG_HEADER_COUNTER:             return  "Wrong header counter\n\n"             ;                break;
    case WRONG_CONNECTION:                 return  "Wrong connection between elements\n\n";                break;
    case WRONG_LAST_ELEM_RIGHT_POINTER:    return  "Wrong last element right pointer\n\n" ;                break;
    case WRONG_FIRST_ELEM_RIGHT_POINTER:   return  "Wrong first element right pointer\n\n";                break;
    case WRONG_FIRST_ELEM_LEFT_POINTER:    return  "Wrong first element left pointer\n\n" ;                break;
    case WRONG_OPERATION_POSITION:         return  "Wrong position to do this operation"  ;                break;
    case LIST_OK:                          return  " ";                                                    break;
    default:                               return  "ERROR";                                                break;

    }

}

void position_control(header_t* header , int position)
{
    if(position < 1 || position > (header->counter))
    {
        ERROR_LIST = WRONG_OPERATION_POSITION;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------




