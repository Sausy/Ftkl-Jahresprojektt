
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    Internet:
                http://www.element14.com/community/message/76985/l/psoc-4-pioneer-kit-community-project015-capsense-proximity-detection#76985                       

 * ========================================
*/

#include <project.h>
#include "proximity.h"
#include "LED_RGB.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


int16 proximityCounts, proximityMax;
uint16 hue;


void CapSense_init(){
    
    hue=0;
    
    /* Start components */
    /* Start LED_RGB module */
    LED_RGB_Start();
    
    /* Start CSD proximity sensor */
    CapSense_CSD_Start();
    CapSense_CSD_EnableWidget(CapSense_CSD_SENSOR_PROXIMITYSENSOR0_0__PROX);
    CapSense_CSD_InitializeAllBaselines();
    
    /* Perform initial proximity read to set max and min */
    CapSense_CSD_UpdateEnabledBaselines();    
	/* Start scanning all enabled sensors */
	CapSense_CSD_ScanEnabledWidgets();
    /* Wait for scanning to complete */
	while(CapSense_CSD_IsBusy() != 0);
    
    /* Set initial limits */
    proximityMax = PROX_RANGE_INIT;
}



bool CapSense_loop_func(){
    
    char capsense_loop_buffer[63];
    /* Read capsense proximity sensor */
    /* Update all baselines */
    CapSense_CSD_UpdateEnabledBaselines();
    /* Start scanning all enabled sensors */
    CapSense_CSD_ScanEnabledWidgets();
    /* Wait for scanning to complete */
    while(CapSense_CSD_IsBusy() != 0);
    proximityCounts = CapSense_CSD_SensorSignal[0] - 5;
    
    /* Floor the counts so no negative values are displayed */
    if(proximityCounts<0)
    {
        proximityCounts=0;
    }
    
    /* Set new limits if appropriate */
    if(proximityCounts>proximityMax)
    {
        proximityMax = proximityCounts;
    }
    
    /* Calculate scaled hue value and write it */
    hue = ((uint32) proximityCounts) * UINT16_MAX / proximityMax;
    
    LED_RGB_SetColorCircle(hue);
    

    UART_UartPutString("\033[u");
    UART_UartPutString("Data: ");
    sprintf(capsense_loop_buffer,"%08lu", (int32) proximityCounts);
    UART_UartPutString(capsense_loop_buffer);
    
    if(proximityCounts > 2000)
        return true;
           
    return false;
    
}

/* [] END OF FILE */
