#ifndef __TIL_AST_OBJECT_NODE_H__
#define __TIL_AST_OBJECT_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace til {

    /**
    * Class for describing stack allocation nodes.
    */
    class object_node: public cdk::unary_operation_node {
    public:
        object_node(int lineno, cdk::expression_node *argument) noexcept:
            cdk::unary_operation_node(lineno, argument) {
        }

        void accept(basic_ast_visitor *sp, int level) override { sp->do_object_node(this, level); }
    };

} // til

#endif
