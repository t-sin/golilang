#ifndef __lis_eval
#define __lis_eval

#include "obj.h"

bool check_arglen(lis_obj * genv, lis_obj * args, int len, lis_obj * opname);
bool check_argge(lis_obj * genv, lis_obj * args, int len, lis_obj * opname);
bool check_argeven(lis_obj * genv, lis_obj * args, lis_obj * opname);

lis_lambdalist * validate_lambdalist(lis_obj * genv, lis_obj * lenv, lis_obj * lambdalist);
lis_obj * bind_lambdalist(lis_obj * genv, lis_obj * fn, lis_obj * args);

lis_obj * apply(lis_obj * genv, lis_obj * fn, lis_obj * args);
lis_obj * eval(lis_obj * genv, lis_obj * lenv, lis_obj * obj);

lis_obj * lisp_apply(lis_obj * genv, lis_obj * args);
lis_obj * lisp_eval(lis_obj * genv, lis_obj * args);

bool macroexpand_1(lis_obj * genv, lis_obj * form, lis_obj * env, lis_obj ** expansion);
bool macroexpand(lis_obj * genv, lis_obj * form, lis_obj * env, lis_obj ** expansion);

lis_obj * lisp_macroexpand_1(lis_obj * genv, lis_obj * args);
lis_obj * lisp_macroexpand(lis_obj * genv, lis_obj * args);

#endif