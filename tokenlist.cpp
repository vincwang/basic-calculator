// CS122_Homework_IV
// Yuehui Wang			      Fall 2014


// Token List Implementation file
//
// This tokenizer will scan a character string representing
// an expression, and will return a list of tokens.
// --- only integers and operators are supported (no names)

// The standard C library has some useful functions for us
#include <string.h>
#include <ctype.h>
#include <string>

// And to get the definition of a token:
#include "tokenlist.h"

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
    ListElement *curr;
    for (curr = t.head; curr != NULL; curr = curr->next)
    {
	stream << " " << curr->token;
    }
    return stream;
}

void TokenList::push_back(Token t)                                             //push a node back to a list  
{
	ListElement* newNode = new ListElement();
	newNode->token = t;
	newNode->next = NULL;
	if (head == NULL)
	{
		head = newNode;
		tail = newNode;
	}
	else
	{
		tail->next = newNode;
		tail = tail->next;
	}
}

void TokenList::push_front(Token t)												 //push a node in front of a list (when using 2nd calculation )
{
	ListElement* newNode = new ListElement();
	newNode->token = t;
	newNode->next = head;
	head = newNode;
}


TokenList::TokenList(const char expr[])                                          //make arry into list
{
	int pos = 0;
	while (expr[pos] != '\0')
	{
		while (expr[pos] == ' ')
				pos = pos + 1;

		if (isalpha(expr[pos]))
		{
			string VarName;
			while (isalpha(expr[pos]))
			{
				VarName = VarName + expr[pos];
				pos++;
			}
			Token VarStr(VarName);
			push_back(VarStr);
		}
		else if (isdigit(expr[pos]))
		{
			int tempValue = 0;
			while (isdigit(expr[pos]))
			{
				tempValue = tempValue * 10 + (int)(expr[pos]-'0');
				pos = pos + 1;
			}
			Token toke(tempValue);
			push_back(toke);
		}
		else
		{
			string oper; string tempcheck;                        //tempcheck for avoiding making )= as a token
			tempcheck = oper = expr[pos];
			pos++;
			if (expr[pos] == '=' && tempcheck != ")")
			{
				oper = oper + expr[pos];
				pos++;
			}
			Token operStr(oper);
			push_back(operStr);
		}
	}
}

//  Creates an iterator to refer to the beginning of the list
ListIterator TokenList::begin()
{
	return ListIterator(this, head);
}

//  Creates an iterator to refer after the end of the list
//  This list is simple:  it ends with NULL
ListIterator TokenList::end()
{
	return ListIterator(this, NULL);
}


void TokenList::print()                                            //dispalying tokenlist
{
	ListIterator l = begin();
	while (l.ended() == false)
	{
		if (l.token().isInteger())
			cout << l.token().integerValue() << " ";
		else
			cout << l.token().strName() << " ";
		l.advance();
	}
	cout << endl;
}