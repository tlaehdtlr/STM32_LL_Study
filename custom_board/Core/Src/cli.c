#include "cli.h"
#include "usart.h"

#define CMD_BUF_SIZE     256
#define CLI_PROMPT    "\r\n>>> "
static char     cli_rx_buf[CMD_BUF_SIZE];
static volatile uint16_t cli_rx_idx;


typedef struct
{
    char *cmd;                                          // Command
    void (*cmd_func)(uint8_t argc, void **argv);        // Function for command processing
    char *cmd_usage;                                    // Command usage
} cli_cmd_t;

static void cli_show_commands(void);
static void cli_process_command(void);

static void cli_show_commands(void)
{
    ;
}

static void cli_process_command(void)
{
   ;
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
        cli_show_commands();
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