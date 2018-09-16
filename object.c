/*
** object.c
*/

#include "object.h"

#include "error.h"
#include "number.h"

#ifdef SMALL_OBJECTS

enum objtype
object_type (Object obj)
{
    if (POINTERP (obj)) {
	return (PAIRTYPE (obj));
    } else if (IMMEDP (obj)) {
	switch (SUBPART (obj)) {
	case TRUESUB:
	    return (True);
	case FALSESUB:
	    return (False);
	case EMPTYSUB:
	    return (EmptyList);
	case CHARSUB:
	    return (Character);
	case EOFSUB:
	    return (EndOfFile);
	case UNSPECSUB:
	    return (Unspecified);
	case UNINITSUB:
	    return (UninitializedSlotValue);
	default:
	    error ("object with unknown immediate tag",
		   make_integer (SUBPART (obj)),
		   0);
	}
    } else {
	return (Integer);
    }
}

#ifdef DEBUG
/*
** This is an attempt to try to make object structs show up in debugging
** symbols so we can use them in the debugger for casting. Without this, there
** may be no declarations using these structures and the compiler or linker
** may not include them. This should really only be necessary if SMALL_OBJECTS
** is defined.
*/
static struct big_integer		dbg_i;
static struct ratio			dbg_r;
static struct double_float		dbg_df;
static struct pair			dbg_p;
static struct byte_string		dbg_bs;
static struct simple_object_vector	dbg_sov;
static struct table_entry		dbg_te;
static struct table			dbg_t;
static struct deque_entry		dbg_de;
static struct deque			dbg_d;
static struct array			dbg_a;
static struct symbol			dbg_s;
static struct slot_descriptor		dbg_sd;
static struct class			dbg_c;
static struct instance			dbg_in;
static struct singleton			dbg_si;
static struct limited_int_type		dbg_lit;
static struct union_type		dbg_ut;
static struct primitive			dbg_pr;
static struct prim			dbg_pri;
static struct generic_function		dbg_gf;
static struct method			dbg_m;
static struct next_method		dbg_nm;
static struct values			dbg_v;
static struct exitproc			dbg_ep;
static struct unwind			dbg_u;
static struct foreign_ptr		dbg_fp;
static struct environment		dbg_en;
static struct object_handle		dbg_oh;

#endif /* DEBUG */

#endif /* SMALL_OBJECTS */

Object
make_handle (Object an_object)
{
    Object new_handle;

    new_handle = allocate_object (sizeof (struct object_handle));

    HDLTYPE (new_handle) = ObjectHandle;
    HDLOBJ (new_handle) = an_object;
    return (new_handle);
}
