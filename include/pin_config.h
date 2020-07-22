#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include "cJSON/cJSON.h"
#include "klib/khash.h"
#include <stdio.h>
#include <stdlib.h>

#define POCKETBEAGLE_PINOUT "{\"pru0\": {\"16\": [[\"P1_20\", 1]], \"7\": [[\"P1_29\", 3]], \"4\": [[\"P1_31\", 3]], \"1\": [[\"P1_33\", 3]], \"0\": [[\"P1_36\", 3]], \"15\": [[\"P2_18\", 1],[\"P2_33\", 2]], \"14\": [[\"P2_22\", 1],[\"P2_24\", 2]], \"6\": [[\"P2_28\", 3]], \"3\": [[\"P2_30\", 3]], \"2\": [[\"P2_32\", 3]], \"5\": [[\"P2_34\", 3]]}, \"pru1\": {\"9\": [[\"P1_02\", 3]], \"11\": [[\"P1_04\", 3]], \"15\": [[\"P1_30\", 3]], \"14\": [[\"P1_32\", 3]], \"10\": [[\"P1_35\", 3]], \"16\": [[\"P2_31\", 1]], \"8\": [[\"P2_35\", 3]]}}"
#define BEAGLEBONEBLACK_PINOUT "{\"pru0\": {\"15\": [[\"P8_15\", 1],[\"P8_11\", 2]], \"14\": [[\"P8_16\", 1],[\"P8_12\", 2]], \"7\": [[\"P9_25\", 3]], \"5\": [[\"P9_27\", 3]], \"3\": [[\"P9_28\", 3]], \"1\": [[\"P9_29\", 3]], \"2\": [[\"P9_30\", 3]], \"0\": [[\"P9_31\", 3]], \"6\": [[\"P9_41\", 3]], \"4\": [[\"P9_92\", 3]]}, \"pru1\": {\"13\": [[\"P8_20\", 3]], \"12\": [[\"P8_21\", 3]], \"8\": [[\"P8_27\", 3]], \"10\": [[\"P8_28\", 3]], \"9\": [[\"P8_29\", 3]], \"6\": [[\"P8_39\", 3]], \"7\": [[\"P8_40\", 3]], \"4\": [[\"P8_41\", 3]], \"5\": [[\"P8_42\", 3]], \"2\": [[\"P8_43\", 3]], \"3\": [[\"P8_44\", 3]], \"0\": [[\"P8_45\", 3]], \"1\": [[\"P8_46\", 3]]}}"

#define IN 1
#define OUT 2

#define MODEL_AUTODETECT 0
#define MODEL_POCKETBEAGLE 1
#define MODEL_BEAGLEBONE_BLACK 2
#define MODEL_BEAGLEBONE_BLACK_WIRELESS 3
#define MODEL_BEAGLEBONE_AI 4

KHASH_MAP_INIT_INT(pin_config, int);

int config_pins(int pru_pin_number, int mode, char *pru_id);
int set_device_model(int model_bb);
int read_device_model();

#endif