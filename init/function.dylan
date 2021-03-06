//
// function.dylan - functionals
//

define method compose(func :: <function>, #rest more-functions)
  if (empty?(more-functions))
    func;
  else
    method (#rest args)
      func(apply(apply(compose, head(more-functions),
                       tail(more-functions)), args));
    end method;
  end if;
end method compose;

define method complement (func :: <function>)
  method (#rest args)
    ~(apply(func, args));
  end method;
end method complement;

define method disjoin ( predicate :: <function>,
		       #rest more-predicates)
  method (#rest args)
    if (empty? (predicates))
      apply (predicate (args))
    else
      apply (predicate, args)
	| apply (apply (disjoin, predicates), args);
    end if;
  end method;
end method disjoin;

define method conjoin (predicate :: <function>,
		       #rest more-predicates)
  method (#rest args)
    if (empty? (more-predicates))
      apply (predciate, args)
    else
      apply (predicate, args)
	& apply (apply (conjoin, more-predicates), args);
    end if;
  end method;
end method conjoin;

define method curry (f :: <function>,
		     #rest curried-args)
  method (#rest regular-args)
    apply (f, concatenate (curried-args, regular-args));
  end method;
end method curry;

define method rcurry (f :: <function>,
		      #rest curried-args)
  method (#rest regular-args)
    apply (f, concatenate (regular-args, curried-args));
  end method;
end  method rcurry;

define method always (obj :: <object>)
  method (#rest args)
    obj
  end method;
end method always;

define method eval(obj)
  %eval (obj);
end method eval;

define method type-union(#rest types)
  apply(%union-type, types);
end method type-union;

// end function.dylan
