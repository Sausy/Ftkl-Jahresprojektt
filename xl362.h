
/* ========================================

    Author: Michael Sausmikat
    Quellen:
            http://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/ADXL362_Sample_C_Code.zip               

 * ========================================
*/

#ifndef _adxl362_h_
#define _adxl362_h_
    
    typedef unsigned char byte;
    
    #include <stdint.h>
    
    
    void ADXL362_begin(); 
    void ADXL362_loop();
        
        
	void ADXL362_beginMeasure(); 
	int16_t ADXL362_readXData();
	int16_t ADXL362_readYData();
	int16_t ADXL362_readZData();
	void ADXL362_readXYZTData(int16_t *XData, int16_t *YData, int16_t *ZData, int16_t *Temperature);
	int16_t ADXL362_readTemp();

    void ADXL362_setupDCActivityInterrupt(int16_t threshold, byte time);	
	void ADXL362_setupDCInactivityInterrupt(int16_t threshold, int16_t time);
    void ADXL362_setupACActivityInterrupt(int16_t threshold, byte time);
	void ADXL362_setupACInactivityInterrupt(int16_t threshold, int16_t time);
	
	void ADXL362_checkAllControlRegs();
	
	byte ADXL362_SPIreadOneRegister(byte regAddress);
	void ADXL362_SPIwriteOneRegister(byte regAddress, byte regValue);
	int16_t  ADXL362_SPIreadTwoRegisters(byte regAddress);
	void ADXL362_SPIwriteTwoRegisters(byte regAddress, int16_t twoRegValue);
    
    

/* --- command --- */

#define XL362_REG_WRITE   0x0a
#define XL362_REG_READ    0x0b
#define XL362_FIFO_READ   0x0d

/* ------- Register names ------- */
/* -- part ID constants -- */
#define XL362_DEVID_AD           0x00
#define XL362_DEVID_MST          0x01
#define XL362_PARTID             0x02
#define XL362_REVID              0x03
#define XL362_XID                0x04

/* -- RO data -- */
#define XL362_XDATA8             0x08
#define XL362_YDATA8             0x09
#define XL362_ZDATA8             0x0A
#define XL362_STATUS             0x0B
#define XL362_FIFO_ENTRIES_L     0x0C
#define XL362_FIFO_ENTRIES_H     0x0D
#define XL362_XDATAL             0x0e
#define XL362_XDATAH             0x0f
#define XL362_YDATAL             0x10
#define XL362_YDATAH             0x11
#define XL362_ZDATAL             0x12
#define XL362_ZDATAH             0x13
#define XL362_TEMPL              0x14
#define XL362_TEMPH              0x15
#define XL362_X_ADCL             0x16
#define XL362_X_ADCH             0x17

/* -- Control and Config -- */
#define XL362_SOFT_RESET         0x1f
#define XL362_THRESH_ACTL        0x20
#define XL362_THRESH_ACTH        0x21
#define XL362_TIME_ACT           0x22
#define XL362_THRESH_INACTL      0x23
#define XL362_THRESH_INACTH      0x24
#define XL362_TIME_INACTL        0x25
#define XL362_TIME_INACTH        0x26
#define XL362_ACT_INACT_CTL      0x27
#define XL362_FIFO_CONTROL       0x28
#define XL362_FIFO_SAMPLES       0x29
#define XL362_INTMAP1            0x2a
#define XL362_INTMAP2            0x2b
#define XL362_FILTER_CTL         0x2c
#define XL362_POWER_CTL          0x2d
#define XL362_SELF_TEST          0x2e

/*----------------------------------------------------------------------
  Bit field definitions and register values
  ----------------------------------------------------------------------*/
//#define XL362_
/* register values for DEVID                                            */
/* The device ID should always read this value, The customer does not
  need to use this value but it can be samity checked to check that the
  device can communicate                                                */
 
#define XL362_ID              0xe2

/* Key Value in soft reset                                              */
#define XL362_SOFT_RESET_KEY      0x52


/* Registers THRESH_ACTL through TIME_INACTH just take values
   in lsbs or samples.
   There are no specific bit fields in these registers                  */

/* Bit values in ACT_INACT_CTL  (6 bits allocated)                      */
#define XL362_ACT_ENABLE      0x01
#define XL362_ACT_DISABLE     0x00
#define XL362_ACT_AC          0x02
#define XL362_ACT_DC          0x00
#define XL362_INACT_ENABLE    0x04
#define XL362_INACT_DISABLE   0x00
#define XL362_INACT_AC        0x08
#define XL362_INACT_DC        0x00
#define XL362_ACT_INACT_LINK  0x10
#define XL362_ACT_INACT_LOOP  0x20

/* Bit values in FIFO_CTL  (4 bits allocated)                            */
#define XL362_FIFO_MODE_OFF     0x00
#define XL362_FIFO_MODE_FIFO    0x01
#define XL362_FIFO_MODE_STREAM  0x02
#define XL362_FIFO_MODE_TRIGGER 0x03
#define XL362_FIFO_TEMP         0x04
#define XL362_FIFO_SAMPLES_AH   0x08



/* bit values in INTMAP1 and INTMAP2 (and status)                        */
#define XL362_INT_DATA_READY     0x01
#define XL362_INT_FIFO_READY     0x02
#define XL362_INT_FIFO_WATERMARK 0x04
#define XL362_INT_FIFO_OVERRUN   0x08
#define XL362_INT_ACT            0x10
#define XL362_INT_INACT          0x20
#define XL362_INT_AWAKE          0x40
#define XL362_INT_LOW            0x80

/* Bit values in FILTER_CTL  (8 bits allocated)                            */
#define XL362_RATE_400        0x05
#define XL362_RATE_200        0x04
#define XL362_RATE_100        0x03  /* default */
#define XL362_RATE_50         0x02
#define XL362_RATE_25         0x01
#define XL362_RATE_12_5       0x00

#define XL362_EXT_TRIGGER     0x08

#define XL362_AXIS_X          0x00
#define XL362_AXIS_Y          0x10
#define XL362_AXIS_Z          0x20

#define XL362_RANGE_2G        0x00
#define XL362_RANGE_4G        0x40
#define XL362_RANGE_8G        0x80


/* Bit values in POWER_CTL  (8 bits allocated)                            */
#define XL362_STANDBY           0x00
#define XL362_MEASURE_1D        0x01
#define XL362_MEASURE_3D        0x02
#define XL362_AUTO_SLEEP        0x04
#define XL362_SLEEP             0x08

#define XL362_LOW_POWER         0x00
#define XL362_LOW_NOISE1        0x10
#define XL362_LOW_NOISE2        0x20
#define XL362_LOW_NOISE3        0x30

#define XL362_EXT_CLOCK         0x40
#define XL362_EXT_ADC           0x80

/* Bit values in SELF_TEST  (1 bit allocated)                            */
#define XL362_SELFTEST_ON       0x01
#define XL362_SELFTEST_OFF      0x00

#endif


/* [] END OF FILE */
