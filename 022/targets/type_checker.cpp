#include <string>
#include "targets/type_checker.h"
#include ".auto/all_nodes.h"  // automatically generated
#include <cdk/types/primitive_type.h>
#include "targets/frame_size_calculator.h"

#include "til_parser.tab.h"

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//------------------------------ TESTE PRÁTICO ---------------------------------

void til::type_checker::do_process_node(til::process_node * const node, int lvl) {
  node->vector()->accept(this, lvl);
  node->low()->accept(this, lvl);
  node->high()->accept(this, lvl);
  node->function()->accept(this, lvl);

  if (node->low()->is_typed(cdk::TYPE_UNSPEC)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->low()->is_typed(cdk::TYPE_INT)) throw std::string("incorrect type for low argument");


  if (node->high()->is_typed(cdk::TYPE_UNSPEC)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->high()->is_typed(cdk::TYPE_INT)) throw std::string("incorrect type for high argument");

  if (!node->vector()->is_typed(cdk::TYPE_POINTER)) throw std::string("incorrect type for vector argument");

  if (!node->function()->is_typed(cdk::TYPE_FUNCTIONAL)) throw std::string("incorrect type for function argument");

  auto vecRef = cdk::reference_type::cast(node->vector()->type());
  auto funcRef = cdk::functional_type::cast(node->function()->type());

  if (funcRef->input_length() != 1) throw("incorrect length for function input");

  if (!compareTypes(true, funcRef->input(0), vecRef->referenced())) throw std::string("types for vector and function don't match");
}


void til::type_checker::do_process_firstversion_node(til::process_node * const node, int lvl) {
  // node->low()->accept(this, lvl);
  // node->high()->accept(this, lvl);
	// node->vector()->accept(this, lvl);
	// node->func()->accept(this, lvl);

  // auto lowType = node->low()->type()->name();
  // auto highType = node->high()->type()->name();
  
  // if (lowType == cdk::TYPE_UNSPEC) {
	// 	node->low()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  // }
  // else if (lowType != cdk::TYPE_INT) throw("wrong type for low argument");

	// if (highType == cdk::TYPE_UNSPEC) {
	// 	node->high()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  // }
  // else if (highType != cdk::TYPE_INT) throw("wrong type for low argument");

	// auto vecType = node->vector()->type()->name();
	// if (vecType != cdk::TYPE_POINTER) throw("wrong type for vector");

	// auto funcTypeName = node->func()->type()->name();
	// if (funcTypeName != cdk::TYPE_FUNCTIONAL) throw("wrong type for function");

	// auto funcType = cdk::functional_type::cast(node->func()->type());
	
	// if (funcType->input_length() != 1) throw("incorrect input size");

	// if (!compareTypes(false, node->vector()->type(), funcType->input(0))) {
	// 	throw("vector type and function input type must match");
	// }
}

void til::type_checker::do_iterate_node(til::iterate_node * const node, int lvl) {
  node->vec()->accept(this, lvl);
  node->count()->accept(this, lvl);
  node->func()->accept(this, lvl);
  node->cond()->accept(this, lvl);

  if (node->count()->is_typed(cdk::TYPE_UNSPEC)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } 
  else if (!node->count()->is_typed(cdk::TYPE_INT)) throw("count must be an integer");

  if (!node->vec()->is_typed(cdk::TYPE_POINTER)) throw("vector must be a pointer");

  if (!node->func()->is_typed(cdk::TYPE_FUNCTIONAL)) throw("incorrect type for function argument");

  auto funcType = cdk::functional_type::cast(node->func());
  auto vecType = cdk::reference_type::cast(node->vec());

  if (funcType->input_length() != 1) throw("incorrect size for function input");

  if (!compareTypes(false, funcType->input(0), vecType)) throw("incompatible types for function and vector arguments");
}


// unless <condition> iterate <vector> for <count> using <function>
void til::type_checker_do_unlesss_node(til::unless_node * const node, int lvl) {
  node->condition()->accept(this, lvl);
  node->vector()->accept(this, lvl);
  node->count()->accept(this, lvl);
  node->function()->accept(this, lvl);

  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->condition()->is_typed(cdk::TYPE_INT)) throw("wrong type for condition argument");

  if (!node->vector()->is_typed(cdk::TYPE_POINTER)) throw("wrong type for vector argument");

  if (node->count()->is_typed(cdk::TYPE_UNSPEC)) {
    node->count()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->count()->is_typed(cdk::TYPE_INT)) throw("wrong type for count argument");

  if (!node->function()->is_typed(cdk::TYPE_FUNCTIONAL)) throw("wrong type for function argument");

  auto funcRef = cdk::functional_type::cast(node->function()->type());
  auto vecRef = cdk::reference_type::cast(node->vector()->type());

  if (!vecRef->input_length() != 1) throw("wrong input size for vector argument");

  if (!compareTypes(false, funcRef->input(0), vecRef->referenced())) throw("vectype must match funcref type");
  
}


// "with function vector low high"
void til::type_checker_do_with_node(til::with_node * const node, int lvl) {
  node->function()->accept(this, lvl);
  node->vector()->accept(this, lvl);
  node->low()->accept(this, lvl);
  node->high()->accept(this, lvl);

  if (node->low()->is_typed(cdk::TYPE_UNSPEC)) {
    node->low()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (node->low->is_typed(cdk::TYPE_INT)) throw std::string("incorrect type for low argument");

  if (node->high()->is_typed(cdk::TYPE_UNSPEC)) {
    node->high()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (node->high()->is_typed(cdk::TYPE_INT)) throw std::string("incorrect type for high argument");

  if (!node->function()->is_typed(cdk::TYPE_FUNCTIONAL)) throw std::string("incorrect type for function argument");

  if (!node->vector()->is_typed(cdk::TYPE_POINTER)) throw std::string("incorrect type for vector argument");

  auto funcRef = cdk::functional_type::cast(node->function()->type());
  auto vecRef = cdk::reference_type::cast(node->vector()->type());

  if (!funcRef->input_length() != 1) throw std::string("incorrect input length for function");

  if (!compareTypes(true, funcRef->input(0), vecRef->referenced())) throw std::string("types don't match");

}

//---------------------------------------------------------------------------

void til::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_next_node(til::next_node *const node, int lvl) {
  // will remain empty on purpose
}

//---------------------------------------------------------------------------

void til::type_checker::do_stop_node(til::stop_node *const node, int lvl) {
  // will remain empty on purpose
}

//---------------------------------------------------------------------------

void til::type_checker::do_block_node(til::block_node *const node, int lvl) {
  // will remain empty on purpose 
}

//---------------------------------------------------------------------------

void til::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // will remain empty on purpose
}
void til::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // will remain empty on purpose
}

//---------------------------------------------------------------------------

void til::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void til::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void til::type_checker::do_unary_minus_node(cdk::unary_minus_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->argument()->is_typed(cdk::TYPE_INT) && !node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    throw std::string("unary expression being called with type: ", node->argument()->type()->name());
  }
  node->type(node->argument()->type());
}

void til::type_checker::do_unary_plus_node(cdk::unary_plus_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->argument()->is_typed(cdk::TYPE_INT) && !node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    throw std::string("unary expression being called with type: ", node->argument()->type()->name());
  }
  node->type(node->argument()->type());
}

void til::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("unary expression being called with type: ", node->argument()->type()->name());
  }
  node->type(node->argument()->type());
}

//---------------------------------------------------------------------------

// helper functions
bool til::type_checker::compareCovariantTypes(std::shared_ptr<cdk::basic_type> leftType, std::shared_ptr<cdk::basic_type> rightType) {
    return rightType->name() == cdk::TYPE_INT || rightType->name() == cdk::TYPE_DOUBLE;
}

// types: TYPE_INT, TYPE_DOUBLE, TYPE_BOOLEAN, TYPE_STRING, TYPE_POINTER, TYPE_STRUCT, TYPE_FUNCTIONAL, TYPE_VOID
bool til::type_checker::compareTypes(bool allowCovariants, std::shared_ptr<cdk::basic_type> leftType, std::shared_ptr<cdk::basic_type> rightType) {
  auto leftName = leftType->name();
  auto rightName = rightType->name();
  if (leftName == cdk::TYPE_UNSPEC || rightName == cdk::TYPE_UNSPEC) return false;

  // POINTERS
  if (leftName == cdk::TYPE_POINTER) {
    if (rightName != cdk::TYPE_POINTER) return false;
    // we may need to recursively compare them (pointer to pointer to ..)
    auto leftRef = cdk::reference_type::cast(leftType)->referenced();
    auto rightRef = cdk::reference_type::cast(rightType)->referenced();
    return compareTypes(allowCovariants, leftRef, rightRef);
  }
  // FUNCTIONS 
  else if (leftName == cdk::TYPE_FUNCTIONAL) {
    if (rightName != cdk::TYPE_FUNCTIONAL) return false;
    
    auto leftFunc = cdk::functional_type::cast(leftType);
    auto rightFunc = cdk::functional_type::cast(rightType);
    // input and output lists must have the same size, and each pair of types must match
    size_t leftInputs = leftFunc->input_length();
    size_t leftOutputs = leftFunc->output_length();
    if (leftInputs != rightFunc->input_length() || leftOutputs != rightFunc->output_length()) {
      return false;
    }
    // inputs
    for (size_t i = 0; i < leftInputs; i++) {
      if (!compareTypes(allowCovariants, rightFunc->input(i), leftFunc->input(i))) {
        return false;
      }
    }
    // outputs
    for (size_t i = 0; i < leftOutputs; i++) {
      if (!compareTypes(allowCovariants, leftFunc->output(i), rightFunc->output(i))) {
        return false;
      }
    }
    return true;
  } else if (rightName == cdk::TYPE_POINTER) {
      return false;
  } else if (allowCovariants && leftName == cdk::TYPE_DOUBLE) {
    return compareCovariantTypes(leftType, rightType);
  }
  else {
    return leftType == rightType;
  }

}

// ---------------------------------------------------------------------------

void til::type_checker::processBinaryArithmeticExprInt(cdk::binary_operation_node *const node, bool canBeDouble) {
  bool rightIsPtr = false;
    // possibilities: int/unspec op unspec, int/unspec op int, int/unspec op double, int/unspec op pointer
    if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
    else if (node->right()->is_typed(cdk::TYPE_INT)) {
      node->type(node->right()->type());
    }
    else if (canBeDouble && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
      node->type(node->right()->type());
    }
    else if (node->right()->is_typed(cdk::TYPE_POINTER)) {
      node->type(node->right()->type());
      rightIsPtr = true;
    }
    else {
      throw std::string("right type of binary arithmetic expression has a wrong type: ", node->right()->type()->name());
    }

    if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
      if (rightIsPtr) {
        node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      }
      else {
        node->left()->type(node->type());
      }
    }
}


void til::type_checker::processBinaryArithmeticExprDouble(cdk::binary_operation_node *const node) {
  // possibilities: double op unspec, double op int, double op double
  if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  }
  else if (node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(node->left()->type());
  }
  else if (node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(node->right()->type());
  }
  else {
    throw std::string("right type of binary arithmetic expression has a wrong type: ", node->right()->type()->name());
  }
}


void til::type_checker::processBinaryArithmeticExprPointer(cdk::binary_operation_node *const node, bool ptrSubtraction) {
  // possibilities: pointer op unspec, pointer op int, pointer op pointer
  if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->left()->type()); // pointer
  }
  else if (node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(node->left()->type());
  }
  else if (ptrSubtraction && node->right()->is_typed(cdk::TYPE_POINTER)) {

    if (!compareTypes(false, node->left()->type(), node->right()->type())) {
      throw std::string("types don't match in pointer difference operation");
    }
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT)); // "número de objectos do tipo apontado entre eles"
  }
  else {
    throw std::string("right type of binary arithmetic expression has a wrong type (3): ", node->right()->type()->name());
  }
}


// processes binary arithmetic expressions (+, -, *, /, %)
// bools allow me not to make many redundant functions, like in Og
void til::type_checker::processBinaryArithmeticExpr(cdk::binary_operation_node *const node, 
int lvl, bool canBeDouble, bool ptrDisplacement, bool ptrSubtraction) {
  ASSERT_UNSPEC;
  // possibilities: int/unspec (1), double (2), pointer (3)
  node->left()->accept(this, lvl+2);
  node->right()->accept(this, lvl+2);
  // int/unspec (1)
  if (node->left()->is_typed(cdk::TYPE_INT) || node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    processBinaryArithmeticExprInt(node, canBeDouble);
  }
  // double (2)
  else if (canBeDouble && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    processBinaryArithmeticExprDouble(node);
    
  }
  // pointer (3)
  else if (ptrDisplacement && node->left()->is_typed(cdk::TYPE_POINTER)) {
    processBinaryArithmeticExprPointer(node, ptrSubtraction);
  }
  else {
    throw std::string("left type of binary arithmetic expression has a wrong type: ", node->left()->type()->name());
  }
}


// process binary comparisons (<, >, <=, >=, ==, !=, &&, ||)
void til::type_checker::processBinaryComparisons(cdk::binary_operation_node *const node, int lvl, 
bool canBeDouble, bool canBePointer) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  // possibilities: unspec, int, double, pointer
  // check for unspecs first
  if (node->left()->is_typed(cdk::TYPE_UNSPEC)) {
    // possibilities for right: unspec, int, double, pointer
    if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
    else if (node->right()->is_typed(cdk::TYPE_INT) || (canBeDouble && node->right()->is_typed(cdk::TYPE_DOUBLE))) {
      node->left()->type(node->right()->type());
    }
    else if (canBePointer && node->right()->is_typed(cdk::TYPE_POINTER)) {
      node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
    else {
      throw std::string("right type of binary comparison expression has a wrong type: ", node->right()->type()->name());
    }
  }
  // left is int
  else if (node->left()->is_typed(cdk::TYPE_INT)) {
    // possibilities for right: unspec, int, double, pointer
    if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
      node->right()->type(node->left()->type());
    } 
    else if (!node->right()->is_typed(cdk::TYPE_INT) 
    && !(canBeDouble && node->right()->is_typed(cdk::TYPE_DOUBLE))
    && !(canBePointer && node->right()->is_typed(cdk::TYPE_POINTER))) {
      throw std::string("right type of binary comparison expression has a wrong type: ", node->right()->type()->name());
    }
  }
  // left is double
  else if (canBeDouble && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    // possibilities for right: unspec, int, double
    if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
      node->right()->type(node->left()->type());
    }
    else if (!node->right()->is_typed(cdk::TYPE_DOUBLE) && !node->right()->is_typed(cdk::TYPE_INT)) {
      throw std::string("right type of binary comparison expression has a wrong type: ", node->right()->type()->name());
    }
  }
  // left is pointer
  else if (canBePointer && node->left()->is_typed(cdk::TYPE_POINTER)) {
    // possibilities for right: unspec, int, pointer
    if (node->right()->is_typed(cdk::TYPE_UNSPEC)) {
      node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
    else if (!node->right()->is_typed(cdk::TYPE_POINTER) && !node->right()->is_typed(cdk::TYPE_INT)) {
      throw std::string("right type of binary comparison expression has a wrong type: ", node->right()->type()->name());
    }
  }
  else {
    throw std::string("left type of binary comparison expression has a wrong type: ", node->left()->type()->name());
  }
  // since it's a comparison, the type is always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}


//---------------------------------------------------------------------------


void til::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryArithmeticExpr(node, lvl, true, true, false);
}
void til::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryArithmeticExpr(node, lvl, true, true, true);
}
void til::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryArithmeticExpr(node, lvl, true, false, false);
}
void til::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryArithmeticExpr(node, lvl, true, false, false);
}
void til::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryArithmeticExpr(node, lvl, false, false, false);
}


//---------------------------------------------------------------------------


void til::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, true, false);
}
void til::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, true, false);
}
void til::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, true, false);
}
void til::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, true, false);
}
void til::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, true, true);
}
void til::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, true, true);
}
void til::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, false, false);
}
void til::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  processBinaryComparisons(node, lvl, false, false);
}


//---------------------------------------------------------------------------

void til::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  // find; set type
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol == nullptr) {
    throw std::string("undeclared variable '" + id + "'");
  }
  node->type(symbol->type());
}

void til::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

void til::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl);
  // first, we must check for unspecs or voids
  if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
    node->rvalue()->type(node->lvalue()->type());
  } else if (node->lvalue()->is_typed(cdk::TYPE_POINTER) && node->rvalue()->is_typed(cdk::TYPE_POINTER)) {
    auto leftRef_name = cdk::reference_type::cast(node->lvalue()->type())->referenced()->name(); 
    auto rightRef_name = cdk::reference_type::cast(node->rvalue()->type())->referenced()->name();

    if (leftRef_name == cdk::TYPE_VOID || rightRef_name == cdk::TYPE_VOID) {
      node->rvalue()->type(node->lvalue()->type());
    }
    if (rightRef_name == cdk::TYPE_UNSPEC) {
      node->rvalue()->type(node->lvalue()->type());
    }
  }
  // then we compare the types for equivalence
  if (!compareTypes(true, node->lvalue()->type(), node->rvalue()->type())) {
    throw std::string("wrong type in right argument of assignment expression");
  }
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void til::type_checker::do_program_node(til::program_node *const node, int lvl) {
  auto symbol = std::make_shared<til::symbol>(node->type(), "@");
  symbol->is_main(true);

  if (!_symtab.insert(symbol->name(), symbol)) {
    _symtab.replace(symbol->name(), symbol);
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_evaluation_node(til::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  // check for unspecs inside pointer and as the argument type itself
  if (node->argument()->is_typed(cdk::TYPE_POINTER)) {
    auto refType = cdk::reference_type::cast(node->argument()->type());
    auto refType_name = refType->referenced()->name();
    if (refType != nullptr && refType_name == cdk::TYPE_UNSPEC) {
      node->argument()->type(cdk::reference_type::create(4, cdk::primitive_type::create(4, cdk::TYPE_INT)));
    }
  }
  // if it's unspec, we initialize type as int
  else if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
}

//---------------------------------------------------------------------------

// according to the specification, we can only print ints, doubles or strings
void til::type_checker::do_print_node(til::print_node *const node, int lvl) {
  // print_node has a sequence_node of arguments (std::vector<basic_node*>), which we need to visit
  // and check for each if it's int, double or string (or unspec)
  for (size_t i = 0; i < node->arguments()->size(); i++) {
    // we cast to expression_node to get expressions if we store them in the sequence_node
    auto argument = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
    // visits to get type
    argument->accept(this, lvl);
    if (argument->is_typed(cdk::TYPE_UNSPEC)) {
      argument->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
    else if (!argument->is_typed(cdk::TYPE_INT) && !argument->is_typed(cdk::TYPE_DOUBLE) && !argument->is_typed(cdk::TYPE_STRING)) {
      throw std::string("wrong type inside print expression");
    }
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_read_node(til::read_node *const node, int lvl) {
  ASSERT_UNSPEC;
  // we just read whatever comes (you're responsible for the ten million unspec checks!)
  // em situações que permitam vários tipos, "deve ser lido um inteiro"
  node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void til::type_checker::do_loop_node(til::loop_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  // also checks for unspecs (same in if and if_else)
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in condition of loop instruction");
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_if_node(til::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if(node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in condition of conditional instruction");
  }
}

void til::type_checker::do_if_else_node(til::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);

  if(node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in condition of conditional instruction");
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_address_node(til::address_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->value()->accept(this, lvl + 2);
  if (node->value()->is_typed(cdk::TYPE_POINTER)) {
    auto ref = cdk::reference_type::cast(node->value()->type());
    if (ref->referenced()->name() == cdk::TYPE_VOID) {
      // "void! e void!!! são equivalentes"
      node->type(node->value()->type());
      return;
    }
  }
  auto thisType = cdk::reference_type::create(4, node->value()->type());
  node->type(thisType);
}

//---------------------------------------------------------------------------

void til::type_checker::handle_var_declaration(til::declaration_node * const node, int lvl) {
  node->initializer()->accept(this, lvl + 2);
  // check for unspec on type or inside pointer
  if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)) {
    node->initializer()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } 
  else if (node->initializer()->is_typed(cdk::TYPE_POINTER)) {
    auto ref = cdk::reference_type::cast(node->initializer()->type());

    if (ref->referenced()->name() == cdk::TYPE_UNSPEC) {
      node->initializer()->type(cdk::reference_type::create(4, cdk::primitive_type::create(4, cdk::TYPE_INT)));
    }
  }
  else if (node->initializer()->is_typed(cdk::TYPE_VOID)) {
    throw std::string("cannot declare variable of type void");
  }
  node->type(node->initializer()->type());
}


void til::type_checker::handle_typed_declaration(til::declaration_node * const node, int lvl) {
  auto init = node->initializer();
  if (init == nullptr) return;
  // otherwise, we need to check the initializer
  node->initializer()->accept(this, lvl+2);
  auto nodeInitName = node->initializer()->type()->name();
  if (nodeInitName == cdk::TYPE_UNSPEC) {
    if (node->is_typed(cdk::TYPE_DOUBLE)) {
      node->initializer()->type(node->type());
    } 
    else {
      node->initializer()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
  } else if (nodeInitName == cdk::TYPE_POINTER && node->is_typed(cdk::TYPE_POINTER)) {
    auto initrefname = cdk::reference_type::cast(node->initializer()->type())->referenced()->name();
    auto noderefname = cdk::reference_type::cast(node->type())->referenced()->name();
    // we need to ensure type compatibility if the pointers are unspec or voids
    if (initrefname == cdk::TYPE_UNSPEC || initrefname == cdk::TYPE_VOID || noderefname == cdk::TYPE_VOID) {
      node->initializer()->type(node->type());
    }
  }

  if (!compareTypes(true, node->type(), node->initializer()->type())) {
    throw std::string("wrong type in initializer for variable");
  }
}


void til::type_checker::do_declaration_node(til::declaration_node * const node, int lvl) {

  if (node->type() == nullptr) { // if it's var, the node type is nullptr and not assigning it a correct type causes a segfault
    handle_var_declaration(node, lvl);
  } 
  else { // not var (has a type)
    handle_typed_declaration(node, lvl);
  }

  // handles externals and inserts symbol
  if (node->qualifier() == tEXTERNAL && !node->is_typed(cdk::TYPE_FUNCTIONAL)) {
    throw std::string("external declaration must be a function");
  }

  auto symbol = std::make_shared<til::symbol>(node->type(), node->identifier(), node->qualifier());
  // insert succesfully
  if (_symtab.insert(node->identifier(), symbol)) {
    _parent->set_new_symbol(symbol);
    return;
  }
  // if we can't insert succesfully, it's a redeclaration
  auto tmp = _symtab.find(node->identifier());

  if (tmp != nullptr && tmp->qualifier() == tFORWARD) {
    if (compareTypes(false, tmp->type(), symbol->type())) {
      _symtab.replace(node->identifier(), symbol);
      _parent->set_new_symbol(symbol);
      return;
    }
  }

  throw std::string("variable '" + node->identifier() + "' redeclared");
}

//---------------------------------------------------------------------------

void til::type_checker::do_nullptr_node(til::nullptr_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}

//---------------------------------------------------------------------------

void til::type_checker::do_object_node(til::object_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl + 2);

  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in argument");
  }

  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}

//---------------------------------------------------------------------------

void til::type_checker::do_sizeof_node(til::sizeof_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->expr()->accept(this, lvl + 2);
  
  if (node->expr()->is_typed(cdk::TYPE_UNSPEC)) { 
    node->expr()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void til::type_checker::do_index_node(til::index_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->base()->accept(this, lvl + 2);
  if (!node->base()->is_typed(cdk::TYPE_POINTER)) {
    throw std::string("wrong type in index's base (expected pointer)");
  }

  node->index()->accept(this, lvl + 2);
  if (node->index()->is_typed(cdk::TYPE_UNSPEC)) {
    node->index()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->index()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type of index (expected integer)");
  }

  auto ref_type = cdk::reference_type::cast(node->base()->type());

  if (ref_type->referenced()->name() == cdk::TYPE_UNSPEC) {
    ref_type = cdk::reference_type::create(4, cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->base()->type(ref_type);
  }

  node->type(ref_type->referenced());
}

//---------------------------------------------------------------------------

void til::type_checker::do_function_node(til::function_node * const node, int lvl) {
  // functions are anonymous
  auto function = std::make_shared<til::symbol>(node->type(), "@");

  if (!_symtab.insert(function->name(), function)) {
    // it already exists locally
    _symtab.replace(function->name(), function);
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_function_call_node(til::function_call_node * const node, int lvl) {
  // cdk::expression_node *_function, that can be nullptr (recursive); cdk::sequence_node *_arguments;
  ASSERT_UNSPEC;
  std::shared_ptr<cdk::functional_type> type;
  if (node->function() != nullptr) {
    node->function()->accept(this, lvl);

    auto funcType_name = node->function()->type()->name();
    if (funcType_name != cdk::TYPE_FUNCTIONAL) throw std::string("trying to call a non-functional type");

    type = cdk::functional_type::cast(node->function()->type());
  }
  else { // recursive function call (function not a nullptr)
    size_t from = 1; // context above
    auto sym = _symtab.find("@", from);
    if (sym == nullptr) throw std::string("trying to do invalid recursive function call");
    else if (sym->is_main()) throw std::string("trying to call main recursively");
    type = cdk::functional_type::cast(sym->type());
  }
  // number and type of actual params must agree with formal params
  size_t actualParams = node->arguments()->size();
  if (actualParams != type->input_length()) throw std::string("number of arguments doesn't match function's arguments");
  
  for (size_t i = 0; i < actualParams; i++) {
    auto param = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
    param->accept(this, lvl);
    auto type_param = type->input(i);
    // creates types for unspecs
    if (param->is_typed(cdk::TYPE_UNSPEC)) {
      if (type_param->name() == cdk::TYPE_DOUBLE) {
        param->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      } else {
        param->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      }
    }
    else if (param->is_typed(cdk::TYPE_POINTER) && type_param->name() == cdk::TYPE_POINTER) {
      auto paramname = cdk::reference_type::cast(type_param)->referenced()->name();
      auto argname = cdk::reference_type::cast(param->type())->referenced()->name() ;
      // same as above, compatibility
      if (argname == cdk::TYPE_UNSPEC || argname == cdk::TYPE_VOID || paramname == cdk::TYPE_VOID) {
        param->type(type_param);
      }
    }
    
    if (!compareTypes(true, type_param, param->type())) throw std::string("wrong type in argument of function call");
  }
  node->type(type->output(0));
}

//---------------------------------------------------------------------------

void til::type_checker::do_return_node(til::return_node *const node, int lvl) {
  auto symbol = _symtab.find("@", 1);

  // functions and main insert the "@" so we must find it
  if (symbol == nullptr) throw std::string("trying to return out of a function!");

  std::shared_ptr<cdk::functional_type> functionType = cdk::functional_type::cast(symbol->type());
  //
  auto returnType = functionType->output(0);

  // no return value
  if (node->return_value() == nullptr) {
    if (returnType->name() != cdk::TYPE_VOID) {
      throw std::string("no return value specified for non-void function");
    }
  } else { // return has a value
    if (returnType->name() == cdk::TYPE_VOID) throw std::string("void function can't return a value");

    node->return_value()->accept(this, lvl + 2);

    if (!compareTypes(true, returnType, node->return_value()->type())) {
      throw std::string("wrong type for return expression");
    }
  }
}