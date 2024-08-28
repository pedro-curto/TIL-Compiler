#ifndef __TIL_AST_FUNCTION_NODE_H__
#define __TIL_AST_FUNCTION_NODE_H__

#include <vector>

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/typed_node.h>
#include <cdk/types/basic_type.h>

namespace til {

  /**
   * Class for describing function nodes.
   */
  class function_node : public cdk::expression_node {
    cdk::sequence_node *_declarations, *_instructions;
    cdk::sequence_node *_arguments;

  public:
    function_node(int lineno, std::shared_ptr<cdk::basic_type> retType, cdk::sequence_node *declarations, 
    cdk::sequence_node *instructions, cdk::sequence_node *arguments) noexcept:
        cdk::expression_node(lineno), _declarations(declarations), 
        _instructions(instructions), _arguments(arguments) {
          std::vector<std::shared_ptr<cdk::basic_type>> args;
          for (size_t i = 0; i < arguments->size(); i++) {
            args.push_back(dynamic_cast<cdk::typed_node*>(arguments->node(i))->type());
          }
          // functional_type(input, output)
          this->type(cdk::functional_type::create(args, retType));
    }

    cdk::sequence_node *declarations() { return _declarations; }
    cdk::sequence_node *instructions() { return _instructions; }
    cdk::sequence_node *arguments() { return _arguments; }

    void accept(basic_ast_visitor *sp, int level) override { sp->do_function_node(this, level); }

  };

} // til

#endif
