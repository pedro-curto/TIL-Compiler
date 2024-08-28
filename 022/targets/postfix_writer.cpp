#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include ".auto/all_nodes.h"  // all_nodes.h is automatically generated

#include "til_parser.tab.h"



//---------------------------------------------------------------------------

void til::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (!_functionLabels.empty()) {
    _pf.INT(node->value());
  } else {
    _pf.SINT(node->value());
  }
}

void til::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (!_functionLabels.empty()) {
    _pf.DOUBLE(node->value());
  } else {
    _pf.SDOUBLE(node->value());
  }
}

void til::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  std::string label = mklbl(++_lbl);
  _pf.RODATA(); // data segment for initialized constant values
  _pf.ALIGN(); 
  _pf.LABEL(label); 
  _pf.SSTRING(node->value()); // declare static NULL-terminated character string

  if (!_functionLabels.empty()) {
    // leaves address on stack
    _pf.TEXT(_functionLabels.top());
    _pf.ADDR(label);
  } else {
    _pf.DATA(); // initialized values
    _pf.SADDR(label);
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}

void til::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string label = mklbl(++_lbl);
  node->left()->accept(this, lvl);
  _pf.DUP32();
  _pf.JZ(label); // guarantees short-circuit evaluation
  node->right()->accept(this, lvl);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(label);
}

void til::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string label = mklbl(++_lbl);
  node->left()->accept(this, lvl);
  _pf.DUP32();
  _pf.JNZ(label); // also guarantees short-circuit evaluation
  node->right()->accept(this, lvl);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(label);
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void til::postfix_writer::do_block_node(til::block_node * const node, int lvl) {
  do_decls_instrs(node->varDeclarations(), node->instructions(), lvl);
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DNEG();
  } else {
    _pf.NEG();
  }
}

void til::postfix_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    auto ref = cdk::reference_type::cast(node->type());

    if (ref->referenced()->name() == cdk::TYPE_VOID) {
      _pf.INT(1);
    } else {
      _pf.INT(ref->referenced()->size());
    }
    _pf.MUL();
  }

  node->right()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    auto ref = cdk::reference_type::cast(node->type());

    if (ref->referenced()->name() == cdk::TYPE_VOID) {
      _pf.INT(1);
    } else {
      _pf.INT(ref->referenced()->size());
    }
    _pf.MUL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DADD();
  } else {
    _pf.ADD();
  }
}

void til::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    auto ref = cdk::reference_type::cast(node->type());

    if (ref->referenced()->name() == cdk::TYPE_VOID) {
      _pf.INT(1);
    } else {
      _pf.INT(ref->referenced()->size());
    }
    _pf.MUL();
  }

  node->right()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    auto ref = cdk::reference_type::cast(node->type());

    if (ref->referenced()->name() == cdk::TYPE_VOID) {
      _pf.INT(1);
    } else {
      _pf.INT(ref->referenced()->size());
    }
    _pf.MUL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DSUB();
  } else {
    _pf.SUB();
  }

  if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    // the difference between two pointers must be divided by the size of what they're referencing
    auto lref = cdk::reference_type::cast(node->left()->type());
    if (lref->referenced()->name() == cdk::TYPE_VOID) {
      _pf.INT(1);
    } else {
      _pf.INT(lref->referenced()->size());
    }
    _pf.DIV();
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}

void til::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}

void til::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void til::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}

void til::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}

void til::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}

void til::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}

void til::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}

void til::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_address_node(til::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->value()->accept(this, lvl + 2);
}

void til::postfix_writer::do_object_node(til::object_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto ref = cdk::reference_type::cast(node->type())->referenced();
  node->argument()->accept(this, lvl);
  // void has size 0, but we want to alloc 1 byte for it
  if (ref->name() == cdk::TYPE_VOID) {
    _pf.INT(1);
  } else {
    _pf.INT(ref->size());
  }
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto symbol = _symtab.find(node->name()); 

  if (symbol->qualifier() == tEXTERNAL) {
    _externalFunctionName = symbol->name();
    return;
  } 
  
  if (symbol->value() == 0) { // we store offset in symbol's value (0, global variable)
    _pf.ADDR(node->name());
  } else {
    _pf.LOCAL(symbol->value());
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_index_node(til::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->base()->accept(this, lvl + 2);
  node->index()->accept(this, lvl + 2);
  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);

  if (_externalFunctionName) {
    return; // nothing to load
  }
  auto nodeTypeName = node->type()->name();

  if (nodeTypeName == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  } else {
    _pf.LDINT(); // everything else has 4 bytes (ints, pointers, strings)
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // checks if rvalue needs conversion
  node->rvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->rvalue()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  }
  // then, duplicates on stack
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }
  // lvalue grabs the stack value and stores it
  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_decls_instrs(cdk::sequence_node * const decls, cdk::sequence_node * const instrs , int lvl) {
  _symtab.push(); // creates local scope
  decls->accept(this, lvl + 2);

  _lastInstr = false;
  for (size_t i = 0; i < instrs->size(); i++) {
    auto instr = instrs->node(i);
    // unreachable; after return
    if (_lastInstr) throw std::string("unreachable code in do_decls_instrs");

    instr->accept(this, lvl + 2);
  }
  _lastInstr = false;

  _symtab.pop();
}

void til::postfix_writer::do_program_node(til::program_node * const node, int lvl) {
  // RTS is weird and we need to call this _main
  ASSERT_SAFE_EXPRESSIONS;
  _functionLabels.push("_main");

  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");

  _offset = 8; // first 8 bytes are for function arguments
  _symtab.push();

  frame_size_calculator fsc(_compiler, _symtab);
  fsc.do_sequence_node(node->declarations(), lvl);
  fsc.do_sequence_node(node->instructions(), lvl);
  _pf.ENTER(fsc.localsize());

  _offset = 0;
  auto tmp = _functionToJumpTo;
  _functionToJumpTo = mklbl(++_lbl);

  do_decls_instrs(node->declarations(), node->instructions(), lvl);

  _pf.ALIGN();
  _pf.LABEL(_functionToJumpTo);
  _pf.LEAVE();
  _pf.RET();

  _functionToJumpTo = tmp;

  _symtab.pop();
  _functionLabels.pop();

  for (auto name : _externalFuncs) {
    _pf.EXTERN(name);
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_return_node(til::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto symbol = _symtab.find("@", 1);
  auto rettype = cdk::functional_type::cast(symbol->type())->output(0);

  if (rettype->name() != cdk::TYPE_VOID) {
    // checks for conversion and returns value
    node->return_value()->accept(this, lvl+2);
    if (rettype->name() == cdk::TYPE_DOUBLE && node->return_value()->is_typed(cdk::TYPE_INT)) {
      _pf.I2D();
    }
    
    if (rettype->name() !=  cdk::TYPE_DOUBLE) {
      _pf.STFVAL32();
    } else {
      _pf.STFVAL64();
    }
  }
  _pf.JMP(_functionToJumpTo);
  _lastInstr = true;
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_evaluation_node(til::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  auto typeSize = node->argument()->type()->size();
  if (typeSize > 0) {
    _pf.TRASH(typeSize); // deletes evaluated value
  }
}

void til::postfix_writer::do_print_node(til::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  for (size_t i = 0; i < node->arguments()->size(); i++) {
    auto expr = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));

    expr->accept(this, lvl); // determines the value (expr to print)

    if (expr->is_typed(cdk::TYPE_INT)) {
      _externalFuncs.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4);
    } else if (expr->is_typed(cdk::TYPE_DOUBLE)) {
      _externalFuncs.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8);
    } else if (expr->is_typed(cdk::TYPE_STRING)) {
      _externalFuncs.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4);
    }
  }

  if (node->newline()) {
    _externalFuncs.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_read_node(til::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _externalFuncs.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else {
    _externalFuncs.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_loop_node(til::loop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lblCond = ++_lbl;
  int lblEnd = ++_lbl;

  // we need these if next or stop nodes arguments are different from 1
  _loopCondLabels.push_back(lblCond);
  _loopEndLabels.push_back(lblEnd);
  
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblCond)); // condition
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lblEnd)); // jump to endloop if condition is false

  node->block()->accept(this, lvl + 2);
  _lastInstr = false;

  _loopCondLabels.pop_back();
  _loopEndLabels.pop_back();

  _pf.JMP(mklbl(lblCond)); // jump to condition
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblEnd)); // label of endcycle
}

void til::postfix_writer::do_for_node(til::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lblCond, lblIncr, lblEnd;

  node->init()->accept(this, lvl);
  _pf.LABEL(mklbl(lblCond = ++_lbl));

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lblEnd = ++_lbl)); // jump to endloop if condition is false

  node->block()->accept(this, lvl + 2);

  _pf.LABEL(mklbl(lblIncr = ++_lbl));
  node->incr()->accept(this, lvl);
  _pf.JMP(mklbl(lblCond));

  _pf.LABEL(mklbl(lblEnd)); // label of endcycle
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_next_node(til::next_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  size_t jmpTo = static_cast<size_t>(node->argument()); // typecasting because compiler is nagging

  if (jmpTo > _loopCondLabels.size()) {
    throw std::string("next statement outside loop body");
  }

  auto jmpLabel = _loopCondLabels[_loopCondLabels.size() - jmpTo];
  _pf.JMP(mklbl(jmpLabel));
  _lastInstr = true;
}

void til::postfix_writer::do_stop_node(til::stop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  size_t jmpTo = static_cast<size_t>(node->argument());

  if (jmpTo > _loopEndLabels.size()) {
    throw std::string("stop statement outside loop body");
  }
  
  auto jmpLabel = _loopEndLabels[_loopEndLabels.size() - jmpTo];
  _pf.JMP(mklbl(jmpLabel));
  _lastInstr = true;
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_node(til::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _lastInstr = false;
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_else_node(til::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _lastInstr = false;
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _lastInstr = false;
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//-----------------------------------------TESTE PRÁTICO ----------------------------------

// process vector low high function
void til::postfix_writer::do_process_node(til::process_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  // declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> var_type, std::string &identifier, cdk::expression_node *initializer)
  // int i = low;
  int lineno = node->lineno();
  std::string it_id = "_it";
  it_dec = new til::declaration_node(lineno, tPRIVATE, cdk::primitive_type::create(4, cdk::TYPE_INT),
  it_id, node->low());

  it_dec->accept(this, lvl);
  it_lval = new cdk::variable_node(lineno, it_id);
  it_rval = new cdk::rvalue_node(lineno, it_lval);

  // index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index)
  // v[i]
  auto vecEl = new til::index_node(lineno, node->vector(), it_rval);
  auto vecEl_rval = new cdk::rvalue_node(lineno, vecEl);

  // function_call_node(int lineno, cdk::expression_node *function, cdk::sequence_node *arguments)
  // f(v[i])
  auto args = new cdk::sequence_node(lineno, vecEl_rval);
  auto funcCall = new til::function_call_node(lineno, node->function(), args);

  // i++
  auto incr_expr = new cdk::add_node(lineno, it_rval, new cdk::integer_node(lineno, 1));
  auto incr_assign = new cdk::assignment_node(lineno, it_lval, incr_expr);
  auto incr_eval = new til::evaluation_node(lineno, incr_assign);

  // while + cond + body
  auto loopBlock = new cdk::sequence_node(lineno, funcCall);
  loopBlock = new cdk::sequence_node(funcCall, incr_eval, loopBlock);
  
  auto loopCond = new cdk::le_node(lineno, it_rval, node->high());
  // loop_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block)
  auto loop = new til::loop_node(lineno, loopCond, loopBlock);

  loop->accept(this, lvl);

  _symtab.pop();

}





void til::postfix_writer::do_process_firstversion_node(til::process_node * const node, int lvl) {
  // low, high, vector, function
  // ASSERT_SAFE_EXPRESSIONS;
  // int lblCond, lblIncr, lblEnd;
  // auto low = dynamic_cast<cdk::integer_node*>(node->low())->value();
  // auto high = dynamic_cast<cdk::integer_node*>(node->high())->value();
  
  // int lineno = node->lineno();
  // auto proc_init = new cdk::integer_node(lineno, low);
  // std::string _identifier = "_i";
  // auto declaration = new til::declaration_node(lineno, tPRIVATE, cdk::primitive_type::create(4, cdk::TYPE_INT), _identifier, proc_init);
  // declaration->accept(this, lvl);

  // _pf.ALIGN()
  // _pf.LABEL(mklbl(lblCond = _lbl++));
  // auto cond_var = new cdk::variable_node(lineno, _identifier);
  // auto cond_rvalue = new cdk::rvalue_node(lineno, cond_var);
  // auto condition = new cdk::le_node(lineno, cond_rvalue, high);
  // condition->accept(this, lvl);
  // _pf.JZ(mklbl(lblEnd));

  // auto vector_el = new til::index_node(lineno, node->vec(), cond_rvalue);
  // auto vector_rvalue = new cdk::rvalue_node(lineno, vector_el);

  // auto function_call = new til::function_call_node(lineno, node->func(), vector_rvalue);
  // function_call->accept(this, lvl);

  // _pf.ALIGN()
  // _pf.LABEL(mklbl(lblIncr = _lbl++));
  // incr_literal = new cdk::integer_node(lineno, 1);
  // incr_add = new cdk::add_node(lineno, cond_rvalue, incr_literal);
  // incr_ass = new cdk::assignment_node(lineno, cond_var, incr_add);
  // incr_eval = new til::evaluation_node //...

  // _pf.JMP(mklbl(lblCond));
  // _pf.ALIGN();
  // _pf.LABEL(mklbl(lblEnd = _lbl++));

}





void til::postfix_writer::do_iterate_node(til::iterate_node * const node, int lvl) {
  /*
  iterate <vec> for <count> using <func> if <cond>;
  Se a condição `<cond>` for verdadeira, executa `<count>` iterações a percorrer
  o vetor `<vec>` (área de memória indexável por ponteiro), chamando a função
  `<func>` para cada elemento do vetor, devendo esta aceitar um argumento
  do tipo apontado por `<vec>`.

  if (cond) {
    while (i <= count) {
      f(vec[i]);
      i++;
    }
  }
  */
	ASSERT_SAFE_EXPRESSIONS;
  // criar iterador; criar count;
  // declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> var_type, std::string &identifier, cdk::expression_node *initializer)
  // 1. create i
  auto strItr = std::string("_itr");
  auto decItr = new til::declaration_node(node->lineno(), tPRIVATE, 
  cdk::primitive_type::create(4, cdk::TYPE_INT), strItr, new cdk::integer_node(node->lineno(), 0));

  decItr->accept(this, lvl);

  auto lvalItr = new cdk::variable_node(node->lineno(), strItr);
  auto itrVal = new cdk::rvalue_node(node->lineno(), lvalItr);

  // 2. create count
  auto strCount = std::string("_count");
  auto count = dynamic_cast<cdk::integer_node*>(node->count())->value();
  auto decCount = new til::declaration_node(node->lineno(), tPRIVATE, 
  cdk::primitive_type::create(4, cdk::TYPE_INT), strCount, count);

  decCount->accept(this, lvl);

  auto lvalCount = cdk::variable_node(node->lineno(), strCount);
  auto countVal = cdk::rvalue_node(node->lineno(), lvalCount);

  // index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index) ; lineno, base=vec, index=rvalue
  // 3. get vectorElement
  auto vecEl = new til::index_node(node->lineno(), node->vec(), itrVal);
  auto vecElVal = new cdk::rvalue_node(node->lineno(), vecEl);

  // 4. create func call
  auto args = new cdk::sequence_node(node->lineno(), vecElVal);
  auto funcCall = new til::function_call_node(node->lineno(), node->func(), args);
  auto funcCallEval = new til::evaluation_node(node->lineno(), funcCall)

  // 5. create increment instr
  auto incr = new cdk::integer_node(node->lineno(), 1);
  auto incrAdd = new cdk::add_node(node->lineno(), itrVal, incr);
  auto incrAssign = new cdk::assignment_node(node->lineno(), lValItr, incrAdd);
  // evaluation_node(int lineno, cdk::expression_node *argument)
  auto incrEval = new til::evaluation_node(node->lineno(), incrAssign);

  // 6. create loopBody and loopCond
  auto loopBody = new cdk::sequence_node(node->lineno(), funcCallEval, incrEval);
  loopBody = new cdk::sequence_node(loopBody, incrEval);

  auto loopCond = new cdk::lt_node(node->lineno(), itrVal, countVal);

  // 7. create loop and if instruction and call visitor
  auto loop = new til::loop_node(node->lineno(), loopCond, loopBody);

  auto ifInstr = new til::if_node(node->lineno(), node->cond(), loop);

  ifInstr->accept(this, lvl);

}


// iterate <vec> for <count> using <func> if <cond>;
void til::postfix_writer::do_unless_node(til::unless_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lineno = node->lineno();
  // 1. declare/init increment
  // declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> var_type, std::string &identifier, cdk::expression_node *initializer)
  i_id = std::string("_incr");

  auto incr_dec = new til::declaration_node(lineno, tPRIVATE, cdk::primitive_type::create(4, cdk::TYPE_INT),
  i_id, new integer_node(lineno, 0));
  i_dec->accept(this, lvl);

  auto i_lval = new cdk::variable_node(lineno, i_id);
  auto i_rval = new cdk::rvalue_node(lineno, i_lval);

  // 2. get vecEl and make funcCall
  // index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index)
  auto vecEl = new til::index_node(lineno, node->vec(), i_rval);
  auto vecEl_rval = new cdk::rvalue_node(lineno, vecEl);

  // function_call_node(int lineno, cdk::expression_node *function, cdk::sequence_node *arguments)
  auto func_call = new til::function_call_node(lineno, node->function(), vecEl_rval);

  // 3. create increment instruction
  auto incr_var = new cdk::integer_node(lineno, 1);
  auto incr_add = new cdk::add_node(lineno, i_rval, incr_var);
  auto incr_assign = new cdk::assignment_node(lineno, i_lval, incr_add);
  auto incr_eval = new cdk::evaluation_node(lineno, incr_assign);

  // 4. create comparison node, make loop body and loop cond
  auto loopCond = new cdk::lt_node(lineno, i_rval, node->count());

  auto loopBlock = new cdk::sequence_node(lineno, func_call, incr_eval);

  // 5. create the loop and the if instr and call the visitor
  // loop_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block)
  auto loop = new til::loop_node(lineno, loopCond, loopBlock);

  // if_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block)
  auto ifInstr = new til::if_node(lineno, node->condition(), loop);

  ifInstr->accept(this, lvl);

  /*
  if (cond) {
    i = 0
    while (i <= count) {
      f(v[i]);
      i++;
    }
  }
  */
}


// "with function vector low high"
void til::postfix_writer::do_with_node(til::with_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _symtab.push();

  int lineno = node->lineno();
  // declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> var_type, std::string &identifier, cdk::expression_node *initializer)
  std::string low_id = "_low";
  auto low_dec = new til::declaration_node(lineno, tPRIVATE, cdk::primitive_type::create(4, cdk::TYPE_INT),
  low_id, node->low());
  low_dec->accept(this, lvl);
  low_lval = new cdk::variable_node(lineno, low_id);
  low_rval = new cdk::rvalue_node(lineno, low_lval);

  // index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index)
  auto vecEl = new til::index_node(lineno, node->function(), low_rval);
  auto vecEl_rval = new cdk::rvalue_node(lineno, vecEl);
  auto args = new cdk::sequence_node(lineno, vecEl_rval);
  // function_call_node(int lineno, cdk::expression_node *function, cdk::sequence_node *arguments)
  auto func_call = new til::function_call_node(lineno, node->function(), args);

  auto incr_add = new cdk::add_node(lineno, low_rval, new cdk::integer_node(lineno, 1));
  auto incr_assign = new cdk::assignment_node(lineno, low_lval, incr_add);
  auto incr_eval = new til::evaluation_node(lineno, incr_assign);

  auto loopBlock = new cdk::sequence_node(lineno, func_call);
  loopBlock = new cdk::sequence_node(lineno, incr_eval, loopBlock);

  auto loopCond = new cdk::le_node(lineno, low_rval, node->high());

  // loop_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block)
  auto loop = new til::loop_node(lineno, loopCond, loopBlock);

  loop->accept(this, lvl);

  _symtab.pop();  

}



//-----------------------------------------TESTE PRÁTICO ----------------------------------


void til::postfix_writer::handle_global_declarations(til::declaration_node * const node, std::shared_ptr<til::symbol> symbol, int lvl) {
  // public, forward, var, external
  auto init = node->initializer();
  auto type = node->type();
  auto symbQualifier = symbol->qualifier();

  if (symbQualifier == tEXTERNAL || symbQualifier == tFORWARD) {
    _externalFuncs.insert(symbol->name());
    return;
  }

  _externalFuncs.erase(symbol->name());

  if (init) { // handles declarations with initializers
    _pf.DATA();
    _pf.ALIGN();

    if (symbQualifier == tPUBLIC) _pf.GLOBAL(symbol->name(), _pf.OBJ());
    _pf.LABEL(symbol->name());

    if (type->name() == cdk::TYPE_DOUBLE && init->is_typed(cdk::TYPE_INT)) {
      auto storedValue = dynamic_cast<cdk::integer_node*>(node->initializer())->value();
      _pf.SDOUBLE(storedValue);
    } else {
      node->initializer()->accept(this, lvl); // does _pf.SINT()
    }
    return; 
  }

  // handles declarations without initializers
  _pf.BSS();
  _pf.ALIGN();
  if (symbQualifier == tPUBLIC) _pf.GLOBAL(symbol->name(), _pf.OBJ());
  _pf.LABEL(symbol->name());
  _pf.SALLOC(type->size());
}


void til::postfix_writer::handle_function_declarations(til::declaration_node * const node, std::shared_ptr<til::symbol> symbol, int lvl) {
  auto init = node->initializer();
  if (init == nullptr) return;
  node->initializer()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    // it's a double; convert it to double if it's an int
    if (init->is_typed(cdk::TYPE_INT)) {
      _pf.I2D();
    }
    _pf.LOCAL(symbol->value());
    _pf.STDOUBLE();
  } else {
    _pf.LOCAL(symbol->value());
    _pf.STINT(); // 4
  }
}

void til::postfix_writer::do_declaration_node(til::declaration_node * const node, int lvl) {
  // declarations are either global or local to a block/function
  ASSERT_SAFE_EXPRESSIONS;
  // need to calculate offset first (either in func body, func args, or global variable)
  int offset = 0, typesize = node->type()->size();
  auto symbol = new_symbol();
  reset_new_symbol();

  if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize; // after offset 8 we have func args
  } else if (!_functionLabels.empty()) {
    _offset -= typesize; // negative addresses are for local variables
    offset = _offset;
  } else {
    offset = 0;
  }
 
  symbol->value(offset);

  // --- handles function scope declarations ---
  if (!_functionLabels.empty()) {
    if (_inFunctionArgs) return;
    handle_function_declarations(node, symbol, lvl);

  } else {
    // --- handles global declarations ---
    handle_global_declarations(node, symbol, lvl);
  }
}



//---------------------------------------------------------------------------

void til::postfix_writer::do_nullptr_node(til::nullptr_node * const node, int lvl) {
  if (!_functionLabels.empty()) {
    _pf.INT(0);
  } else {
    _pf.SINT(0);
  }
}

void til::postfix_writer::do_sizeof_node(til::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->expr()->type()->size());
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_function_node(til::function_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string functionLabel;
  functionLabel = mklbl(++_lbl);
  _functionLabels.push(functionLabel);

  _pf.TEXT(_functionLabels.top());
  _pf.ALIGN();
  _pf.LABEL(_functionLabels.top());

  auto oldOffset = _offset;
   // for func args to be in the right offset
  _offset = 8;
  _symtab.push();

  _inFunctionArgs = true;
  node->arguments()->accept(this, lvl);
  _inFunctionArgs = false;

  frame_size_calculator fsc(_compiler, _symtab);
  fsc.do_sequence_node(node->declarations(), lvl);
  fsc.do_sequence_node(node->instructions(), lvl);
  _pf.ENTER(fsc.localsize());

  auto tmp = _functionToJumpTo;
  _functionToJumpTo = mklbl(++_lbl);

  _offset = 0;

  do_decls_instrs(node->declarations(), node->instructions(), lvl);

  _pf.ALIGN();
  _pf.LABEL(_functionToJumpTo);
  _pf.LEAVE();
  _pf.RET();

  _functionToJumpTo = tmp; // swaps back to previous function
  _offset = oldOffset;
  _symtab.pop();
  _functionLabels.pop();

  if (!_functionLabels.empty()) {
    // inside a function, we need to leave the address on the stack
    _pf.TEXT(_functionLabels.top());
    _pf.ADDR(functionLabel);
  } else {
    // declares name for an address
    _pf.DATA();
    _pf.SADDR(functionLabel);
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_function_call_node(til::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // determines function type to call - either stored in node (expression) or recursive (symtab)
  std::shared_ptr<cdk::functional_type> func_type;
  if (node->function() == nullptr) { // recursion
    auto symbol = _symtab.find("@", 1); // context 0 is current, 1 is one up from this one
    func_type = cdk::functional_type::cast(symbol->type());
  } else {
    func_type = cdk::functional_type::cast(node->function()->type());
  }

  int arg_size = 0;
  // reverse iteration because of func args pushing order; generates code for each argument
  for (size_t i = node->arguments()->size(); i > 0; i--) {
    auto arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i - 1));

    arg_size += arg->type()->size();

    arg->accept(this, lvl + 2);
    if (func_type->input(i - 1)->name() == cdk::TYPE_DOUBLE && arg->is_typed(cdk::TYPE_INT)) {
      _pf.I2D();
    }
  }

  // checks if function is external
  _externalFunctionName = std::nullopt;
  
  if (node->function() == nullptr) { // recursive call
    _pf.ADDR(_functionLabels.top());
  } else {
    node->function()->accept(this, lvl);
  }
  
  if (_externalFunctionName) {
    _pf.CALL(*_externalFunctionName);
    _externalFunctionName = std::nullopt;
  } else {
    _pf.BRANCH();
  }

  // discards arguments from stack and leaves return value
  if (arg_size > 0) {
    _pf.TRASH(arg_size);
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  } else if (!node->is_typed(cdk::TYPE_VOID)) {
    _pf.LDFVAL32();
  }
}
