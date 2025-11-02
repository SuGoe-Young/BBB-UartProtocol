#include "uart_protocol.h"


// hash
static HashNode *hash_table[HASH_TABLE_SIZE];

static uint32_t hash_key(const char *func, const char *action)
{
    uint32_t hash = 5381;
    const unsigned char *p = (const unsigned char *)func;
    while (*p)
        hash = ((hash << 5) + hash) + *p++;  // hash * 33 + c

    p = (const unsigned char *)action;
    while (*p)
        hash = ((hash << 5) + hash) + *p++;

    return hash;
}
// ---------------------------------------------------------


// for respons (callback)
static void (*uart_responder)(const char *msg);

void uart_response_init(void resp_function(const char *msg))
{
    uart_responder = resp_function;
}
// ---------------------------------------------------------


// set command
static CommandInfo cmd_table[] = {
    {"FOV", "SET", 0, TYPE_FLOAT, NULL, fov_set_handler},
    {"FOV", "GET", 0, TYPE_UNDEFINED, NULL, fov_get_handler},
};
static const int CMD_COUNT = sizeof(cmd_table) / sizeof(cmd_table[0]);


void register_command(CommandInfo *cmd)
{
    char resp[UART_RESPONSE_BUF_SIZE];

    uint32_t key = hash_key(cmd->function, cmd->action);
    uint32_t idx = key % HASH_TABLE_SIZE;
    
    HashNode *node = malloc(sizeof(HashNode));
    if (!node)
    {
        snprintf(resp, "[register_command] new node fail!\n");
        if (uart_responder) uart_responder(resp);
    }

    node->cmd = *cmd;
    node->next = hash_table[idx];
    hash_table[idx] = node;
}
// ---------------------------------------------------------

void parse_command(const char *cmd)
{
    if (!cmd || strlen(cmd) == 0) return;

    char prefix[PREFIX_CHAR_SIZE + 1];
    char function[FUNCTION_CHAR_SIZE + 1];
    char action[ACTION_CHAR_SIZE + 1];
    char data[DATA_CHAR_SIZE + 1];
    char endfix[2] = {0};
    int data_len = 0;
    char resp[UART_RESPONSE_BUF_SIZE];

    int ret = sscanf(cmd, "%4[^:]:%4[^:]:%3[^:]:%d:%100[^@]%1s",
                     prefix, function, action, &data_len, data, endfix);

    if (ret < 5 || strcmp(endfix, "@") != 0) {
        snprintf(resp, sizeof(resp), "ERR:BadFormat@\n");
        if (uart_responder) uart_responder(resp);
        return;
    }

    if ((int)strlen(data) != data_len) {
        snprintf(resp, sizeof(resp), "ERR:LenMismatch@\n");
        if (uart_responder) uart_responder(resp);
        return;
    }

    uint32_t key = hash_key(function, action);
    uint32_t idx = key % HASH_TABLE_SIZE;
    HashNode *node = hash_table[idx];

    while (node) {
        if (strcmp(node->cmd.function, function) == 0 &&
            strcmp(node->cmd.action, action) == 0)
        {
            if (node->cmd.handler) {
                node->cmd.handler(data);
                return;
            }
        }
        node = node->next;
    }

    snprintf(resp, sizeof(resp), "ERR:UnknownCmd@\n");
    if (uart_responder) uart_responder(resp);
}


static void fov_set_handler(const char*data)
{
    float horizon, vertical;
    char resp[UART_RESPONSE_BUF_SIZE];
    if (sscanf(data, "%f,%f", &horizon, &vertical) == 2)
    {
        // printf("[FOV] SET → H=%.2f, V=%.2f\n", horizon, vertical);
        snprintf(resp, sizeof(resp), "FOV set ok:H=%.2f,V=%.2f@", horizon, vertical);
        if (uart_responder) uart_responder(resp);
    }
    else
    {
        // printf("[FOV] Invalid data format, expected H,V\n");
        snprintf(resp, sizeof(resp), "Invalid data format, expected H,V\n");
        if (uart_responder) uart_responder(resp);
    }
}

static void fov_get_handler(const char *data)
{
    //... next step would using cJSON to compeletd
    char resp[UART_RESPONSE_BUF_SIZE];
    printf("VSEC:FOV:VALUE:45.0,30.0@\n");
    snprintf(resp, sizeof(resp), "FOV get ok:H=wow,V=yeah");
    if (uart_responder) uart_responder(resp);
}

