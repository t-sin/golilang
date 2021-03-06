#ifndef __lis_ptr_table
#define __lis_ptr_table

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct _table_entry {
    struct _table_entry * next;
    void * key;
    void * value;
} _table_entry;

typedef struct _table {
    size_t size;
    _table_entry * array;
    size_t num;
    size_t (* hash_fn)(void *, size_t);
    bool (* eq_fn)(void *, void *);
} _table;

size_t _hash(void * ptr, size_t hash_size);
bool _table_ptr_eq(void * a, void * b);

_table * _make_table(size_t size);
void _free_table(_table * table);
void _table_add(_table * table, void * key, void * value);
_table_entry * _table_find(_table * table, void * key);
void _table_remove(_table * table, void * key);

_table * _copy_table(_table * table);

#endif
