#ifndef __lis_error
#define __lis_error

void not_string_error(lis_obj * genv, lis_obj * obj);
void not_int_error(lis_obj * genv, lis_obj * obj);
void not_list_error(lis_obj * genv, lis_obj * obj);
void not_seq_error(lis_obj * genv, lis_obj * obj);
void not_stream_error(lis_obj * genv, lis_obj * obj);

#endif
