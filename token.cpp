//  Token Implementation file
//  This stores some of the more complicated features of the token object,
//  that would not be compiled in-line.

#include "token.h"
#include <string>

ostream& operator<<( ostream &stream, Token &t)
{
    if (t.isNull())
	return stream << "(null)";
    if (t.isInteger())
	return stream <<  t.value ;
    else return stream <<  t.str ;
}
