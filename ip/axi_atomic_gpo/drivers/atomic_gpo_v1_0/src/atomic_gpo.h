
#ifndef ATOMIC_GPO_H
#define ATOMIC_GPO_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"

#define ATOMIC_GPO_S00_AXI_SLV_REG0_OFFSET 0
#define ATOMIC_GPO_S00_AXI_SLV_REG1_OFFSET 4
#define ATOMIC_GPO_S00_AXI_SLV_REG2_OFFSET 8
#define ATOMIC_GPO_S00_AXI_SLV_REG3_OFFSET 12


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a ATOMIC_GPO register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the ATOMIC_GPOdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void ATOMIC_GPO_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define ATOMIC_GPO_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a ATOMIC_GPO register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the ATOMIC_GPO device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 ATOMIC_GPO_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define ATOMIC_GPO_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the ATOMIC_GPO instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus ATOMIC_GPO_Reg_SelfTest(void * baseaddr_p);

typedef struct {
  u32 base_address;
} gpo_control_t;


void setup_atomic_gpo(gpo_control_t * gc, u32 base_address);
void atomic_gpo_set_master_frame_ptr(gpo_control_t *gc, u32 index);
u32 atomic_gpo_get_slave_frame_ptr(gpo_control_t *gc, u32 slave_index);
void atomic_gpo_ack_interrupt(gpo_control_t *gc);
void atomic_gpo_enable(gpo_control_t *gc, bool enable);
u32 atomic_gpo_get_value(gpo_control_t *gc);
u32 atomic_gpo_get_version(gpo_control_t *gc);

#endif // ATOMIC_GPO_H
