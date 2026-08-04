#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "stdio.h"
#include "usart.h"
#include "stdarg.h"
#define DEBUG_ENABLE 1

#ifdef DEBUG_ENABLE
#define debug_printf(format,...) printf("[%s:%d]" format "\r\n",__FILE__,__LINE__,##__VA_ARGS__)
#else
#define debug_printf(...)
#endif

#endif 
