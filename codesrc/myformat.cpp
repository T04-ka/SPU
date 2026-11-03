#ifndef MYMACRO_H
#define MYMACRO_H

#include <stdio.h>

typedef const char* str_t;

#define RED     "\e[31m"
#define BLUE    "\e[34m"
#define GREEN   "\e[32m"
#define YELLOW  "\e[33m"
#define FAT     "\e[1m"
#define DEF     "\e[0m"
#define BLACK   "\033[30m"


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


#ifdef DEBUG
#define $                   fprintf(stderr, RED "ZZZ FROM %s:%d\n" DEF, __FILE__, __LINE__);
#define $d(VR)              fprintf(stderr, "%s = %d\n", #VR, (VR));
#define $du(VR)             fprintf(stderr, "%s = %u\n", #VR, (VR));
#define $zu(VR)             fprintf(stderr, "%s = %zu\n", #VR, (VR));
#define $lg(VR)             fprintf(stderr, "%s = %lg\n", #VR, (VR));
#define $s(VR)              fprintf(stderr, "%s = |%s|\n", #VR, (VR));
#define $p(VR)              fprintf(stderr, "%s = %p\n", #VR, (VR));
#define $c(VR)              fprintf(stderr, "%s = %c\n", #VR, (VR));
#define $err(FRMT, ...)     fprintf(stderr, FRMT, ##__VA_ARGS__);
#define $errm(MSG)          fprintf(stderr, #MSG);
#define $std(FRMT, ...)     printf(FRMT, ##__VA_ARGS__);
#define $stdm(MSG)          printf(#MSG);
#define $sn                 fprintf(stdout, "\n");
#define $en                 fprintf(stderr, "\n");

#define $ad(ARR, N)         fprintf(stderr, "%s = [", #ARR);                                                                                \
                            for (size_t DONT_SHADOW_THIS_NAME_BLYAT = 0; DONT_SHADOW_THIS_NAME_BLYAT < N; DONT_SHADOW_THIS_NAME_BLYAT++) {  \
                                fprintf(stderr, "%d, ", ARR[DONT_SHADOW_THIS_NAME_BLYAT]);                                                  \
                            }                                                                                                               \
                            fprintf(stderr, "\b\b]\n");

#define $ac(ARR, N)         fprintf(stderr, "%s = [", #ARR);                                                                                \
                            for (size_t DONT_SHADOW_THIS_NAME_BLYAT = 0; DONT_SHADOW_THIS_NAME_BLYAT < N; DONT_SHADOW_THIS_NAME_BLYAT++) {  \
                                fprintf(stderr, "%c, ", ARR[DONT_SHADOW_THIS_NAME_BLYAT]);                                                  \
                            }                                                                                                               \
                            fprintf(stderr, "\b\b]\n");

#else
#define $
#define $d(VR)
#define $du(VR)
#define $zu(VR)
#define $lg(VR)
#define $s(VR)
#define $p(VR)
#define $c(VR)
#define $err(FRMT, ...)
#define $errm(MSG)
#define $std(FRMT, ...)
#define $stdm(MSG)
#define $sn
#define $en
#define $ad(ARR, N)
#define $ac(ARR, N)

#endif


#endif
