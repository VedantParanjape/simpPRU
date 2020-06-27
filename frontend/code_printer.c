#include "code_printer.h"

void code_printer(ast_node* ast)
{
    ast_node_type(ast->node_type);
    FILE* handle = fopen("../generated_code/temp.c", "w");

    int i = 0;
    ast_node *temp;

    fprintf(handle, "%s", BEGIN);
    vec_foreach(&ast->child_nodes, temp, i)
    {
        // ast_node_action(temp, temp->node_type, handle);   
    }
    fprintf(handle, "%s", END);
    fclose(handle);
}