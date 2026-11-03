#include "io.h"

#include "strfuncs.h"
#include <cstddef>
#include <cstdio>
#include <assert.h>

//-------------------------------------------------------------------------------------
int opnfls(FILE** inp, FILE** out, struct io_data io_data){

    *inp = fopen(io_data.inp, "r");
    *out = fopen(io_data.out, "w");

    if (*out == NULL) {

        *out = stdout;
    }
    if (*inp == NULL){

        return 1;
    }

    return 0;
}


//-------------------------------------------------------------------------------------
int clsfls(FILE* inp, FILE* out){

    if (inp == NULL){

        return 1;
    }

    int errout = 0;
    if (out != stdout){

        errout = fclose(out);
    }

    int errinp = fclose(inp);
    //printf("inp = %d, out = %d\n", errinp, errout);
    if (errinp || errout){

        return 1;
    }

    return 0;
}


//-------------------------------------------------------------------------------------
void printArr(string arr[], int nlines, FILE* fl){


    //printf("nlines = %d\n", nlines);
    for (int i = 0; i < nlines; i++){

        fprintf(fl, "|%s|\n", arr[i].str);
    }

    getchar();
}


//-------------------------------------------------------------------------------------
#define FLDT_ fldt ->

int rdfrmfl(struct filedata* fldt){

    long long sz = rdflsz(FLDT_ fl);

    if (sz == -1){

        return 1;
    }

    FLDT_ sz = (size_t) sz;
    FLDT_ rdbffr = (char*) calloc(FLDT_ sz + 1, 1);

    fread(FLDT_ rdbffr, sizeof(char), FLDT_ sz, FLDT_ fl);

    /*
    int fldiscr = open(FLDT_ flnm, 1, "r");
    read(FLDT_ rdbffr, sizeof(char), FLDT_ sz, fldiscr);
    fclose(file);
    */

    prsdata(fldt);

    return 0;
}

#undef FLDT_
//-------------------------------------------------------------------------------------
