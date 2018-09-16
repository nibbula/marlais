/*
**  keyword.h
*/

#ifndef KEYWORD_H
#define KEYWORD_H

#include "object.h"

void init_keyword_prims (void);
Object keyword_to_symbol (Object keyword);
Object symbol_to_keyword (Object symbol);
Object find_keyword_val (Object keyword, Object lst);
Object all_keywords (Object lst);

#endif /* KEYWORD_H */
