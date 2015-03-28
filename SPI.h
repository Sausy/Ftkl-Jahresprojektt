
/* ========================================

    Author:     Michael Sausmikat
    Quellen:    Bücher: 
                C für Dummies                      

 * ========================================
*/


#ifndef _spi_h_
#define _spi_h_

    #include <stdint.h>
    
    void output_high();
    void output_low();
    void spi_write(int8_t outdata);
    int8_t spi_read(int8_t outdata);
    void spi_write_array( uint8_t len,uint8_t data[]);
    void spi_write_read_array(uint8_t tx_Data[],uint8_t tx_len, uint8_t *rx_data, uint8_t rx_len );
    uint8_t spi_write_read(uint8_t tx_Data);
    void delayMicroseconds(uint32_t $delay);
    void delayMillieconds(uint32_t delay);

#endif

/* [] END OF FILE */