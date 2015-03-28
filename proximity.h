
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    http://www.element14.com/community/message/76985/l/psoc-4-pioneer-kit-community-project015-capsense-proximity-detection#76985
                Bücher: 
                C für Dummies

 * ========================================
*/

#include <stdbool.h>

#ifndef PROX_RANGE_INIT
    #define PROX_RANGE_INIT     100
#endif


void CapSense_init();
bool CapSense_loop_func();

/* [] END OF FILE */

