/* 
** character.c
*/

#include "character.h"

#include "number.h"
#include "prim.h"

static Object integer_to_character (Object i);
static Object character_to_integer (Object ch);

static struct primitive char_prims[] =
{
    {"%integer->character", prim_1, integer_to_character},
    {"%character->integer", prim_1, character_to_integer},
};

void
init_char_prims (void)
{
    int num;

    num = sizeof (char_prims) / sizeof (struct primitive);

    init_prims (num, char_prims);
}

#ifndef SMALL_OBJECTS
Object
make_character (char ch)
{
    Object obj;

    obj = allocate_object (sizeof (struct object));

    TYPE (obj) = Character;
    CHARVAL (obj) = ch;
    return (obj);
}
#else
Object
make_character (char ch)
{
    return (MAKE_CHAR (ch));
}
#endif

static Object
integer_to_character (Object i)
{
    return (make_character (INTVAL (i)));
}

static Object
character_to_integer (Object ch)
{
    return (make_integer (CHARVAL (ch)));
}
