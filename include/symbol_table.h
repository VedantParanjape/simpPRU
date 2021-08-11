#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "klib/khash.h"
#include "vec/vec.h"
#include "pin_config.h"

#define DT_UNDEF 0
#define DT_INTEGER 1
#define DT_BOOLEAN 2
#define DT_VOID_ 3
#define DT_CHAR_ 4
#define DT_INT_ARR 5
#define DT_BOOL_ARR 6
#define DT_CHAR_ARR 7

typedef vec_t(struct symbol*) param;
typedef struct symbol
{
    char* identifier;
    int data_type;
    int value;
    int array_size;
    int scope; 
    struct symbol* next;
    int is_hidden;
    int is_function;
    int is_constant;
    param params;
}*sym_ptr, sym;

KHASH_MAP_INIT_STR(symbol_table, sym_ptr);

void init_symbol_table();
int insert_symbol_table(char* id, int dt_type, int val, int is_function);
sym_ptr lookup_symbol_table(char* id, int scope);
void increment_scope();
void decrement_scope();
int get_scope();
void dump_symbol_table();
void close_symbol_table();

#endif