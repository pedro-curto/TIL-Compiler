#ifndef __TIL_AST_PROGRAM_NODE_H__
#define __TIL_AST_PROGRAM_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace til {

  /**
   * Class for describing program nodes.
   */
  class program_node : public cdk::expression_node {
    cdk::sequence_node *_declarations, *_instructions;

  public:
    program_node(int lineno, cdk::sequence_node *declarations, cdk::sequence_node *instructions) noexcept:
        expression_node(lineno), _declarations(declarations), _instructions(instructions) {
          // program nodes are always int
          this->type(cdk::functional_type::create(cdk::primitive_type::create(4, cdk::TYPE_INT)));
    }

    cdk::sequence_node *declarations() { return _declarations; }
    cdk::sequence_node *instructions() { return _instructions; }
    

    void accept(basic_ast_visitor *sp, int level) override { sp->do_program_node(this, level); }

  };

} // til

#endif
