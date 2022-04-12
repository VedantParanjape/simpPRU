#ifndef CODE_PRINTER_H
#define CODE_PRINTER_H

#include "ast.h"
#include "symbol_table.h"
#include "pin_config.h"
#include "vec/vec.h"

#include <stdio.h>
#include <stdlib.h>

#define _OPR_LFT " << "
#define _OPR_RGT " >> "
#define _OPR_ADD " + " 
#define _OPR_SUB " - " 
#define _OPR_MUL " * " 
#define _OPR_DIV " / " 
#define _OPR_MOD " % "

#define _OPR_GT " > "  
#define _OPR_LT " < " 
#define _OPR_EQ " == " 
#define _OPR_NE " != " 
#define _OPR_GE " >= " 
#define _OPR_LE " <= " 

#define _OPR_BW_NOT  " ~ " 
#define _OPR_BW_AND  " & " 
#define _OPR_BW_OR   " | "
#define _OPR_BW_XOR  " ^ " 
#define _OPR_LGL_NOT " ! " 
#define _OPR_LGL_AND " && " 
#define _OPR_LGL_OR  " | " 

#define _OPR_ASSIGNMENT " = " 

#define _DT_INT_ "int"
#define _DT_VOID_ "void"
#define _DT_BOOL_ "bool"
#define _DT_CHAR_ "char"

#define TEST "#include<stdio.h>\n\nvoid digital_write(int pin, int value)\n{\n    printf(\"digital_write called with pin %d and value %d\\n\", pin, value);\n}\n\nint digital_read(int pin)\n{\n    printf(\"digital_read called with pin %d\\n\", pin);\n    return 0;\n}\n\nvoid start_counter()\n{\n    printf(\"start_counter called\\n\");\n}\n\nvoid stop_counter()\n{\n    printf(\"stop_counter called\\n\");\n}\n\nint read_counter()\n{\n    printf(\"read_counter called\\n\");\n    return 0;\n}\n\nvoid init_rpmsg()\n{\n    printf(\"init_rpmsg called\\n\");\n}\n\nvoid send_rpmsg(int data)\n{\n    printf(\"send_rpmsg called\\n\");\n}\n\nint receive_rpmsg()\n{\n    printf(\"receive_rpmsg called\\n\");\n    return 0;\n}\n"
#define BEGIN "#include <stdint.h>\n#include <pru_types.h>\n#include <stddef.h>\n#include <pru/io.h>\n\n"
#define MAIN "int main(void)\n{\n"
#define END "\n\treturn 0;\n}\n"
#define DIGITAL_WRITE "void digital_write(int pin, int value)\n{\n\tif(value == 0) write_r30(read_r31()|0);\n\telse write_r30(read_r31()|((uint32_t) 1 << (pin%32)));\n}\n\n"
#define DIGITAL_READ "int digital_read(int pin)\n{\n\tif (read_r31() & ((uint32_t) 1 << (pin%32))) return 1;\n\telse return 0;\n}\n\n"
#define START_COUNTER "void start_counter()\n{\n\tPRU_CTRL.CYCLE = 0;\n\tPRU_CTRL.CONTROL_bit.COUNTER_ENABLE = 1;\n}\n\n"
#define STOP_COUNTER "void stop_counter()\n{\n\tPRU_CTRL.CONTROL_bit.COUNTER_ENABLE = 0;\n}\n\n"
#define READ_COUNTER "int read_counter()\n{\n\treturn PRU_CTRL.CYCLE;\n}\n\n"
#define RPMSG_DEFS "#if CONFIG_ENABLE_RPMSG == 1\n #include <stdlib.h>\n #include <string.h>\n #include \"pru_cfg.h\"\n #include \"pru_intc.h\"\n #include \"rsc_types.h\"\n #include \"pru_virtqueue.h\"\n #include \"pru_rpmsg.h\"\n\n #if defined(__AM335X_PRU0__)\n  #include \"resource_table_0.h\"\n  #define HOST_INT   ((uint32_t) 1 << 30)\n  #define TO_ARM_HOST       16\n  #define FROM_ARM_HOST 17\n  #define CHAN_NAME  \"rpmsg-pru\"\n  #if defined(__AM572X_ICSS1_PRU0__)\n   #define CHAN_DESC  \"Channel 32\"\n   #define CHAN_PORT  32\n  #else\n   #define CHAN_DESC  \"Channel 30\"\n   #define CHAN_PORT  30\n  #endif\n #elif defined(__AM335X_PRU1__)\n  #include \"resource_table_1.h\"\n  #define HOST_INT      ((uint32_t) 1 << 31)\n  #define TO_ARM_HOST   18\n  #define FROM_ARM_HOST 19\n  #define CHAN_NAME     \"rpmsg-pru\"\n  #if defined(__AM572X_ICSS1_PRU1__)\n   #define CHAN_DESC  \"Channel 33\"\n   #define CHAN_PORT  33\n  #else\n   #define CHAN_DESC  \"Channel 31\"\n   #define CHAN_PORT  31\n  #endif\n #endif\n\n #define VIRTIO_CONFIG_S_DRIVER_OK 4\n\n char payload[RPMSG_BUF_SIZE];\n int start, stop, i;\n\n static uint16_t src, dst, len;\n static struct pru_rpmsg_transport transport;\n static volatile uint8_t *status;\n\n void init_rpmsg()\n {\n  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;\n  CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;\n\n  status = &resourceTable.rpmsg_vdev.status;\n  while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));\n\n  /* Initialize the RPMsg transport structure */\n  pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);\n\n  /* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */\n  while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);\n  while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) != PRU_RPMSG_SUCCESS);\n }\n\n void send_int_rpmsg(int data)\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n\n  itoa(data, payload, 10);\n  strcat(payload, \"\\n\");\n\n  pru_rpmsg_send(&transport, dst, src, payload, strlen(payload));\n }\n\n void send_char_rpmsg(char data)\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n\n  payload[0] = data;\n  payload[1] = '\\n';\n\n  pru_rpmsg_send(&transport, dst, src, payload, strlen(payload));\n }\n\n void send_bool_rpmsg(int data)\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n\n  if (data == 0)\n  {\n   strcpy(payload, \"False\\n\");\n  }\n  else\n  {\n   strcpy(payload, \"True\\n\");\n  }\n\n  pru_rpmsg_send(&transport, dst, src, payload, strlen(payload));\n }\n\n void send_ints_rpmsg(int *arr, int len)\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n\n  int start = 0;\n  for (int i = 0; i < len; i++)\n  {\n   if (start < RPMSG_BUF_SIZE - 11)\n   {\n    itoa(arr[i], payload + start, 10);\n    start += strlen(payload + start);\n    payload[start++] = ' ';\n   }\n  }\n  payload[start] = '\\n';\n\n  pru_rpmsg_send(&transport, dst, src, payload, strlen(payload));\n }\n\n void send_chars_rpmsg(char *arr, int len)\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n\n  stop = (len < 511) ? len : 510;\n  for (i = 0; i < stop; i++)\n  {\n   payload[i] = arr[i];\n  }\n  if (arr[len - 1] == '\\0')\n  {\n   payload[i - 1] = '\\n';\n  }\n  else\n  {\n   payload[i] = '\\n';\n  }\n\n  pru_rpmsg_send(&transport, dst, src, payload, strlen(payload));\n }\n\n void send_bools_rpmsg(int *arr, int len)\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n\n  stop = (len < 511) ? len : 510;\n  for (i = 0; i < stop; i++)\n  {\n   itoa(arr[i], payload + i, 10);\n  }\n  payload[i] = '\\n';\n\n  pru_rpmsg_send(&transport, dst, src, payload, strlen(payload));\n }\n\n int receive_rpmsg()\n {\n  memset(payload, '\\0', RPMSG_BUF_SIZE);\n  while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) != PRU_RPMSG_SUCCESS);\n\n  return atoi(payload);\n }\n#else\n #include \"resource_table.h\"\n#endif\n"
// replace all " with \", also replace all \0 with \\0, and \n with \\n

void ast_compound_statement_printer(ast_node_compound_statement *cmpd_stmt, FILE* handle, int is_func_def);
void ast_declaration_printer(ast_node_declaration *decl, FILE* handle);
void ast_array_declaration_printer(ast_node_array_declaration *decl, FILE* handle);
void ast_assignment_printer(ast_node_assignment *assg, FILE* handle);
void ast_array_assignment_printer(ast_node_array_assignment *assign, FILE *handle);
void ast_array_access_printer(ast_node_array_access *access, FILE* handle);
void ast_expression_printer(ast_node_expression *node, FILE* handle);
void ast_conditional_if_printer(ast_node_conditional_if *node, FILE* handle);
void ast_loop_for_printer(ast_node_loop_for *node, FILE* handle);
void ast_loop_while_printer(ast_node_loop_while *node, FILE* handle);
void ast_function_call_printer(ast_node_function_call *fc, FILE* handle);
void ast_utility_function_call_printer(ast_node_utility_function_call *ufc, FILE* handle);
void ast_print_string_function_call_printer(ast_node_print_string_function_call *pfc, FILE* handle);
void ast_print_expression_function_call_printer(ast_node_print_expression_function_call *pfc, FILE *handle);
void ast_function_definition(ast_node_function_def *def, FILE* handle);
int code_printer(ast_node* ast, int pru_id, int test);

#endif
