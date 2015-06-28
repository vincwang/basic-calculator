// CS122_Homework_IV
// Yuehui Wang			      Fall 2014

// Token List Header file
// This is a linked list for use with the tokens for an
// arithmetic expression.  Although it is used for that
// particular application, this implementation more
// resembles a generic linked list, which may be used
// either as a stack or a queue.

// There will be support for displaying the entire list at once
#include <iostream>
using namespace std;

// And of course, the tokens themselves
#include "token.h"
class ListIterator;

// Since this is a linked list, we actually need a linked list node.
struct ListElement {
    Token		token;				// the token data itself
    struct ListElement *next;		// next element of list                                          
};

class TokenList  {

    friend ostream& operator<<( ostream &, TokenList &);
    private:
	ListElement *head, 	// front of the list
	            *tail;	// tail of the list
	ListElement *current;	// facilitates traversal of the list
    public:
	TokenList()		// create an empty list
	{
	    head = NULL;
	    tail = NULL;
	    current = NULL;
	}

	TokenList( const char str[] );							// or create initial list
															// to appear in 'tokenlist.c'
															//
	~TokenList()			// destructor -- clear the list
	{
	    ListElement *remove;
	    while ( (remove = head) != NULL)
	    {
		head = head->next;	// find the successor
		delete remove;		// and deallocate this
	    }
	}

	//  A couple functions to add to the list (in 'tokenlist.cpp')
	void push_front( Token t );													 // if input /7,  then push front the prv ans.
	void push_back( Token t );													 //putting tokens in list 
	ListIterator begin();
	ListIterator end();
	//  A couple functions to support list traversal:
	//  They are all very short, using the 'current' member variable
	void start()													        //current to point at head	
	{	
		current = head;
	}

	void advance()															//advance the current	
	{
		current = current->next;
	}

	bool atEnd() const                                                      //check if current is at the end of list, or at a ')'
	{
		if (current==NULL || current->token.strName()==")" )
			return true;
		else
			return false;
	}

	Token nextToken() const													//return the token
	{
		if (current == tail->next)							//if the current is pointing at tail, return NULL to solve memory issue
			return NULL;
		else
			return current->token;
	}

	void pop_front()                                                      //pop from the front of stack 
	{
		ListElement *removal = head;
		if (head->next != NULL)
			current = head = head->next;
		else
			current = head = NULL;
		delete removal;
	}

	void print();

};


class ListIterator
{
	friend class TokenList;	// let that class create iterators
private:
	TokenList *list;	// which list
	ListElement *curr;	// pointer to current node
	ListIterator(TokenList *l, ListElement *le)
	{
		list = l;
		curr = le;
	}
public:
	Token& token() const
	{
		return curr->token;
	}
	bool ended() const
	{
		return (curr == NULL)  ;	// list ends with NULL
	}
	void advance()
	{
		if (curr != NULL)
			curr = curr->next;
	}
	int operator !=(const ListIterator &other) const
	{
		return list != other.list || curr != other.curr;
	}
};


