
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    -                       

 * ========================================
*/

#include <stdbool.h>
#include <stdint.h>


//--- identifierer form ccr_bit
#define _ccr_count_bit_ 0
#define _ccr_start_bit_ 1
#define _ccr_stop_bit_ 2

//If the name of teh Controll Register chanches 
//  ---_name_ccr_--- has to be given the current name
#define _name_ccr_ Counter_Controll_Register

//-------------[public funktion]-----------------
void set_bit_CCR (uint8_t ccr_bit, bool ccr_value);
bool get_bit_CCR (uint8_t ccr_bit);

/* [] END OF FILE */

