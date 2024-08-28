#!/bin/zsh

# eclipse et al.
/bin/rm -rf checked-out/**/.project
/bin/rm -rf checked-out/**/.cproject
/bin/rm -rf checked-out/**/.cvsignore
/bin/rm -rf checked-out/**/.gitignore
/bin/rm -rf checked-out/**/.settings
/bin/rm -rf checked-out/**/.docsdone

# CDK (other files
/bin/rm -rf checked-out/**/cdk
/bin/rm -rf checked-out/**/cdk.doxyfile
/bin/rm -rf checked-out/**/Makefile
/bin/rm -rf checked-out/**/LICENSE
/bin/rm -rf checked-out/**/README.md

# misc auto files
/bin/rm -rf checked-out/**/all_nodes.h
/bin/rm -rf checked-out/**/visitor_decls.h
/bin/rm -rf checked-out/**/mml_parser.tab.*
/bin/rm -rf checked-out/**/mml_scanner.cpp
/bin/rm -rf checked-out/**/.makedeps

# RTS
/bin/rm -rf checked-out/**/file.c
/bin/rm -rf checked-out/**/file.h
/bin/rm -rf checked-out/**/float.c
/bin/rm -rf checked-out/**/float.h
/bin/rm -rf checked-out/**/integer.c
/bin/rm -rf checked-out/**/integer.h
/bin/rm -rf checked-out/**/kernel.asm
/bin/rm -rf checked-out/**/kernel.h
/bin/rm -rf checked-out/**/lib32.asm
/bin/rm -rf checked-out/**/string.c
/bin/rm -rf checked-out/**/string.h

# other
/bin/rm -rf checked-out/**/mknodedecls.pl
/bin/rm -rf checked-out/**/Makefile
/bin/rm -rf checked-out/**/examples
/bin/rm -rf checked-out/**/tests
/bin/rm -rf checked-out/**/testes
/bin/rm -rf checked-out/**/*.so
/bin/rm -rf checked-out/**/*.a
/bin/rm -rf checked-out/**/*.o
/bin/rm -rf checked-out/**/*.s
/bin/rm -rf checked-out/**/*~
/bin/rm -rf checked-out/**/.auto/all_nodes.h
/bin/rm -rf checked-out/**/.auto/visitor_decls.h
/bin/rm -rf checked-out/**/*.output

/bin/rm -rf checked-out/**/*.asm
/bin/rm -rf checked-out/**/*.xml
/bin/rm -rf checked-out/**/*.m19
/bin/rm -rf checked-out/**/*.out
/bin/rm -rf checked-out/**/*.mml
/bin/rm -rf checked-out/**/mml

## CDK
/bin/rm -rf checked-out/**/main.cpp
/bin/rm -rf checked-out/**/cdk.doxyfile
/bin/rm -rf checked-out/**/ast/not_node.h
/bin/rm -rf checked-out/**/ast/unary_operation_node.h
/bin/rm -rf checked-out/**/ast/literal_node.h
/bin/rm -rf checked-out/**/ast/rvalue_node.h
/bin/rm -rf checked-out/**/ast/mul_node.h
/bin/rm -rf checked-out/**/ast/integer_node.h
/bin/rm -rf checked-out/**/ast/string_node.h
/bin/rm -rf checked-out/**/ast/typed_node.h
/bin/rm -rf checked-out/**/ast/or_node.h
/bin/rm -rf checked-out/**/ast/mod_node.h
/bin/rm -rf checked-out/**/ast/lt_node.h
/bin/rm -rf checked-out/**/ast/expression_node.h
/bin/rm -rf checked-out/**/ast/ne_node.h
/bin/rm -rf checked-out/**/ast/lvalue_node.h
/bin/rm -rf checked-out/**/ast/sub_node.h
/bin/rm -rf checked-out/**/ast/le_node.h
/bin/rm -rf checked-out/**/ast/variable_node.h
/bin/rm -rf checked-out/**/ast/unary_minus_node.h
/bin/rm -rf checked-out/**/ast/unary_plus_node.h
/bin/rm -rf checked-out/**/ast/nil_node.h
/bin/rm -rf checked-out/**/ast/ge_node.h
/bin/rm -rf checked-out/**/ast/binary_operation_node.h
/bin/rm -rf checked-out/**/ast/data_node.h
/bin/rm -rf checked-out/**/ast/sequence_node.h
/bin/rm -rf checked-out/**/ast/eq_node.h
/bin/rm -rf checked-out/**/ast/double_node.h
/bin/rm -rf checked-out/**/ast/gt_node.h
/bin/rm -rf checked-out/**/ast/assignment_node.h
/bin/rm -rf checked-out/**/ast/add_node.h
/bin/rm -rf checked-out/**/ast/div_node.h
/bin/rm -rf checked-out/**/ast/basic_node.h
/bin/rm -rf checked-out/**/ast/and_node.h
/bin/rm -rf checked-out/**/emitters/postfix_debug_emitter.h
/bin/rm -rf checked-out/**/emitters/postfix_ix86_emitter.cpp
/bin/rm -rf checked-out/**/emitters/basic_postfix_emitter.cpp
/bin/rm -rf checked-out/**/emitters/postfix_debug_emitter.cpp
/bin/rm -rf checked-out/**/emitters/postfix_ix86_emitter.h
/bin/rm -rf checked-out/**/emitters/basic_postfix_emitter.h
/bin/rm -rf checked-out/**/compiler.h
/bin/rm -rf checked-out/**/compiler.cpp
/bin/rm -rf checked-out/**/basic_factory.cpp
/bin/rm -rf checked-out/**/yy_scanner.h
/bin/rm -rf checked-out/**/basic_scanner.cpp
/bin/rm -rf checked-out/**/yy_parser.h
/bin/rm -rf checked-out/**/basic_parser.cpp
/bin/rm -rf checked-out/**/basic_scanner.h
/bin/rm -rf checked-out/**/basic_parser.h
/bin/rm -rf checked-out/**/basic_factory.h
/bin/rm -rf checked-out/**/yy_factory.cpp
/bin/rm -rf checked-out/**/symbol_table.h
/bin/rm -rf checked-out/**/types/functional_type.cpp
/bin/rm -rf checked-out/**/types/functional_type.h
/bin/rm -rf checked-out/**/types/primitive_type.cpp
/bin/rm -rf checked-out/**/types/reference_type.h
/bin/rm -rf checked-out/**/types/basic_type.cpp
/bin/rm -rf checked-out/**/types/primitive_type.h
/bin/rm -rf checked-out/**/types/structured_type.cpp
/bin/rm -rf checked-out/**/types/typename_type.h
/bin/rm -rf checked-out/**/types/basic_type.h
/bin/rm -rf checked-out/**/types/reference_type.cpp
/bin/rm -rf checked-out/**/types/structured_type.h
/bin/rm -rf checked-out/**/types/types.h
/bin/rm -rf checked-out/**/targets/basic_target.cpp
/bin/rm -rf checked-out/**/targets/basic_target.h
/bin/rm -rf checked-out/**/yy_factory.h
/bin/rm -rf checked-out/**/yy_parser.cpp
/bin/rm -rf checked-out/**/yy_scanner.cpp

