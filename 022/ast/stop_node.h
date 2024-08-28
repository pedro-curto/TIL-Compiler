#ifndef __TIL_AST_STOP_NODE_H__
#define __TIL_AST_STOP_NODE_H__

#include <cdk/ast/basic_node.h>

namespace til {

    /**
     * Class for describing stop nodes.
     */
    class stop_node : public cdk::basic_node {
        int _argument;

        public:
            stop_node(int lineno, int argument) noexcept: 
            cdk::basic_node(lineno), _argument(argument) {}

            int argument() { return _argument; }

            void accept(basic_ast_visitor *sp, int level) override { sp->do_stop_node(this, level); }
        
    };
} // til

#endif