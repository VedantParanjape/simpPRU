#include "klib/khash.h"

#define DT_UNDEF 0
#define DT_INTEGER 1
#define DT_BOOLEAN 2

typedef struct symbol
{
    char* identifier;
    int data_type;
    int value;
    int scope; 
    struct symbol* next;
}*sym_ptr, sym;

KHASH_MAP_INIT_STR(symbol_table, sym_ptr);

void init_symbol_table();
int insert_symbol_table(char* id, int dt_type, int val);
sym_ptr lookup_symbol_table(char* id, int scope);
void increment_scope();
void decrement_scope();
int get_scope();
void close_symbol_table();