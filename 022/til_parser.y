%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;          /* integer value */
  double                d;          /* double value */
  std::string          *s;          /* symbol name or string literal */
  cdk::basic_node      *node;       /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  til::block_node      *block;
  til::program_node    *program;
  std::vector<std::shared_ptr<cdk::basic_type>> *type_vec;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tINT_T tDOUBLE_T tSTRING_T tVOID_T
%token tEXTERNAL tFORWARD tPUBLIC tVAR tPRIVATE
%token tBLOCK tIF tELSE tLOOP tSTOP tNEXT tRETURN tPRINT tPRINTLN tITERATE tFOR tUSING tUNLESS tWITH tPROCESS
%token tREAD tNULL tSET tINDEX tOBJECTS tSIZEOF
%token tPROGRAM tFUNCTION
%token tGE tLE tEQ tNE tAND tOR

%type <node> instr decl private_decl fun_arg
%type <sequence> exprs instrs decls p_decls opt_exprs fun_args
%type <type> type type_ref void_ref reference return_type fun_type
%type <type_vec> types
%type <expression> expr fundef
%type <lvalue> lval
%type <block> block
%type <program> program

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}

%%

file : decls program    { compiler->ast(new cdk::sequence_node(LINE, $2, $1)); }
     | decls            { compiler->ast($1); }
     |       program    { compiler->ast(new cdk::sequence_node(LINE, $1)); }
     | /* empty */      { compiler->ast(new cdk::sequence_node(LINE)); }
     ;

program : '(' tPROGRAM p_decls instrs ')'         { $$ = new til::program_node(LINE, $3, $4); }
        | '(' tPROGRAM p_decls        ')'         { $$ = new til::program_node(LINE, $3, new cdk::sequence_node(LINE)); }
        | '(' tPROGRAM               instrs ')'   { $$ = new til::program_node(LINE, new cdk::sequence_node(LINE), $3); }
        | '(' tPROGRAM ')' { $$ = new til::program_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
        ;


decls : decls decl    { $$ = new cdk::sequence_node(LINE, $2, $1); }
      |       decl    { $$ = new cdk::sequence_node(LINE, $1); }
      ;


decl : '(' tPUBLIC type tIDENTIFIER ')'           { $$ = new til::declaration_node(LINE, tPUBLIC, $3, *$4, nullptr); delete $4; }
     | '(' tPUBLIC      tIDENTIFIER expr ')'      { $$ = new til::declaration_node(LINE, tPUBLIC, nullptr, *$3, $4); delete $3; }
     | '(' tPUBLIC type tIDENTIFIER expr ')'      { $$ = new til::declaration_node(LINE, tPUBLIC, $3, *$4, $5); delete $4; }
     | '(' tPUBLIC tVAR tIDENTIFIER expr ')'      { $$ = new til::declaration_node(LINE, tPUBLIC, nullptr, *$4, $5); delete $4; } 
     | '(' tFORWARD type tIDENTIFIER ')'          { $$ = new til::declaration_node(LINE, tFORWARD, $3, *$4, nullptr); delete $4; }
     | '(' tEXTERNAL type tIDENTIFIER ')'         { $$ = new til::declaration_node(LINE, tEXTERNAL, $3, *$4, nullptr); delete $4; }
     | private_decl                               { $$ = $1; }
     ;


type : type_ref         { $$ = $1; } 
     | void_ref         { $$ = $1; }
     ;


type_ref : tINT_T           { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }
         | tDOUBLE_T        { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }
         | tSTRING_T        { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
         | fun_type         { $$ = $1; }
         | reference        { $$ = $1; }
         ;


fun_type : '(' return_type ')'                         { $$ = cdk::functional_type::create($2); }
         | '(' return_type '(' types ')' ')'           { $$ = cdk::functional_type::create(*$4, $2); delete $4; }
         ;


void_ref : void_ref '!' { $$ = $1; }
         | tVOID_T '!'  { $$ = cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_VOID)); }
         ;


return_type : type            { $$ = $1; }
            | tVOID_T         { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID); }
            ;


types : types type     { $$ = $1; $$->push_back($2); }
      | type           { $$ = new std::vector<std::shared_ptr<cdk::basic_type>>(1, $1); }
      ;


reference : type_ref '!' { $$ = cdk::reference_type::create(4, $1); }
          ;


p_decls : p_decls private_decl          { $$ = new cdk::sequence_node(LINE, $2, $1); }
        | private_decl                  { $$ = new cdk::sequence_node(LINE, $1); }
        ;


private_decl : '(' type tIDENTIFIER ')'              { $$ = new til::declaration_node(LINE, tPRIVATE, $2, *$3, nullptr); delete $3; }
             | '(' type tIDENTIFIER expr ')'         { $$ = new til::declaration_node(LINE, tPRIVATE, $2, *$3, $4); delete $3; }
             | '(' tVAR tIDENTIFIER expr ')'         { $$ = new til::declaration_node(LINE, tPRIVATE, nullptr, *$3, $4); delete $3; }
             ;


instrs : instrs instr          { $$ = new cdk::sequence_node(LINE, $2, $1); }
       | instr                 { $$ = new cdk::sequence_node(LINE, $1); }
       ;


instr :  expr                                                     { $$ = new til::evaluation_node(LINE, $1); }
      | '(' tPRINT exprs ')'                                      { $$ = new til::print_node(LINE, $3, false); }
      | '(' tPRINTLN exprs ')'                                    { $$ = new til::print_node(LINE, $3, true); }
      | '(' tSTOP tINTEGER ')'                                    { $$ = new til::stop_node(LINE, $3); }
      | '(' tSTOP ')'                                             { $$ = new til::stop_node(LINE, 1); }
      | '(' tNEXT tINTEGER ')'                                    { $$ = new til::next_node(LINE, $3); }
      | '(' tNEXT ')'                                             { $$ = new til::next_node(LINE, 1); }
      | '(' tRETURN expr ')'                                      { $$ = new til::return_node(LINE, $3); }
      | '(' tRETURN ')'                                           { $$ = new til::return_node(LINE, nullptr); }
      | '(' tIF expr instr instr ')'                              { $$ = new til::if_else_node(LINE, $3, $4, $5); }
      | '(' tIF expr instr ')'                                    { $$ = new til::if_node(LINE, $3, $4); } 
      | '(' tLOOP expr instr ')'                                  { $$ = new til::loop_node(LINE, $3, $4); }
      | '(' tITERATE expr tFOR expr tUSING expr tIF expr ')'      { $$ = new til::iterate_node(LINE, $3, $5, $7, $9); }
      | '(' tUNLESS expr tITERATE expr tFOR expr tUSING expr ')'  { $$ = new til::unless_node(LINE, $3, $5, $7, $9); }
      | '(' tWITH expr expr expr expr')'                          { $$ = new til::with_node(LINE, $3, $4, $5, $6); }
      | '(' tPROCESS expr expr expr expr ')'                      { $$ = new til::process_node(LINE, $3, $4, $5, $6); }
      | block                                                     { $$ = $1; }
      ;
      

block : '(' tBLOCK decls instrs ')'  { $$ = new til::block_node(LINE, $3, $4); }
      | '(' tBLOCK decls        ')'  { $$ = new til::block_node(LINE, $3, new cdk::sequence_node(LINE)); }
      | '(' tBLOCK       instrs ')'  { $$ = new til::block_node(LINE, new cdk::sequence_node(LINE), $3); }
      | '(' tBLOCK /* empty */  ')'  { $$ = new til::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
      ;


exprs: exprs expr                  { $$ = new cdk::sequence_node(LINE, $2, $1); }
     | expr                        { $$ = new cdk::sequence_node(LINE, $1); }
     ;

expr : tINTEGER                           { $$ = new cdk::integer_node(LINE, $1); }
     | tSTRING                            { $$ = new cdk::string_node(LINE, $1); }
     | tDOUBLE                            { $$ = new cdk::double_node(LINE, $1); }
     | tNULL                              { $$ = new til::nullptr_node(LINE); }
     | lval                               { $$ = new cdk::rvalue_node(LINE, $1); }
     | '(' '-' expr ')'                   { $$ = new cdk::unary_minus_node(LINE, $3); }
     | '(' '+' expr ')'                   { $$ = new cdk::unary_plus_node(LINE, $3); }
     | '(' '+' expr expr ')'              { $$ = new cdk::add_node(LINE, $3, $4); }
     | '(' '-' expr expr ')'              { $$ = new cdk::sub_node(LINE, $3, $4); }
     | '(' '*' expr expr ')'              { $$ = new cdk::mul_node(LINE, $3, $4); }
     | '(' '/' expr expr ')'              { $$ = new cdk::div_node(LINE, $3, $4); }
     | '(' '%' expr expr ')'              { $$ = new cdk::mod_node(LINE, $3, $4); }
     | '(' '<' expr expr ')'              { $$ = new cdk::lt_node(LINE, $3, $4); }
     | '(' '>' expr expr ')'              { $$ = new cdk::gt_node(LINE, $3, $4); }
     | '(' tGE expr expr ')'              { $$ = new cdk::ge_node(LINE, $3, $4); }
     | '(' tLE expr expr ')'              { $$ = new cdk::le_node(LINE, $3, $4); }
     | '(' tNE expr expr ')'              { $$ = new cdk::ne_node(LINE, $3, $4); }
     | '(' tEQ expr expr ')'              { $$ = new cdk::eq_node(LINE, $3, $4); }
     | '(' '~' expr ')'                   { $$ = new cdk::not_node(LINE, $3); }
     | '(' tAND expr expr ')'             { $$ = new cdk::and_node(LINE, $3, $4); }
     | '(' tOR expr expr ')'              { $$ = new cdk::or_node(LINE, $3, $4); }
     | '(' tSIZEOF expr ')'               { $$ = new til::sizeof_node(LINE, $3); }
     | '(' tSET lval expr ')'             { $$ = new cdk::assignment_node(LINE, $3, $4); }
     | '(' '?' lval ')'                   { $$ = new til::address_node(LINE, $3); }
     | '(' tOBJECTS expr ')'              { $$ = new til::object_node(LINE, $3); }
     | '(' tREAD ')'                      { $$ = new til::read_node(LINE); }
     | '(' expr opt_exprs ')'             { $$ = new til::function_call_node(LINE, $2, $3); }
     | '(' '@' opt_exprs ')'              { $$ = new til::function_call_node(LINE, nullptr, $3); }
     | '(' fundef ')'                     { $$ = $2; }
     ;
     
lval :  tIDENTIFIER                     { $$ = new cdk::variable_node(LINE, $1); }
     |  '(' tINDEX expr expr ')'        { $$ = new til::index_node(LINE, $3, $4); }
     ;

opt_exprs : exprs                         { $$ = $1; }
          | /* empty */                   { $$ = new cdk::sequence_node(LINE); }
          ;

fundef : tFUNCTION '(' return_type ')' p_decls instrs             { $$ = new til::function_node(LINE, $3, $5, $6, new cdk::sequence_node(LINE)); }
       | tFUNCTION '(' return_type ')' p_decls                    { $$ = new til::function_node(LINE, $3, $5, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
       | tFUNCTION '(' return_type ')'         instrs             { $$ = new til::function_node(LINE, $3, new cdk::sequence_node(LINE), $5, new cdk::sequence_node(LINE)); }
       | tFUNCTION '(' return_type ')'                            { $$ = new til::function_node(LINE, $3, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
       | tFUNCTION '(' return_type fun_args ')' p_decls instrs    { $$ = new til::function_node(LINE, $3, $6, $7, $4); }
       | tFUNCTION '(' return_type fun_args ')' p_decls           { $$ = new til::function_node(LINE, $3, $6, new cdk::sequence_node(LINE), $4); }
       | tFUNCTION '(' return_type fun_args ')'         instrs    { $$ = new til::function_node(LINE, $3, new cdk::sequence_node(LINE), $6, $4); }
       | tFUNCTION '(' return_type fun_args ')'                   { $$ = new til::function_node(LINE, $3, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $4); }
       ;

fun_args : fun_args fun_arg         { $$ = new cdk::sequence_node(LINE, $2, $1); }
         | fun_arg                  { $$ = new cdk::sequence_node(LINE, $1); }
         ; 

fun_arg : '(' type tIDENTIFIER ')'  { $$ = new til::declaration_node(LINE, tPRIVATE, $2, *$3, nullptr); delete $3; }

%%
