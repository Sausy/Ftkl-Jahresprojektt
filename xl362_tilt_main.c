
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    Internet:
                http://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/ADXL362_Sample_C_Code.zip
                https://github.com/annem/ADXL362   
            

 * ========================================
*/
#include "xl362.h"
//#include "xl362_io.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cytypes.h>
#include <project.h>
#include "SPI.h"

char xl362_buffer[63];


//---- Wakup------
void ADXL362_wakup_idle(){
    output_low();
    delayMicroseconds(2);
    output_high();
    
}
void ADXL362_wakup_sleep(){
    output_low();
    delayMillieconds(1); 
    output_high();
}



void ADXL362_init(){
        	       
    ADXL362_begin();                   // Setup SPI protocol, issue device soft reset
    ADXL362_beginMeasure();              // Switch ADXL362 to measure mode  
}



void ADXL362_begin() {
    
    //----- SPI ------------------
    //CPHA = CPOL = 0    MODE = 0
    //----------------------------

	ADXL362_SPIwriteOneRegister(XL362_SOFT_RESET, XL362_SOFT_RESET_KEY);  // Write to SOFT RESET, "R"
	delayMillieconds(10);

	#ifdef ADXL362_DEBUG
		UART_UartPutString("Soft Reset\n\r");
	#endif
 }
 

void ADXL362_beginMeasure() {
	byte temp = ADXL362_SPIreadOneRegister(XL362_POWER_CTL);	// read Reg 2D before modifying for measure mode

#ifdef ADXL362_DEBUG
	UART_UartPutString(  "Setting Measeurement Mode - Reg 2D before = "); 
	sprintf(xl362_buffer, "%02lu", (int8_t)temp);
	UART_UartPutString(xl362_buffer); 
#endif

	// turn on measurement mode
	byte tempwrite = temp | 0x02;			                    // turn on measurement bit in Reg 2D
	ADXL362_SPIwriteOneRegister(XL362_POWER_CTL, tempwrite);    // Write to POWER_CTL_REG, Measurement Mode
	delayMillieconds(10);	
  
#ifdef ADXL362_DEBUG
	temp = SPIreadOneRegister(XL362_POWER_CTL);
	UART_UartPutString(  ", Reg 2D after = "); 
	sprintf(xl362_buffer, "%02lu\n\r", (int8_t)temp);
	UART_UartPutString(xl362_buffer);
#endif
}

//
//  readXData(), readYData(), readZData(), readTemp()
//  Read X, Y, Z, and Temp registers
//
int16_t ADXL362_readXData(){
	int16_t XDATA = ADXL362_SPIreadTwoRegisters(XL362_XDATAL);
	
    #ifdef ADXL362_DEBUG
    	UART_UartPutString("XDATA = ");
    	sprintf(xl362_buffer, "%04lu\n\r", (int16_t)XDATA);
		UART_UartPutString(xl362_buffer);
    #endif
	
	return XDATA;
}

int16_t ADXL362_readYData(){
	int16_t YDATA = ADXL362_SPIreadTwoRegisters(XL362_YDATAL);

#ifdef ADXL362_DEBUG
	UART_UartPutString("\033[2C YDATA = ");
	sprintf(xl362_buffer, "%04lu\n\r", (int16_t)YDATA);
	UART_UartPutString(xl362_buffer);
#endif
	
	return YDATA;
}

int16_t ADXL362_readZData(){
	int16_t ZDATA = ADXL362_SPIreadTwoRegisters(XL362_ZDATAL);

	#ifdef ADXL362_DEBUG
		UART_UartPutString("\033[2C ZDATA = ");
		sprintf(xl362_buffer, "%04lu\n\r", (int16_t)ZDATA);
		UART_UartPutString(xl362_buffer);
	#endif

	return ZDATA;
}

int16_t ADXL362_readTemp(){
	int16_t TEMP = ADXL362_SPIreadTwoRegisters(XL362_TEMPL);

	#ifdef ADXL362_DEBUG
		UART_UartPutString("\033[2C TEMP = "); 
		sprintf(xl362_buffer, "%04lu\n\r", (int16_t)TEMP);
		UART_UartPutString(xl362_buffer);
	#endif

	return TEMP;
}

void ADXL362_readXYZTData(int16_t *XData, int16_t *YData, int16_t *ZData, int16_t *Temperature){

	//SET SS Low
	output_low();

	spi_write(0x0B);  // read instruction
	spi_write(0x0E);  // Start at XData Reg		
	*XData = spi_write_read(0x00);
	*XData = *XData + (spi_write_read(0x00) << 8);
	*YData = spi_write_read(0x00);
	*YData = *YData + (spi_write_read(0x00) << 8);
	*ZData = spi_write_read(0x00);
	*ZData = *ZData + (spi_write_read(0x00) << 8);
	*Temperature = spi_write_read(0x00);
	*Temperature = *Temperature + (spi_write_read(0x00) << 8);

	//SET SS HIGH
	output_high();
  
	#ifdef ADXL362_DEBUG
		UART_UartPutString("XDATA = "); 
			sprintf(xl362_buffer, "%04lu\n\r", (int16_t)*XData);
			UART_UartPutString(xl362_buffer);
		UART_UartPutString(" YDATA = "); 
			sprintf(xl362_buffer, "%04lu\n\r", (int16_t)*XData);
			UART_UartPutString(xl362_buffer);
	 	UART_UartPutString(" ZDATA = "); 
			sprintf(xl362_buffer, "%04lu\n\r", (int16_t)*XData);
			UART_UartPutString(xl362_buffer);
		UART_UartPutString(" Temperature = "); 	
			sprintf(xl362_buffer, "%04lu\n\r", (int16_t)*XData);
			UART_UartPutString(xl362_buffer);
	#endif
}

void ADXL362_setupDCActivityInterrupt(int16_t threshold, byte time){
	//  Setup motion and time thresholds
	ADXL362_SPIwriteTwoRegisters(0x20, threshold);
	ADXL362_SPIwriteOneRegister(0x22, time);

	// turn on activity interrupt
	byte ACT_INACT_CTL_Reg = ADXL362_SPIreadOneRegister(0x27);  // Read current reg value
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x01);     // turn on bit 1, ACT_EN  
	ADXL362_SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);       // Write new reg value 
	ACT_INACT_CTL_Reg = ADXL362_SPIreadOneRegister(0x27);       // Verify properly written

	#ifdef ADXL362_DEBUG
		UART_UartPutString("DC Activity Threshold set to ");  	
			sprintf(xl362_buffer, "%02lu", (int8_t)SPIreadTwoRegisters(0x20));
			UART_UartPutString(xl362_buffer);								//Serial.print(SPIreadTwoRegisters(0x20));
		UART_UartPutString(", Time threshold set to ");  	
			sprintf(xl362_buffer, "%02lu", (int8_t)SPIreadOneRegister(0x22));
			UART_UartPutString(xl362_buffer);								//Serial.print(SPIreadOneRegister(0x22)); 
		UART_UartPutString(", ACT_INACT_CTL Register is 0x");  	
			sprintf(xl362_buffer, "%x", (int16_t)ACT_INACT_CTL_Reg);
			UART_UartPutString(xl362_buffer);  								//Serial.println(ACT_INACT_CTL_Reg, HEX);
	#endif
}

void ADXL362_setupACActivityInterrupt(int16_t threshold, byte time){
	//  Setup motion and time thresholds
	ADXL362_SPIwriteTwoRegisters(0x20, threshold);
	ADXL362_SPIwriteOneRegister(0x22, time);
  
	// turn on activity interrupt
	byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);  // Read current reg value
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x03);     // turn on bit 2 and 1, ACT_AC_DCB, ACT_EN  
	ADXL362_SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);       // Write new reg value 
	ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);       // Verify properly written

	#ifdef ADXL362_DEBUG
		UART_UartPutString("AC Activity Threshold set to ");  	
			sprintf(xl362_buffer, "%02lu", (int8_t)SPIreadTwoRegisters(0x20));
			UART_UartPutString(xl362_buffer);				//Serial.print(SPIreadTwoRegisters(0x20));
		UART_UartPutString(", Time Activity set to ");  	
			sprintf(xl362_buffer, "%02lu", (int8_t)SPIreadTwoRegisters(0x20));
			UART_UartPutString(xl362_buffer);				//Serial.print(SPIreadOneRegister(0x22));  
		UART_UartPutString(", ACT_INACT_CTL Register is ");  	
			sprintf(xl362_buffer, "%x", ACT_INACT_CTL_Reg);
			UART_UartPutString(xl362_buffer);  				//Serial.println(ACT_INACT_CTL_Reg, HEX);
	#endif
}

void ADXL362_setupDCInactivityInterrupt(int16_t threshold, int16_t time){
	// Setup motion and time thresholds
	ADXL362_SPIwriteTwoRegisters(0x23, threshold);
	ADXL362_SPIwriteTwoRegisters(0x25, time);

	// turn on inactivity interrupt
	byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);   // Read current reg value 
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x04);      // turn on bit 3, INACT_EN  
	ADXL362_SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);        // Write new reg value 
	ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);        // Verify properly written

	#ifdef ADXL362_DEBUG
		Serial.print("DC Inactivity Threshold set to ");  Serial.print(SPIreadTwoRegisters(0x23));
		Serial.print(", Time Inactivity set to ");  Serial.print(SPIreadTwoRegisters(0x25));
		Serial.print(", ACT_INACT_CTL Register is ");  Serial.println(ACT_INACT_CTL_Reg, HEX);
	#endif
}

void ADXL362_setupACInactivityInterrupt(int16_t threshold, int16_t time){
	//  Setup motion and time thresholds
	ADXL362_SPIwriteTwoRegisters(0x23, threshold);
	ADXL362_SPIwriteTwoRegisters(0x25, time);
 
	// turn on inactivity interrupt
	byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);   // Read current reg value
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x0C);      // turn on bit 3 and 4, INACT_AC_DCB, INACT_EN  
	ADXL362_SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);        // Write new reg value 
	ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);        // Verify properly written

	#ifdef ADXL362_DEBUG
		UART_UartPutString("AC Inactivity Threshold set to ");  Serial.print(SPIreadTwoRegisters(0x23));
		UART_UartPutString(", Time Inactivity set to ");  Serial.print(SPIreadTwoRegisters(0x25)); 
		UART_UartPutString(", ACT_INACT_CTL Register is ");  Serial.println(ACT_INACT_CTL_Reg, HEX);
	#endif
}

void ADXL362_checkAllControlRegs(){
	output_low();

	spi_write(0x0B);  
	spi_write(0x20);

	#ifdef ADXL362_DEBUG
		Serial.println("Start Burst Read of all Control Regs - Library version 6-5-2014:");
		UART_UartPutString("Reg 20 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 21 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 22 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 23 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 24 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 25 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 26 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 27 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 28 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 29 = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 2A = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 2B = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 2C = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 2D = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
		UART_UartPutString("Reg 2E = "); 	sprintf(xl362_buffer, "%x ", spi_write_read(0x00)); UART_UartPutString(xl362_buffer);
	#endif

	output_high();
}

// Basic SPI routines to simplify code
// read and write one register
byte ADXL362_SPIreadOneRegister(byte regAddress){
	byte regValue = 0;
  
	output_low();


	spi_write(0x0B);  // read instruction
	spi_write(regAddress);
	regValue = spi_write_read(0x00);
	

	output_high();

	return regValue;
}

void ADXL362_ADXL362_SPIwriteOneRegister(byte regAddress, byte regValue){
  
	output_low();

	spi_write(0x0A);  // write instruction
	spi_write(regAddress);
	spi_write(regValue);

	output_high();
}

int16_t ADXL362_SPIreadTwoRegisters(byte regAddress){
	int16_t twoRegValue = 0;
  


	output_low();

	spi_write(0x0B);  // read instruction
	spi_write(regAddress);  
	twoRegValue = spi_write_read(0x00);
	twoRegValue = twoRegValue + (spi_write_read(0x00) << 8);
	
	output_high();

	return twoRegValue;
}  

void ADXL362_SPIwriteTwoRegisters(byte regAddress, int16_t twoRegValue){

	byte twoRegValueH = twoRegValue >> 8;
	byte twoRegValueL = twoRegValue;
  
	output_low();

	spi_write(0x0A);  		// write instruction
	spi_write(regAddress);  
	spi_write(twoRegValueL);
	spi_write(twoRegValueH);
	
	output_high();
}

/* [] END OF FILE */


