

/***************************** Include Files *******************************/
#include "atomic_gpo.h"

#define REG_CONTROL               (0 << 2)
#define REG_GPO_VALUE             (1 << 2)
#define REG_GPO_ACK               (2 << 2)
#define REG_MASTER_FRAME_PTR      (3 << 2)
#define REG_CAM0_SLAVE_FRAME_PTR  (4 << 2)
#define REG_CAM1_SLAVE_FRAME_PTR  (5 << 2)
#define REG_CAM2_SLAVE_FRAME_PTR  (6 << 2)
#define REG_VERSION               (7 << 2)

#define CTRL_BIT_ENABLE           0



/************************** Function Definitions ***************************/

void setup_atomic_gpo(gpo_control_t * gc, u32 base_address){
  gc->base_address = base_address;
}

void atomic_gpo_enable(gpo_control_t *gc, bool enable){
  u32 value = ATOMIC_GPO_mReadReg(gc->base_address, REG_CONTROL);
  value = (enable) ? (value | (1 << CTRL_BIT_ENABLE)) : (value & ~(1 << CTRL_BIT_ENABLE));
  ATOMIC_GPO_mWriteReg(gc->base_address, REG_CONTROL, 1);
}

u32 atomic_gpo_get_slave_frame_ptr(gpo_control_t *gc, u32 slave_index){
  u32 offset = REG_CAM0_SLAVE_FRAME_PTR + (slave_index << 4);
  return ATOMIC_GPO_mReadReg(gc->base_address, offset);
}
void atomic_gpo_ack_interrupt(gpo_control_t *gc){
  ATOMIC_GPO_mWriteReg(gc->base_address, REG_GPO_ACK, 0);
}
u32 atomic_gpo_get_value(gpo_control_t *gc){
  return ATOMIC_GPO_mReadReg(gc->base_address, REG_GPO_VALUE);
}
void atomic_gpo_set_master_frame_ptr(gpo_control_t *gc, u32 index){
  ATOMIC_GPO_mWriteReg(gc->base_address, REG_MASTER_FRAME_PTR, index);
}
u32 atomic_gpo_get_version(gpo_control_t *gc){
  ATOMIC_GPO_mWriteReg(gc->base_address, REG_VERSION);
}

