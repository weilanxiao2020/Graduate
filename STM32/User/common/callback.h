#ifndef __CALLBACK_H
#define __CALLBACK_H

#include "constants.h"
#include "gps.h"
#include "debug.h"

#ifdef GZ_DEBUG
    #define Callback_TAG       "callback"
#else 
    #define Callback_TAG       ""  
#endif

typedef void (*Tim2Callback)(void);
void Tim2_1s_Callback(void);

#endif
