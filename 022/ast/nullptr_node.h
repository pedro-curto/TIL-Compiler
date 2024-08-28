#ifndef __TIL_AST_NULLPTR_NODE_H__
#define __TIL_AST_NULLPTR_NODE_H__

#include <cdk/ast/expression_node.h>

namespace til {

    /**
     * Class for describing nullptr nodes.
     */
    class nullptr_node: public cdk::expression_node {
    public:
        nullptr_node(int lineno) noexcept:
            cdk::expression_node(lineno) {
        }

        void accept(basic_ast_visitor *sp, int level) override { sp->do_nullptr_node(this, level); }
    };

} // til

#endif
