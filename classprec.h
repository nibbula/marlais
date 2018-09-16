/*
** classprec.h
*/

#ifndef CLASSPREC_H_
#define CLASSPREC_H_

#ifdef USE_LL_PRECEDENCE
#include "classprec-ll.h"
#else
#include "classprec-clos.h"
#endif

#endif /* CLASSPREC_H_ */
