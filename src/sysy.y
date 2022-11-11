%code requires {
  #include <memory>
  #include <string>
}

%{

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include "ast.h"
int yylex();
void yyerror(std::unique_ptr<class BaseAST> &ast, const char *s);
std::stack<std::vector<BaseAST*>> temp_stack_vec_block_item;
std::stack<std::stack<BaseAST*>> temp_stack_stack_stmt_need_to_add_last_while_addr;
std::stack<BaseAST*> temp_stack_comp_unit;
std::vector<BaseAST*> temp_vec_const_def;
std::vector<BaseAST*> temp_vec_var_def;
std::vector<BaseExpAST*> temp_vec_const_exp;
std::vector<BaseExpAST*> temp_vec_exp;
std::stack<std::vector<BaseExpAST*>> stack_vec_const_exp_for_array_len;
std::stack<std::vector<BaseExpAST*>> stack_vec_exp_for_array_len;
std::stack<std::vector<BaseAST*>> stack_vec_const_init_val;
std::stack<std::vector<BaseAST*>> stack_vec_init_val;
std::stack<FuncFParamAST*> temp_stack_func_f_params;
std::stack<ExpAST*> temp_stack_func_r_params;
BaseAST* cur_while=NULL;
using namespace std;

%}


%parse-param { std::unique_ptr<class BaseAST> &ast }

%union {
  std::string *str_val;
  int int_val;
  class BaseAST *base_ast_val;
  class BaseExpAST* base_exp_ast_val;
  //class DeclAST* decl_ast_val;
  class BaseOpAST* base_op_ast_val;
  class CompUnitEle* base_comp_unit_ele_ast_val;
  char one_len_op_val;
  char two_len_op_val[3];
}


%token KW_RETURN KW_CONST KW_IF KW_ELSE KW_WHILE KW_BREAK KW_CONTINUE KW_INT KW_VOID    //KW:KeyWord
%token <str_val> KW_TYPE
%token <str_val> IDENT
%token <int_val> INT_CONST
%token <one_len_op_val> UNARY_OP
%token <one_len_op_val> ADD_OP
%token <one_len_op_val> MUL_OP
%token <one_len_op_val> ONE_LEN_REL_OP
%token <two_len_op_val> TWO_LEN_REL_OP
%token <two_len_op_val> EQ_OP
%token <two_len_op_val> LAND_OP
%token <two_len_op_val> LOR_OP;

%type<base_comp_unit_ele_ast_val> CompUnitEle
%type <base_ast_val> FuncDef Block Stmt 
Decl ConstDecl ConstDef ConstInitVal BlockItem LVal
VarDecl VarDef InitVal MatchedStmt OpenStmt WHILE FuncFParams FuncRParams FuncFParam CompUnit
__FuncDef__ __Block__ __Stmt__ 
__Decl__ __ConstDecl__ __ConstDef__ __ConstInitVal__ __BlockItem__ __LVal__
__VarDecl__ __VarDef__ __InitVal__ __MatchedStmt__ __OpenStmt__ __WHILE__ __FuncFParams__ __FuncRParams__ __FuncFParam__ __CompUnitEle__
%type <base_exp_ast_val> Number ConstExp Exp PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp
__Number__ __ConstExp__ __Exp__ __PrimaryExp__ __UnaryExp__ __MulExp__ __AddExp__ __RelExp__ __EqExp__ __LAndExp__ __LOrExp__ 
%type <base_op_ast_val> UnaryOp MulOp AddOp RelOp LAndOp LOrOp EqOp
__UnaryOp__ __MulOp__ __AddOp__ __RelOp__ __LAndOp__ __LOrOp__ __EqOp__
%%

CompUnit
  :  CompUnitEle  CompUnitEleSeq 
  {
  auto comp_unit = make_unique<CompUnitAST>();
  comp_unit->comp_unit_ele=unique_ptr<CompUnitEle>($1);
  if(temp_stack_comp_unit.empty()==true)
  {
    ast = move(comp_unit);
  }
  else
  {
  auto temp_ast_next=temp_stack_comp_unit.top();
  temp_stack_comp_unit.pop();   
  while(temp_stack_comp_unit.empty()==false)
  {
    auto temp_ast_cur=temp_stack_comp_unit.top();
    temp_stack_comp_unit.pop();
    dynamic_cast<CompUnitAST*>(temp_ast_cur)->next_comp_unit_ele=unique_ptr<BaseAST>(temp_ast_next);
    temp_ast_next=temp_ast_cur;
  };
  ast = move(comp_unit);
  dynamic_cast<CompUnitAST*>(ast.get())->next_comp_unit_ele=unique_ptr<BaseAST>(temp_ast_next);
  }
  }
  ;

CompUnitEle: __CompUnitEle__ {PRINT("    Reduce:CompUnitEle\n")};

__CompUnitEle__
  :FuncDef
  {
    auto ast=new CompUnitAST();
    ast->comp_unit_ele=unique_ptr<CompUnitEle>(new CompUnitEle(TAG_0));  
    ast->comp_unit_ele->comp_unit_ele_union.comp_unit_ele_case_0->func_def=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  | Decl
  {
    auto ast=new CompUnitAST();
    ast->comp_unit_ele=unique_ptr<CompUnitEle>(new CompUnitEle(TAG_1));  
    ast->comp_unit_ele->comp_unit_ele_union.comp_unit_ele_case_1->decl=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  ;


CompUnitEleSeq: __CompUnitEleSeq__ {PRINT("    Reduce:CompUnitEleSeq\n")};

__CompUnitEleSeq__
  :  CompUnitEle {temp_stack_comp_unit.push($1);} CompUnitEleSeq 
  {
  }
  |
  {
  }
  ;


FuncDef: __FuncDef__{PRINT("    Reduce:FuncDef\n")};

__FuncDef__
  :  KW_INT IDENT '(' ')' Block 
  {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseTypeAST>(new FuncTypeAST("int"));
    ast->ident = *unique_ptr<string>($2);
    ast->func_f_params=unique_ptr<BaseAST>(new FuncFParamsAST());
    ast->block = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  |  KW_VOID IDENT '(' ')' Block 
  {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseTypeAST>(new FuncTypeAST("void"));
    ast->ident = *unique_ptr<string>($2);
    ast->func_f_params=unique_ptr<BaseAST>(new FuncFParamsAST());
    ast->block = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  |  KW_INT IDENT '('  FuncFParams ')'  Block
  {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseTypeAST>(new FuncTypeAST("int"));
    ast->ident = *unique_ptr<string>($2);
    ast->func_f_params=unique_ptr<BaseAST>($4);
    ast->block = unique_ptr<BaseAST>($6);
    $$ = ast;
  }
  |  KW_VOID IDENT '('  FuncFParams ')'  Block
  {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseTypeAST>(new FuncTypeAST("void"));
    ast->ident = *unique_ptr<string>($2);
    ast->func_f_params=unique_ptr<BaseAST>($4);
    ast->block = unique_ptr<BaseAST>($6);
    $$ = ast;
  }
  ;

FuncFParams: __FuncFParams__{printf("    Reduce:FuncFParams\n");};

__FuncFParams__
  : FuncFParam FuncFParamSeq
  {
    temp_stack_func_f_params.push(dynamic_cast<FuncFParamAST*>($1));
    auto ast=new FuncFParamsAST();
    while(temp_stack_func_f_params.empty()==false)
    {
      ast->func_f_params.push_back(temp_stack_func_f_params.top());
      temp_stack_func_f_params.pop();
    }
    $$=ast;
  }
  ;

FuncFParamSeq: __FuncFParamSeq__{PRINT("    Reduce:FuncFParamSeq\n");};

__FuncFParamSeq__
  : ',' FuncFParam FuncFParamSeq
  {
    temp_stack_func_f_params.push(dynamic_cast<FuncFParamAST*>($2));
  }
  |
  {
  }
  ;

FuncFParam: __FuncFParam__{PRINT("    Reduce:FuncFParam\n");};

__FuncFParam__
  : KW_INT IDENT
  {
    auto ast=new FuncFParamAST(TAG_0);
    ast->func_f_param_union.func_f_param_case_0->b_type=unique_ptr<BaseTypeAST>(new BTypeAST("int"));
    ast->func_f_param_union.func_f_param_case_0->ident=*($2);
    $$=ast;
  }
  | KW_INT IDENT '[' ']' {stack_vec_const_exp_for_array_len.push(vector<BaseExpAST*>());}ConstExpInArray_Prime
  {
    auto ast=new FuncFParamAST(TAG_1);
    ast->func_f_param_union.func_f_param_case_1->b_type=unique_ptr<BaseTypeAST>(new BTypeAST("int"));
    ast->func_f_param_union.func_f_param_case_1->ident=*($2);
    for(auto iter=stack_vec_const_exp_for_array_len.top().begin();iter!=stack_vec_const_exp_for_array_len.top().end();++iter)
    {
        ast->func_f_param_union.func_f_param_case_1->vec_const_exp.push_back(unique_ptr<BaseExpAST>(*iter));
    }
    if(stack_vec_const_exp_for_array_len.empty())
        assert(false);
    stack_vec_const_exp_for_array_len.top().clear();
    stack_vec_const_exp_for_array_len.pop();
    $$=ast;
  }
  ;


Block: __Block__{PRINT("    Reduce:Block\n");};

__Block__
  : {temp_stack_vec_block_item.push(vector<BaseAST*>());}'{' BlockItemSeq '}' 
  {
    auto ast=new BlockAST();
    for(auto iter=temp_stack_vec_block_item.top().begin();iter!=temp_stack_vec_block_item.top().end();++iter)
    {
      unique_ptr<BaseAST> temp=unique_ptr<BaseAST>(*iter);
      (ast->vec_block_item).push_back(std::move(temp));
    }
    if(temp_stack_vec_block_item.empty())
        assert(false);
    temp_stack_vec_block_item.pop();
    $$ = ast;
  }
  ;

BlockItemSeq: __BlockItemSeq__{PRINT("    Reduce:BlockItemSeq\n");};

__BlockItemSeq__
  : BlockItem {temp_stack_vec_block_item.top().push_back($1);} BlockItemSeq
  {
  }
  | /*empty*/
  {
  }
  ;

Stmt: __Stmt__{PRINT("    Reduce:Stmt\n");};

__Stmt__
  : MatchedStmt
  {
    $$=($1);
  }
  | OpenStmt
  {
    $$=($1);
  }
  ;

MatchedStmt: __MatchedStmt__{PRINT("    Reduce:MatchedStmt\n");};

__MatchedStmt__
  :  LVal '=' Exp ';'
  {
    auto ast=new StmtAST(TAG_0);
    ast->stmt_union.stmt_case_0->l_val=unique_ptr<BaseAST>($1);
    ast->stmt_union.stmt_case_0->exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  } 
  | ';'
  {
    auto ast=new StmtAST(TAG_1);
    $$=ast;
  }
  | Exp ';'
  {
    auto ast=new StmtAST(TAG_2);
    ast->stmt_union.stmt_case_2->exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | Block
  {
    auto ast=new StmtAST(TAG_3);
    ast->stmt_union.stmt_case_3->block=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  | KW_IF '(' Exp ')' MatchedStmt KW_ELSE MatchedStmt
  {
    auto ast=new StmtAST(TAG_5);
    ast->stmt_union.stmt_case_5->exp=unique_ptr<BaseExpAST>($3);
    ast->stmt_union.stmt_case_5->stmt_if=unique_ptr<BaseAST>($5);
    ast->stmt_union.stmt_case_5->stmt_else=unique_ptr<BaseAST>($7);
    $$=ast; 
  }
  | WHILE  '(' Exp ')' MatchedStmt
  {
    auto ast=new StmtAST(TAG_6);
    ast->stmt_union.stmt_case_6->exp=unique_ptr<BaseExpAST>($3);
    ast->stmt_union.stmt_case_6->stmt=unique_ptr<BaseAST>($5);
    while(temp_stack_stack_stmt_need_to_add_last_while_addr.top().empty()==false)
    {
        auto temp=dynamic_cast<StmtAST*>(temp_stack_stack_stmt_need_to_add_last_while_addr.top().top());
        temp_stack_stack_stmt_need_to_add_last_while_addr.top().pop();
        if(temp->tag==7)
          temp->stmt_union.stmt_case_7->last_while_ins=ast;
        else if(temp->tag==8)
          temp->stmt_union.stmt_case_8->last_while_ins=ast;
        else
           assert(false);
    }
    temp_stack_stack_stmt_need_to_add_last_while_addr.pop();
    $$=ast;
  }
  | KW_BREAK ';'
  {
    auto ast=new StmtAST(TAG_7);
    temp_stack_stack_stmt_need_to_add_last_while_addr.top().push(ast);
    $$=ast;
  }
  | KW_CONTINUE ';'
  {
    auto ast=new StmtAST(TAG_8);
    ast->stmt_union.stmt_case_8->last_while_ins=cur_while;
    temp_stack_stack_stmt_need_to_add_last_while_addr.top().push(ast);
    $$=ast;
  }
  | KW_RETURN Exp ';' 
  {
    auto ast=new StmtAST(TAG_9);
    ast->stmt_union.stmt_case_9->exp=unique_ptr<BaseExpAST>($2);
    $$ = ast;
  }
  | KW_RETURN ';'
  {
     auto ast=new StmtAST(TAG_10);
     $$ = ast; 
  }
  ;

OpenStmt: __OpenStmt__{PRINT("    Reduce:OpenStmt\n");};

__OpenStmt__
  : KW_IF '(' Exp ')' Stmt
  {
    auto ast=new StmtAST(TAG_4);
    ast->stmt_union.stmt_case_4=new StmtCase4();
    ast->stmt_union.stmt_case_4->exp=unique_ptr<BaseExpAST>($3);
    ast->stmt_union.stmt_case_4->stmt=unique_ptr<BaseAST>($5);
    ast->tag=4;
    $$=ast;
  }
  | KW_IF '(' Exp ')' MatchedStmt KW_ELSE OpenStmt
  {
    auto ast=new StmtAST(TAG_5);
    ast->stmt_union.stmt_case_5->exp=unique_ptr<BaseExpAST>($3);
    ast->stmt_union.stmt_case_5->stmt_if=unique_ptr<BaseAST>($5);
    ast->stmt_union.stmt_case_5->stmt_else=unique_ptr<BaseAST>($7);
    $$=ast; 
  }
  | WHILE  '(' Exp ')' OpenStmt
  {
    auto ast=new StmtAST(TAG_6);
    ast->stmt_union.stmt_case_6->exp=unique_ptr<BaseExpAST>($3);
    ast->stmt_union.stmt_case_6->stmt=unique_ptr<BaseAST>($5);
    while(temp_stack_stack_stmt_need_to_add_last_while_addr.top().empty()==false)
    {
        auto temp=dynamic_cast<StmtAST*>(temp_stack_stack_stmt_need_to_add_last_while_addr.top().top());
        if(temp_stack_stack_stmt_need_to_add_last_while_addr.top().empty())
            assert("error:stack is empty,cannot pop\n");
        temp_stack_stack_stmt_need_to_add_last_while_addr.top().pop();
        if(temp->tag==7)
          temp->stmt_union.stmt_case_7->last_while_ins=ast;
        else if(temp->tag==8)
          temp->stmt_union.stmt_case_8->last_while_ins=ast;
        else
           assert(false);
    }
    if(temp_stack_stack_stmt_need_to_add_last_while_addr.empty())
         assert("error:stack is empty,cannot pop\n");
    temp_stack_stack_stmt_need_to_add_last_while_addr.pop();
    $$=ast;
  }
  ;

WHILE: __WHILE__{PRINT("    Reduce:WHILE\n");};

__WHILE__
  : KW_WHILE
  {
    temp_stack_stack_stmt_need_to_add_last_while_addr.push(stack<BaseAST*>());
  }
  ;

Exp: __Exp__{PRINT("    Reduce:Exp\n");};

__Exp__
  : LOrExp
  {
    auto ast=new ExpAST();
    ast->l_or_exp=unique_ptr<BaseExpAST>($1);
    $$ = ast;
  }
  ;

UnaryExp: __UnaryExp__{PRINT("    Reduce:UnaryExp\n");};

__UnaryExp__
  : PrimaryExp
  {
    auto ast=new UnaryExpAST(TAG_0);
    ast->unary_exp_union.unary_exp_case_0->primary_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  |  IDENT '(' ')'
  {
    auto ast=new UnaryExpAST(TAG_1);
    ast->unary_exp_union.unary_exp_case_1->ident=*($1);
    ast->unary_exp_union.unary_exp_case_1->func_r_params=unique_ptr<BaseAST>(new FuncRParamsAST());
    $$=ast;
  }
  |  IDENT '(' FuncRParams ')'
  {
    auto ast=new UnaryExpAST(TAG_1);
    ast->unary_exp_union.unary_exp_case_1->ident=*($1);
    ast->unary_exp_union.unary_exp_case_1->func_r_params=unique_ptr<BaseAST>($3);
    $$=ast;
  }
  | UnaryOp UnaryExp
  {
    auto ast=new UnaryExpAST(TAG_2);
    ast->unary_exp_union.unary_exp_case_2->unary_op=unique_ptr<BaseOpAST>($1);
    ast->unary_exp_union.unary_exp_case_2->unary_exp=unique_ptr<BaseExpAST>($2);
    $$=ast;
  }
  ;

FuncRParams: __FuncRParams__{PRINT("    Reduce:FuncRParams\n");};

__FuncRParams__
  : Exp FuncRParamSeq
  {
    temp_stack_func_r_params.push(dynamic_cast<ExpAST*>($1));
    auto ast=new FuncRParamsAST();
    while(temp_stack_func_r_params.empty()==false)
    {
      ast->func_r_params.push_back(temp_stack_func_r_params.top());
      if(temp_stack_func_r_params.empty())
          assert("error:stack is empty,cannot pop\n");
      temp_stack_func_r_params.pop();
    }
    $$=ast;
  }
  ;

FuncRParamSeq: __FuncRParamSeq__{PRINT("    Reduce:FuncRParamSeq\n");};

__FuncRParamSeq__
  : ',' Exp FuncRParamSeq
  {
    temp_stack_func_r_params.push(dynamic_cast<ExpAST*>($2));
  }
  |
  {
  }
  ;

PrimaryExp: __PrimaryExp__{PRINT("    Reduce:PrimaryExp\n");};

__PrimaryExp__
  : '(' Exp ')'
  {
    auto ast=new PrimaryExpAST(TAG_0);
    ast->primary_exp_union.primary_exp_case_0->exp=unique_ptr<BaseExpAST>($2);
    $$=ast;
  }
  |LVal
  {
    auto ast=new PrimaryExpAST(TAG_1);
    ast->primary_exp_union.primary_exp_case_1->l_val=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  | Number
  {
    auto ast=new PrimaryExpAST(TAG_2);
    ast->primary_exp_union.primary_exp_case_2->number=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }

Number: __Number__{PRINT("    Reduce:Number\n");};

__Number__
  : INT_CONST {
    auto ast=new NumberAST();
    ast->num=string(to_string($1));
    $$ = ast;
  }
  ;

UnaryOp: __UnaryOp__{PRINT("    Reduce:UnaryOp\n");};

__UnaryOp__
  : UNARY_OP
  {
    auto ast=new UnaryOpAST();
    ast->op=($1);
    $$=ast;
  }
  | ADD_OP
  {
    auto ast=new UnaryOpAST();
    ast->op=($1);
    $$=ast;
  }
  ;

MulOp: __MulOp__{PRINT("    Reduce:MulOp\n");};

__MulOp__
  : MUL_OP
  {
    auto ast=new MulOpAST();
    ast->op=($1);
    $$=ast;
  }
  ;

AddOp: __AddOp__{PRINT("    Reduce:AddOp\n");};

__AddOp__
  : ADD_OP
  {
    auto ast=new AddOpAST();
    ast->op=($1);
    $$=ast;
  }
  ;

MulExp: __MulExp__{PRINT("    Reduce:MulExp\n");};

__MulExp__
  : UnaryExp
  {
    auto ast=new MulExpAST(TAG_0);
    ast->mul_exp_union.mul_exp_case_0->unary_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | MulExp MulOp UnaryExp
  {
    auto ast=new MulExpAST(TAG_1);
    ast->mul_exp_union.mul_exp_case_1->mul_exp=unique_ptr<BaseExpAST>($1);
    ast->mul_exp_union.mul_exp_case_1->mul_op=unique_ptr<BaseOpAST>($2);
    ast->mul_exp_union.mul_exp_case_1->unary_exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  }
  ;

AddExp: __AddExp__{PRINT("    Reduce:AddExp\n");};

__AddExp__
  : MulExp
  {
    auto ast=new AddExpAST(TAG_0);
    ast->add_exp_union.add_exp_case_0->mul_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | AddExp AddOp MulExp
  {
    auto ast=new AddExpAST(TAG_1);
    ast->add_exp_union.add_exp_case_1->add_exp=unique_ptr<BaseExpAST>($1);
    ast->add_exp_union.add_exp_case_1->add_op=unique_ptr<BaseOpAST>($2);
    ast->add_exp_union.add_exp_case_1->mul_exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  }
  ;

RelOp: __RelOp__{PRINT("    Reduce:RelOp\n");};

__RelOp__
  : ONE_LEN_REL_OP
  {
    auto ast=new RelOpAST();
    ast->op[0]=($1);
    ast->op[1]='\0';
    ast->op[2]='\0';
    $$=ast;
  }
  | TWO_LEN_REL_OP
  {
    auto ast=new RelOpAST();
    ast->op[0]=($1)[0];
    ast->op[1]=($1)[1];
    ast->op[2]='\0';
    $$=ast;
  }
  ;

LAndOp: __LAndOp__{PRINT("    Reduce:LAndOp\n");};

__LAndOp__
  : LAND_OP
  {
    auto ast=new LAndOpAST();
    ast->op[0]=($1)[0];
    ast->op[1]=($1)[1];
    ast->op[2]='\0';
    $$=ast;
  }
  ;

LOrOp: __LOrOp__{PRINT("    Reduce:LOrOp\n");};

__LOrOp__
  : LOR_OP
  {
    auto ast=new LOrOpAST();
    ast->op[0]=($1)[0];
    ast->op[1]=($1)[1];
    ast->op[2]='\0';
    $$=ast;
  }
  ;

EqOp: __EqOp__{PRINT("    Reduce:EqOp\n");};

__EqOp__
  : EQ_OP
  {
    auto ast=new EqOpAST();
    ast->op[0]=($1)[0];
    ast->op[1]=($1)[1];
    ast->op[2]='\0';
    $$=ast;
  }
  ;

RelExp: __RelExp__{PRINT("    Reduce:RelExp\n");};

__RelExp__
  : AddExp
  {
    auto ast=new RelExpAST(TAG_0);
    ast->rel_exp_union.rel_exp_case_0->add_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | RelExp RelOp AddExp
  {
    auto ast=new RelExpAST(TAG_1);
    ast->rel_exp_union.rel_exp_case_1->rel_exp=unique_ptr<BaseExpAST>($1);
    ast->rel_exp_union.rel_exp_case_1->rel_op=unique_ptr<BaseOpAST>($2);
    ast->rel_exp_union.rel_exp_case_1->add_exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  }
  ;

EqExp: __EqExp__{PRINT("    Reduce:EqExp\n");};

__EqExp__
  : RelExp
  {
    auto ast=new EqExpAST(TAG_0);
    ast->eq_exp_union.eq_exp_case_0->rel_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | EqExp EqOp RelExp
  {
    auto ast=new EqExpAST(TAG_1);
    ast->eq_exp_union.eq_exp_case_1->eq_exp=unique_ptr<BaseExpAST>($1);
    ast->eq_exp_union.eq_exp_case_1->eq_op=unique_ptr<BaseOpAST>($2);
    ast->eq_exp_union.eq_exp_case_1->rel_exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  }
  ;

LAndExp: __LAndExp__{PRINT("    Reduce:LAndExp\n");};

__LAndExp__
  : EqExp
  {
    auto ast=new LAndExpAST(TAG_0);
    ast->l_and_exp_union.l_and_exp_case_0->eq_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | LAndExp LAndOp EqExp
  {
    auto ast=new LAndExpAST(TAG_1);
    ast->l_and_exp_union.l_and_exp_case_1->l_and_exp=unique_ptr<BaseExpAST>($1);
    ast->l_and_exp_union.l_and_exp_case_1->l_and_op=unique_ptr<BaseOpAST>($2);
    ast->l_and_exp_union.l_and_exp_case_1->eq_exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  }
  ;

LOrExp: __LOrExp__{PRINT("    Reduce:LOrExp\n");};

__LOrExp__
  : LAndExp
  {
    auto ast=new LOrExpAST(TAG_0);
    ast->l_or_exp_union.l_or_exp_case_0->l_and_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | LOrExp LOrOp LAndExp
  {
    auto ast=new LOrExpAST(TAG_1);
    ast->l_or_exp_union.l_or_exp_case_1->l_or_exp=unique_ptr<BaseExpAST>($1);
    ast->l_or_exp_union.l_or_exp_case_1->l_or_op=unique_ptr<BaseOpAST>($2);
    ast->l_or_exp_union.l_or_exp_case_1->l_and_exp=unique_ptr<BaseExpAST>($3);
    $$=ast;
  }
  ;
  
BlockItem: __BlockItem__{PRINT("    Reduce:BlockItem\n");};

__BlockItem__
  : Decl
  {
    auto ast=new BlockItemAST();
    ast->decl=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  | Stmt
  {
    auto ast=new BlockItemAST();
    ast->tag=1;
    ast->stmt=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  ;

Decl: __Decl__{PRINT("    Reduce:Decl\n");};

__Decl__
  :  ConstDecl
  {
    auto ast=new DeclAST(TAG_0);
    ast->decl_union.decl_case_0->const_decl=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  |  VarDecl
  {
    auto ast=new DeclAST(TAG_1);
    ast->decl_union.decl_case_1->var_decl=unique_ptr<BaseAST>($1);
    $$=ast;
  }
  ;

ConstDecl: __ConstDecl__{PRINT("    Reduce:ConstDecl\n");};

__ConstDecl__
  :  KW_CONST KW_INT ConstDef {temp_vec_const_def.push_back($3);} ConstDefSeq';'
  {
    auto ast=new ConstDeclAST();
    ast->b_type=unique_ptr<BaseTypeAST>(new BTypeAST("int"));
    for(auto iter=temp_vec_const_def.begin();iter!=temp_vec_const_def.end();++iter)
    {
            unique_ptr<BaseAST> temp=unique_ptr<BaseAST>(*iter);
            (ast->vec_const_def).push_back(std::move(temp));
    }
    temp_vec_const_def.clear();
    $$=ast;
  }
  ; 

ConstDefSeq: __ConstDefSeq__{PRINT("    Reduce:ConstDefSeq\n");};

__ConstDefSeq__
  : ',' ConstDef {temp_vec_const_def.push_back($2);} ConstDefSeq
  | /*empty*/
  {
  }
  ;


ConstDef: __ConstDef__{PRINT("    Reduce:ConstDef\n");};

__ConstDef__
  : IDENT '=' ConstInitVal
  {
    auto ast=new ConstDefAST(TAG_0);
    ast->const_def_union.const_def_case_0->ident=*($1);
    ast->const_def_union.const_def_case_0->const_init_val=unique_ptr<BaseAST>($3);
    $$=ast;
  }
  | IDENT ConstExpInArray '=' ConstInitVal
  {
    auto ast=new ConstDefAST(TAG_1);
    ast->const_def_union.const_def_case_1->ident=*($1);
    ast->size=1;
    for(auto iter=stack_vec_const_exp_for_array_len.top().begin();iter!=stack_vec_const_exp_for_array_len.top().end();++iter)
        ast->const_def_union.const_def_case_1->vec_const_exp.push_back(unique_ptr<BaseExpAST>(*iter));
    stack_vec_const_exp_for_array_len.top().clear();
    if(stack_vec_const_exp_for_array_len.empty())
        assert("error:vector is empty,cannot pop\n");
    stack_vec_const_exp_for_array_len.pop();
    ast->const_def_union.const_def_case_1->const_init_val=unique_ptr<BaseAST>($4);
    $$=ast;
  } 
  ;



ConstInitVal: __ConstInitVal__{PRINT("    Reduce:ConstInitVal\n");};

__ConstInitVal__
  : ConstExp
  {
    auto ast=new ConstInitValAST(TAG_0);
    ast->const_init_val_union.const_init_val_case_0->const_exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | '{'  '}'
  {
    auto ast=new ConstInitValAST(TAG_1);
    $$=ast;
  }
  | '{' ConstInitVal { stack_vec_const_init_val.push(vector<BaseAST*>());stack_vec_const_init_val.top().push_back($2);} ConstInitValSeq '}'
  {
    auto ast=new ConstInitValAST(TAG_1);
    auto iter=stack_vec_const_init_val.top().begin();
    while(iter!=stack_vec_const_init_val.top().end())
    {
      ast->const_init_val_union.const_init_val_case_1->vec_const_init_val.push_back(unique_ptr<BaseAST>(*iter));
      ++iter;
    }
    stack_vec_const_init_val.top().clear();
    if(stack_vec_const_init_val.empty())
        assert("error:stack is empty,cannot pop\n");
    stack_vec_const_init_val.pop();
    $$=ast;
  }
  ;

ConstInitValSeq
  : ',' ConstInitVal { stack_vec_const_init_val.top().push_back($2);} ConstInitValSeq
  {
  }
  |
  {
  }
  ;


ConstExp: __ConstExp__{PRINT("    Reduce:ConstExp\n");};

__ConstExp__
  : Exp
  {
    auto ast=new ConstExpAST();
    ast->exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  ;

LVal: __LVal__{PRINT("    Reduce:LVal\n");};

__LVal__
  : IDENT
  {
    auto ast=new LValAST(TAG_0);
    ast->l_val_union.l_val_case_0->ident=*($1);
    $$=ast;
  }
  | IDENT ExpInArray
  {
    auto ast=new LValAST(TAG_1);
    ast->l_val_union.l_val_case_1->ident=*($1);
    for(auto iter=stack_vec_exp_for_array_len.top().begin();iter!=stack_vec_exp_for_array_len.top().end();++iter)
    {
      ast->l_val_union.l_val_case_1->vec_exp.push_back(unique_ptr<BaseExpAST>(*iter));      
    }
    stack_vec_exp_for_array_len.top().clear();
    if(stack_vec_exp_for_array_len.empty())
        assert("error:stack is empty,cannot pop\n");
    stack_vec_exp_for_array_len.pop();
    $$=ast;
  }
  ;

ExpInArray
  : '[' Exp ']'  { stack_vec_exp_for_array_len.push(vector<BaseExpAST*>()); stack_vec_exp_for_array_len.top().push_back($2);} ExpInArray_Prime
  {
  }
  ;

ExpInArray_Prime
  : '[' Exp ']'  { stack_vec_exp_for_array_len.top().push_back($2);} ExpInArray_Prime
  {
  }
  |
  {
  }
  ;


VarDecl: __VarDecl__{PRINT("    Reduce:VarDecl\n");};

__VarDecl__
  :  KW_INT VarDef {temp_vec_var_def.push_back($2);} VarDefSeq';'
  {
    auto ast=new VarDeclAST();
    ast->b_type=unique_ptr<BaseTypeAST>(new BTypeAST("type"));
    for(auto iter=temp_vec_var_def.begin();iter!=temp_vec_var_def.end();++iter)
    {
            unique_ptr<BaseAST> temp=unique_ptr<BaseAST>(*iter);
            (ast->vec_var_def).push_back(std::move(temp));
    }
    temp_vec_var_def.clear();
    $$=ast;
  }
  ; 

VarDefSeq: __VarDefSeq__{PRINT("    Reduce:VarDefSeq\n");};

__VarDefSeq__
  : ',' VarDef {temp_vec_var_def.push_back($2);} VarDefSeq
  | /*empty*/
  {
  }
  ;

//Continue From There
ConstExpInArray
  : '[' ConstExp ']'  { stack_vec_const_exp_for_array_len.push(vector<BaseExpAST*>());stack_vec_const_exp_for_array_len.top().push_back($2);} ConstExpInArray_Prime
  {
  }
  ;

ConstExpInArray_Prime
  : '[' ConstExp ']'  { stack_vec_const_exp_for_array_len.top().push_back($2);} ConstExpInArray_Prime
  {
  }
  |
  {
  }
  ;




VarDef: __VarDef__{PRINT("    Reduce:VarDef\n");};

__VarDef__
  : IDENT
  {
    auto ast=new VarDefAST(TAG_0);
    ast->var_def_union.var_def_case_0->ident=*($1);
    $$=ast;
  }
  |  IDENT ConstExpInArray
  {
    auto ast=new VarDefAST(TAG_1);
    ast->var_def_union.var_def_case_1->ident=*($1);
    for(auto iter=stack_vec_const_exp_for_array_len.top().begin();iter!=stack_vec_const_exp_for_array_len.top().end();++iter)
    {
      ast->var_def_union.var_def_case_1->vec_const_exp.push_back(unique_ptr<BaseExpAST>(*iter));
    }
    stack_vec_const_exp_for_array_len.top().clear();
    if(stack_vec_const_exp_for_array_len.empty())
        assert("error:stack is empty,cannot pop\n");
    stack_vec_const_exp_for_array_len.pop();
    ast->var_def_union.var_def_case_1->init_val=unique_ptr<BaseAST>(new InitValAST(TAG_1));
    $$=ast;
  }
  |  IDENT '=' InitVal
  {
    auto ast=new VarDefAST(TAG_2);
    ast->var_def_union.var_def_case_2->ident=*($1);
    ast->var_def_union.var_def_case_2->init_val=unique_ptr<BaseAST>($3);
    $$=ast;
  }
  |  IDENT ConstExpInArray '=' InitVal
  {
    auto ast=new VarDefAST(TAG_1);
    ast->var_def_union.var_def_case_1->ident=*($1);
    for(auto iter=stack_vec_const_exp_for_array_len.top().begin();iter!=stack_vec_const_exp_for_array_len.top().end();++iter)
    {
      ast->var_def_union.var_def_case_1->vec_const_exp.push_back(unique_ptr<BaseExpAST>(*iter)); 
    }
    stack_vec_const_exp_for_array_len.top().clear();
    if(stack_vec_const_exp_for_array_len.empty())
        assert("stack is empty,cannot pop\n");
    stack_vec_const_exp_for_array_len.pop();
    ast->var_def_union.var_def_case_1->init_val=unique_ptr<BaseAST>($4);
    $$=ast;
  }
  ;

InitVal: __InitVal__{PRINT("    Reduce:InitVal\n");};

__InitVal__
  : Exp
  {
    auto ast=new InitValAST(TAG_0);
    ast->init_val_union.init_val_case_0->exp=unique_ptr<BaseExpAST>($1);
    $$=ast;
  }
  | '{'  '}'
  {
    auto ast=new InitValAST(TAG_1);
    $$=ast;
  }
  | '{' InitVal { stack_vec_init_val.push(vector<BaseAST*>());stack_vec_init_val.top().push_back($2);} InitValSeq '}'
  {
    auto ast=new InitValAST(TAG_1);
    auto iter=stack_vec_init_val.top().begin();
    while(iter!=stack_vec_init_val.top().end())
    {
      ast->init_val_union.init_val_case_1->vec_init_val.push_back(unique_ptr<BaseAST>(*iter));
      ++iter;
    }
    stack_vec_init_val.top().clear();
    if(stack_vec_init_val.empty())
        assert("error:stack is empty,cannot pop\n");
    stack_vec_init_val.pop();
    $$=ast;
  }
  ;

InitValSeq
  : ',' InitVal { stack_vec_init_val.top().push_back($2);} InitValSeq
  {
  }
  |
  {
  }
  ;

%%


void yyerror(unique_ptr<class BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
