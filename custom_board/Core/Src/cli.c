#include "cli.h"
#include "usart.h"
#include "debug.h"

#define CMD_BUF_SIZE     256
#define CLI_PROMPT    "\r\n>>> "
static char     cli_rx_buf[CMD_BUF_SIZE];
static volatile uint16_t cli_rx_idx;


typedef struct
{
    char *cmd;                                          // Command
    void (*cmd_func)(uint8_t argc, char **argv);        // Function for command processing
    char *cmd_usage;                                    // Command usage
} cli_cmd_t;

static void cli_show_command(char* cmd);
static void cli_process_command(void);

/**
 * register commands and functions
*/
static void cli_cmd_help(uint8_t argc, char **argv);
static void cli_cmd_reset(uint8_t argc, char **argv);
static void cli_cmd_debug(uint8_t argc, char **argv);
const cli_cmd_t cli_commands[] =
{
    {
        "help",
        cli_cmd_help,
        "usage : help \r\n \
        "
    },
    {
        "reset",
        cli_cmd_reset,
        "usage : reset \r\n \
        \r\t\t  sw \r\n \
        \r\t\t  wdt \r\n \
        \r\t\t  error \r\n \
        \r\t\t  assert \r\n \
        \r\t\t  hard \r\n \
        "
    },
    {
        "debug",
        cli_cmd_debug,
        "usage : debug \r\n \
        \r\t\t  log \r\n \
        "
    }
};

static void cli_cmd_help(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        printf("Registered commands \r\n");
        for (uint8_t idx=0; idx < sizeof(cli_commands)/sizeof(cli_cmd_t); idx++)
        {
            printf("\t\t\t| %s\t| \r\n", cli_commands[idx].cmd);
        }
    }
    else
    {
        cli_show_command(argv[0]);
    }

}

static void cli_cmd_reset(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        cli_show_command(argv[0]);
    }
    else
    {
        if (strcmp(argv[1], "sw") == 0)
        {
            NVIC_SystemReset();
        }
        else if (strcmp(argv[1], "wdt") == 0)
        {
            while (1);
        }
        else if (strcmp(argv[1], "error") == 0)
        {
            Error_Handler();
        }
        else if (strcmp(argv[1], "assert") == 0)
        {
            assert_param(false);
        }
        else if (strcmp(argv[1], "hard") == 0)
        {
            printf("hard fault \r\n");
            uint8_t zero = 1/0;
            (char)zero;
        }
        else
        {
            cli_show_command(argv[0]);
        }
    }
}

static void cli_cmd_debug(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        cli_show_command(argv[0]);
    }
    else
    {
        if (strcmp(argv[1], "log") == 0)
        {
            debug_show_log();   
        }
        else
        {
            cli_show_command(argv[0]);
        }
    }
}


static void cli_show_command(char* cmd)
{
    for (uint8_t idx=0; idx < sizeof(cli_commands)/sizeof(cli_cmd_t); idx++)
    {
        if (strcmp(cmd, cli_commands[idx].cmd) == 0)
        {
            printf("%s %s", cli_commands[idx].cmd_usage, CLI_PROMPT);
            return ;
        }
    }
}

static void cli_process_command(void)
{
    char *arguments[32];
    uint8_t argc = 0;
    char *token = strtok(cli_rx_buf, " ");

    uint8_t cmd_idx = 0;
    uint8_t cmd_max = sizeof(cli_commands)/sizeof(cli_cmd_t);

    while (token != NULL)
    {
        arguments[argc++] = token;
        token = strtok(NULL, " ");
    }

    for (cmd_idx=0; cmd_idx < cmd_max; cmd_idx++)
    {
        if (strcmp(arguments[0], cli_commands[cmd_idx].cmd) == 0)
        {
            cli_commands[cmd_idx].cmd_func(argc, arguments);
            break;
        }
    }

    if (cmd_idx == cmd_max)
    {
        printf("'%s ...' is not registered \r\n", arguments[0]);
    }

    // clear
    memset(cli_rx_buf, 0, sizeof(cli_rx_buf));
    cli_rx_idx = 0;
    printf(CLI_PROMPT);
}

void cli_read_character(uint8_t ch)
{
    /* check valid character */
    if ((ch >= ' ') && (ch <= '~'))
    {
        cli_rx_buf[cli_rx_idx++ & (CMD_BUF_SIZE - 1)] = ch;
        printf("%c", ch);
    }

    if ((ch == '\b') && cli_rx_idx>0)
    {
        cli_rx_idx--;
        printf("\b\x1B[K");
    }

    if ((ch == '\t') && cli_rx_idx == 0)
    {
        memcpy(cli_rx_buf, "help", 4);
        cli_process_command();
        printf(CLI_PROMPT);
    }

    if ((ch == '\r') || (ch == '\n'))
    {
        printf("\r\n");
        if (cli_rx_idx > 1)
        {
            cli_process_command();
        }
        cli_rx_idx = 0;
        printf(CLI_PROMPT);
    }

}