//
// array.dyl
//
// Author: Brent Benson
//

define method element (a :: <array>,
		       index :: <integer>,
		       #key default = %default-object)
  %array-element (a, index, default);
end method element;

define method element (a :: <array>,
		       indices :: <list>,
		       #key default = %default-object)
  %array-ref (a, indices, default);
end method element;

define method element-setter (new-value, a :: <array>, index :: <integer>)
  %array-element-setter (a, index, new-value);
end method element-setter;

define method element-setter (new-value, a :: <array>, indices :: <list>)
  %array-ref-setter (a, indices, new-value);
end method element-setter;

define method dimensions (a :: <array>)
  %array-dimensions (a);
end method dimensions;

define method size (a :: <array>)
  %array-size (a);
end method size;

define generic rank (a :: <array>) => rank :: <integer>;

define method rank (a :: <array>)
  %array-dimensions (a).size;
end method rank;

define generic row-major-index (a :: <array>, #rest subscripts)
 => index :: <integer>;

define method row-major-index (a :: <array>, #rest subscripts)
  %array-row-major-index (a, subscripts);
end method row-major-index;

define generic aref (a :: <array>, #rest indices);

define method aref (a :: <array>, #rest indices)
  %array-ref (a, indices, %default-object);
end method aref;

define generic aref-setter (new-val, a :: <array>, #rest indices);

define method aref-setter (new-val, a :: <array>, #rest indices)
  %array-ref-setter (a, indices, new-val);
end method aref-setter;

define method aref (v :: <vector>, #rest indices)
  %vector-element (v, indices.first, %default-object);
end method aref;

define method aref-setter (new-val, v :: <vector>, #rest indices)
  %vector-element-setter (v, indices.first, new-val);
end method aref-setter;

define generic dimension (a :: <array>, axis :: <integer>)
 => dimension :: <integer>;

define method dimension (array :: <array>, axis :: <small-integer>)
  element (dimensions (array), axis);
end method dimension;

define method shallow-copy (a :: <array>)
  let new-array = make (<array>, dimensions: dimensions (a));
  for (i from 0 below a.size)
    element-setter (element(a, i), new-array, i);
  end for;
  new-array;
end method shallow-copy;

//
// iteration protocol
//

define method initial-state (a :: <array>) => value :: <integer-state>;
  %array-initial-state (a);
end method initial-state;

define method next-state (a :: <array>, s :: <integer-state>)
  %array-next-state (a, s);
end method next-state;

define method current-element (a :: <array>, s :: <integer-state>)
  %array-current-element (a, s);
end method current-element;

// end array.dyl
