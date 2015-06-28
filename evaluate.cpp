// CS122_Homework_V
// Yuehui Wang			      Fall 2014

// Simple Expression Evaluation 
// This program will evaluate simple arithmetic expressions
// represented as a linked-list of tokens.  Keyboard input
// will be accepted into a string, which will be converted
// into that linked list.
//
// If the first symbol in the input string is an operator,
// then the value of the previous expression will be taken
// as an implicit first operand.
//
// The expressions may consist of the following:
// -- integer values (which may have multiple digits)
// -- simple arithmetic operators ( +, -, *, /, % )
// -- matched parentheses for grouping

// This implementation consists of a set of mutually-recursive 
// functions. which will track the structure of the expression.
//
// A sum expression is the sum or difference of one or more products.
// A product expression is the product or quotient of one or more factors.
// A factor may be a number or a parenthesized sum expression.

#include <iostream>
using namespace std;
#include "tokenlist.h"
#include "evaluate.h"

static int preAns = 0;   
void evaluateEqul(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);
void evaluateCond(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);
void evaluateComp(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);
void evaluateSum(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);                                 //convert the infix expression list into postfix expression list
void evaluateProduct(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);							 //using recursursion method accompanied by product and factor
void evaluateFun(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);
void evaluateFactor(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs);

int evaluate(const char expr[], VarTree &vars, FunctionDef &funs)
{
	TokenList IFX_expr(expr);

	ListIterator IFX_iter = IFX_expr.begin();
	ExprNode* exprTree;

	if (IFX_iter.token().isInteger()==false && IFX_iter.token().isVarStr()==false && IFX_iter.token().strName()!="(")            //for continued calculation
	{
		Token preAnsToke(preAns);
		IFX_expr.push_front(preAnsToke);
		IFX_iter = IFX_expr.begin();
	}
	
	if (IFX_iter.token().strName() == "deffn")
	{
		FunDef NewFun;
		NewFun.locals = new VarTree();                        //deffn sqr(x) = x*x
		IFX_iter.advance();
		NewFun.name = IFX_iter.token().strName();
		IFX_iter.advance();
		IFX_iter.advance();                                                                   // to pamameter
		for (int i = 0; IFX_iter.token().strName()!= ")"; i++)
		{
			NewFun.parameter[i] = IFX_iter.token().strName();
			IFX_iter.advance();
			if (IFX_iter.token().strName() == ",")
				IFX_iter.advance();
			NewFun.locals->assign(NewFun.parameter[i], 0);                        //assign the variabls to be 0 for now
		}
		IFX_iter.advance();
		IFX_iter.advance();
		evaluateEqul(IFX_iter,NewFun.functionBody,funs);                               //
		funs.insert({ NewFun.name, NewFun });                                          //
		return 0;
	}  
	else
	{
		evaluateEqul(IFX_iter, exprTree, funs);
		cout << "The infix expression is: ";
		IFX_expr.print();

		cout << "The tree in parentheses: " << exprTree->toString() << endl;
		preAns = exprTree->evaluate(vars);                                                     //evaluate vars will return the answer recursively from exprtree.cpp
		return preAns;
	}
	
}

void evaluateEqul(ListIterator &IFX_pos, ExprNode *&exprTree, FunctionDef &funs)
{
	ExprNode *rightSub = exprTree;
	Token oper;
	
	evaluateCond(IFX_pos, exprTree, funs);

	if (IFX_pos.ended())
		oper = 0;
	else
		oper = IFX_pos.token().strName();

	while (oper.strName() == "=")
	{
		IFX_pos.advance();
		evaluateCond(IFX_pos, rightSub, funs);
		exprTree = new Operation(exprTree, oper.strName(), rightSub);
		if (IFX_pos.ended())
			oper = 0;
		else
			oper = IFX_pos.token().strName();
	}
}

void evaluateCond(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs)
{
	ExprNode *trueCase;
	ExprNode *falseCase = NULL;
	Token oper;
	evaluateComp(IFX_pos, exprTree, funs);

	if (IFX_pos.ended())
		oper = 0;
	else
		oper = IFX_pos.token().strName();

	while (oper.strName() == "?")
	{
		IFX_pos.advance();
		evaluateComp(IFX_pos, trueCase, funs);

		if (IFX_pos.token().strName() == ":")
		{
			IFX_pos.advance();
			evaluateComp(IFX_pos, falseCase, funs);
		}
		exprTree = new Conditional(exprTree, trueCase, falseCase);
		if (IFX_pos.ended())
			oper = 0;
		else
			oper = IFX_pos.token().strName();
	}
}

void evaluateComp(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs)
{
	ExprNode *rightsub = exprTree;
	Token oper;
	evaluateSum(IFX_pos, exprTree, funs);

	if (IFX_pos.ended())
		oper = 0;
	else
		oper = IFX_pos.token().strName();

	while (oper.strName() == ">" || oper.strName() == "<" || oper.strName() == ">=" ||
		oper.strName() == "<=" || oper.strName() == "==" || oper.strName() == "!=")
	{
		IFX_pos.advance();
		evaluateSum(IFX_pos, rightsub, funs);
		exprTree = new Operation(exprTree, oper.strName(), rightsub);
		if (IFX_pos.ended())
			oper = 0;
		else
			oper = IFX_pos.token().strName();
	}
}

void evaluateSum(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs)
{
	ExprNode *rightSub = exprTree;
	Token oper;

	if (IFX_pos.token().strName() == "-")				                     /////   if token is "-" sign
	{
		IFX_pos.advance();
		ExprNode *leftSub, *rightSub;
		evaluateProduct(IFX_pos, rightSub, funs);
		leftSub = new Value(0);
		exprTree = new Operation(leftSub,"-", rightSub);
	}
	else 
		evaluateProduct(IFX_pos, exprTree, funs);  


	if ( IFX_pos.ended())
		oper = 0;
	else
		oper = IFX_pos.token().strName();

	while (oper.strName() == "+" || oper.strName() == "-")
	{
		IFX_pos.advance();
		evaluateProduct(IFX_pos, rightSub, funs);
		exprTree = new Operation(exprTree, oper.strName(), rightSub);
		if (IFX_pos.ended())
			oper = 0;
		else
			oper = IFX_pos.token().strName();
	}
}

void evaluateProduct(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs)
{
	ExprNode *rightSub = exprTree;

	evaluateFactor(IFX_pos, exprTree, funs);
	IFX_pos.advance();
	Token oper;

	if (IFX_pos.ended())
		oper = 0;
	else
		oper = IFX_pos.token().strName();

	while (oper.strName() == "*" || oper.strName() == "/" || oper.strName() == "%")
	{
		IFX_pos.advance();
		evaluateFactor(IFX_pos, rightSub, funs);
		exprTree = new Operation (exprTree, oper.strName(), rightSub);
		IFX_pos.advance();
		if (IFX_pos.ended())
			oper = 0;
		else
			oper = IFX_pos.token().strName();
	}
}

void evaluateFactor(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs)
{

	if (IFX_pos.token().strName() == "(")
	{
		IFX_pos.advance();
		evaluateEqul(IFX_pos, exprTree, funs);
	}
	else
	{
		if (IFX_pos.token().isInteger())
			exprTree = new Value(IFX_pos.token().integerValue());
		else
		{
			ListIterator JustForFuntion = IFX_pos;
			JustForFuntion.advance();
			if (JustForFuntion.ended() == false && JustForFuntion.token().strName() == "(")
				evaluateFun(IFX_pos,exprTree, funs);
			else
				exprTree = new Variable(IFX_pos.token().strName());
		}
	}	
}

void evaluateFun(ListIterator &IFX_pos, ExprNode* &exprTree, FunctionDef &funs)
{
	string funName = IFX_pos.token().strName();
	ExprNode *prmtTree[10] = { NULL };
	IFX_pos.advance();
	IFX_pos.advance();
	for (int i = 0; IFX_pos.token().strName() != ")"; i++)
	{
		if (IFX_pos.token().strName() == ",")
			IFX_pos.advance();
		evaluateEqul(IFX_pos, prmtTree[i], funs);
	}
	exprTree = new Functional(funName, prmtTree, &funs);
}