#ifndef __TIL_AST_PROCESS_NODE_H__
#define __TIL_AST_PROCESS_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace til {

  /**
   * Class for describing process nodes.
   */
  class process_node : public cdk::basic_node {
    cdk::expression_node *_vector, *_low, *_high, *_function;

  public:
    process_node(int lineno, cdk::expression_node *vector, cdk::expression_node *low,
    cdk::expression_node *high, cdk::expression_node *function) :
        basic_node(lineno), _vector(vector), _low(low), _high(high), _function(function) {
    }

    cdk::expression_node *vector() { return _vector; }
    cdk::expression_node *low() { return _low; }
    cdk::expression_node *high() { return _high; }
    cdk::expression_node *function() { return _function; }


    cdk::basic_node *block() { return _block; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_process_node(this, level); }

  };

} // til

#endif
