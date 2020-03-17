#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "obj.h"
#include "utf8.h"
#include "stream.h"
#include "eval.h"
#include "print.h"

void print_usage() {
    printf("usage: lis LISPATH\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("    -h --help       Print this help.\n");
    printf("\n");
}

struct Option {
    char* lispath;
    bool help;
};

#define optcmp(sname, lname) \
(short_opt && strcmp(argv[i]+1, sname) == 0) || (long_opt && strcmp(argv[i]+2, lname) == 0)

bool parse_option(int argc, char** argv, struct Option* opt) {
    bool lispath_filled = false;

    for (int i=1; i<argc; i++) {
        bool short_opt = false;
        bool long_opt = false;

        if (argv[i][0] == '-') {
            if (strlen(argv[i]) >= 2 && argv[i][1] == '-') {
                long_opt = true;
            } else {
                short_opt = true;
            }

            if (optcmp("h", "help")) {
                printf("aaaa\n");
                opt->help = true;
            }

        } else {
            if (!lispath_filled) {
                lispath_filled = true;
                opt->lispath = argv[i];
            } else {
                printf("LISPATH specified twice?\n");
                return false;
            }
        }
    }

    return true;
}

#include <stdlib.h>

int main(int argc, char** argv) {
    struct Option opt = { "", false };
    if (!parse_option(argc, argv, &opt)) {
        return 1;
    }

    if (opt.help) {
        print_usage();
        return 0;
    }

    lis_stream * stream = make_lis_stream(1024, LIS_STREAM_IN, LIS_STREAM_TEXT);
    stream->fin = stdin;
    while (true) {
        lis_char cp;
        lis_byte bytes[4];
        if (!stream_read_char(stream, &cp)) break;
        int len = utf8_encode_codepoint(cp, bytes);
        if (len <= 0) break;
        for (int i=0; i<len; i++) printf("%c", bytes[i]);
    }
    printf("\n");

    lis_obj * num = make_int(42);
    lis_obj * name = make_string();
    lis_char * str = (lis_char *)malloc(sizeof(lis_char) * 3);
    str[0] = 's';
    str[1] = 'y';
    str[2] = 'm';
    name->data.str->size = 3;
    name->data.str->body = str;
    lis_obj * symbol = make_symbol(name);
    symbol->data.sym->value = num;
    print(eval(symbol));
    printf("\n");

    printf("------------\n");
    lis_obj * cons = make_cons();
    cons->data.cons->car = num;
    cons->data.cons->cdr = symbol;
    print(eval(cons));
    printf("\n");

    return 0;
}
