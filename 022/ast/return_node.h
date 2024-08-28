#ifndef __TIL_AST_RETURN_NODE_H__
#define __TIL_AST_RETURN_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace til {

    /**
     * Class for describing return nodes.
     */
    class return_node : public cdk::basic_node {
        cdk::expression_node *_return_value;

        public:
            return_node(int lineno, cdk::expression_node *return_value) noexcept: 
            cdk::basic_node(lineno), _return_value(return_value) {}

            cdk::expression_node* return_value() { return _return_value; }

            void accept(basic_ast_visitor *sp, int level) override { sp->do_return_node(this, level); }
        
    };
} // til

#endif