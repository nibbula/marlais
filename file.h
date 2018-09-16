/*
**  file.h
*/

#ifndef FILE_H
#define FILE_H

#include "object.h"

#ifdef DEBUG
extern char * load_filename;
#endif

void init_file_prims (void);
Object load (Object filename);
Object i_load (Object filename);
void close_open_files (void);

#endif /* FILE_H */
