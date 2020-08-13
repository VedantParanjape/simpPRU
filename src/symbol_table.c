#include "symbol_table.h"
#include <stdio.h>

static int scope;
khash_t(symbol_table) *handle = NULL;

void init_symbol_table()
{
    handle = kh_init(symbol_table);
    scope = 0;

    cJSON *json_head = NULL;
    switch (read_device_model())
    {
        case MODEL_POCKETBEAGLE:
            json_head = cJSON_Parse(POCKETBEAGLE_PINOUT);
            break;
        
        case MODEL_BEAGLEBONE_BLACK:
        case MODEL_BEAGLEBONE_BLACK_WIRELESS:
            json_head = cJSON_Parse(BEAGLEBONEBLACK_PINOUT);
            break;

        case MODEL_BEAGLEBONE_AI:
            json_head = cJSON_Parse(BEAGLEBONEAI_PINOUT);
            break;
            
        default:    
            json_head = cJSON_Parse(POCKETBEAGLE_PINOUT);
            break;
    }
    
    if (json_head != NULL)
    {
        cJSON *pru_head_0 = cJSON_GetObjectItemCaseSensitive(json_head, "pru0");
        cJSON *pru_head_1 = cJSON_GetObjectItemCaseSensitive(json_head, "pru1");
        
        if (pru_head_0 != NULL && pru_head_1 != NULL)
        {
            cJSON *pru_0_pins = NULL;
            char *key_0 = NULL;
            cJSON_ArrayForEach(pru_0_pins, pru_head_0)
            {
                key_0 = pru_0_pins->string;
                for (int i = 0; i < cJSON_GetArraySize(pru_0_pins); i++)
                {
                    cJSON* elem = cJSON_GetArrayItem(pru_0_pins, i);
                    if (elem != NULL)
                    {
                        sym_ptr pin = (sym_ptr) malloc(sizeof(sym));
                        pin->identifier = strdup(cJSON_GetArrayItem(elem, 0)->valuestring);
                        pin->data_type = DT_INTEGER;
                        pin->value = atoi(key_0);
                        pin->scope = 0;
                        pin->next = NULL;
                        pin->is_hidden = 0;
                        pin->is_function = 0;
                        pin->is_constant = 1;

                        int err;
                        khint_t temp = kh_put(symbol_table, handle, strdup(pin->identifier), &err);
                        if (temp != -1)
                        {
                            kh_value(handle, temp) = pin;
                        }
                    }
                }
            }

            cJSON *pru_1_pins = NULL;
            char *key_1 = NULL;

            cJSON_ArrayForEach(pru_1_pins, pru_head_1)
            {
                key_1 = pru_1_pins->string;
                for (int i = 0; i < cJSON_GetArraySize(pru_1_pins); i++)
                {
                    cJSON* elem = cJSON_GetArrayItem(pru_1_pins, i);
                    if (elem != NULL)
                    {
                        sym_ptr pin = (sym_ptr) malloc(sizeof(sym));
                        pin->identifier = strdup(cJSON_GetArrayItem(elem, 0)->valuestring);
                        pin->data_type = DT_INTEGER;
                        pin->value = atoi(key_1);
                        pin->scope = 0;
                        pin->next = NULL;
                        pin->is_hidden = 0;
                        pin->is_function = 0;
                        pin->is_constant = 1;

                        int err;
                        khint_t temp = kh_put(symbol_table, handle, strdup(pin->identifier), &err);
                        if (temp != -1)
                        {
                            kh_value(handle, temp) = pin;
                        }
                    }
                }
            }
        }

        if (read_device_model() == MODEL_BEAGLEBONE_AI)
        {
            cJSON *pru_head_2 = cJSON_GetObjectItemCaseSensitive(json_head, "pru2");
            cJSON *pru_head_3 = cJSON_GetObjectItemCaseSensitive(json_head, "pru3");
            
            if (pru_head_2 != NULL && pru_head_3 != NULL)
            {
                cJSON *pru_2_pins = NULL;
                char *key_2 = NULL;
                cJSON_ArrayForEach(pru_2_pins, pru_head_2)
                {
                    key_2 = pru_2_pins->string;
                    for (int i = 0; i < cJSON_GetArraySize(pru_2_pins); i++)
                    {
                        cJSON* elem = cJSON_GetArrayItem(pru_2_pins, i);
                        if (elem != NULL)
                        {
                            sym_ptr pin = (sym_ptr) malloc(sizeof(sym));
                            pin->identifier = strdup(cJSON_GetArrayItem(elem, 0)->valuestring);
                            pin->data_type = DT_INTEGER;
                            pin->value = atoi(key_2);
                            pin->scope = 0;
                            pin->next = NULL;
                            pin->is_hidden = 0;
                            pin->is_function = 0;
                            pin->is_constant = 1;

                            int err;
                            khint_t temp = kh_put(symbol_table, handle, strdup(pin->identifier), &err);
                            if (temp != -1)
                            {
                                kh_value(handle, temp) = pin;
                            }
                        }
                    }
                }

                cJSON *pru_3_pins = NULL;
                char *key_3 = NULL;

                cJSON_ArrayForEach(pru_3_pins, pru_head_3)
                {
                    key_3 = pru_3_pins->string;
                    for (int i = 0; i < cJSON_GetArraySize(pru_3_pins); i++)
                    {
                        cJSON* elem = cJSON_GetArrayItem(pru_3_pins, i);
                        if (elem != NULL)
                        {
                            sym_ptr pin = (sym_ptr) malloc(sizeof(sym));
                            pin->identifier = strdup(cJSON_GetArrayItem(elem, 0)->valuestring);
                            pin->data_type = DT_INTEGER;
                            pin->value = atoi(key_3);
                            pin->scope = 0;
                            pin->next = NULL;
                            pin->is_hidden = 0;
                            pin->is_function = 0;
                            pin->is_constant = 1;

                            int err;
                            khint_t temp = kh_put(symbol_table, handle, strdup(pin->identifier), &err);
                            if (temp != -1)
                            {
                                kh_value(handle, temp) = pin;
                            }
                        }
                    }
                }
            }
        }
    }
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
        new->is_constant = 0;

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
        sym_ptr base = kh_value(handle, kh_get(symbol_table, handle, id));            
        sym_ptr temp = NULL;

        if (is_function == 0 && base->is_function == 0)
        {
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
            base->is_constant = 0;

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
    else if (kh_value(handle, temp)->next == NULL && kh_value(handle, temp)->scope <= scope && kh_value(handle, temp)->is_hidden == 0)
    {   
        return kh_value(handle, temp);
    }
    else if (kh_value(handle, temp)->next != NULL)
    {
        sym_ptr temp2 = kh_value(handle, temp);
        sym_ptr temp3 = NULL;

        while (temp2 != NULL)
        {
            if (temp2->scope == scope && temp2->is_hidden == 0)
            {
                return temp2;
            }
            temp3 = temp2;
            temp2 = temp2->next;
        }

        if (temp3 != NULL)
        {
            if (temp3->scope < scope)
            {
                return temp3;
            }
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
    printf("|%-10s |%-10s |%-10s |%-10s |%-10s |%-10s\n", "id", "scope", "value", "hidden", "function", "constant");

    for (khint_t k = kh_begin(handle); k != kh_end(handle); ++k)
    {
        if (kh_exist(handle, k))
        {
            sym_ptr temp = kh_value(handle, k);

            if (temp->next == NULL)
            {
                printf("%-10s %-10d %-10d %-10d %-10d %-10d\n", temp->identifier, temp->scope, temp->value, temp->is_hidden, temp->is_function, temp->is_constant);
                
                if (temp->is_function == 1)
                {
                    int i = 0;
                    sym_ptr tmp = NULL;

                    vec_foreach(&temp->params, tmp, i)
                    {
                        printf("--> %-10s %-10d %-10d %-10d %-10d %-10d\n", tmp->identifier, tmp->scope, tmp->value, tmp->is_hidden, tmp->is_function, tmp->is_constant);
                    }
                }
            }
            else 
            {
                printf("%-10s %-10d %-10d %-10d %-10d %-10d\n", temp->identifier, temp->scope, temp->value, temp->is_hidden, temp->is_function, temp->is_constant);
                temp = temp->next;

                while (temp != NULL)
                {
                    printf("--> %-10s %-10d %-10d %-10d %-10d %-10d\n", temp->identifier, temp->scope, temp->value, temp->is_hidden, temp->is_function, temp->is_constant);    
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