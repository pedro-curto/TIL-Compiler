#ifndef __TIL_AST_INDEX_NODE_H__
#define __TIL_AST_INDEX_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace til {

  /**
   * Class for describing index nodes (as pointers).
   */
  class index_node : public cdk::lvalue_node {
    cdk::expression_node *_base, *_index;

  public:
    index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index) noexcept:
        lvalue_node(lineno), _base(base), _index(index){
    }

    cdk::expression_node *base() { return _base; }

    cdk::expression_node *index() { return _index; }

    void accept(basic_ast_visitor *sp, int level) override { sp->do_index_node(this, level); }

  };

} // til

#endif
