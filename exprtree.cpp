// Expression Tree Implementation File
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and evaluated.
#include <iostream>
#include <sstream>

using namespace std;
#include "exprtree.h"
#include "tokenlist.h"

// Outputting any tree node will simply output its string version
ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

// A Value is just an integer value -- easy to evaluate
// Unfortunately, the string class does not have a constructor for it
string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

int Value::evaluate( VarTree &v) const
{
    return value;
}

//  A variable is just an alphabetic string -- easy to display
//  TO evaluate, would need to look it up in the data structure
string Variable::toString() const
{
    return name;
}

int Variable::evaluate( VarTree &v ) const
{
    return v.lookup( name );
}

string Operation::toString() const
{	
	ostringstream convert;
	convert << "(" << left->toString() << oper << right->toString() << ")";
	return convert.str();
}


int Operation::evaluate( VarTree &v) const
{
	if (oper == "+")
		return left->evaluate(v) + right->evaluate(v);
	else if (oper == "-")
		return left->evaluate(v) - right->evaluate(v);
	else if (oper == "*")
		return left->evaluate(v) * right->evaluate(v);
	else if (oper == "/")
		return left->evaluate(v) / right->evaluate(v);
	else if (oper == "%")
		return left->evaluate(v) % right->evaluate(v);
	else if (oper == "=")
	{
		v.assign(left->toString(), right->evaluate(v));                //assign the value to the variables
		return right->evaluate(v);                                     
	}
	else if (oper == ">")                                              //return c language family values for true and false
		return left->evaluate(v) > right->evaluate(v);
	else if (oper == "<")
		return left->evaluate(v) < right->evaluate(v);
	else if (oper == ">=")
		return left->evaluate(v) >= right->evaluate(v);
	else if (oper == "<=")
		return left->evaluate(v) <= right->evaluate(v);
	else if (oper == "==")
		return left->evaluate(v) == right->evaluate(v);
	else 
		return left->evaluate(v) != right->evaluate(v);
}


string Conditional::toString() const
{
	ostringstream convert;
	convert << "(" << test->toString() << "?" << trueCase->toString() << ":" << falseCase->toString() << ")";
	return convert.str();
}

string Functional::toString() const
{
	string convert = name + "(";
	for (int i = 0; prmtList[i] != NULL; i++)
	{
		if (i != 0)
			convert += ",";
		convert = convert+ prmtList[i]->toString();
	}
	convert += ")";
	return convert;
}

int Conditional::evaluate(VarTree &v) const
{
	if (test->evaluate(v) != 0)
		return trueCase->evaluate(v);
	else
		return falseCase->evaluate(v);
}

int Functional::evaluate (VarTree &v) const
{
	FunDef *function = &funDef->find(name)->second;
	VarTree localVTree;
	for (int i = 0; prmtList[i] != NULL; i++)
	{
		localVTree.assign(function->parameter[i],prmtList[i]->evaluate(v));
	}
	return function->functionBody->evaluate(localVTree);
}