#ifndef __I2C_CONTROL_H__
#define __I2C_CONTROL_H__


#include <stdint.h>
#include <stdbool.h>
#if defined(XPAR_XINTC_NUM_INSTANCES)
#include "xintc.h"
#endif

typedef void (*i2c_rx_finished_callback_t)(uint8_t *data, uint32_t length, void * user);
typedef void (*i2c_tx_finished_callback_t)(void * user);
typedef void (*i2c_error_callback_t)(uint32_t i2c_status, void * user);

typedef struct {
  //Address of I2C Core
  uint32_t base_addr;

  uint8_t state;
  uint8_t *buffer;
  uint8_t buffer_pos;
  uint16_t buffer_len;
  uint8_t addr;
  uint8_t reg;
  uint8_t reg_data;


  //Interrupt
  bool interrupt_enable;
  i2c_rx_finished_callback_t rxf_cb;
  i2c_tx_finished_callback_t txf_cb;
  i2c_error_callback_t error_cb;
  void * rxf_user;
  void * txf_user;
  void * error_user;
  
  bool print_errors;
} i2c_control_t;

//Register Addresses
#define REG_I2C_CONTROL           0x00
#define REG_I2C_STATUS            0x04
#define REG_I2C_INTERRUPT         0x08
#define REG_I2C_INTERRUPT_EN      0x0C
#define REG_I2C_CLOCK_RATE        0x10
#define REG_I2C_CLOCK_DIVISOR     0x14
#define REG_I2C_COMMAND           0x18
#define REG_I2C_TRANSMIT          0x1C
#define REG_I2C_RECEIVE           0x20
#define REG_I2C_VERSION           0x24

//Control Bit Values
#define CTRL_I2C_BIT_EN           0
#define CTRL_I2C_BIT_INT_EN       1
#define CTRL_I2C_BIT_SET_100KHZ   2
#define CTRL_I2C_BIT_SET_400KHZ   3
#define CTRL_I2C_BIT_RESET        7

//Status
#define STS_I2C_BIT_TIP           0
#define STS_I2C_BIT_ARB_LOST      5
#define STS_I2C_BIT_BUSY          6
#define STS_I2C_BIT_READ_ACK_N    7
#define STS_I2C_ERROR_MASK        ((1 << STS_I2C_BIT_ARB_LOST) | (1 << STS_I2C_BIT_READ_ACK_N))

//Command
#define CMD_I2C_BIT_START        1 << 0
#define CMD_I2C_BIT_STOP         1 << 1
#define CMD_I2C_BIT_READ         1 << 2
#define CMD_I2C_BIT_WRITE        1 << 3
#define CMD_I2C_BIT_NACK         1 << 4

//Interrupt Enable
#define INT_EN_I2C_BIT_TXRX_FIN   0
#define INT_EN_I2C_BIT_ARB_LOST   1
#define INT_EN_I2C_BIT_RXACK      2


void setup_i2c_control(i2c_control_t *ic, uint32_t base_addr);
uint32_t i2c_control_get_version(i2c_control_t *ic);

void i2c_control_enable_print_errors(i2c_control_t *ic, bool enable); 
bool i2c_control_is_print_errors(i2c_control_t *ic);
uint32_t i2c_control_get_bus_speed(i2c_control_t *ic);
void i2c_control_set_bus_speed_to_100khz(i2c_control_t *ic);
void i2c_control_set_bus_speed_to_400khz(i2c_control_t *ic);
void i2c_control_set_custom_bus_speed(i2c_control_t *ic, uint32_t speed);
void i2c_control_soft_reset(i2c_control_t *ic);
void i2c_control_enable(i2c_control_t *ic);
bool i2c_control_is_enabled(i2c_control_t *ic);

int i2c_control_write_to_i2c_no_stop(i2c_control_t *ic, uint8_t i2c_id, uint8_t *data, uint32_t length);
int i2c_control_write_to_i2c(i2c_control_t *ic, uint8_t i2c_id, uint8_t *data, uint32_t length);
int i2c_control_read_from_i2c(i2c_control_t *ic, uint8_t i2c_id, uint8_t *data, uint32_t length);
void i2c_control_write_to_i2c_reg(i2c_control_t *ic, uint8_t i2c_id, uint8_t reg, uint8_t data);
uint8_t i2c_control_read_from_i2c_reg(i2c_control_t *ic, uint8_t i2c_id, uint8_t reg);

void i2c_control_ll_bus_start_write(i2c_control_t *ic, uint8_t addr);
void i2c_control_ll_bus_start_read(i2c_control_t *ic, uint8_t addr);
void i2c_control_ll_bus_write(i2c_control_t *ic, uint8_t data);
uint8_t i2c_control_ll_bus_read(i2c_control_t *ic, uint8_t last);
void i2c_control_ll_bus_stop(i2c_control_t *ic);
uint8_t i2c_control_ll_bus_is_busy(i2c_control_t *ic);
uint32_t i2c_control_ll_bus_errors(i2c_control_t *ic);


#if defined(XPAR_XINTC_NUM_INSTANCES)
void i2c_control_enable_interrupt(i2c_control_t *ic,
                                  bool enable,
                                  XIntc *interrupt_controller,
                                  uint32_t i2c_interrupt_index);
void i2c_control_set_rx_finished_callback(i2c_control_t *ic, i2c_rx_finished_callback_t callback, void * user);
void i2c_control_set_tx_finished_callback(i2c_control_t *ic, i2c_tx_finished_callback_t callback, void * user);
void i2c_control_set_error_callback(i2c_control_t *ic, i2c_error_callback_t callback, void * user);
#endif


#endif //__I2C_CONTROL_H__
