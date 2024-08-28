#!/bin/zsh

# eclipse et al.
/bin/rm -rf checked-out-ref/**/.project
/bin/rm -rf checked-out-ref/**/.cproject
/bin/rm -rf checked-out-ref/**/.cvsignore
/bin/rm -rf checked-out-ref/**/.gitignore
/bin/rm -rf checked-out-ref/**/.settings
/bin/rm -rf checked-out-ref/**/.docsdone

# CDK (other files
/bin/rm -rf checked-out-ref/**/cdk
/bin/rm -rf checked-out-ref/**/cdk.doxyfile
/bin/rm -rf checked-out-ref/**/Makefile
/bin/rm -rf checked-out-ref/**/LICENSE
/bin/rm -rf checked-out-ref/**/README.md

# misc auto files
/bin/rm -rf checked-out-ref/**/all_nodes.h
/bin/rm -rf checked-out-ref/**/visitor_decls.h
/bin/rm -rf checked-out-ref/**/mml_parser.tab.*
/bin/rm -rf checked-out-ref/**/mml_scanner.cpp
/bin/rm -rf checked-out-ref/**/.makedeps

# RTS
/bin/rm -rf checked-out-ref/**/file.c
/bin/rm -rf checked-out-ref/**/file.h
/bin/rm -rf checked-out-ref/**/float.c
/bin/rm -rf checked-out-ref/**/float.h
/bin/rm -rf checked-out-ref/**/integer.c
/bin/rm -rf checked-out-ref/**/integer.h
/bin/rm -rf checked-out-ref/**/kernel.asm
/bin/rm -rf checked-out-ref/**/kernel.h
/bin/rm -rf checked-out-ref/**/lib32.asm
/bin/rm -rf checked-out-ref/**/string.c
/bin/rm -rf checked-out-ref/**/string.h

# other
/bin/rm -rf checked-out-ref/**/mknodedecls.pl
/bin/rm -rf checked-out-ref/**/Makefile
/bin/rm -rf checked-out-ref/**/examples
/bin/rm -rf checked-out-ref/**/tests
/bin/rm -rf checked-out-ref/**/testes
/bin/rm -rf checked-out-ref/**/*.so
/bin/rm -rf checked-out-ref/**/*.a
/bin/rm -rf checked-out-ref/**/*.o
/bin/rm -rf checked-out-ref/**/*.s
/bin/rm -rf checked-out-ref/**/*~
/bin/rm -rf checked-out-ref/**/.auto/all_nodes.h
/bin/rm -rf checked-out-ref/**/.auto/visitor_decls.h
/bin/rm -rf checked-out-ref/**/*.output

/bin/rm -rf checked-out-ref/**/*.asm
/bin/rm -rf checked-out-ref/**/*.xml
/bin/rm -rf checked-out-ref/**/*.m19
/bin/rm -rf checked-out-ref/**/*.out
/bin/rm -rf checked-out-ref/**/*.mml
/bin/rm -rf checked-out-ref/**/mml

## CDK
/bin/rm -rf checked-out-ref/**/main.cpp
/bin/rm -rf checked-out-ref/**/cdk.doxyfile
/bin/rm -rf checked-out-ref/**/ast/not_node.h
/bin/rm -rf checked-out-ref/**/ast/unary_operation_node.h
/bin/rm -rf checked-out-ref/**/ast/literal_node.h
/bin/rm -rf checked-out-ref/**/ast/rvalue_node.h
/bin/rm -rf checked-out-ref/**/ast/mul_node.h
/bin/rm -rf checked-out-ref/**/ast/integer_node.h
/bin/rm -rf checked-out-ref/**/ast/string_node.h
/bin/rm -rf checked-out-ref/**/ast/typed_node.h
/bin/rm -rf checked-out-ref/**/ast/or_node.h
/bin/rm -rf checked-out-ref/**/ast/mod_node.h
/bin/rm -rf checked-out-ref/**/ast/lt_node.h
/bin/rm -rf checked-out-ref/**/ast/expression_node.h
/bin/rm -rf checked-out-ref/**/ast/ne_node.h
/bin/rm -rf checked-out-ref/**/ast/lvalue_node.h
/bin/rm -rf checked-out-ref/**/ast/sub_node.h
/bin/rm -rf checked-out-ref/**/ast/le_node.h
/bin/rm -rf checked-out-ref/**/ast/variable_node.h
/bin/rm -rf checked-out-ref/**/ast/unary_minus_node.h
/bin/rm -rf checked-out-ref/**/ast/unary_plus_node.h
/bin/rm -rf checked-out-ref/**/ast/nil_node.h
/bin/rm -rf checked-out-ref/**/ast/ge_node.h
/bin/rm -rf checked-out-ref/**/ast/binary_operation_node.h
/bin/rm -rf checked-out-ref/**/ast/data_node.h
/bin/rm -rf checked-out-ref/**/ast/sequence_node.h
/bin/rm -rf checked-out-ref/**/ast/eq_node.h
/bin/rm -rf checked-out-ref/**/ast/double_node.h
/bin/rm -rf checked-out-ref/**/ast/gt_node.h
/bin/rm -rf checked-out-ref/**/ast/assignment_node.h
/bin/rm -rf checked-out-ref/**/ast/add_node.h
/bin/rm -rf checked-out-ref/**/ast/div_node.h
/bin/rm -rf checked-out-ref/**/ast/basic_node.h
/bin/rm -rf checked-out-ref/**/ast/and_node.h
/bin/rm -rf checked-out-ref/**/emitters/postfix_debug_emitter.h
/bin/rm -rf checked-out-ref/**/emitters/postfix_ix86_emitter.cpp
/bin/rm -rf checked-out-ref/**/emitters/basic_postfix_emitter.cpp
/bin/rm -rf checked-out-ref/**/emitters/postfix_debug_emitter.cpp
/bin/rm -rf checked-out-ref/**/emitters/postfix_ix86_emitter.h
/bin/rm -rf checked-out-ref/**/emitters/basic_postfix_emitter.h
/bin/rm -rf checked-out-ref/**/compiler.h
/bin/rm -rf checked-out-ref/**/compiler.cpp
/bin/rm -rf checked-out-ref/**/basic_factory.cpp
/bin/rm -rf checked-out-ref/**/yy_scanner.h
/bin/rm -rf checked-out-ref/**/basic_scanner.cpp
/bin/rm -rf checked-out-ref/**/yy_parser.h
/bin/rm -rf checked-out-ref/**/basic_parser.cpp
/bin/rm -rf checked-out-ref/**/basic_scanner.h
/bin/rm -rf checked-out-ref/**/basic_parser.h
/bin/rm -rf checked-out-ref/**/basic_factory.h
/bin/rm -rf checked-out-ref/**/yy_factory.cpp
/bin/rm -rf checked-out-ref/**/symbol_table.h
/bin/rm -rf checked-out-ref/**/types/functional_type.cpp
/bin/rm -rf checked-out-ref/**/types/functional_type.h
/bin/rm -rf checked-out-ref/**/types/primitive_type.cpp
/bin/rm -rf checked-out-ref/**/types/reference_type.h
/bin/rm -rf checked-out-ref/**/types/basic_type.cpp
/bin/rm -rf checked-out-ref/**/types/primitive_type.h
/bin/rm -rf checked-out-ref/**/types/structured_type.cpp
/bin/rm -rf checked-out-ref/**/types/typename_type.h
/bin/rm -rf checked-out-ref/**/types/basic_type.h
/bin/rm -rf checked-out-ref/**/types/reference_type.cpp
/bin/rm -rf checked-out-ref/**/types/structured_type.h
/bin/rm -rf checked-out-ref/**/types/types.h
/bin/rm -rf checked-out-ref/**/targets/basic_target.cpp
/bin/rm -rf checked-out-ref/**/targets/basic_target.h
/bin/rm -rf checked-out-ref/**/yy_factory.h
/bin/rm -rf checked-out-ref/**/yy_parser.cpp
/bin/rm -rf checked-out-ref/**/yy_scanner.cpp

