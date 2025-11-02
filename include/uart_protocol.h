#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <stdio.h>     // printf()
#include <stdlib.h>    // atoi(), atof()
#include <string.h>    // strcmp(), sscanf()

#define UART_RESPONSE_BUF_SIZE 128

// command's things
#define HASH_TABLE_SIZE 32 // tmp
#define PREFIX_CHAR_SIZE 4 // tmp, 4(byte)
#define FUNCTION_CHAR_SIZE 4 //tmp, 4(byte)
#define ACTION_CHAR_SIZE 3 //tmp, 3(byte)
#define DATA_CHAR_SIZE  100 //tmp, 100(byte)




enum DATA_TYPE
{
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_UNDEFINED
};

typedef struct HashNode
{
    CommandInfo cmd;
    struct HashNode *next;
}HashNode;

typedef struct CommandInfo
{
    const char *function;
    const char *action;
    int data_len;
    enum DATA_TYPE data_type;
    void *data;
    void (*handler)(const char *data);
} CommandInfo;

void uart_response_callback(const char *msg);
void uart_response_init(void resp_function(const char *msg));
void parse_command(const char *cmd);
void register_command(CommandInfo *cmd);

static void fov_set_handler(const char*data);
static void fov_get_handler(const char *data);

#endif // UART_PROTOCOL_H