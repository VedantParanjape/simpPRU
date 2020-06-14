#include "symbol_table.h"
#include <stdio.h>

static int scope;
khash_t(symbol_table) *handle = NULL;

void init_symbol_table()
{
    handle = kh_init(symbol_table);
    scope = 0;
}

int insert_symbol_table(char* id, int dt_type, int val)
{    
    khint_t temp = kh_get(symbol_table, handle, id);

    if (temp == kh_end(handle))
    {
        sym_ptr new = (sym_ptr) malloc(sizeof(sym));
        new->identifier = strdup(id);
        new->data_type = dt_type;
        new->value = val;
        new->scope = scope;
        new->next = NULL;
        
        int err;
        temp = kh_put(symbol_table, handle, id, &err);
        kh_value(handle, temp) = new;
        
        return 1;
    }
    else
    {
        sym_ptr base = kh_value(handle, kh_get(symbol_table, handle, id));
        sym_ptr temp = NULL;

        while (base != NULL)
        {
            if (base->scope == scope)
            {
                return -1;
            }
            temp = base;
            base = base->next;
        }

        base = (sym_ptr) malloc(sizeof(sym));
        temp->next = base;
        
        base->identifier = strdup(id);
        base->data_type = dt_type;
        base->value = val;
        base->scope = scope;
        base->next = NULL;

        return 1;
    }
}

sym_ptr lookup_symbol_table(char* id, int scope)
{
    khint_t temp = kh_get(symbol_table, handle, id);

    if (temp == kh_end(handle))
    {
        return NULL;
    }
    else if (kh_value(handle, temp)->next == NULL && kh_value(handle, temp)->scope == scope)
    {   
        return kh_value(handle, temp);
    }
    else if (kh_value(handle, temp)->next != NULL)
    {
        sym_ptr temp2 = kh_value(handle, temp);
        while (temp2 != NULL)
        {
            if (temp2->scope == scope)
            {
                return temp2;
            }
            temp2 = temp2->next;
        }

        return NULL;
    }
}

void increment_scope()
{
    scope = scope + 1;
}

void decrement_scope()
{
    scope = scope - 1;
}

int get_scope()
{
    return scope;
}

void close_symbol_table()
{
    for (khint_t k = kh_begin(handle); k != kh_end(handle); ++k)
    {
        if (kh_exist(handle, k))
        {
            if (kh_value(handle, k)->next == NULL)
            {
                free((sym_ptr)kh_value(handle, k));
            }
            else 
            {
                sym_ptr temp = kh_value(handle, k), backup;

                while (temp != NULL)
                {
                    backup = temp->next;
                    free((sym_ptr)temp);
                    temp = backup;
                }
            }
        }
    }
}

// int main()
// {
//     init_symbol_table();
//     printf("inserting in table: %d\n", insert_symbol_table("test", DT_INTEGER, 1));
//     increment_scope();
//     decrement_scope();
//     printf("inserting in table: %d\n", insert_symbol_table("test", DT_INTEGER, 3));
//     insert_symbol_table("ea", DT_BOOLEAN, 1);

//     if (lookup_symbol_table("ea", 0) == NULL)
//     {
//         printf("not found\n");
//     }
//     else
//     {
//         printf("lookup: %d\n", lookup_symbol_table("ea", 0)->value);
//     }

//     close_symbol_table();
// }