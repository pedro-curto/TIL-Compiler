#ifndef __TIL_TARGETS_POSTFIX_WRITER_H__
#define __TIL_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <optional>
#include <set>
#include <stack>
#include <sstream>
#include <cdk/emitters/basic_postfix_emitter.h>

namespace til {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<til::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    
    int _lbl; // to create labels!
    int _offset; // current framepointer offset (0 means no vars defined)

    bool _lastInstr = false; 
    bool _inFunctionArgs = false;

    std::vector<int> _loopCondLabels;
    std::vector<int> _loopEndLabels;

    std::string _functionToJumpTo;
    std::stack<std::string> _functionLabels; // functionlabels, useful to acess top one (current)
    std::optional<std::string> _externalFunctionName; // only way i can think to either be null or string
    std::set<std::string> _externalFuncs; // set of external functions to be used in the program


  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<til::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }
  
  protected:
    void handle_global_declarations(til::declaration_node * const node, std::shared_ptr<til::symbol> symbol, int lvl);
    void handle_function_declarations(til::declaration_node * const node, std::shared_ptr<til::symbol> symbol, int lvl);
    void do_decls_instrs(cdk::sequence_node * const decls, cdk::sequence_node * const instrs , int lvl);

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }



  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // til

#endif
