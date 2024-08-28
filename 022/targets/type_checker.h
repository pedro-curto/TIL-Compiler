#ifndef __TIL_TARGETS_TYPE_CHECKER_H__
#define __TIL_TARGETS_TYPE_CHECKER_H__

#include "targets/basic_ast_visitor.h"

namespace til {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class type_checker: public basic_ast_visitor {
    cdk::symbol_table<til::symbol> &_symtab;

    basic_ast_visitor *_parent;

  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<til::symbol> &symtab, basic_ast_visitor *parent) :
        basic_ast_visitor(compiler), _symtab(symtab), _parent(parent) {
    }

  public:
    ~type_checker() {
      os().flush();
    }

  protected:
    bool compareCovariantTypes(std::shared_ptr<cdk::basic_type> leftType, std::shared_ptr<cdk::basic_type> rightType);
    bool compareTypes(bool allowCovariants, std::shared_ptr<cdk::basic_type> leftType, std::shared_ptr<cdk::basic_type> rightType);
    void processBinaryArithmeticExprInt(cdk::binary_operation_node *const node, bool canBeDouble);
    void processBinaryArithmeticExprDouble(cdk::binary_operation_node *const node);
    void processBinaryArithmeticExprPointer(cdk::binary_operation_node *const node, bool ptrSubtraction);
    void processBinaryArithmeticExpr(cdk::binary_operation_node *const node, int lvl, bool canBeDouble, bool ptrDisplacement, bool ptrSubtraction);
    void processBinaryComparisons(cdk::binary_operation_node *const node, int lvl, bool canBeDouble, bool canBePointer);
    void handle_var_declaration(til::declaration_node * const node, int lvl);
    void handle_typed_declaration(til::declaration_node * const node, int lvl);
    template<typename T>
    void process_literal(cdk::literal_node<T> *const node, int lvl) {
    }

  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end

  };

} // til

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING
//---------------------------------------------------------------------------

#define CHECK_TYPES(compiler, symtab, node) { \
  try { \
    til::type_checker checker(compiler, symtab, this); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, node)

#endif
