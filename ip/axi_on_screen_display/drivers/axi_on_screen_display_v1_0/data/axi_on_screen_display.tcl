
proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "axi_on_screen_display" "NUM_INSTANCES" "DEVICE_ID"  "C_BASEADDR" "C_HIGHADDR"
}
