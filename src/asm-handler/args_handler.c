//
// Created by C4V4H.exe on 10/06/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "include/args_handler.h"

#include "include/asm_file_parser.h"

static void print_help(const char *program_name);
static char **split_args(const char *args_str, int *count);
static void add_breakpoint(options_t *opts, const char *addr);


void print_options(const options_t *opts) {
    if (opts->args_count > 0 && opts->args != NULL) {
        printf("Binary arguments: ");
        for (int i = 0; i < opts->args_count; i++) {
            printf("%s ", opts->args[i]);
        }
        printf("\n");
    }

    if (opts->breakpoint_count > 0 && opts->breakpoints != NULL) {
        printf("Breakpoints: ");
        for (int i = 0; i < opts->breakpoint_count; i++) {
            printf("%s ", opts->breakpoints[i]);
        }
        printf("\n");
    }
}

/**
 * @brief print help message
 * @param program_name name of the binary
 */
void print_help(const char *program_name) {
    printf("Usage: %s [binary_file] [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -f, --full-execution      Execute the binary without stopping\n");
    printf("  -s, --step                Execute step-by-step\n");
    printf("  -h, --help                Show this help message\n");
    printf("  -a, --args <ARGS>         Arguments to pass to the binary (space-separated)       NOTE: NOT IMPLEMENTED YET \n");
    printf("  -b, --breakpoint <ADDR>   Set breakpoint at address (can be used multiple times)  NOTE: NOT IMPLEMENTED YET \n");
    printf("\nExample:\n");
    printf("  %s program.bin -a \"arg1 arg2\" -b 0x1004\n", program_name);
}

/**
 * @brief splits the args str
 * @param args_str string containing the args
 * @param count number of the args
 * @return separated args
 */
char** split_args(const char *args_str, int *count) {
    if (!args_str) {
        *count = 0;
        return nullptr;
    }

    // count the arg number
          char *temp  = strdup(args_str);
    const char *token = strtok(temp, " ");

    *count = 0;
    while (token) {
        (*count)++;
        token = strtok(nullptr, " ");
    }
    free(temp);

    if (*count == 0) return nullptr;

    char **argv = malloc((*count + 1) * sizeof(char*));

    temp = strdup(args_str);
    token = strtok(temp, " ");
    int i = 0;
    while (token && i < *count) {
        argv[i] = strdup(token);
        token = strtok(nullptr, " ");
        i++;
    }
    argv[*count] = nullptr;

    free(temp);
    return argv;
}

/**
 * @brief adds in the options the list of the breakpoints to add
 * @param opts options
 * @param addr address where to add a breakpoint
 */
void add_breakpoint(options_t *opts, const char *addr) {
    opts->breakpoint_count++;
    char** breakpoints = realloc(opts->breakpoints,
                               opts->breakpoint_count * sizeof(char*));
    if (!breakpoints) return;
    opts->breakpoints = breakpoints;
    opts->breakpoints[opts->breakpoint_count - 1] = strdup(addr);
}


/**
 * @brief free options
 * @param opts options obj to free
 */
void free_options(options_t *opts) {
    if (!opts) return;

    if (opts->binary_file) free(opts->binary_file);

    if (opts->args) {
        for (int i = 0; i < opts->args_count; i++) {
            free(opts->args[i]);
        }
        free(opts->args);
    }

    if (opts->breakpoints) {
        for (int i = 0; i < opts->breakpoint_count; i++) {
            free(opts->breakpoints[i]);
        }
        free(opts->breakpoints);
    }

    if (opts->text_data) free(opts->text_data);


    if (opts->data_data) free(opts->data_data);

    free(opts);
}

/**
 * @brief handle the program's args
 * @param argc arg count
 * @param argv arg vector
 * @param opts options (obj containing the options)
 * @return 1: printed help. 0: success, -1: error occurred
 */
int handle_args(
    int argc,
    char *argv[],
    options_t *opts
) {
    const struct option long_options[] = {
        {"full-execution", no_argument, nullptr, 'f'},
        {"step", no_argument, nullptr, 's'},
        {"help", no_argument, nullptr, 'h'},
        {"args", required_argument, nullptr, 'a'},
        {"breakpoint", required_argument, nullptr, 'b'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    int option_index = 0;

    // if the first element does not start with the '-' then it is the bin path
    if (argc > 1 && argv[1][0] != '-') {
        opts->binary_file = strdup(argv[1]);
        for (int i = 1; i < argc - 1; i++) {
            argv[i] = argv[i + 1];
        }
        argc--;
    }

    opts->execution_mode = DEFAULT;
    while (
        (opt = getopt_long(
            argc,
            argv,
            "fsha:b:",
            long_options,
            &option_index
        )) != -1
    ) {
        switch (opt) {
            case 'f':
                if (opts->execution_mode == DEFAULT) {
                    opts->execution_mode = FULL;
                    // printf("Full execution mode enabled\n");
                } else
                    fprintf(stderr, "Can't use both step by step and full execution mode, try again with only one of the two\n");
                break;

            case 's':
                if (opts->execution_mode == DEFAULT) {
                    opts->execution_mode = STEP_BY_STEP;
                    // printf("Step by step mode enabled\n");
                } else
                    fprintf(stderr, "Can't use both step by step and full execution mode, try again with only one of the two\n");
                break;
            case 'h':
                print_help(argv[0]);
                return 1;

            case 'a':
                fprintf(stderr, "Arguments for the asm execution are not implemented yet\n");
                // opts->args = split_args(optarg, &opts->args_count);
                // // printf("Arguments for binary (%d args):\n", opts->args_count);
                // for (int i = 0; i < opts->args_count; i++)
                //     printf("  argv[%d] = '%s'\n", i, opts->args[i]);
                break;
            case 'b':
                fprintf(stderr, "Breakpoints for the asm execution are not implemented yet\n");
                // add_breakpoint(opts, optarg);
                // printf("Breakpoint added at: %s\n", optarg);
                break;

            default:
                fprintf(stderr, "Unknown option. Use -h for help.\n");
                return -1;
        }
    }

    // check if the binary has been passed
    if (!opts->binary_file) {
        fprintf(stderr, "Error: No binary file specified\n");
        print_help(argv[0]);
        return -1;
    }

    // loads the binary
    if (parse_riscv_file(&opts) == -1)
        return -1;

    return 0;
}

