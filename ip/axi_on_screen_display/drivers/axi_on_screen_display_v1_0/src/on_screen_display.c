/*
 * on_screen_display.c
 *
 *  Created on: Apr 19, 2017
 *      Author: davem
 */


#include <stdio.h>
#include <string.h>
#include <xstatus.h>
#include <xio.h>
#include <microblaze_sleep.h>
#include "on_screen_display.h"

#define REG_CONTROL               (0  << 2)
#define REG_STATUS                (1  << 2)
#define REG_IMAGE_WIDTH           (2  << 2)
#define REG_IMAGE_HEIGHT          (3  << 2)
#define REG_IMAGE_SIZE            (4  << 2)
#define REG_FG_COLOR              (5  << 2)
#define REG_BG_COLOR              (6  << 2)
#define REG_CONSOLE_CHAR          (7  << 2)
#define REG_CONSOLE_COMMAND       (8  << 2)
#define REG_TAB_COUNT             (9  << 2)
#define REG_X_START               (10 << 2)
#define REG_X_END                 (11 << 2)
#define REG_Y_START               (12 << 2)
#define REG_Y_END                 (13 << 2)
#define REG_ADAPTER_DEBUG	      (14 << 2)
#define REG_VERSION               (15 << 2)


#define BIT_CTRL_EN               0
#define BIT_CTRL_CLEAR_SCREEN_STB 1
#define BIT_CTRL_SCROLL_EN        4
#define BIT_CTRL_SCROLL_UP_STB    5
#define BIT_CTRL_SCROLL_DOWN_STB  6

/*****************************************************************************/
/**
 * Initialize On Screen Display
 *
 * Upon successful initialization the InstancePtr will populate with a
 *  a CDOSD instance
 *
 * @param InstancePtr is the On Screen Display Pointer
 * @param EffectiveAdr is the base address of the device
 *
 * @return
 *  - XST_SUCCESS if everything goes well
 *  - XST_FAILURE if an error
 *
 * @note    Caching must be turned off for this function to work.
 *****************************************************************************/
int CDOnScreenDisplay_Initialize(CDOSD * InstancePtr, uint32_t EffectiveAddr){
	InstancePtr->addr = EffectiveAddr;
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * Run a basic test to determine if the core is available
 *
 * @param InstancePtr is the On Screen Display Pointer
 *
 * @return
 *  - XST_SUCCESS if everything goes fine
 *  - XST_FAILURE if reset or self test failed
 *
 *****************************************************************************/
int CDOnScreenDisplay_SelfTest(CDOSD * InstancePtr){
	  uint32_t value;
	  value = XIo_In32(InstancePtr->addr + REG_VERSION);
	  //printf ("value: 0x%02X\r\n", (unsigned int) value);
	  return (value > 0) ? 1 : 0;
}

/*****************************************************************************/
/**
 * Enable the On Screen Display
 *
 * @param InstancePtr is the on screen display
 * @param enable:
 *  true: Enable on screen display
 *  false: Disable on screen display
 *
 * @return
 *  - XST_SUCCESS if everything goes fine
 *  - XST_FAILURE if reset or self test failed
 *
 *****************************************************************************/
int CDOnScreenDisplay_Enable(CDOSD * InstancePtr, uint8_t enable){
	uint32_t control = XIo_In32(InstancePtr->addr + REG_CONTROL);
	if (enable)
		control	|= (1 << BIT_CTRL_EN);
	else
		control	&= ~(1 << BIT_CTRL_EN);

	XIo_Out32(InstancePtr->addr + REG_CONTROL, control);
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * Clear the screen and sets the cursor at the top left corner
 *
 * @param InstancePtr is the on screen display
 *
 * @return
 *  - XST_SUCCESS if everything goes fine
 *  - XST_FAILURE if reset or self test failed
 *
 *****************************************************************************/
int CDOnScreenDisplay_ClearScreen(CDOSD * InstancePtr){
	uint32_t control = XIo_In32(InstancePtr->addr + REG_CONTROL);
	control	|= (1 << BIT_CTRL_CLEAR_SCREEN_STB);
	XIo_Out32(InstancePtr->addr + REG_CONTROL, control);
	usleep(100 * 1000);
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * Return true if enabled
 *
 * @param InstancePtr is the on screen display
 *
 * @return
 *  - 1: enabled
 *  - 0: not enabled
 *
 *****************************************************************************/
int CDOnScreenDisplay_IsEnabled(CDOSD * InstancePtr){
	return ((XIo_In32(InstancePtr->addr + REG_CONTROL) | (1 << BIT_CTRL_EN)) > 0);
}

/*****************************************************************************/
/**
 * Write a string to the screen
 *
 * @param InstancePtr is the on screen display
 * @param str pointer to a string
 * @param len length of the str in bytes
 *
 * @return
 *  - 1: enabled
 *  - 0: not enabled
 *
 *****************************************************************************/
void CDOnScreenDisplay_WriteString(CDOSD * InstancePtr, char* str, uint16_t len){
	for (int i = 0; i < len; i++){
		CDOnScreenDisplay_PutChar(InstancePtr, str[i]);
	}
}

/*****************************************************************************/
/**
 * Put a character on the screen
 *
 * @param InstancePtr is the on screen display
 * @param c, character to put on the screen
 *
 * @return
 *  - XST_SUCCESS if everything goes fine
 *  - XST_FAILURE if reset or self test failed
 *
 *****************************************************************************/
int CDOnScreenDisplay_PutChar(CDOSD * InstancePtr, char c){
	XIo_Out32(InstancePtr->addr + REG_CONSOLE_CHAR, (uint32_t) c);
	if ((c == '\r') || (c == '\n')){
	}
	return XST_SUCCESS;
}


/*****************************************************************************/
/**
 * Put a character on the screen, treat special characters as raw characters
 *
 * @param InstancePtr is the on screen display
 * @param c, character to put on the screen
 *
 * @return
 *  - XST_SUCCESS if everything goes fine
 *  - XST_FAILURE if reset or self test failed
 *
 *****************************************************************************/
int CDOnScreenDisplay_PutRawChar(CDOSD * InstancePtr, uint8_t c){
	uint32_t value = c;
	XIo_Out32(InstancePtr->addr + REG_CONSOLE_CHAR, (uint32_t) value);
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * Set the background color of the screen, color is represented as a 24-bit
 *  value:
 *    Red:    [23:16]
 *    Green:  [15: 8]
 *    Blue:   [ 7: 0]
 *
 * @param InstancePtr is the on screen display
 * @param color 24-bit color described above
 *
 * <b> Examples</b>
 *  CDOnScreenDisplay_SetBackgroundColor(&cdosd, 0x00FF0000); //Red
 *  CDOnScreenDisplay_SetBackgroundColor(&cdosd, 0x0000FF00); //Green
 *  CDOnScreenDisplay_SetBackgroundColor(&cdosd, 0x000000FF); //Blue
 *
 *****************************************************************************/
void CDOnScreenDisplay_SetBackgroundColor(CDOSD * InstancePtr, uint32_t color){
	XIo_Out32(InstancePtr->addr + REG_BG_COLOR, (uint32_t) color);
}

/*****************************************************************************/
/**
 * Dump all internal registers (FOR DEBUG)
 *
 * @param InstancePtr is the on screen display
 *
 *****************************************************************************/
void CDOnScreenDisplay_DumpRegisters(CDOSD * InstancePtr){
	for (int i =0; i < REG_VERSION + 1; i++){
		unsigned int reg = XIo_In32(InstancePtr->addr + i);
		printf("0x%08X: 0x%08X\r\n", i, reg);
	}
}

