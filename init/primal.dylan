module: dylan
synopsis: Core functionality for Marlais
copyright: (c) 2001, Marlais Hackers, LGPL (see "doc/COPYRIGHT" file)

//
// primal.dylan
//

// This is usually the first thing loaded. It contains really basic stuff,
// which may be needed by everything else and isn't in the category of any of
// the other init files.

// %trace(#t); // @@@

define method make (c :: <class>, #rest args, #key, #all-keys)
  %make(c, args);
end method make;

define constant \== = method(a, b) %identical?(a, b) end;
define constant \~ = method(a) %not(a) end;
define constant \~== = method(a, b) %not(%identical?(a, b)) end;

define constant instance? =
  method (obj, typ :: <type>)
    %instance?(obj, typ);
  end method;

define constant singleton =
  method (o :: <object>) => <singleton>;
    %singleton (o);
  end method;

define method as (c :: <class>, obj :: <object>)
  if (object-class(obj) = c)
    obj;
  else
    error("No method to coerce first object to second", obj, c);
  end if;
end method as;

define method as (sc == <string>, s :: <symbol>)
  %symbol->string(%keyword->symbol (s));
end method as;

/* <pcb> added so we can use the reader. */
define method as (sc == <string>, s :: <variable-name>)
  %symbol->string(s);
end method as;

define method as (sc == <symbol>, s :: <string>)
  %symbol->keyword (%string->symbol (s));
end method as;

define method error (msg :: <string>, #rest args)
  %apply(%error, %pair(msg, args));
end method error;

define method warning (msg :: <string>, #rest args)
  %apply(%warning, %pair(msg, args));
end method warning;

define method cerror (#rest args)
  format(*standard-error*, "cerror: called with arguments %=", args);
end method cerror;

define method signal (#rest args)
  %signal-error-jump();
end method signal;

//
//  This just doesn't look right to me!!!!  jnw.
// (Could you be a little more specific? What would you like it to do?)

define method initialize (instance, #key, #all-keys)
end method initialize;

define method generic-function-methods (gf :: <generic-function>)
  %generic-function-methods(gf);
end method generic-function-methods;

define method add-method (gf :: <generic-function>, meth :: <method>)
  %add-method(gf, meth);
end method add-method;

define method generic-function-mandatory-keywords (gf :: <generic-function>)
  %generic-function-mandatory-keywords(gf);
end method generic-function-mandatory-keywords;

define method function-specializers (func :: <function>)
 => type-sequence :: <sequence>;
  %function-specializers(func);
end method function-specializers;

define method function-return-values (func :: <function>)
 => (type-sequence :: <sequence>, rest-value :: <object>);
  %function-values(func);
end method function-return-values;

define method function-arguments (f :: <function>)
  %function-arguments(f);
end method function-arguments;

define method applicable-method? (m :: <function>, #rest args)
  %apply(%applicable-method?, %pair(m, args));
end method applicable-method?;

define method sorted-applicable-methods (gf :: <generic-function>, #rest args)
  %apply(%sorted-applicable-methods, %pair(gf, args));
end method sorted-applicable-methods;

define method find-method (gf :: <generic-function>, #rest sample-args)
  %find-method(gf, sample-args);
end method find-method;

define method remove-method (gf :: <generic-function>, meth :: <method>)
  %remove-method(gf, meth);
end method remove-method;

define method make (gftype == <generic-function>,
		    #key required, rest, key, all-keys)
  if (instance? (required, <number>))
    required := make (<list>, size: required, fill: <object>);
  end if;
  if (instance? (required, <list>))
    %generic-function-make (list (required, rest, key, all-keys));
  else
    error ("make: bad key value", required:, required);
  end if;
end method make;

define method debug-name-setter (s :: <variable-name>, m :: <method>)
  %debug-name-setter(m, s);
end method debug-name-setter;

define method debug-name (c :: <class>) => (name :: <variable-name>)
   %class-debug-name(c);
end method debug-name;


define method apply (f :: <function>, #rest args)

  // collect-args: collects the arguments into a single list.
  //  Splices in the contents of the last (collection) argument.
  local method collect-args (args)
	  case
	    empty?(args) => #();
	    empty?(tail(args)) =>
	      if (~instance?(head(args), <sequence>))
		error("apply:  last arg must be a sequence", head(args));
	      else
		as (<list>, head(args));
	      end if;
	    otherwise =>
	      // first reverse the arguments.
	      let res = reverse (args);
	      let argseq = head (res);
	      let res = tail (res);

	      // Then splice in the elements of the sequence.
	      for (val in argseq)
		res := pair (val, res);
	      end for;
	      
	      // now fix up the arguments again
	      reverse!(res);
	  end case;
	end collect-args;

  %apply(f, collect-args(args));
end method apply;

define constant identity =
  method (x :: <object>)
    x;
  end method;

//
// comparisons.
//

// according to DIRM, = should be a generic function so it can be extended
// by user classes. most primitive version just checks if they are ==.

define method \= (o1, o2)
  o1 == o2;
end method \=;

// \~= just calls \= and complements the result.

define constant \~= =
  method (o1, o2)
    ~(o1 = o2);
  end method;

// IRM definition:  < is a generic function.

define method \< (o1, o2)
  error("\<: Can't compare these objects", o1, o2);
end method \<;	

// >, <=, and >= are defined by <.

define constant \> =
  method (o1, o2)
    o2 < o1;
  end method;

define constant \<= =
  method (o1, o2)
    ~(o2 < o1);
  end method;

define constant \>= =
  method (o1, o2)
    ~(o1 < o2);
  end method;

// define method =hash (obj)
//   %=hash(obj);
// end method =hash;

//
// classes
//

define constant subtype? =
  method (t1 :: <type>, t2 :: <type>)
    %subtype?(t1, t2);
  end method;

define constant object-class =
  method (o :: <object>) => <class>;
      %object-class (o);
  end method;

define constant all-superclasses =
  method (c :: <class>)
    %all-superclasses(c);
  end method;

define constant direct-superclasses =
  method (c :: <class>)
    %direct-superclasses(c);
  end method;

define constant direct-subclasses =
  method (c :: <class>)
    %direct-subclasses(c);
  end method;

define method shallow-copy (o :: <object>) => new-object :: <object>;
  o;
end method shallow-copy;

define method type-for-copy (o :: <object>) => value :: <class>;
  o.object-class
end method type-for-copy;

//
// Seal is no longer in the DIRM, but Marlais still provides it.
//

define method seal (c :: <class>)
  %seal(c);
end method seal;

define constant slot-initialized? =
  method (obj, slot)
     slot(obj) ~== %uninitialized-slot-value;
  end method;


// limited <integer>

define method limited (int == <integer>, #rest args, #key min, max)
  %limited-integer(args);
end method limited;

define generic union (o1, o2, #rest args);

define method union (t1 :: <type>, t2 :: <type>)
  %union-type(list(t1, t2));
end method union;

// end primal.dylan
