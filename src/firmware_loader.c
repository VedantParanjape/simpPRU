#include "firmware_loader.h"

static int model_beaglebone_ = 0;

int firmware_loader(char* output_filename, int pru_id)
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
        model_beaglebone_ = MODEL_BEAGLEBONE_BLACK_WIRELESS;
        printf("Detected %s\n", model_name);
    }
    // Add BBB also
    else if (!strcmp(model_name, "TI AM335x PocketBeagle"))
    {
        model_beaglebone_ = MODEL_POCKETBEAGLE;
        printf("Detected %s\n", model_name);
    }
    else if (!strcmp(model_name, "BeagleBoard.org BeagleBone AI"))
    {
        model_beaglebone_ = MODEL_BEAGLEBONE_AI;
        printf("Detected %s\n", model_name);
    }
    else
    {
        model_beaglebone_ = MODEL_POCKETBEAGLE;
        printf("Detected %s\n", model_name);
    }
    // different for bb ai
    char command[700];
    snprintf(command, 700, "cp %s /lib/firmware/am335x-pru%d-fw", output_filename, pru_id);
    if (system(command) == -1)
    {
        return -1;
    }

    return 1;
}