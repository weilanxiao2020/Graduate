#ifndef __GPS_H
#define __GPS_H

#include "bsp_gps.h"

#ifdef GZ_DEBUG
    #define Gps_TAG       "gps"
#else 
    #define Gps_TAG       ""  
#endif


void Gps_Init(void);
void Gps_Data_Get(void);
GpsData* Gps_Data_Buf(void);
void Gps_Data_Parse(void);
void Gps_Data_Print(void);
#endif
