
proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "axi_sony_imx_controller" "NUM_INSTANCES" "DEVICE_ID"  "C_S00_AXI_BASEADDR" "C_S00_AXI_HIGHADDR"
}
