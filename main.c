
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    Internet:
                http://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm
                Bücher: 
                C für Dummies

 * ========================================
*/
#include <project.h>
#include "CCR.h"
#include "common.h"
#include "proximity.h"
#include "SPI.h"
#include "xl362.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//---- is set true if the tilt sensor is plugged to the curcuid ----
#ifndef _SPI_tilt_
    #define _SPI_tilt_ false
#endif    

//---- is set true if the nfc is plugged to the circuid ----
#ifndef _NFC_DEVICE_
    #define _NFC_DEVICE_ false
#endif    

//----- Those parameters have to be gather empiricly ----
//  TODO:   find the valid value, when a person steps on the step
#ifndef _tilt_MAX_PARAM_
    #define _tilt_MAX_PARAM_
    #define _XValue_MAX_ 20
    #define _YValue_MAX_ 20
    #define _ZValue_MAX_ 20
#endif


int16_t XValue, YValue, ZValue, Temperature;

//----------- ADXL362 functions ----------
void ADXL362_data(){
    char xl362_buffer[63];

	UART_UartPutString("\033[3C XVALUE=");

	ADXL362_readXYZTData(&XValue, &YValue, &ZValue, &Temperature);        
	
    #if DEBUGMODE
    	UART_UartPutString("\033[3C XVALUE=");
    		sprintf(xl362_buffer, "%04lu\n\r", (int16) XValue);
    		UART_UartPutString(xl362_buffer);
    	UART_UartPutString("\033[3C YVALUE=");
    		sprintf(xl362_buffer, "%04lu\n\r", (int16) YValue);
    		UART_UartPutString(xl362_buffer);
    	UART_UartPutString("\033[3C ZVALUE=");
    		sprintf(xl362_buffer, "%04lu\n\r", (int16) ZValue);
    		UART_UartPutString(xl362_buffer);
    	UART_UartPutString("\033[3C TEMPERATURE=");
    		sprintf(xl362_buffer, "%04lu\n\r", (int16) Temperature);
    		UART_UartPutString(xl362_buffer);
    #endif
}


//----------- NFC functions ----------
#if _NFC_DEVICE_
    // ----- tells if the NFC is in use ----
    //if it's not in use you shouldn't be abel to write (because its nonesense
    //if therer is no mobilephone edg.
    bool NFC_Ready(){
        return false;       
    }
    
    
    // ----- purpose is to hand over the data in the correct form -----
    void NFC_sendDATA(uint32_t *time_stamp, uint32_t *people_stamp){
        
        //if sucess then second and human counter should be set to zero
        *time_stamp = 0;
        *people_stamp=0;
        
        
    }
    
#endif 


int main()
{
    #if DEBUGMODE
        char main_foo_buffer[63];   //is just used for the sprintf function
    #endif
    
     // --- set var to init value ---
    glob_sec_counter = 0;
    glob_human_counter = 0;
    XValue = 0;
    YValue = 0;
    ZValue = 0;
    Temperature = 0;
    
    // --- Start devices ---
    Clock_Start();
    PWM_Start();
    CapSense_CSD_Start();
    UART_Start();
    
    #if _SPI_tilt_
        SPI_Start();    
        ADXL362_init();
    #endif       
    #if _NFC_DEVICE_
        NFC_Init(); //init nfc (that means i2c interface)
    #endif
    
    // --- Init Routins ---
    CyGlobalIntEnable;
    
    // --- allow global Interrupt --- 
    CapSense_init();
    
    #if DEBUGMODE
        UART_UartPutString("\033[H\033[2J");
        UART_UartPutString("..... FTKL Projekt Klettersteig Terminal .......");
        UART_UartPutString("\n\r");
        UART_UartPutString("\033[10C");
        UART_UartPutString("by Michael Sausmikat");
        UART_UartPutString("\n\n\r\033[s");
    #endif
    
    // --- Set init for LED`s ---
    Pin_GreenLED_Write(LED_OFF);
    Pin_BlueLED_Write(LED_OFF);
    Pin_BlueLED_Write(LED_OFF);
    
    set_bit_CCR(_ccr_count_bit_, 1);
    set_bit_CCR(_ccr_start_bit_, 1);
    set_bit_CCR(_ccr_stop_bit_, 0);            
        
    // --- Start counter interrupt ---
    counter_isr_StartEx(counter_isr_Interrupt);
        
	for(;;)
    {
        #if DEBUGMODE
            UART_UartPutString("\033[u\033[2B");
            UART_UartPutString("Time counter: ");
            sprintf(main_foo_buffer,"%08lu", (int32) glob_sec_counter);
            /*UART_UartPutString(main_foo_buffer);
            sprintf(main_foo_buffer, " %u ", (uint8_t)(glob_sec_counter >> 8));
            UART_UartPutString(main_foo_buffer);
            sprintf(main_foo_buffer, " %i", glob_sec_counter & 0xff);*/
            UART_UartPutString(main_foo_buffer);
        #endif

        if(CapSense_loop_func()){
            #if _SPI_tilt_==true
                ADXL362_wakup_sleep();
                ADXL362_data();
                
                if(YValue > _YValue_MAX_){
                    glob_human_counter++;
                    if(glob_human_counter >= UINT32_MAX)
                        glob_human_counter=0;
                  
                }
                
                
            #endif  
        }
        
        #if _NFC_DEVICE_
            //This Function must be given from the team mate that has writen the nfc code
            if(NFC_Ready()){
                CyGlobalIntDisable;                
                NFC_sendDATA(&glob_sec_counter, &glob_human_counter);                                  
                CyGlobalIntEnable;                
            }
        #endif                               
       
    }
}


/* [] END OF FILE */
