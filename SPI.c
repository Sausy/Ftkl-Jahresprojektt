
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    Bücher:    
                Diplomarbeit - BART                       

 * ========================================
*/
#include "SPI.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <project.h>

//-----------------------------------------
//---------SS-Pin-CMD----------------------
//-----------------------------------------

//---- sets SS high -----
void output_high(){
	//if($_Pin == 10)
    	SS_Write(1u);
}
//---- sets SS high -----
void output_low(){
	//if($_Pin == 10)
    	SS_Write(0u);
}

//-----------------------------------------
//---------SPI_low_level-------------------
//*** This function could be outsourced to  ***
//*** make the code more Portable           ***
//-----------------------------------------

//---- writes data to tx buffer _-----
//---- *** Blocking Funiction *** ----
void spi_write(int8_t outdata){
    SPI_iso_ClearRxBuffer();
    SPI_iso_WriteTxData(outdata);
    while (SPI_iso_GetRxBufferSize()==0);
}

//---- reads data from rx buffer -----
//---- *** Blocking Funiction *** ----
int8_t spi_read(int8_t outdata){
    uint8 result=0;
    SPI_iso_ClearRxBuffer();
    SPI_iso_WriteTxData(outdata);
    while (SPI_iso_GetRxBufferSize()==0);
    
    result=(uint8) SPI_iso_ReadRxData();
    return result;
}

//-----------------------------------------
//---------SPI-top-cmd---------------------
//-----------------------------------------
//
void spi_write_array( uint8_t len,   // Option: Number of bytes to be written on the SPI port
                      uint8_t data[] //Array of bytes to be written on the SPI port
                    )
{
  uint8_t i;
  for( i = 0; i < len; i++){
     spi_write((int8_t)data[i]);
  }
}

//---- SPI bidirctional comunication -----
void spi_write_read_array(uint8_t tx_Data[],//array of data to be written on SPI port 
          uint8_t tx_len, //length of the tx data arry
          uint8_t *rx_data,//Input: array that will store the data read by the SPI port
          uint8_t rx_len //Option: number of bytes to be read from the SPI port
          )
{

  uint8_t i;
  for( i = 0; i < tx_len; i++){
      spi_write(tx_Data[i]);
  }

  for( i = 0; i < rx_len; i++){
      rx_data[i] = (uint8_t)spi_read(0xFF);
  }

}

uint8_t spi_write_read(uint8_t tx_Data)
{

      spi_write(tx_Data);

      return ( (uint8_t)spi_read(0xFF) );

}




//---- waits for $_delayMillieconds microseconds -----
//*** This function could be outsourced to  ***
//*** make the code more Portable           ***
void delayMicroseconds(uint32_t $delay){
  CyDelayUs($delay);
}
//---- waits for $_delay microseconds -----
//*** This function could be outsourced to  ***
//*** make the code more Portable           ***
void delayMillieconds(uint32_t delay){
  CyDelay(delay);
}

/* [] END OF FILE */

