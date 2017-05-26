#ifndef LIST_AST_H
#define LIST_AST_H
#include <stdbool.h>
#include "struct.h"

/* basic fonction to a list */
l_ast l_ast_new_empty(void);
l_ast l_ast_cons(sprite_t ast, l_ast L);
void l_ast_reverse(l_ast *L);
l_ast l_ast_copy(l_ast L);

bool l_ast_is_empty(l_ast L);
sprite_t l_ast_head(l_ast L);
l_ast l_ast_tail(l_ast L);
sprite_t l_ast_pop(l_ast *L);

void l_ast_free(l_ast *L);

#endif
