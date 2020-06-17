#include "symbol_table.h"
#include <stdio.h>

static int scope;
khash_t(symbol_table) *handle = NULL;

void init_symbol_table()
{
    handle = kh_init(symbol_table);
    scope = 0;
}

int insert_symbol_table(char* id, int dt_type, int val, int is_function)
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
        new->is_hidden = 0;
        new->is_function = is_function;

        int err;
        temp = kh_put(symbol_table, handle, strdup(id), &err);
        if (temp == -1)
        {
            return -1;
        }
        kh_value(handle, temp) = new;

        return 1;
    }
    else
    {
        if (is_function == 0)
        {
            sym_ptr base = kh_value(handle, kh_get(symbol_table, handle, id));
            sym_ptr temp = NULL;

            while (base != NULL)
            {
                if (base->scope == scope && base->is_hidden == 0)
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
            base->is_hidden = 0;
            base->is_function = 0;
            
            return 1;
        }

        return -1;
    }
}

sym_ptr lookup_symbol_table(char* id, int scope)
{
    khint_t temp = kh_get(symbol_table, handle, id);

    if (temp == kh_end(handle))
    {
        return NULL;
    }
    else if (kh_value(handle, temp)->next == NULL && kh_value(handle, temp)->scope == scope && kh_value(handle, temp)->is_hidden == 0)
    {   
        return kh_value(handle, temp);
    }
    else if (kh_value(handle, temp)->next != NULL)
    {
        sym_ptr temp2 = kh_value(handle, temp);
        while (temp2 != NULL)
        {
            if (temp2->scope == scope && temp2->is_hidden == 0)
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
    for (khint_t i = kh_begin(handle); i != kh_end(handle); ++i)
    {
        if (kh_exist(handle, i))
        {
            sym_ptr temp = kh_value(handle, i);

            if (temp->next == NULL && temp->scope == scope && temp->is_hidden == 0)
            {
                temp->is_hidden = 1;
            }
            else if (temp->next != NULL)
            {
                while (temp != NULL)
                {
                    if (temp->scope == scope)
                    {
                        temp->is_hidden = 1;
                    }
                    temp = temp->next;
                }
            }
        }
    }
    
    scope = scope - 1;
}

int get_scope()
{
    return scope;
}

void dump_symbol_table()
{
    printf("|%-10s |%-10s |%-10s |%-10s |%-10s\n", "id", "scope", "value", "hidden", "function");

    for (khint_t k = kh_begin(handle); k != kh_end(handle); ++k)
    {
        if (kh_exist(handle, k))
        {
            sym_ptr temp = kh_value(handle, k);

            if (temp->next == NULL)
            {
                printf("%-10s %-10d %-10d %-10d %-10d\n", temp->identifier, temp->scope, temp->value, temp->is_hidden, temp->is_function);
                
                if (temp->is_function == 1)
                {
                    int i = 0;
                    sym_ptr tmp = NULL;

                    vec_foreach(&temp->params, tmp, i)
                    {
                        printf("--> %-10s %-10d %-10d %-10d %-10d\n", tmp->identifier, tmp->scope, tmp->value, tmp->is_hidden, tmp->is_function);
                    }
                }
            }
            else 
            {
                printf("%-10s %-10d %-10d %-10d %-10d\n", temp->identifier, temp->scope, temp->value, temp->is_hidden, temp->is_function);
                temp = temp->next;

                while (temp != NULL)
                {
                    printf("--> %-10s %-10d %-10d %-10d %-10d\n", temp->identifier, temp->scope, temp->value, temp->is_hidden, temp->is_function);    
                    temp = temp->next;
                }
            }
        }
    }
}
void close_symbol_table()
{
    for (khint_t k = kh_begin(handle); k != kh_end(handle); ++k)
    {
        if (kh_exist(handle, k))
        {
            if (kh_value(handle, k)->next == NULL)
            {
                if (kh_value(handle, k)->is_function == 1)
                {
                    vec_deinit(&kh_value(handle, k)->params);
                }
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