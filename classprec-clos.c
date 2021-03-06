/*
** classprec-clos.c
*/

#include "classprec.h"

#include "error.h"
#include "list.h"
#include "print.h"

extern Object standard_output_stream;

/* local functions */
static void print_pnode (Object pnode);
static void print_slist (Object slist);

static void construct_slist (Object *sptr, Object class);
static void insert_precedes_arc (Object pred_node, Object succ_node);
static void insert_succeeds_arc (Object succ_node, Object pred_node);
static void record_precedence (Object slist, Object pred_class,
			       Object succ_class);
static void decorate_slist_with_precedence (Object slist, Object class);
static void remove_one_predecessor_arc (Object node, Object node_to_remove);
static void remove_predecessor_arcs (Object node);
static void remove_one_successor_arc (Object node, Object node_to_remove);
static void remove_successor_from_predecessors (Object node);
static void remove_node_from_slist (Object *slist, Object node);
static int direct_superclassp (Object super, Object sub);
static Object find_minimal_elements (Object slist);

/* macros */

#define MAKE_PNODE(class) (cons (class, cons (make_empty_list(), (cons (make_empty_list(), make_empty_list())))))
#define PNODE_CLASS(pnode)        (CAR (pnode))
#define PNODE_SUCCESSORS(pnode)   (CAR (CDR (pnode)))
#define PNODE_PREDECESSORS(pnode) (CAR (CDR (CDR (pnode))))

/* functions */

Object
compute_class_precedence_list (Object class)
{
    Object slist, class_list, *candidate_list_ptr;
    Object minimal_element_set, node;
    Object precedence_list_rev;
    Object a_set;

    slist = make_empty_list ();

    /* Prepare list S */
    construct_slist (&slist, class);
    decorate_slist_with_precedence (slist, class);

#if EBUG
    fprintf (stdout, "** Initial slist **\n  ");
    print_slist (slist);
    fprintf (stdout, "\n");
#endif

    /*
       We now have the slist decorated with all the precedences in
       a graph initially rooted at the node for class.  At each step, we
       remove the appropriate minimal element from the graph
       (there may be more than one), and insert it into the precedence
       list.  The prec list is maintained in reverse order.

     */

    precedence_list_rev = make_empty_list ();
    minimal_element_set = cons (CAR (slist), make_empty_list ());
    while (PAIRP (minimal_element_set)) {

#if EBUG
	fprintf (stdout, " ** Minimal elements **\n   ");
	for (a_set = minimal_element_set;
	     PAIRP (a_set);
	     a_set = CDR (a_set)) {
	    print_object (standard_output_stream,
			  CLASSNAME (PNODE_CLASS (CAR (a_set))),
			  0);
	    fprintf (stdout, " ");
	}
	fprintf (stderr, "\n");
#endif
	if (EMPTYLISTP (CDR (minimal_element_set))) {
	    /*
	     * There is a unique minimal element in the graph.
	     *  Add it to the precedence list, remove it from the
	     * predecessors list of all its successors, and remove
	     *  it from slist.
	     */
	    node = CAR (minimal_element_set);
	    remove_predecessor_arcs (node);
	    precedence_list_rev = cons (PNODE_CLASS (node),
					precedence_list_rev);
	    remove_node_from_slist (&slist, node);
	    minimal_element_set = find_minimal_elements (slist);
	} else {
	    /*
	     * We have more than one minimal element.
	     * We must choose the one that has a direct subclass that is
	     * furthest to the right in the precedence list.  That is,
	     * the one that is closest to the head of precedence_list_rev.
	     */
	    for (class_list = precedence_list_rev;
		 PAIRP (class_list);
		 class_list = CDR (class_list)) {
		for (candidate_list_ptr = &minimal_element_set;
		     PAIRP (*candidate_list_ptr);
		     candidate_list_ptr = &CDR (*candidate_list_ptr)) {
		    if (direct_superclassp (PNODE_CLASS (CAR (*candidate_list_ptr)),
					    CAR (class_list))) {
			break;
		    }
		}
		if (PAIRP (*candidate_list_ptr)) {
		    /* We found the right candidate.
		     * Remove the predecessor arcs for this node.
		     * Remove it from the slist.
		     * Update the precedence list.
		     * Update the min. element set
		     */
		    node = CAR (*candidate_list_ptr);
		    remove_predecessor_arcs (node);
		    precedence_list_rev = cons (PNODE_CLASS (node),
						precedence_list_rev);
		    remove_node_from_slist (&slist, node);
		    *candidate_list_ptr = CDR (*candidate_list_ptr);
#if 0
		    /* This interesting bug leads to a monotonic class
		     * ordering. Such an ordering is, in a sense more
		     * desirable than the class ordering that is yielded
		     * by the normal rule.
		     */
		    if (EMPTYLISTP (minimal_element_set)) {
			minimal_element_set = find_minimal_elements (slist);
		    }
#else
		    minimal_element_set = find_minimal_elements (slist);
#endif
		    break;
		}
	    }
	    if (EMPTYLISTP (class_list)) {
		error ("Whoa!  the class list was empty making precedence list",
		       NULL);
	    }
	}
    }
    if (PAIRP (slist)) {
	error ("Unable to construct class precedence list", class, NULL);
    }
    /* Cache the result */
    CLASSPRECLIST (class) = list_reverse_bang (precedence_list_rev);
    return CLASSPRECLIST (class);
}

static void
construct_slist (Object *sptr, Object class)
{
    Object *tmp_sptr = sptr;
    Object sclist;

    while (PAIRP (*tmp_sptr)) {
	if (class == PNODE_CLASS (CAR (*tmp_sptr)))
	    break;
	tmp_sptr = &CDR (*tmp_sptr);
    }
    if (EMPTYLISTP (*tmp_sptr)) {
	*tmp_sptr = cons (MAKE_PNODE (class), make_empty_list ());
    }
    for (sclist = CLASSSUPERS (class);
	 PAIRP (sclist);
	 sclist = CDR (sclist)) {
	construct_slist (sptr, CAR (sclist));
    }
}

static void
insert_precedes_arc (Object pred_node, Object succ_node)
{
    add_new_at_end (&PNODE_SUCCESSORS (pred_node), (succ_node));
}

static void
insert_succeeds_arc (Object succ_node, Object pred_node)
{
    add_new_at_end (&PNODE_PREDECESSORS (succ_node), pred_node);
/*

   PNODE_PREDECESSORS (succ_node) = cons (pred_node,
   PNODE_PREDECESSORS (succ_node));
 */
}

static void
record_precedence (Object slist, Object pred_class, Object succ_class)
{
    Object q, p;

    /* find nodes associated with pred and succ classes */
    for (q = slist; PNODE_CLASS (CAR (q)) != pred_class; q = CDR (q)) ;
    for (p = slist; PNODE_CLASS (CAR (p)) != succ_class; p = CDR (p)) ;
    insert_precedes_arc (CAR (q), CAR (p));
    insert_succeeds_arc (CAR (p), CAR (q));
}

static void
decorate_slist_with_precedence (Object slist, Object class)
{
    Object q, p;

    for (q = cons (class, make_empty_list ()), p = CLASSSUPERS (class);
	 PAIRP (p);
	 q = p, p = CDR (p)) {
	record_precedence (slist, CAR (q), CAR (p));
    }
    for (p = CLASSSUPERS (class); PAIRP (p); p = CDR (p)) {
	decorate_slist_with_precedence (slist, CAR (p));
    }
}

static void
remove_predecessor_arcs (Object node)
{
    Object succ_list;

    for (succ_list = PNODE_SUCCESSORS (node);
	 PAIRP (succ_list);
	 succ_list = CDR (succ_list)) {
	remove_one_predecessor_arc (CAR (succ_list), node);
    }
}

static void
remove_one_predecessor_arc (Object node, Object node_to_remove)
{
    Object *tmp_ptr;

    for (tmp_ptr = &PNODE_PREDECESSORS (node);
	 CAR (*tmp_ptr) != node_to_remove;
	 tmp_ptr = &CDR (*tmp_ptr)) ;

    *tmp_ptr = CDR (*tmp_ptr);

}

static void
remove_one_successor_arc (Object node, Object node_to_remove)
{
    Object *tmp_ptr;

    for (tmp_ptr = &PNODE_SUCCESSORS (node);
	 CAR (*tmp_ptr) != node_to_remove;
	 tmp_ptr = &CDR (*tmp_ptr)) ;

    *tmp_ptr = CDR (*tmp_ptr);

}

static void
remove_successor_from_predecessors (Object node)
{
    Object pred_list;

    for (pred_list = PNODE_PREDECESSORS (node);
	 PAIRP (pred_list);
	 pred_list = CDR (pred_list)) {
	remove_one_successor_arc (CAR (pred_list), node);
    }
}
static void
remove_node_from_slist (Object *slist, Object node)
{
    Object *tmp_ptr;

    tmp_ptr = slist;
    while (PAIRP (*tmp_ptr)) {
	if (CAR (*tmp_ptr) == node) {
	    *tmp_ptr = CDR (*tmp_ptr);
	    return;
	}
	tmp_ptr = &CDR (*tmp_ptr);
    }
    /*
     * If we get here, then the object being removed has already been
     * removed by virtue of having been encountered as a successor
     * of a class included twice in the inheritance hierarchy
     */
}

static int
direct_superclassp (Object super, Object sub)
{
    Object supers;

    for (supers = CLASSSUPERS (sub);
	 PAIRP (supers);
	 supers = CDR (supers)) {
	if (CAR (supers) == super)
	    return 1;
    }
    return 0;
}

/*
 * I left this in for debugging purposes
 */

static void
print_pnode (Object pnode)
{
    Object nlist;

    fprintf (stderr, "[%s]\n Successors: ",
	     SYMBOLNAME (CLASSNAME (PNODE_CLASS (pnode))));
    for (nlist = PNODE_SUCCESSORS (pnode);
	 PAIRP (nlist);
	 nlist = CDR (nlist)) {
	fprintf (stderr, "%s ",
		 SYMBOLNAME (CLASSNAME (PNODE_CLASS (CAR (nlist)))));
    }
    fprintf (stderr, "\n Predecessors: ");
    for (nlist = PNODE_PREDECESSORS (pnode);
	 PAIRP (nlist);
	 nlist = CDR (nlist)) {
	fprintf (stderr, "%s ",
		 SYMBOLNAME (CLASSNAME (PNODE_CLASS (CAR (nlist)))));
    }
    fprintf (stderr, "\n");
}

static void
print_slist (Object slist)
{
    Object p;

    for (p = slist; PAIRP (p); p = CDR (p)) {
	print_object (standard_output_stream, CLASSNAME (PNODE_CLASS (CAR (p))), 0);
	fprintf (stdout, " ");
    }
}


static Object
find_minimal_elements (Object slist)
{
    Object mins = make_empty_list ();

    while (PAIRP (slist)) {
	if (EMPTYLISTP (PNODE_PREDECESSORS (CAR (slist)))) {
	    mins = cons (CAR (slist), mins);
	}
	slist = CDR (slist);
    }
    return mins;
}
