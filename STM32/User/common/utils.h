#ifndef   __UTILS_H
#define   __UTILS_H    

#include "stdint.h"
#include "constants.h"

/*
**************************************************************************************************************
*										    
**************************************************************************************************************
*/
// uint8[4]->uint32
uint32_t u8_to_u32(uint8_t* data);        
uint64_t u8_to_u64(uint8_t *data);
void u32_to_u8(uint32_t data, uint8_t* res); 
uint32_t str_to_u32(uint8_t *str);
uint8_t* u32_to_str(uint32_t data);

void Buf_Clear(uint8_t* data, uint8_t size);
boolean Find_Str(const char* str, const char* match);
#endif
