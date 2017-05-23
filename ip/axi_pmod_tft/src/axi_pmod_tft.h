

/*****************************************************************************/
/**
*
* @file pmod_tft.h
* @addtogroup pmod_tft
* @{
* @details
* This is the Cospan Design Pmod TFT device driver. The core will transfer
* raw RGB image data to a Pmod TFT LCD Screen using the SS1963 LCD driver
*
* It supports the following features:
*   - Enable/Disable LCD screen
*   - Enable/Disable the backlight
*
* The device does not work without the use of a AXI-Stream to Video core.
*
* For a more comprehensive description refer to the hardware product guide
*
* <b> Usage </b>
*
* The driver must first be initialized with
*
*   CDAxiPmodTft_Initialize
*
* When the driver is first initialized the core and the driver is reset and
* configured. If the user must reset the core call:
* 
*   CDAxiPmodTft_Reset
*
* <b> Note: </b> reset is not required when first initialized
*
* The LCD is too dark and the user must enable the backlight to view anything
*
*   CDAxiPmodTft_EnableBacklight
* 
* This core is not designed to operate independently. It should be coupled
* with a video timing core, AXI4 Stream to video
*
* <b> Examples</b>
*
* We provide an example that enables users to write data to the LCD screen
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 0.01  dm   04/03/17 First release
*
* </pre>
*
******************************************************************************/


#ifndef __PMOD_TFT_H__
#define __PMOD_TFT_H__

#include <stdint.h>

typedef struct {
  u32 addr;
  u32 control;
  int backlight_enabled;
}CDPmodTft;

#define LCD_WIDTH                   480
#define LCD_HEIGHT                  272

#define HSYNC_TOTAL                 525
#define HBLANK                      68
#define HSYNC_PULSE                 40
#define HSYNC_PULSE_START           0

#define VSYNC_TOTAL                 360
#define VBLANK                      12
#define VSYNC_PULSE                 9
#define VSYNC_PULSE_START           0

#define COLUMN_START                0
#define COLUMN_END                  LCD_WIDTH - 1
#define PAGE_START                  0
#define PAGE_END                    LCD_HEIGHT - 1


//Functions
int CDAxiPmodTft_Initialize(CDPmodTft * InstancePtr, uint32_t EffectiveAddr);
int CDAxiPmodTft_Selftest(CDPmodTft * InstancePtr);
//uint32_t CDAxiPmodTft_GetPixelCount(CDPmodTft * InstancePtr);
//void CDAxiPmodTft_SetPixelCount(CDPmodTft * InstancePtr, uint32_t pixelCount);
uint32_t CDAxiPmodTft_GetStatus(CDPmodTft * InstancePtr);
uint32_t CDAxiPmodTft_GetImageWidth(CDPmodTft * InstancePtr);
uint32_t CDAxiPmodTft_GetImageHeight(CDPmodTft * InstancePtr);
void CDAxiPmodTft_SetBacklightLevel(CDPmodTft * InstancePtr, uint8_t pwm);
int CDAxiPmodTft_Reset(CDPmodTft * InstancePtr);
void CDAxiPmodTft_RunTestPattern(CDPmodTft * InstancePtr, uint8_t red, uint8_t green, uint8_t blue);
void CDAxiPmodTft_DumpRegisters(CDPmodTft * InstancePtr);


#endif //__PMOD_TFT_H__
