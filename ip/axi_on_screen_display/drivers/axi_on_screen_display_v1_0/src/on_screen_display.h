
/*****************************************************************************/
/**
*
* @on_screen_display.h
* @addtogroup on_screen_display
* @{
* @details
* This is the Cospan Design on screen display device driver. The core will
* evenutally enable controls to draw elements on to a screen but at the
* moment it is simply a console output for a soft processor core like a
* microblaze.
*
* The goal of the console output is to simplify interaction with an MCU
* users can output status on something besides a UART connections with a
* host computer.
*
* The user simply outputs data to the screen using 'CDOnScreenDisplay_PutChar'
* If printf support is desired this function can be put into the 'putChar'
* function instead of the standard uart interface to the host.
*
* The screen will automatically scroll once it hits the bottom.
*
* 'Backspace' can be used to delete lines. cursor movement is not supported
* at this time.
*
* A font based on the Adafruit Graphics library is loaded in. Currently
* there is no support for a custom font.
*
* <b> Usage </b>
*
* The driver must first be initialized with
*
*   CDOnScreenDisplay_Initialize
*
* Then enabled with
*
*   CDOnScreenDisplay_Enable(1)
*
* Test if the core is enabled:
*
*   CDOnScreenDisplay_IsEnabled()
*
* When the driver is first initialized the screen is cleared and the cursor
* is moved to the top left of the screen. In order to clear the screen
* call the following function:
*
*   CDOnScreenDisplay_ClearScreen
*
* Change the background color with:
*
*   CDOnScreenDisplay_SetBackgroundColor(<24-bit color>)
*
*     24-bit color:
*       Red:    [23:16]
*       Green:  [15: 8]
*       Blue:   [ 7: 0]
*
*
* Writing a character to the screen is accomplished with
*
*   CDOnScreenDisplay_PutChar
*
* <b> Note: </b> special characters like carriage return, tab and backspace
*   will behave as expected, in order to output the raw characters use
*   the following function.
*
* <b> Note: </b> As stated above users can put this function within stdio's
*   'putChar' implementation to output 'printf' statements.
*
* An experimental function will write a raw hex value to the character buffer.
*   Instead of outputting a carriage return the character will be whatever the
*   character representation of that hex value.
*
*   CDOnScreenDisplay_PutRawChar
*
* Instead of using printf users can use the following functions
* to write strings to the screen.
*
*   CDOnScreenDisplay_WriteString(const char string, length)
*
* <b> Examples</b>
*
*   CDOSD cdosd;
*   char str[] = "Hello World!\n";
*
*   CDOnScreenDisplay_Initialize(&cdosd, XPAR_AXI_ON_SCREEN_DISPLAY_0_BASEADDR);
*   CDOnScreenDisplay_SetBackgroundColor(&cdosd, 0x000000); //Black
*   //CDOnScreenDisplay_SetBackgroundColor(&cdosd, 0x00FF00); //Green
*   CDOnScreenDisplay_Enable(&cdosd, 1);
*   CDOnScreenDisplay_WriteString(&cdosd, str, strlen(str));
*
*   //Clear the screen by commenting out the next line
*   //CDOnScreenDisplay_ClearScreen(&cdosd);
*
*   //Write out a single character at a time
*   //CDOnScreenDisplay_PutChar(&cdosd, 'A');
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 0.01  dm   04/19/17 First release
*
* </pre>
*
******************************************************************************/

#ifndef SRC_ON_SCREEN_DISPLAY_H_
#define SRC_ON_SCREEN_DISPLAY_H_

#include <stdint.h>

typedef struct {
	u32 addr;
}CDOSD;

int CDOnScreenDisplay_Initialize(CDOSD * InstancePtr, uint32_t EffectiveAddr);
int CDOnScreenDisplay_SelfTest(CDOSD * InstancePtr);
int CDOnScreenDisplay_Enable(CDOSD * InstancePtr, uint8_t enable);
int CDOnScreenDisplay_ClearScreen(CDOSD * InstancePtr);
int CDOnScreenDisplay_PutRawChar(CDOSD * InstancePtr, uint8_t c);
void CDOnScreenDisplay_SetBackgroundColor(CDOSD * InstancePtr, uint32_t color);
void CDOnScreenDisplay_WriteString(CDOSD * InstancePtr, char* str, uint16_t len);
int CDOnScreenDisplay_PutChar(CDOSD * InstancePtr, char c);
int CDOnScreenDisplay_IsEnabled(CDOSD * InstancePtr);
void CDOnScreenDisplay_DumpRegisters(CDOSD * InstancePtr);

#endif /* SRC_ON_SCREEN_DISPLAY_H_ */
