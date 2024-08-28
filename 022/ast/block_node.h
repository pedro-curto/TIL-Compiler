#ifndef __TIL_AST_BLOCK_NODE_H__
#define __TIL_AST_BLOCK_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace til {

  /**
   * Class for describing block nodes.
   */
  class block_node : public cdk::basic_node {
    cdk::sequence_node *_varDeclarations;
    cdk::sequence_node *_instructions;

  public:
    block_node(int lineno, cdk::sequence_node *_varDeclarations, 
               cdk::sequence_node *_instructions) noexcept:
        cdk::basic_node(lineno), _varDeclarations(_varDeclarations), _instructions(_instructions) {
    }

    cdk::sequence_node *varDeclarations() { return _varDeclarations; }
    cdk::sequence_node *instructions() { return _instructions; }

    void accept(basic_ast_visitor *sp, int level) override { sp->do_block_node(this, level); }

  };

} // til

#endif
