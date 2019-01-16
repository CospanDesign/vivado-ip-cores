
#include <stdint.h>
#include <stdbool.h>
#include <xil_io.h>
#include "xil_types.h"
#include "i2c_control.h"
#include "sleep.h"
#if defined(XPAR_XINTC_NUM_INSTANCES)
#include "xintc.h"
#endif

//i2c_control_t ic;
#ifndef I2C_DEBUG
#define I2C_DEBUG 0
#endif

enum I2C_STATE {
  IDLE        = 0,
  WRITE_REG   = 1,
  READ_REG    = 2,
  READ_RSTART = 3,
  READ_ADDR   = 4,
  WRITE       = 5,
  READ        = 6
};

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

void i2c_set_register_bit(i2c_control_t *ic, uint32_t addr, uint32_t bit);
void i2c_clear_register_bit(i2c_control_t *ic, uint32_t addr, uint32_t bit);
void i2c_enable_interrupts(i2c_control_t *ic, bool enable);
uint32_t i2c_read_register(i2c_control_t *ic, uint32_t addr);
void i2c_write_register(i2c_control_t *ic, uint32_t addr, uint32_t value);
void i2c_enable_register_bit(i2c_control_t *ic, uint32_t addr, uint32_t bit, bool enable);
bool i2c_is_register_bit_set(i2c_control_t *ic, uint32_t addr, uint32_t bit);
uint32_t i2c_get_clock_rate(i2c_control_t *ic);
uint32_t i2c_get_clock_divisor(i2c_control_t *ic);
uint32_t i2c_check_for_errors(i2c_control_t *ic);

/*****************************************************************************
 * Public Functions
 *****************************************************************************/
/*
void setup_i2c_control_with_debug(i2c_control_t *ic, uint32_t base_addr){
  setup_i2c_control(ic, base_addr);
  I2C_DEBUG = true;
}
*/

void setup_i2c_control(i2c_control_t *ic, uint32_t base_addr){

  ic->base_addr = base_addr;
  //I2C_DEBUG = false;

  ic->state = IDLE;
  ic->buffer = NULL;
  ic->buffer_pos = 0;
  ic->buffer_len = 0;
  ic->print_errors = false;

  ic->interrupt_enable = false;
  ic->rxf_cb = NULL;
  ic->txf_cb = NULL;
  ic->error_cb = NULL;
  ic->rxf_user = NULL;
  ic->txf_user = NULL;
  ic->error_user = NULL;

}

void i2c_control_interrupt(i2c_control_t *ic, void * callback_reg){
  uint32_t errors;
  if (errors = i2c_check_for_errors(ic), errors > 0){
    //Cancel the rest of the transaction
    ic->state = IDLE;
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    if (ic->error_cb != NULL)
      ic->error_cb(errors, ic->error_user);
  }

  switch(ic->state){
    case (WRITE_REG):
      i2c_write_register(ic, REG_I2C_TRANSMIT, ic->reg);
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
      ic->state = WRITE;
      break;
    case (READ_REG):
      i2c_write_register(ic, REG_I2C_TRANSMIT, ic->reg);
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
      ic->state = READ_RSTART;
      break;
    case (READ_RSTART):
      //i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_START);
      i2c_write_register(ic, REG_I2C_TRANSMIT, ((ic->addr << 1) | 0x0));
      //Restart
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ | CMD_I2C_BIT_START);
      ic->state = READ;
      break;
    case (WRITE):
      if (ic->buffer_pos == ic->buffer_len){
        //Wrote Everything, we're done
  			i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
        ic->state = IDLE;
        if (ic->txf_cb != NULL)
          ic->txf_cb(ic->txf_user);
      }
      else {
        //Still more to write, send out what's in the buffer and increment the pointer
        i2c_write_register(ic, REG_I2C_TRANSMIT, ic->buffer[ic->buffer_pos]);
        i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
        ic->buffer_pos++;
      }
      break;
    case (READ):
      if (ic->buffer_pos == ic->buffer_len){
        //Wrote Everything, send out the stop bit
        i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
        ic->state = IDLE;
        if (ic->rxf_cb != NULL)
          ic->rxf_cb(ic->buffer, ic->buffer_len, ic->rxf_user);
      }
      else if ((ic->buffer_pos > 0) && (ic->buffer_pos == (ic->buffer_len - 1))){
        //The last byte to read, set the nack so that it will tell the slave to stop
        ic->buffer[ic->buffer_pos] = i2c_read_register(ic, REG_I2C_RECEIVE);
        i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ | CMD_I2C_BIT_NACK);
        ic->buffer_pos++;
      }
      else {
        //Still reading more data
        ic->buffer[ic->buffer_pos] = i2c_read_register(ic, REG_I2C_RECEIVE);
        i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ);
        ic->buffer_pos++;
      }
      break;
    default:
      break;
  }
  //Acknowledge the interrupt
  i2c_write_register(ic, REG_I2C_INTERRUPT, INT_EN_I2C_BIT_TXRX_FIN);
}

uint32_t i2c_control_get_version(i2c_control_t *ic){
  return i2c_read_register(ic, REG_I2C_VERSION);
}

uint32_t i2c_control_get_bus_speed(i2c_control_t *ic){
  uint32_t clock_rate = i2c_get_clock_rate(ic);
  uint32_t clock_divisor = (i2c_get_clock_divisor(ic) + 1) * 5;
  return clock_rate / clock_divisor;
}

void i2c_control_set_bus_speed_to_100khz(i2c_control_t *ic){
 i2c_control_set_custom_bus_speed(ic, 100000);
}

void i2c_control_set_bus_speed_to_400khz(i2c_control_t *ic){
  //i2c_control_set_custom_bus_speed(ic, 400000);
  i2c_enable_register_bit(ic, REG_I2C_CONTROL, CTRL_I2C_BIT_SET_400KHZ, true);
}

void i2c_control_set_custom_bus_speed(i2c_control_t *ic, uint32_t speed){
  uint32_t clock_rate = i2c_get_clock_rate(ic);
  int32_t divisor = ((clock_rate / speed) / 5) - 1;
  //Set a minimum threshold
  if (divisor <= 0){
    divisor = 1;
  }
  i2c_write_register(ic, REG_I2C_CLOCK_DIVISOR, (uint32_t) divisor);
}

void i2c_control_soft_reset(i2c_control_t *ic){
  i2c_enable_register_bit(ic, REG_I2C_CONTROL, CTRL_I2C_BIT_RESET, true);
	usleep(10000);
}

void i2c_control_enable(i2c_control_t *ic){
  i2c_enable_register_bit(ic, REG_I2C_CONTROL, CTRL_I2C_BIT_EN, true);
}

bool i2c_control_is_enabled(i2c_control_t *ic){
  return i2c_is_register_bit_set(ic, REG_I2C_CONTROL, CTRL_I2C_BIT_EN);
}

void i2c_control_ll_bus_start_write(i2c_control_t *ic, uint8_t addr){
  i2c_write_register(ic, REG_I2C_TRANSMIT, (addr << 1));
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);
}
void i2c_control_ll_bus_start_read(i2c_control_t *ic, uint8_t addr){
  i2c_write_register(ic, REG_I2C_TRANSMIT, (addr << 1) | 0x01);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);
}
void i2c_control_ll_bus_write(i2c_control_t *ic, uint8_t data){
  i2c_write_register(ic, REG_I2C_TRANSMIT, data);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
}
uint8_t i2c_control_ll_bus_read(i2c_control_t *ic, uint8_t last){
  uint32_t command = CMD_I2C_BIT_READ;
  if (last)
    command |= CMD_I2C_BIT_NACK;
  i2c_write_register(ic, REG_I2C_COMMAND, command);
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)){} 
  return i2c_read_register(ic, REG_I2C_RECEIVE);
}
void i2c_control_ll_bus_stop(i2c_control_t *ic){
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
}
uint8_t i2c_control_ll_bus_is_busy(i2c_control_t *ic){
  return i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP);
}
uint32_t i2c_control_ll_bus_errors(i2c_control_t *ic){
  return i2c_check_for_errors(ic);
}

int i2c_control_write_to_i2c_no_stop(i2c_control_t *ic, uint8_t i2c_id, uint8_t *data, uint32_t length){
  uint32_t errors = 0;

  i2c_write_register(ic, REG_I2C_TRANSMIT, (i2c_id << 1));
  if (I2C_DEBUG) xil_printf("%s: Start Write Transaction\r\n", __func__);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);
  if (ic->interrupt_enable){
    ic->buffer = data;
    ic->buffer_pos = 0;
    ic->buffer_len = length;
    ic->state = WRITE;
    return XST_SUCCESS;
  }

  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending address\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return XST_FAILURE;
  }
  if (I2C_DEBUG) xil_printf("%s: Writing Data\r\n", __func__);
  for (uint32_t i = 0; i < length; i++){
    if (I2C_DEBUG) xil_printf("%s: Sending 0x%02X\r\n", __func__, data[i]);
    i2c_write_register(ic, REG_I2C_TRANSMIT, data[i]);
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
    //Poll for I2C to be finished
    while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
    //Check for Errors
    if (errors = i2c_check_for_errors(ic), errors > 0){
      if (I2C_DEBUG) xil_printf("I2C Error detected while sending data\r\n");
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
      return XST_FAILURE;
    }
  }
  return XST_SUCCESS;
}


int i2c_control_write_to_i2c(i2c_control_t *ic, uint8_t i2c_id, uint8_t *data, uint32_t length){
  uint32_t errors = 0;
  //uint32_t d = 0;

  //if (I2C_DEBUG) xil_printf("%s: Entered\r\n", __func__);
  //i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_START);
  i2c_write_register(ic, REG_I2C_TRANSMIT, (i2c_id << 1));
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);
  if (ic->interrupt_enable){
    ic->buffer = data;
    ic->buffer_pos = 0;
    ic->buffer_len = length;
    ic->state = WRITE;
    return XST_SUCCESS;
  }


  //if (I2C_DEBUG) xil_printf("%s: Sent Address\r\n", __func__);
  //Poll for I2C to be finished
  //if (I2C_DEBUG) xil_printf("%s: Wait for a the transmit to finish\r\n", __func__);
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //Check for Errors
  //if (I2C_DEBUG) xil_printf("%s: finish\r\n", __func__);
  //if (I2C_DEBUG) xil_printf("%s: Checking for errors\r\n", __func__);
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending address\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return XST_FAILURE;
  }
  //if (I2C_DEBUG) d = i2c_read_register(REG_I2C_COMMAND);
  //if (I2C_DEBUG) xil_printf("%s: Command: 0x%08X\r\n", __func__, d);

  //if (I2C_DEBUG) xil_printf("%s: Write data addr\r\n", __func__);
  for (uint32_t i = 0; i < length; i++){
    //if (I2C_DEBUG) xil_printf("%s: Sending 0x%02X\r\n", __func__, data[i]);
    i2c_write_register(ic, REG_I2C_TRANSMIT, data[i]);
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
    //Poll for I2C to be finished
    //if (I2C_DEBUG) xil_printf("%s: Wait for a the transmit to finish\r\n", __func__);
    while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
    //Check for Errors
    //if (I2C_DEBUG) xil_printf("%s: finish\r\n", __func__);
    //if (I2C_DEBUG) xil_printf("%s: Checking for errors\r\n", __func__);
    if (errors = i2c_check_for_errors(ic), errors > 0){
      if (I2C_DEBUG) xil_printf("I2C Error detected while sending data\r\n");
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
      return XST_FAILURE;
    }
  }
  //if (I2C_DEBUG) xil_printf("%s: Wrote all data\r\n", __func__);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
  return XST_SUCCESS;
}

int i2c_control_read_from_i2c(i2c_control_t *ic, uint8_t i2c_id, uint8_t *data, uint32_t length){
  uint32_t errors;
  if (I2C_DEBUG) xil_printf("%s: Entered\r\n", __func__);
  //Keep a reference to the data pointer we will modify
  //i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_START);
  i2c_write_register(ic, REG_I2C_TRANSMIT, ((i2c_id << 1) | 0x01));
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);
  if (ic->interrupt_enable){
    ic->buffer = data;
    ic->buffer_pos = 0;
    ic->buffer_len = length;
    ic->state = READ;
  }

  //Poll Mode
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return XST_FAILURE;
  }
  if (I2C_DEBUG) xil_printf("%s: Reading Data\r\n", __func__);
  for (uint32_t i = 0; i < length; i++){
    //Check for last item
    if (i == (length - 1))
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ | CMD_I2C_BIT_NACK);
    else
      i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ);
    while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
    data[i] = i2c_read_register(ic, REG_I2C_RECEIVE);
    if (I2C_DEBUG) xil_printf("%s: Read %02X\r\n", __func__, data[i]);
  }
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
  return XST_SUCCESS;
}

void i2c_control_write_to_i2c_reg(i2c_control_t *ic, uint8_t i2c_id, uint8_t reg, uint8_t data){
  uint32_t errors;

  //Start by a write
  //i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_START);
  i2c_write_register(ic, REG_I2C_TRANSMIT, ((i2c_id << 1) | 0x0));
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ | CMD_I2C_BIT_START);
  if (ic->interrupt_enable){
    //Interrupt Based Transaction
    ic->reg = reg;
    ic->reg_data = data;
    ic->buffer = &ic->reg_data;
    ic->buffer_pos = 0;
    ic->buffer_len = 1;
    ic->state = WRITE_REG;
    //Exit immidiately, the interrupts will take it from here
    return;
  }


  //Poll for I2C to be finished
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending address\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return;
  }


  //Set Register Value
  i2c_write_register(ic, REG_I2C_TRANSMIT, reg);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
  //Poll for I2C to be finished
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending data\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return;
  }


  //Data
  i2c_write_register(ic, REG_I2C_TRANSMIT, data);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
  //Poll for I2C to be finished
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending data\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return;
  }
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
}

uint8_t i2c_control_read_from_i2c_reg(i2c_control_t *ic, uint8_t i2c_id, uint8_t reg){
  uint32_t errors;
  //if (I2C_DEBUG) xil_printf("%s: Entered\r\n", __func__);

  //Start by a write
  //if (I2C_DEBUG) xil_printf("%s: Sent Start\r\n", __func__);
  i2c_write_register(ic, REG_I2C_TRANSMIT, ((i2c_id << 1) | 0x0));
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);
  //if (I2C_DEBUG) xil_printf("%s: Sent Address\r\n", __func__);
  if (ic->interrupt_enable){
    //Interrupt Based Transaction
    ic->reg = reg;
    ic->addr = i2c_id;
    ic->reg_data = reg;
    ic->buffer = &ic->reg_data;
    ic->buffer_pos = 0;
    ic->buffer_len = 1;
    ic->state = READ_REG;
    //Exit immidiately, the interrupts will take it from here
    return 0xFF;
  }

  //Poll for I2C to be finished
  //if (I2C_DEBUG) xil_printf("%s: Wait for a the transmit to finish\r\n", __func__);
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //if (I2C_DEBUG) xil_printf("%s: finish\r\n", __func__);
  //Check for Errors
  //if (I2C_DEBUG) xil_printf("%s: Checking for errors\r\n", __func__);
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending address\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return 0xFF;
  }

  //Set Register Value
  //if (I2C_DEBUG) xil_printf("%s: Write reg addr\r\n", __func__);
  i2c_write_register(ic, REG_I2C_TRANSMIT, reg);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE);
  //if (I2C_DEBUG) xil_printf("%s: Wait for a the transmit to finish\r\n", __func__);
  //Poll for I2C to be finished
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //if (I2C_DEBUG) xil_printf("%s: finish\r\n", __func__);
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    if (I2C_DEBUG) xil_printf("I2C Error detected while sending Register Address\r\n");
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return 0xFF;
  }

  //Restart The bus
  //if (I2C_DEBUG) xil_printf("%s: Restart\r\n", __func__);

  //Keep a reference to the data pointer we will modify
  i2c_write_register(ic, REG_I2C_TRANSMIT, ((i2c_id << 1) | 0x01));
  //if (I2C_DEBUG) xil_printf("%s: Sending address\r\n", __func__);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_WRITE | CMD_I2C_BIT_START);

  //if (I2C_DEBUG) xil_printf("%s: Wait for a the transmit to finish\r\n", __func__);
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  //if (I2C_DEBUG) xil_printf("%s: finish\r\n", __func__);
  //Check for Errors
  if (errors = i2c_check_for_errors(ic), errors > 0){
    i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
    return 0xFF;
  }

  //if (I2C_DEBUG) xil_printf("%s: Reading Data\r\n", __func__);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_READ | CMD_I2C_BIT_NACK);

  //if (I2C_DEBUG) xil_printf("%s: Wait for a the transmit to finish\r\n", __func__);
  while (i2c_is_register_bit_set(ic, REG_I2C_STATUS, STS_I2C_BIT_TIP)) {};
  ic->reg_data = i2c_read_register(ic, REG_I2C_RECEIVE);
  //if (I2C_DEBUG) xil_printf("%s: finish\r\n", __func__);
	i2c_check_for_errors(ic);
  i2c_write_register(ic, REG_I2C_COMMAND, CMD_I2C_BIT_STOP);
  return ic->reg_data;
}

void i2c_control_enable_print_errors(i2c_control_t *ic, bool enable){
  ic->print_errors = enable;
}

bool i2c_control_is_print_errors(i2c_control_t *ic){
  return ic->print_errors;
}

#if defined(XPAR_XINTC_NUM_INSTANCES)
void i2c_control_enable_interrupt(i2c_control_t *ic, bool enable, XIntc *interrupt_controller, uint32_t i2c_interrupt_index){
  int status;
  ic->interrupt_enable = enable;
  if (enable){
    if (status = XIntc_Connect(
                  interrupt_controller,
                  i2c_interrupt_index,
                  (XInterruptHandler) i2c_control_interrupt,
                  (void *) NULL),
        status != XST_SUCCESS){
      xil_printf("Failed to connect I2C Interrupt\r\n");
    }
    XIntc_Enable(interrupt_controller, i2c_interrupt_index);
    i2c_write_register(ic, REG_I2C_INTERRUPT_EN, INT_EN_I2C_BIT_TXRX_FIN);
    i2c_enable_interrupts(ic, true);
  }
  else {
    XIntc_Disable(interrupt_controller, i2c_interrupt_index);
    i2c_enable_interrupts(ic, false);
  }
}

void i2c_control_set_rx_finished_callback(i2c_control_t *ic, i2c_rx_finished_callback_t callback, void * user){
  ic->rxf_cb = callback;
  ic->rxf_user = user;
}

void i2c_control_set_tx_finished_callback(i2c_control_t *ic, i2c_tx_finished_callback_t callback, void * user){
  ic->txf_cb = callback;
  ic->txf_user = user;
}

void i2c_control_set_error_callback(i2c_control_t *ic, i2c_error_callback_t callback, void * user){
  ic->error_cb = callback;
  ic->error_user = user;
}
#endif

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

uint32_t ic_get_status(i2c_control_t *ic){
  return i2c_read_register(ic, REG_I2C_STATUS);
}

void i2c_enable_interrupts(i2c_control_t *ic, bool enable){
  i2c_enable_register_bit(ic, REG_I2C_CONTROL, CTRL_I2C_BIT_INT_EN, enable);
}

bool ic_is_interrupt_enabled(i2c_control_t *ic){
  return i2c_is_register_bit_set(ic, REG_I2C_CONTROL, CTRL_I2C_BIT_INT_EN);
}

uint32_t i2c_read_register(i2c_control_t *ic, uint32_t addr){
  return (uint32_t) Xil_In32((UINTPTR)(ic->base_addr + addr));
}

void i2c_write_register(i2c_control_t *ic, uint32_t addr, uint32_t value){
  Xil_Out32((UINTPTR) (ic->base_addr + addr), value);
  usleep(100);
}

void i2c_set_register_bit(i2c_control_t *ic, uint32_t addr, uint32_t bit){
  uint32_t value = i2c_read_register(ic, addr);
  value |= 1 << bit;
  i2c_write_register(ic, addr, value);
}

void i2c_clear_register_bit(i2c_control_t *ic, uint32_t addr, uint32_t bit){
  uint32_t value = i2c_read_register(ic, addr);
  value &= ~(1 << bit);
  i2c_write_register(ic, addr, value);
}

void i2c_enable_register_bit(i2c_control_t *ic, uint32_t addr, uint32_t bit, bool enable){
  if (enable)
    i2c_set_register_bit(ic, addr, bit);
  else
    i2c_clear_register_bit(ic, addr, bit);
}

bool i2c_is_register_bit_set(i2c_control_t *ic, uint32_t addr, uint32_t bit){
  uint32_t value = i2c_read_register(ic, addr);
  return ((value & (1 << bit)) > 0);
}

uint32_t i2c_get_clock_rate(i2c_control_t *ic){
  return i2c_read_register(ic, REG_I2C_CLOCK_RATE);
}

uint32_t i2c_get_clock_divisor(i2c_control_t *ic){
  return i2c_read_register(ic, REG_I2C_CLOCK_DIVISOR);
}

uint32_t i2c_check_for_errors(i2c_control_t *ic){
  uint32_t i2c_status = i2c_read_register(ic, REG_I2C_STATUS);
  //if (I2C_DEBUG) xil_printf ("Status: 0x%08X\r\n", i2c_status);
  if (i2c_status & STS_I2C_ERROR_MASK){
    if (i2c_status & STS_I2C_BIT_ARB_LOST){
      if (ic->print_errors)
        xil_printf("I2C: Arbitration lost\r\n");
    }
    if (i2c_status & STS_I2C_BIT_READ_ACK_N){
      if (ic->print_errors)
        xil_printf("I2C: Data Not Acked!\r\n");
    }
  }
  return (i2c_status & STS_I2C_ERROR_MASK);
}
