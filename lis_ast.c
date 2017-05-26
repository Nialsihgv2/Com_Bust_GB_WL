#include <stdlib.h>

#include "lis_ast.h"

l_ast l_ast_new_empty(void)
{
  return NULL;
}

l_ast l_ast_cons(sprite_t ast,l_ast L)
{
  l_ast new = l_ast_new_empty();

  new = (l_ast)malloc(sizeof(struct L_ast));
  new->first = ast;
  new->next = L;
  return new;
}

void l_ast_reverse(l_ast *L)
{
  l_ast tmp1 = *L;
  *L = l_ast_new_empty();

  while(!l_ast_is_empty(tmp1)){
    l_ast tmp2 = l_ast_tail(tmp1);
    tmp1->next = *L;
    *L = tmp1;
    tmp1 = tmp2;}
}

l_ast l_ast_copy(l_ast L)
{
  if(l_ast_is_empty(L))
    return l_ast_new_empty();

  l_ast new = l_ast_new_empty();
  while(!l_ast_is_empty(L)){
    new = l_ast_cons(l_ast_head(L), new);
    L = l_ast_tail(L);
  }
  l_ast_reverse(&new);
  return new;
}

bool l_ast_is_empty(l_ast L)
{
  return (L==l_ast_new_empty());
}

sprite_t l_ast_head(l_ast L)
{
  return L->first;
}

l_ast l_ast_tail(l_ast L)
{
  if(!l_ast_is_empty(L))
    L=L->next;
  return L;
}

sprite_t l_ast_pop(l_ast *L)
{
  l_ast tmp = *L;
  sprite_t head_data = l_ast_head(*L);
  *L = l_ast_tail(*L);
  free(tmp);
  return head_data;
}

void l_ast_free(l_ast *L)
{
  l_ast tmp;
  while(!l_ast_is_empty(*L)){
    tmp = *L;
    *L = l_ast_tail(*L);
    free(tmp);}
}
