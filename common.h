
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    -                       

 * ========================================
*/

#include <stdint.h>

#define HIGH 1
#define LOW 0
#define LED_ON 0u
#define LED_OFF 1u

#ifndef UINT16_MAX
    #define UINT16_MAX     65535
#endif
#ifndef UINT32_MAX
    #define UINT32_MAX     4294967294
#endif

uint32_t glob_sec_counter;
uint32_t glob_human_counter;


#ifndef DEBUGMODE
    #define DEBUGMODE true
#endif

/* [] END OF FILE */




