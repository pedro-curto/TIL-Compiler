#ifndef __TIL_AST_ITERATE_NODE_H__
#define __TIL_AST_ITERATE_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace til {

  /**
   * Class for describing iterate nodes.
   */
  class iterate_node : public cdk::basic_node {
    cdk::expression_node *_vec, *_count, *_func, *_cond;

  public:
    iterate_node(int lineno, cdk::expression_node *vec,  cdk::expression_node *count,
     cdk::expression_node *func, cdk::expression_node *cond) :
        basic_node(lineno), _vec(vec), _count(count), _func(func), _cond(cond) {
    }

    cdk::expression_node *vec() { return _vec; }
    cdk::expression_node *count() { return _count; }
    cdk::expression_node *func() { return _func; }
    cdk::expression_node *cond() { return _cond; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_iterate_node(this, level); }

  };

} // til

#endif
