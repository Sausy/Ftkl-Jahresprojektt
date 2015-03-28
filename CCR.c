
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    Bücher: 
                C für Dummies                       

 * ========================================
*/

//  In following Code I tried to provied device fuunktion without 
//  knowing what the device is named -> The name is set in the header
#include <project.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "CCR.h"

#define concat(x, y) x##y
#define _ccr_write_(x) concat(x, _Write)
#define _ccr_read_(x) concat(x, _Read)

//---private---
uint8_t ccr_read_byte(){
    return (_ccr_read_(_name_ccr_)());
}    

//---private---  
void ccr_set_byte(uint8_t ccr_byte){
     _ccr_write_(_name_ccr_)(ccr_byte);
}

//---public---
void set_bit_CCR (uint8_t ccr_bit, bool ccr_value){
    uint8_t buffer = ccr_read_byte();
    
    if(ccr_value)
        buffer |= (1<<ccr_bit);
    else
        buffer &= ~(1<<ccr_bit);
    ccr_set_byte(buffer);
  
}

//---public---
bool get_bit_CCR (uint8_t ccr_bit){
    uint8_t buffer = ccr_read_byte();
    
    if(buffer &(0x00|(1<<ccr_bit)))
        return true;
    
    return false;
}

/* [] END OF FILE */
