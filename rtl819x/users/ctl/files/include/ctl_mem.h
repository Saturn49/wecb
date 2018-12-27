#ifndef __CTL_MEM_H__
#define __CTL_MEM_H__
#include "tsl_common.h"

#define CTLMEM_FREE_BUF_AND_NULL_PTR(p) \
   do { \
      if ((p) != NULL) {free((p)); (p) = NULL;}   \
   } while (0)

#define CTLMEM_REPLACE_STRING(p, s) \
   do {\
      if ((p) != NULL) {free((p));p=NULL;} \
	  if ((s) != NULL) (p) = strdup((s)); \
   } while (0)
   
#define REPLACE_STRING_IF_NOT_EQUAL_FLAGS(p, s) CTLMEM_REPLACE_STRING(p, s)
#define CTLMEM_REPLACE_STRING_FLAGS(p, s) CTLMEM_REPLACE_STRING(p, s)

#endif
