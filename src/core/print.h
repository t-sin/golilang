#ifndef __lis_print
#define __lis_print

#include "obj.h"

void print(lis_obj * genv, lis_obj * obj, lis_stream * stream);
void print_mval(lis_obj * genv, lis_obj * obj, lis_stream * stream);
lis_obj * lisp_print(lis_obj * genv, lis_obj * args);

#endif
