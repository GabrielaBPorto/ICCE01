#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "newtonOpt.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("uso: %s file_in\n\n", argv[0]);
        return 1;
    }

    FILE *f_in = fopen(argv[1], "r");
    int n;
    fscanf(f_in, "%d", &n);
    rewind(f_in);

    FILE *f_out = stdout;
    
    f_out = fopen(markerName("resultadoOpt_", n), "w+");
    trabalho2(f_in, f_out);
    fclose(f_out);
    fclose(f_in);
    return 0;
}