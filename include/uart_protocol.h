#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define UART_RESPONSE_BUF_SIZE 128
#define HASH_TABLE_SIZE 32
#define PREFIX_CHAR_SIZE 4
#define FUNCTION_CHAR_SIZE 4
#define ACTION_CHAR_SIZE 3
#define DATA_CHAR_SIZE 100

enum DATA_TYPE {
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_UNDEFINED
};

typedef struct CommandInfo {
    const char *function;
    const char *action;
    int data_len;
    enum DATA_TYPE data_type;
    void *data;
    void (*handler)(const char *data);
} CommandInfo;

typedef struct HashNode {
    CommandInfo cmd;
    struct HashNode *next;
} HashNode;

void uart_response_init(void (*resp_function)(const char *msg));
void parse_command(const char *cmd);
void register_command(const CommandInfo *cmd);
void register_all_commands(void);

#endif
