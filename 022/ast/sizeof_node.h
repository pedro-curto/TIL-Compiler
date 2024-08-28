#ifndef __TIL_AST_SIZEOF_NODE_H__
#define __TIL_AST_SIZEOF_NODE_H__

#include <cdk/ast/lvalue_node.h>    
#include <cdk/ast/expression_node.h>

namespace til {

  /**
   * Class for describing sizeof nodes.
   */
  class sizeof_node : public cdk::expression_node {
    cdk::expression_node *_expr;

  public:
    sizeof_node(int lineno, cdk::expression_node *expr) noexcept:
        cdk::expression_node(lineno), _expr(expr) {
    }

    cdk::expression_node *expr() { return _expr; }

    void accept(basic_ast_visitor *sp, int level) override { sp->do_sizeof_node(this, level); }

  };

} // til

#endif
