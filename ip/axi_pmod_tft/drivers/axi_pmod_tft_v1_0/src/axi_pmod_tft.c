/*****************************************************************************/
/**
*
* @file pmod_tft.c
* @addtogroup pmod_tft
* @{
*
* Implementation of the driver API functions for the Pmod TFT
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a dm   04/03/17 First release
*
* </pre>
*
******************************************************************************/

#define DEBUG 1

/***************************** Include Files *********************************/
#include <xstatus.h>
#include <xio.h>
#include <microblaze_sleep.h>
#include "axi_pmod_tft.h"

/************************** Constant Definitions *****************************/

//Registers
#define REG_CONTROL                 (0x00 << 2)
#define REG_STATUS                  (0x01 << 2)
#define REG_REG_DATA                (0x02 << 2)
#define REG_IMAGE_WIDTH				(0x03 << 2)
#define REG_IMAGE_HEIGHT            (0x04 << 2)
#define REG_IMAGE_SIZE              (0x05 << 2)
#define REG_VERSION					(0x06 << 2)

//Control Bits
#define CONTROL_ENABLE              0
#define CONTROL_ENABLE_INTERRUPT    1
#define CONTROL_COMMAND_MODE        2
#define CONTROL_BACKLIGHT_ENABLE    3
#define CONTROL_RESET_DISPLAY       4
#define CONTROL_COMMAND_WRITE       5
#define CONTROL_COMMAND_READ        6
#define CONTROL_COMMAND_PARAMETER   7
#define CONTROL_WRITE_OVERRIDE      8
#define CONTROL_CHIP_SELECT         9
#define CONTROL_ENABLE_TEARING      10
#define CONTROL_TP_RED				      12
#define CONTROL_TP_GREEN			      13
#define CONTROL_TP_BLUE				      14

//MCU Addresses
#define MEM_ADR_NOP                 0x00
#define MEM_ADR_RESET               0x01
#define MEM_ADR_PWR_MODE            0x0A
#define MEM_ADR_ADR_MODE            0x0B
#define MEM_ADR_DISP_MODE           0x0D
#define MEM_ADR_GET_TEAR_EF         0x0E
#define MEM_ADR_ENTER_SLEEP_MODE    0x10
#define MEM_ADR_EXIT_SLEEP_MODE     0x11
#define MEM_ADR_ENTER_PARTIAL_MODE  0x12
#define MEM_ADR_EXIT_PARTIAL_MODE   0x13
#define MEM_ADR_EXIT_INVERT_MODE    0x20
#define MEM_ADR_ENTER_INVERT_MODE   0x21
#define MEM_ADR_SET_GAMMA_CURVE     0x26
#define MEM_ADR_SET_DISPLAY_OFF     0x28
#define MEM_ADR_SET_DISPLAY_ON      0x29
#define MEM_ADR_SET_COLUMN_ADR      0x2A
#define MEM_ADR_SET_PAGE_ADR        0x2B
#define MEM_ADR_WRITE_MEM_START     0x2C
#define MEM_ADR_READ_MEM_START      0x2E
#define MEM_ADR_SET_PARTIAL_AREA    0x30
#define MEM_ADR_SET_SCROLL_AREA     0x33
#define MEM_ADR_SET_TEAR_OFF        0x34
#define MEM_ADR_SET_TEAR_ON         0x35
#define MEM_ADR_SET_ADR_MODE        0x36
#define MEM_ADR_SET_SCROLL_START    0x37
#define MEM_ADR_EXIT_IDLE_MODE      0x38
#define MEM_ADR_ENTER_IDLE_MODE     0x39
#define MEM_ADR_SET_PIXEL_FORMAT    0x3A
#define MEM_ADR_WRITE_MEM_CONT      0x3C
#define MEM_ADR_READ_MEM_CONT       0x3E
#define MEM_ADR_SET_TEAR_SCANLINE   0x44
#define MEM_ADR_GET_SCANLINE        0x45
#define MEM_ADR_READ_DDB            0xA1
#define MEM_ADR_SET_LCD_MODE        0xB0
#define MEM_ADR_GET_LCD_MODE        0xB1
#define MEM_ADR_SET_HORIZ_PERIOD    0xB4
#define MEM_ADR_GET_HORIZ_PERIOD    0xB5
#define MEM_ADR_SET_VERT_PERIOD     0xB6
#define MEM_ADR_GET_VERT_PERIOD     0xB7
#define MEM_ADR_SET_GPIO_CONF       0xB8
#define MEM_ADR_GET_GPIO_CONF       0xB9
#define MEM_ADR_SET_GPIO_VAL        0xBA
#define MEM_ADR_GET_GPIO_STATUS     0xBB
#define MEM_ADR_SET_POST_PROC       0xBC
#define MEM_ADR_GET_POST_PROC       0xBD
#define MEM_ADR_SET_PWM_CONF        0xBE
#define MEM_ADR_GET_PWM_CONF        0xBF
#define MEM_ADR_SET_LCD_GEN0        0xC0
#define MEM_ADR_GET_LCD_GEN0        0xC1
#define MEM_ADR_SET_LCD_GEN1        0xC2
#define MEM_ADR_GET_LCD_GEN1        0xC3
#define MEM_ADR_SET_LCD_GEN2        0xC4
#define MEM_ADR_GET_LCD_GEN2        0xC5
#define MEM_ADR_SET_LCD_GEN3        0xC6
#define MEM_ADR_GET_LCD_GEN3        0xC7
#define MEM_ADR_SET_GPIO0_ROP       0xC8
#define MEM_ADR_GET_GPIO0_ROP       0xC9
#define MEM_ADR_SET_GPIO1_ROP       0xCA
#define MEM_ADR_GET_GPIO1_ROP       0xCB
#define MEM_ADR_SET_GPIO2_ROP       0xCC
#define MEM_ADR_GET_GPIO2_ROP       0xCD
#define MEM_ADR_SET_GPIO3_ROP       0xCE
#define MEM_ADR_GET_GPIO3_ROP       0xCF
#define MEM_ADR_SET_DBC_CONF        0xD0
#define MEM_ADR_GET_DBC_CONF        0xD1
#define MEM_ADR_SET_DBC_TH          0xD4
#define MEM_ADR_GET_DBC_TH          0xD5
#define MEM_ADR_SET_PLL             0xE0
#define MEM_ADR_SET_PLL_MN          0xE2
#define MEM_ADR_GET_PLL_MN          0xE3
#define MEM_ADR_GET_PLL_STATUS      0xE4
#define MEM_ADR_SET_LSHIFT_FREQ     0xE6
#define MEM_ADR_GET_LSHIFT_FREQ     0xE7
#define MEM_ADR_SET_PIX_DAT_INT     0xF0
#define MEM_ADR_GET_PIX_DAT_INT     0xF1

/*************************** Private Functions *******************************/
//Private Functions
void CDAxiPmodTft_SetControl(CDPmodTft * InstancePtr, uint32_t value);
uint16_t CDAxiPmodTft_PixelFrequencyToUint16(uint16_t PllClock, float pixel_frequency);
void CDAxiPmodTft_LowLevelReset(CDPmodTft * InstancePtr);
uint32_t CDAxiPmodTft_GetControl(CDPmodTft * InstancePtr);
int CDAxiPmodTft_WriteDriverRegister(CDPmodTft * InstancePtr, uint8_t address, uint8_t *buffer, uint16_t length);
int CDAxiPmodTft_ReadDriverRegister(CDPmodTft * InstancePtr, uint8_t address, uint8_t *buffer, uint16_t length);
uint32_t CDAxiPmodTft_PixelFrequencyToUint(uint16_t PllClock, float PixelFreq);
void CDAxiPmodTft_SetImageWidth(CDPmodTft * InstancePtr, u32 width);
void CDAxiPmodTft_SetImageHeight(CDPmodTft * InstancePtr, u32 height);
void CDAxiPmodTft_SetImageSize(CDPmodTft * InstancePtr, u32 size);

/*****************************************************************************/

int CDAxiPmodTft_Initialize(CDPmodTft * InstancePtr, uint32_t EffectiveAddr){

  uint8_t buffer[20];
  //float clk_freq = ((float)XPAR_CPU_M_AXI_DP_FREQ_HZ) / 1000000;

  //Initialize the Instance Pointer
  //memset((void *)InstancePtr, 0, sizeof(CDPmodTft));
  //Set the address
  InstancePtr->addr = EffectiveAddr;
  InstancePtr->control = 0x00;      //Disable Everything
  InstancePtr->backlight_enabled = 0x00;  //Disable Backlight

  //Make sure we can at least talk to the device
  if (CDAxiPmodTft_Selftest(InstancePtr) == XST_FAILURE){
    //printf ("Failed self test!\r\n");
    return XST_FAILURE;
  }
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  CDAxiPmodTft_LowLevelReset(InstancePtr);

  //Enable the GPIO (which is a display enable for the LCD)
  buffer[0] = 0x00;
  buffer[1] = 0x01;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_GPIO_CONF, buffer, 2);

  //Start the PLL
  buffer[0] = 0x01;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_PLL, buffer, 1);
  usleep(1 * 1000);

  //Lock the PLL
  buffer[0] = 0x03;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_PLL, buffer, 1);
  usleep(1 * 1000);

  //Setup the LCD Mode
  buffer[0] = 0x20;
  buffer[1] = 0x00;
  buffer[2] = ( (uint8_t) (0xFF & ((LCD_WIDTH - 1)  >> 8)));
  buffer[3] = ( (uint8_t) (0xFF & ((LCD_WIDTH - 1)  >> 0)));
  buffer[4] = ( (uint8_t) (0xFF & ((LCD_HEIGHT - 1) >> 8)));
  buffer[5] = ( (uint8_t) (0xFF & ((LCD_HEIGHT - 1) >> 0)));
  buffer[6] = 0x00;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_LCD_MODE, buffer, 7);

  //Set Pixel Format (R G B Format: 6 6 6)
  //buffer[0] = 0x70;
  //CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_PIXEL_FORMAT, buffer, 1);

  //Set Pixel Data Interface (8-bits at a time)
  buffer[0] = 0x00;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_PIX_DAT_INT, buffer, 1);

  //Setup PCLK Frequency
  buffer[0] = 0x01;
  buffer[1] = 0x45;
  buffer[2] = 0x47;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_LSHIFT_FREQ, buffer, 3);


  /*
  buffer[0] = 0x00;
  buffer[1] = 0x00;
  buffer[2] = 0x00;
  CDAxiPmodTft_ReadDriverRegister(InstancePtr, MEM_ADR_GET_PLL_STATUS, buffer, 1);
  if (DEBUG) printf ("PLL Locked: %d\r\n", (buffer[0] == 4));
  */


  //Setup Horizontal Behavior
  buffer[0] = ( (uint8_t) (0xFF & (HSYNC_TOTAL >> 8)));
  buffer[1] = ( (uint8_t) (0xFF & (HSYNC_TOTAL >> 0)));
  buffer[2] = ( (uint8_t) (0xFF & (HBLANK >> 8)));
  buffer[3] = ( (uint8_t) (0xFF & (HBLANK >> 0)));
  buffer[4] = ( (uint8_t) (0xFF & (HSYNC_PULSE >> 0)));
  buffer[5] = ( (uint8_t) (0xFF & (HSYNC_PULSE_START >> 8)));
  buffer[6] = ( (uint8_t) (0xFF & (HSYNC_PULSE_START >> 0)));
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_HORIZ_PERIOD, buffer, 7);

  //Setup Vertical Behavior
  buffer[0] = ( (uint8_t) (0xFF & (VSYNC_TOTAL >> 8)));
  buffer[1] = ( (uint8_t) (0xFF & (VSYNC_TOTAL >> 0)));
  buffer[2] = ( (uint8_t) (0xFF & (VBLANK >> 8)));
  buffer[3] = ( (uint8_t) (0xFF & (VBLANK >> 0)));
  buffer[4] = ( (uint8_t) (0xFF & (VSYNC_PULSE >> 0)));
  buffer[5] = ( (uint8_t) (0xFF & (VSYNC_PULSE_START >> 8)));
  buffer[6] = ( (uint8_t) (0xFF & (VSYNC_PULSE_START >> 0)));
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_VERT_PERIOD, buffer, 7);

  //Setup Column Address
  buffer[0] = ( (uint8_t) (0xFF & (COLUMN_START >> 8)));
  buffer[1] = ( (uint8_t) (0xFF & (COLUMN_START >> 0)));
  buffer[2] = ( (uint8_t) (0xFF & ((COLUMN_END) >> 8)));
  buffer[3] = ( (uint8_t) (0xFF & ((COLUMN_END) >> 0)));
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_COLUMN_ADR, buffer, 4);

  //Setup Page Address
  buffer[0] = ( (uint8_t) (0xFF & (PAGE_START >> 8)));
  buffer[1] = ( (uint8_t) (0xFF & (PAGE_START >> 0)));
  buffer[2] = ( (uint8_t) (0xFF & ((PAGE_END) >> 8)));
  buffer[3] = ( (uint8_t) (0xFF & ((PAGE_END) >> 0)));
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_PAGE_ADR, buffer, 4);

  //Setup address mode
  buffer[0] = 0x00;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_ADR_MODE, buffer, 1);

  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_EXIT_PARTIAL_MODE, NULL, 0);
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_EXIT_IDLE_MODE, NULL, 0);
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_DISPLAY_ON, NULL, 0);

   CDAxiPmodTft_SetImageWidth(InstancePtr, LCD_WIDTH);
  CDAxiPmodTft_SetImageHeight(InstancePtr, LCD_HEIGHT);
  CDAxiPmodTft_SetImageSize(InstancePtr, LCD_WIDTH * LCD_HEIGHT);

  //Configure the driver to output tear signals only on VBLANKS
  buffer[0] = 0x00;
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_SET_TEAR_ON, buffer, 1);

  InstancePtr->control |= 1 << CONTROL_ENABLE_TEARING;
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  return XST_SUCCESS;
}

int CDAxiPmodTft_Selftest(CDPmodTft * InstancePtr){
  uint32_t value;
  //value = 0x000000CD;
  //XIo_Out32(InstancePtr->addr + REG_REG_DATA, value);
  //XIo_Out32(InstancePtr->addr + 0x02, value);
  value = XIo_In32(InstancePtr->addr + REG_VERSION);
  //value = XIo_In32(InstancePtr->addr + 0x02);
  //printf ("value: 0x%02X\r\n", (unsigned int) value);
  return (value > 0) ? XST_SUCCESS : XST_FAILURE;
}

/*****************************************************************************/
/**
 * Ruturn the total pixel count of the image (Width * Height)
 *
 * @param InstancePtr is the Pmod TFT pointer
 *
 * @return
 *  Total Pixel Count
 *
 *****************************************************************************/
//uint32_t CDAxiPmodTft_GetPixelCount(CDPmodTft * InstancePtr) {
//  return XIo_In32(InstancePtr->addr + REG_PIXEL_COUNT);
//}

/*****************************************************************************/
/**
 * Sets the total pixel count of the image (Width * Height)
 *
 * @param InstancePtr is the Pmod TFT pointer
 * @param pixelCount the total number of pixels to send to the screen
 *
 * @return
 *  Nothing
 *
 *****************************************************************************/
//void CDAxiPmodTft_SetPixelCount(CDPmodTft * InstancePtr, uint32_t pixelCount) {
//  XIo_Out32(InstancePtr->addr + REG_PIXEL_COUNT, pixelCount);
//}

uint32_t CDAxiPmodTft_GetStatus(CDPmodTft * InstancePtr){
  return XIo_In32(InstancePtr->addr + REG_STATUS);
}

uint32_t CDAxiPmodTft_GetImageWidth(CDPmodTft * InstancePtr){
  return LCD_WIDTH;
}

uint32_t CDAxiPmodTft_GetImageHeight(CDPmodTft * InstancePtr){
  return LCD_HEIGHT;
}

void CDAxiPmodTft_SetBacklightLevel(CDPmodTft * InstancePtr, uint8_t pwm){
  /* There wasn't enough pins on the PMODs so the Driver is controlling
   * The driver needs to control the chip select
   */
  //In the future there should be manual control of how bright the
  //backlight is, but for now I'll be happy if it turns on at all
  uint8_t buffer[] = {0x0E, 0xFF, 0x01, 0x00, 0x00, 0x00};
  uint16_t length = (uint16_t) sizeof(buffer);

  buffer[1] = pwm;


  InstancePtr->backlight_enabled = (pwm > 0);
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, (uint8_t) MEM_ADR_SET_PWM_CONF, buffer, length);
}

/*****************************************************************************/
/**
 * Enable the backlight
 *
 * @param InstancePtr is the Pmod TFT pointer
 *
 * @return
 *  - 1 = On
 *  - 0 = Off
 *
 * @note
 *****************************************************************************/
int CDAxiPmodTft_IsBacklightEnabled(CDPmodTft * InstancePtr){
  return InstancePtr->backlight_enabled;
}

int CDAxiPmodTft_Reset(CDPmodTft * InstancePtr){
  return CDAxiPmodTft_Initialize(InstancePtr, InstancePtr->addr);
}

void CDAxiPmodTft_RunTestPattern(CDPmodTft * InstancePtr, uint8_t red, uint8_t green, uint8_t blue){
	if (red){
		InstancePtr->control |= (1 << CONTROL_TP_RED);
	}
	if (green){
		InstancePtr->control |= (1 << CONTROL_TP_GREEN);
	}
	if (blue){
		InstancePtr->control |= (1 << CONTROL_TP_BLUE);
	}
	CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
	InstancePtr->control &= ~(1 << CONTROL_TP_RED);
	InstancePtr->control &= ~(1 << CONTROL_TP_GREEN);
	InstancePtr->control &= ~(1 << CONTROL_TP_BLUE);
}

void CDAxiPmodTft_DumpRegisters(CDPmodTft * InstancePtr){
	uint32_t reg = 0;
	for (int i = 0; i < 4; i++){
		reg = XIo_In32(InstancePtr->addr + i);
		//printf ("Register: 0x%02X: 0x%08X\r\n", (unsigned int) i, (unsigned int) reg);
	}
}

/**************************** Private Functions ******************************/
uint32_t CDAxiPmodTft_GetControl(CDPmodTft * InstancePtr){
  return XIo_In32(InstancePtr->addr + REG_CONTROL);
}

void CDAxiPmodTft_SetControl(CDPmodTft * InstancePtr, uint32_t value){
  XIo_Out32(InstancePtr->addr + REG_CONTROL, value);
}

void CDAxiPmodTft_EnableTearing(CDPmodTft * InstancePtr, int enable){
  if (enable)
    InstancePtr->control |= 1 << CONTROL_ENABLE_TEARING;
  else
    InstancePtr->control &= ~(1 << CONTROL_ENABLE_TEARING);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
}

int CDAxiPmodTft_IsTearingEnabled(CDPmodTft * InstancePtr){
  return (InstancePtr->control & (1 << CONTROL_ENABLE_TEARING)) ? 1 : 0;
}

void CDAxiPmodTft_LowLevelReset(CDPmodTft * InstancePtr){

  //Reset the LCD
  InstancePtr->control &= ~(1 << CONTROL_ENABLE);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  InstancePtr->control |= (1 << CONTROL_ENABLE);
  InstancePtr->control |= (1 << CONTROL_WRITE_OVERRIDE);
  InstancePtr->control |= (1 << CONTROL_CHIP_SELECT);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  InstancePtr->control |= (1 << CONTROL_RESET_DISPLAY);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
  usleep(400 * 1000); //XXX: Double check these timer values... they seem long
  InstancePtr->control &= ~(1 << CONTROL_RESET_DISPLAY);

  //Toggle Chip Select
  InstancePtr->control &= ~(1 << CONTROL_WRITE_OVERRIDE);
  InstancePtr->control &= ~(1 << CONTROL_CHIP_SELECT);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  InstancePtr->control |= 1 << CONTROL_CHIP_SELECT;
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  //Reset the internal memory pointer of the driver
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_RESET, NULL, 0);
  usleep(500 * 1000); //XXX: Double check these timer values... they seem long
  CDAxiPmodTft_WriteDriverRegister(InstancePtr, MEM_ADR_RESET, NULL, 0);
  usleep(200 * 1000); //XXX: Double check these timer values... they seem long

  //Toggle Chip Select
  InstancePtr->control &= ~(1 << CONTROL_CHIP_SELECT);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  InstancePtr->control |= 1 << CONTROL_CHIP_SELECT;
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);



}

int CDAxiPmodTft_WriteDriverRegister(CDPmodTft * InstancePtr, uint8_t address, uint8_t *buffer, uint16_t length){

  //Go to command mode
  InstancePtr->control |= (1 << CONTROL_COMMAND_MODE);
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_PARAMETER);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  //Put the address in the data register
  XIo_Out32(InstancePtr->addr + REG_REG_DATA, address);

  //Send the Register Write Command Down
  InstancePtr->control |= (1 << CONTROL_COMMAND_WRITE);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
  //The bit is self clearing
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_WRITE);

  //We're going to send down a parameter
  InstancePtr->control |= (1 << CONTROL_COMMAND_PARAMETER);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  for (uint16_t i = 0; i < length; i++){

    //Put the data in the data register
    XIo_Out32(InstancePtr->addr + REG_REG_DATA, buffer[i]);

    //Send the Register Write Command Down
    InstancePtr->control |= (1 << CONTROL_COMMAND_WRITE);
    CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
    //The bit is self clearing
    InstancePtr->control &= ~(1 << CONTROL_COMMAND_WRITE);
  }
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_PARAMETER);
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_MODE);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  return XST_SUCCESS;
}

int CDAxiPmodTft_ReadDriverRegister(CDPmodTft * InstancePtr, uint8_t address, uint8_t *buffer, uint16_t length){
  //Go to command mode
  InstancePtr->control |= (1 << CONTROL_COMMAND_MODE);
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_PARAMETER);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  //Put the address in the data register
  XIo_Out32(InstancePtr->addr + REG_REG_DATA, address);

  //Send the Register Write Command Down
  InstancePtr->control |= (1 << CONTROL_COMMAND_WRITE);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
  //The bit is self clearing
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_WRITE);

  //We're going to send down a parameter
  InstancePtr->control |= (1 << CONTROL_COMMAND_PARAMETER);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  for (uint16_t i = 0; i < length; i++){

	usleep(10000);
    //Send the Register Write Command Down
    InstancePtr->control |= (1 << CONTROL_COMMAND_READ);
    CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);
    //The bit is self clearing
    InstancePtr->control &= ~(1 << CONTROL_COMMAND_READ);

    //Put the data in the data register
    buffer[i] = (uint8_t) XIo_In32(InstancePtr->addr + REG_REG_DATA);
  }
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_PARAMETER);
  InstancePtr->control &= ~(1 << CONTROL_COMMAND_MODE);
  CDAxiPmodTft_SetControl(InstancePtr, InstancePtr->control);

  return XST_SUCCESS;
}

uint32_t CDAxiPmodTft_PixelFrequencyToUint(uint16_t PllClock, float PixelFreq){
  uint32_t value = 0;
  value = (uint32_t) (((PixelFreq) / PllClock) * 0x100000);
  return value;
}

void CDAxiPmodTft_SetImageWidth(CDPmodTft * InstancePtr, u32 width){
  XIo_Out32(InstancePtr->addr + REG_IMAGE_WIDTH, width);
}

void CDAxiPmodTft_SetImageHeight(CDPmodTft * InstancePtr, u32 height){
  XIo_Out32(InstancePtr->addr + REG_IMAGE_HEIGHT, height);
}

void CDAxiPmodTft_SetImageSize(CDPmodTft * InstancePtr, u32 size){
	XIo_Out32(InstancePtr->addr + REG_IMAGE_SIZE, size);
}

