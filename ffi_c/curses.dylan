Module:   dylan
Synopsis: Curses interface
Author:   Nibby Nebbulous

define constant curses_fh = load-foreign("/sw/lib/libncurses.dylib");

// Initialization

define constant ff_initscr = make(<ff>,
                                 name: #"initscr",
                                 file-handle: curses_fh);

// Windows

define constant ff_newwin = make
  (<ff>,
   name: #"newwin",
   file-handle: curses_fh,
   arg-types: list(<small-integer>, <small-integer>, <small-integer>,
                   <small-integer>),
   return-type: <foreign-pointer>);

define constant ff_delwin = make
  (<ff>,
   name: #"delwin",
   file-handle: curses_fh,
   arg-types: list(<foreign-pointer>),
   return-type: <integer>);


define constant ff_endwin = make(<ff>,
                                name: #"endwin",
                                file-handle: curses_fh);

define constant ff_wrefresh = make(<ff>,
                                   name: #"wrefresh",
                                   file-handle: curses_fh,
                                   arg-types: list(<foreign-pointer>));

define constant ff_refresh = make(<ff>,
                                name: #"refresh",
                                file-handle: curses_fh);

// Terminal modes

define constant ff_resetty = make(<ff>,
                                name: #"resetty",
                                file-handle: curses_fh);

define constant ff_reset_shell_mode = make(<ff>,
                                name: #"reset_shell_mode",
                                file-handle: curses_fh);

define constant ff_cbreak = make(<ff>,
                                 name: #"cbreak",
                                 file-handle: curses_fh);

define constant ff_noecho = make(<ff>,
                                 name: #"noecho",
                                 file-handle: curses_fh);

define constant ff_nonl = make(<ff>,
                               name: #"nonl",
                               file-handle: curses_fh);

// Output

define constant ff_mvaddch = make
  (<ff>,
   name: #"mvaddch",
   file-handle: curses_fh,
   arg-types: list(<small-integer>, <small-integer>, <small-integer>));

define constant ff_addstr = make
  (<ff>,
   name: #"addstr",
   file-handle: curses_fh,
   arg-types: list(<string>),
   return-type: <integer>);

// Moving

define constant ff_move = make
  (<ff>,
   name: #"move",
   file-handle: curses_fh,
   arg-types: list(<integer>, <integer>));

// Clearing

define constant ff_clear = make(<ff>,
                                name: #"clear",
                                file-handle: curses_fh);

define constant ff_wclear = make(<ff>,
                                 name: #"wclear",
                                 file-handle: curses_fh,
                                 arg-types: list(<foreign-pointer>));

// Input

define constant ff_getch = make
  (<ff>,
   name: #"getch",
   file-handle: curses_fh,
   return-type: <small-integer>);

// Colors and attributes

define constant ff_start_color = make(<ff>,
                                      name: #"start_color",
                                      file-handle: curses_fh);

define constant ff_has_colors = make
  (<ff>,
   name: #"has_colors",
   file-handle: curses_fh,
   return-type: <small-integer>);

define constant ff_can_change_color = make
  (<ff>,
   name: #"can_change_color",
   file-handle: curses_fh,
   return-type: <small-integer>);

define constant ff_init_pair = make
  (<ff>,
   name: #"init_pair",
   file-handle: curses_fh,
//   arg-types: list(<C-short>, <C-short>, <C-short>));
   arg-types: list(<integer>, <integer>, <integer>));

define constant ff_attron = make
  (<ff>,
   name: #"attron",
   file-handle: curses_fh,
   arg-types: list(<integer>),
   return-type: <integer>);

define constant ff_attroff = make
  (<ff>,
   name: #"attroff",
   file-handle: curses_fh,
   arg-types: list(<integer>),
   return-type: <integer>);

define constant ff_color_pair = make
  (<ff>,
   name: #"COLOR_PAIR",
   file-handle: curses_fh,
   arg-types: list(<integer>),
   return-type: <integer>);

// Boxes and lines

define constant ff_border = make
  (<ff>,
   name: #"border",
   file-handle: curses_fh,
   arg-types: list(<integer>, <integer>, <integer>, <integer>,
                   <integer>, <integer>, <integer>, <integer>),
   return-type: <integer>);

define constant ff_wborder = make
  (<ff>,
   name: #"wborder",
   file-handle: curses_fh,
   arg-types: list(<foreign-pointer>,
                   <integer>, <integer>, <integer>, <integer>,
                   <integer>, <integer>, <integer>, <integer>),
   return-type: <integer>);

// These should be from gotten from the curses library variables
define constant COLOR_PAIRS :: <integer> = 64;
define constant LINES :: <integer> = 24;
define constant COLS :: <integer> = 80;

// define function COLOR_PAIR(i :: <integer>)
//  
// end function;

define constant A_NORMAL :: <integer>     = #x00000000;
define constant A_ATTRIBUTES :: <integer> = #xffffff00;
define constant A_CHARTEXT :: <integer>   = #x000000ff;
define constant A_COLOR :: <integer>      = #x0000ff00;
define constant A_STANDOUT :: <integer>   = #x00010000;
define constant A_UNDERLINE :: <integer>  = #x00020000;
define constant A_REVERSE :: <integer>    = #x00040000;
define constant A_BLINK :: <integer>      = #x00080000;
define constant A_DIM :: <integer>        = #x00100000;
define constant A_BOLD :: <integer>       = #x00200000;
define constant A_ALTCHARSET :: <integer> = #x00400000;
define constant A_INVIS :: <integer>      = #x00800000;
define constant A_PROTECT :: <integer>    = #x01000000;
define constant A_HORIZONTAL :: <integer> = #x02000000;
define constant A_LEFT :: <integer>       = #x04000000;
define constant A_LOW :: <integer>        = #x08000000;
define constant A_RIGHT :: <integer>      = #x10000000;
define constant A_TOP :: <integer>        = #x20000000;
define constant A_VERTICAL :: <integer>   = #x40000000;

define method as(s == <string>, d :: <integer>)
  let hexi = "0123456789ABCDEF";
  let str-num = make(<deque>);
  let base = 10;
  while(d > base)
    let digit = modulo(d, base);
    push(str-num, hexi[digit]);
    d := floor/(d, base);
  end while;
  push(str-num, hexi[d]);
  as(<string>, str-num);
end method write-number;

define method test()
  let NCOLORS :: <integer> = 8;
  initscr();
  start_color();
  cbreak();
  noecho();
  nonl();
  clear();
  let has_color = if (has_colors() = 0) #f else #t end;

  if (~has_color)
    addstr("!");
  end;
  addstr("has_colors\n");
  if (can_change_color() = 0)
    addstr("!");
  end;
  addstr("can_change_color\n");
  addstr("COLOR_PAIRS = "); addstr(as(<string>, COLOR_PAIRS)); addstr("\n");

  if (has_color)

    // Initialize all the color pairs
    let pair :: <integer> = 0;
    for (fg :: <integer> from (NCOLORS - 1) to 0 by -1)
      for (bg :: <integer> from 0 below NCOLORS by 1)
        if (pair > 0)               // Pair 0 defaults to WHITE on BLACK
          init_pair(pair, fg, bg);
        end;
        pair := pair + 1;
      end for;
    end for;

    // Show the color pairs
    for (i from 0 below COLOR_PAIRS by 1)
      if (modulo(i, 8) = 0)
        addstr("\n");
      end if;
      attron(COLOR_PAIR(i));
      addstr(as(<string>, i)); addstr(" ");
      attroff(COLOR_PAIR(i));
    end for;
    addstr("\n");
    refresh();
    getch();

    // Show the color pairs with bold on
    for (i from 0 below COLOR_PAIRS by 1)
      if (modulo(i, 8) = 0)
	addstr("\n");
      end if;

      attron(COLOR_PAIR(i));
      attron(A_BOLD);
      addstr(as(<string>, i)); addstr(" ");
      attroff(COLOR_PAIR(i));
    end for;

  end if; // has_color
  refresh();
  getch();

  // Do borders and windows
  border(0, 0, 0, 0, 0, 0, 0, 0);
  refresh();
  for (i from 1 below truncate/(LINES, 2) by 1)
     let w = newwin(LINES - (2 * i), COLS - (2 * i), i, i);
     wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
     wrefresh(w);
     refresh();
     getch();
     delwin(w);
  end for;
  refresh();
  let c = getch();

  let w = newwin (LINES - 2, COLS - 2, 1, 1);
  wclear(w);
  wrefresh(w);

  let y :: <integer> = modulo(random(), (LINES - 2)) + 1;
  let x :: <integer> = modulo(random(), (COLS - 2)) + 1;
  let c :: <character> = if (modulo(random(), 2) = 0) '*' else ' ' end;
  let color :: <integer> = modulo(random(), COLOR_PAIRS);
  for (i from 0 to 2000 by 1)
    y := modulo(abs(random()), (LINES - 2)) + 1;
    x := modulo(abs(random()), (COLS - 2)) + 1;
    c := if (logand(random(), #x1000) = 0) '*' else ' ' end;
    if (has_color)
      color := modulo(random(), COLOR_PAIRS);
      attron(COLOR_PAIR(color));
    end;
    mvaddch(y, x, as(<small-integer>, c));
    refresh();
    if (has_color)
      attroff(COLOR_PAIR(color));
    end;
  end;
  move(LINES - 1, 0);

// Old stuff:
//   for (i from 0 to 10)
//     mvaddch(i, i, as(<small-integer>, '*'));
//   end;
//   refresh();
//   let c = getch();

  reset_shell_mode();
  endwin();
  princ("Done.\n");
  #t;
end method;

test();
