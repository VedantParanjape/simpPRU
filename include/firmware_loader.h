#ifndef FIRMWARE_LOADER_H
#define FIRMWARE_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODEL_POCKETBEAGLE 1
#define MODEL_BEAGLEBONE_BLACK 2
#define MODEL_BEAGLEBONE_BLACK_WIRELESS 3
#define MODEL_BEAGLEBONE_AI 4
#define MODEL_BEAGLEBONE_BLUE 5

int firmware_loader(char* output_filename, int pru_id);

#endif