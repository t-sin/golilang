#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"
#include "lstring.h"
#include "lerror.h"
#include "list.h"
#include "eval.h"

#include "package.h"

bool _table_string_eq(void * a, void * b) {
    return _string_equal(LIS_STR((lis_obj *)a), LIS_STR((lis_obj *)b));
}

lis_obj * _package_make_package(lis_obj * genv, lis_obj * name_str) {
    if (LIS_TAG3(name_str) != LIS_TAG3_BUILTIN ||
        LIS_TAG_TYPE(name_str) != LIS_TAG_TYPE_STR) {
        not_string_error(genv, name_str);
        return NULL;
    }

    lis_obj * pkg = _make_package(name_str);
    _table_add(LIS_GENV(genv)->package_table, (void *)name_str, (void *)pkg, _table_string_eq);

    return pkg;
}

lis_obj * _package_list_all_packages(lis_obj * genv) {
    lis_obj * pkg_list = LIS_GENV(genv)->symbol_nil;
    _table * table = LIS_GENV(genv)->package_table;

    for (size_t i=0; i<table->size; i++) {
        _table_entry * e = table->array + i;
        if (e->key != NULL) {
            while (e != NULL) {
                pkg_list = _list_cons(genv, (lis_obj *)e->value, pkg_list);
                e = e->next;
            }
        }
    }

    return pkg_list;
}

lis_obj * package_list_all_packages(lis_obj * genv, lis_obj * args) {
    if (!check_arglen(genv, args, 0, LSTR(U"list-all-packages"))) {
        return NULL;
    }
    lis_obj * arg = _package_list_all_packages(genv);
}

lis_obj * add_symbol(lis_obj * package, lis_obj * symbol) {
    assert(LIS_TAG3(package) == LIS_TAG3_BUILTIN);
    assert(LIS_TAG_TYPE(package) == LIS_TAG_TYPE_PKG);

    assert(LIS_TAG3(symbol) == LIS_TAG3_BUILTIN);
    assert(LIS_TAG_TYPE(symbol) == LIS_TAG_TYPE_SYM);

    lis_package * pkg = package->data.pkg;

    if (pkg->num > pkg->size) {
        size_t new_size = pkg->size * 2;
        lis_obj ** new_symbols = (lis_obj **)malloc(sizeof(lis_obj *) * new_size);
        memset(new_symbols, 0, new_size);

        for (int i=0; i<pkg->num; i++) {
            new_symbols[i] = pkg->symbols[i];
        }
        free(pkg->symbols);
        pkg->symbols = new_symbols;
        pkg->size = new_size;
    }

    size_t i = 0;
    while (i<pkg->size) {
        if (pkg->symbols[i] == NULL) {
            break;
        }
        i++;
    }

    pkg->symbols[i] = symbol;
    pkg->num++;

    return symbol;
}

package_intern_status intern(lis_obj * package, lis_obj * name, lis_obj ** sym) {
    assert(LIS_TAG3(package) == LIS_TAG3_BUILTIN);
    assert(LIS_TAG_TYPE(package) == LIS_TAG_TYPE_PKG);

    assert(LIS_TAG3(name) == LIS_TAG3_BUILTIN);
    assert(LIS_TAG_TYPE(name) == LIS_TAG_TYPE_STR);

    lis_package * pkg = package->data.pkg;

    for (int i=0; i<pkg->num; i++) {
        if (pkg->symbols[i] != NULL &&
            _string_equal(pkg->symbols[i]->data.sym->name->data.str, name->data.str)) {
            *sym = pkg->symbols[i];
            return PKG_INTERNAL;  // TODO: INTERNAL, EXTERNAL or INHERITED
        }
    }

    *sym = _make_symbol(name);
    add_symbol(package, *sym);
    return PKG_INTERNAL;
}
