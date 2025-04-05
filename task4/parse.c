#include <stdio.h>
#include <getopt.h>
#include <string.h>

const char *short_options = "mcst";
const struct option long_options[] = {
    { "elbrus", required_argument, NULL, 'e' },
    { NULL, 0, NULL, 0 }
};
const char *long_args[] = {"1c+", "2c+", "2c3", "4c", "8c", "16c"};

#define MAX_ARGS_LEN 3

// check whether args passed to --elpus are correct
int long_arg_is_valid(const char *arg)
{
    int n_opts = sizeof(long_args) / sizeof(long_args[0]);
    for (int i = 0; i < n_opts; ++i)
    {
        if(!strncmp(arg, long_args[i], MAX_ARGS_LEN))
        {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // forbid error messages
    opterr = 0;

    int rez = 0;
    int opt_i = 0;
    while((rez = getopt_long(argc, argv, short_options, long_options, &opt_i)) != -1)
    {
        // --elbrus option
        if (rez == 'e')
        {
            if(!long_arg_is_valid(optarg))
            {
                printf("Options are incorrect.\n");
                return 1;
            }
        }
        // found unknown option
        else if (rez == '?')
        {
            printf("Options are incorrect.\n");
            return 1;
        }
    }

    printf("Options are correct.\n");
    return 0;
}

