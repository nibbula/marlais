/*
** yystype.h
*/

#ifndef YYSTYPE_h_
#define YYSTYPE_h_

#include "dylan_lexer.h"
#include "object.h"
#define YYSTYPE Object

int yyparse (void);

#endif /* YYSTYPE_h_ */
