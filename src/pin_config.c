#include "pin_config.h"

khash_t(pin_config) *handle_pin_config = NULL;
static int init = 0;
static int model_beaglebone = 1;

int config_pins(int pru_pin_number, int mode, char* pru_id)
{
    if (init == 0)
    {
        handle_pin_config = kh_init(pin_config);
        init = 1;
    }

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

        default:    
            json_head = cJSON_Parse(POCKETBEAGLE_PINOUT);
            break;
    }
    if (json_head == NULL)
    {
        return -1;
    }

    cJSON *pru_head = cJSON_GetObjectItemCaseSensitive(json_head, pru_id);
    if (pru_head == NULL)
    {
        return -1;
    }

    char buffer[3];
    sprintf(buffer, "%d", pru_pin_number);
    cJSON *pin_head = cJSON_GetObjectItemCaseSensitive(pru_head, buffer);
    if (pin_head == NULL)
    {
        return -1;
    }

    int array_size = cJSON_GetArraySize(pin_head);
    for (int i = 0; i < array_size; i++)
    {
        cJSON* array = cJSON_GetArrayItem(pin_head, i);
        int pin_mode = cJSON_GetArrayItem(array, 1)->valueint;

        if (pin_mode == mode || pin_mode == 3)
        {
            char command_string[50];
            khint_t temp = kh_get(pin_config, handle_pin_config, pru_pin_number);
            
            switch(mode)
            {
                case IN:
                    if (temp == kh_end(handle_pin_config))
                    {
                        sprintf(command_string, "%s %s %s", "config-pin", cJSON_GetArrayItem(array, 0)->valuestring, "pruin");
                        printf("[%d] : setting %s as input\n", pru_pin_number, cJSON_GetArrayItem(array, 0)->valuestring);
                        
                        int err = 0;
                        int index = kh_put(pin_config, handle_pin_config, pru_pin_number, &err);
                        if (err == -1)
                        {
                            return err;
                        }
                        kh_value(handle_pin_config, index) = IN;
                    }
                    else if (kh_value(handle_pin_config, temp) != IN)
                    {   
                        printf("[%d] : pin already set\n", pru_pin_number);
                        return -1;
                    }
                    else if (kh_value(handle_pin_config, temp) == IN)
                    {
                        return 1;
                    }
                    break;

                case OUT:
                    if (temp == kh_end(handle_pin_config))
                    {
                        sprintf(command_string, "%s %s %s", "config-pin", cJSON_GetArrayItem(array, 0)->valuestring, "pruout");
                        printf("[%d] : setting %s as output\n", pru_pin_number, cJSON_GetArrayItem(array, 0)->valuestring);
                        
                        int err = 0;
                        int index = kh_put(pin_config, handle_pin_config, pru_pin_number, &err);
                        if (err == -1)
                        {
                            return err;
                        }
                        kh_value(handle_pin_config, index) = OUT;
                    }
                    else if (kh_value(handle_pin_config, temp) != OUT)
                    {
                        printf("[%d] : pin already set\n", pru_pin_number);
                        return -1;
                    }
                    else if (kh_value(handle_pin_config, temp) == OUT)
                    {
                        return 1;
                    }
                    break;
            }
            system(command_string);

            return 1;
        }
    }

    return -1;
}

int set_device_model(int model_bb)
{
    if (model_bb == MODEL_AUTODETECT)
    {
        FILE *model = fopen("/proc/device-tree/model", "r");
        if (model == NULL)
        {
            fprintf(stderr, "Error opening /proc/device-tree/model\n");
            return -1;
        }

        char model_name[50];
        for (int i = 0; fread(&model_name[i], sizeof(char), 1, model) == 1 ; i++);

        if (!strcmp(model_name, "TI AM335x BeagleBone Black Wireless"))
        {
            model_beaglebone = MODEL_BEAGLEBONE_BLACK_WIRELESS;
            printf("Detected %s\n", model_name);
        }
        else if (!strcmp(model_name, "TI AM335x PocketBeagle"))
        {
            model_beaglebone = MODEL_POCKETBEAGLE;
            printf("Detected %s\n", model_name);
        }
        else if (!strcmp(model_name, "BeagleBoard.org BeagleBone AI"))
        {
            model_beaglebone = MODEL_BEAGLEBONE_AI;
            printf("Detected %s\n", model_name);
        }
        else
        {
            model_beaglebone = MODEL_POCKETBEAGLE;
            printf("Detected %s\n", model_name);
        }
    }
    else
    {
        model_beaglebone = model_bb;
    }

    return 1;
}

int read_device_model()
{
    return model_beaglebone;
}