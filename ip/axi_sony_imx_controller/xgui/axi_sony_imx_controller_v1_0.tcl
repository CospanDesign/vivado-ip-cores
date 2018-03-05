# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0" -display_name {Camera Interface Configuration}]
  ipgui::add_param $IPINST -name "LANE_WIDTH" -parent ${Page_0}
  set DEFAULT_TRIGGER_PERIOD [ipgui::add_param $IPINST -name "DEFAULT_TRIGGER_PERIOD" -parent ${Page_0}]
  set_property tooltip {Number of clock cycles (relative to the Camera Clock) that a pulse will occur.} ${DEFAULT_TRIGGER_PERIOD}
  set DEFAULT_TRIGGER_LEN [ipgui::add_param $IPINST -name "DEFAULT_TRIGGER_LEN" -parent ${Page_0}]
  set_property tooltip {Length of trigger pulse} ${DEFAULT_TRIGGER_LEN}

  #Adding Page
  set AXI_Configuration [ipgui::add_page $IPINST -name "AXI Configuration"]
  ipgui::add_param $IPINST -name "INVERT_VDMA_RESET" -parent ${AXI_Configuration}
  ipgui::add_param $IPINST -name "INVERT_AXI_RESET" -parent ${AXI_Configuration}


}

proc update_PARAM_VALUE.AXIS_DATA_WIDTH { PARAM_VALUE.AXIS_DATA_WIDTH } {
	# Procedure called to update AXIS_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXIS_DATA_WIDTH { PARAM_VALUE.AXIS_DATA_WIDTH } {
	# Procedure called to validate AXIS_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.AXIS_STROBE_WIDTH { PARAM_VALUE.AXIS_STROBE_WIDTH } {
	# Procedure called to update AXIS_STROBE_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXIS_STROBE_WIDTH { PARAM_VALUE.AXIS_STROBE_WIDTH } {
	# Procedure called to validate AXIS_STROBE_WIDTH
	return true
}

proc update_PARAM_VALUE.AXI_ADDR_WIDTH { PARAM_VALUE.AXI_ADDR_WIDTH } {
	# Procedure called to update AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_ADDR_WIDTH { PARAM_VALUE.AXI_ADDR_WIDTH } {
	# Procedure called to validate AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.AXI_DATA_WIDTH { PARAM_VALUE.AXI_DATA_WIDTH } {
	# Procedure called to update AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_DATA_WIDTH { PARAM_VALUE.AXI_DATA_WIDTH } {
	# Procedure called to validate AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.AXI_STROBE_WIDTH { PARAM_VALUE.AXI_STROBE_WIDTH } {
	# Procedure called to update AXI_STROBE_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_STROBE_WIDTH { PARAM_VALUE.AXI_STROBE_WIDTH } {
	# Procedure called to validate AXI_STROBE_WIDTH
	return true
}

proc update_PARAM_VALUE.BRAM_DATA_DEPTH { PARAM_VALUE.BRAM_DATA_DEPTH } {
	# Procedure called to update BRAM_DATA_DEPTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.BRAM_DATA_DEPTH { PARAM_VALUE.BRAM_DATA_DEPTH } {
	# Procedure called to validate BRAM_DATA_DEPTH
	return true
}

proc update_PARAM_VALUE.BRAM_DATA_WIDTH { PARAM_VALUE.BRAM_DATA_WIDTH } {
	# Procedure called to update BRAM_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.BRAM_DATA_WIDTH { PARAM_VALUE.BRAM_DATA_WIDTH } {
	# Procedure called to validate BRAM_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.DEFAULT_TRIGGER_LEN { PARAM_VALUE.DEFAULT_TRIGGER_LEN } {
	# Procedure called to update DEFAULT_TRIGGER_LEN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEFAULT_TRIGGER_LEN { PARAM_VALUE.DEFAULT_TRIGGER_LEN } {
	# Procedure called to validate DEFAULT_TRIGGER_LEN
	return true
}

proc update_PARAM_VALUE.DEFAULT_TRIGGER_PERIOD { PARAM_VALUE.DEFAULT_TRIGGER_PERIOD } {
	# Procedure called to update DEFAULT_TRIGGER_PERIOD when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEFAULT_TRIGGER_PERIOD { PARAM_VALUE.DEFAULT_TRIGGER_PERIOD } {
	# Procedure called to validate DEFAULT_TRIGGER_PERIOD
	return true
}

proc update_PARAM_VALUE.INVERT_AXI_RESET { PARAM_VALUE.INVERT_AXI_RESET } {
	# Procedure called to update INVERT_AXI_RESET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.INVERT_AXI_RESET { PARAM_VALUE.INVERT_AXI_RESET } {
	# Procedure called to validate INVERT_AXI_RESET
	return true
}

proc update_PARAM_VALUE.INVERT_VDMA_RESET { PARAM_VALUE.INVERT_VDMA_RESET } {
	# Procedure called to update INVERT_VDMA_RESET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.INVERT_VDMA_RESET { PARAM_VALUE.INVERT_VDMA_RESET } {
	# Procedure called to validate INVERT_VDMA_RESET
	return true
}

proc update_PARAM_VALUE.LANE_WIDTH { PARAM_VALUE.LANE_WIDTH } {
	# Procedure called to update LANE_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.LANE_WIDTH { PARAM_VALUE.LANE_WIDTH } {
	# Procedure called to validate LANE_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.DEFAULT_TRIGGER_LEN { MODELPARAM_VALUE.DEFAULT_TRIGGER_LEN PARAM_VALUE.DEFAULT_TRIGGER_LEN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEFAULT_TRIGGER_LEN}] ${MODELPARAM_VALUE.DEFAULT_TRIGGER_LEN}
}

proc update_MODELPARAM_VALUE.DEFAULT_TRIGGER_PERIOD { MODELPARAM_VALUE.DEFAULT_TRIGGER_PERIOD PARAM_VALUE.DEFAULT_TRIGGER_PERIOD } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEFAULT_TRIGGER_PERIOD}] ${MODELPARAM_VALUE.DEFAULT_TRIGGER_PERIOD}
}

proc update_MODELPARAM_VALUE.LANE_WIDTH { MODELPARAM_VALUE.LANE_WIDTH PARAM_VALUE.LANE_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.LANE_WIDTH}] ${MODELPARAM_VALUE.LANE_WIDTH}
}

proc update_MODELPARAM_VALUE.INVERT_AXI_RESET { MODELPARAM_VALUE.INVERT_AXI_RESET PARAM_VALUE.INVERT_AXI_RESET } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.INVERT_AXI_RESET}] ${MODELPARAM_VALUE.INVERT_AXI_RESET}
}

proc update_MODELPARAM_VALUE.AXIS_DATA_WIDTH { MODELPARAM_VALUE.AXIS_DATA_WIDTH PARAM_VALUE.AXIS_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXIS_DATA_WIDTH}] ${MODELPARAM_VALUE.AXIS_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.AXIS_STROBE_WIDTH { MODELPARAM_VALUE.AXIS_STROBE_WIDTH PARAM_VALUE.AXIS_STROBE_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXIS_STROBE_WIDTH}] ${MODELPARAM_VALUE.AXIS_STROBE_WIDTH}
}

proc update_MODELPARAM_VALUE.BRAM_DATA_DEPTH { MODELPARAM_VALUE.BRAM_DATA_DEPTH PARAM_VALUE.BRAM_DATA_DEPTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BRAM_DATA_DEPTH}] ${MODELPARAM_VALUE.BRAM_DATA_DEPTH}
}

proc update_MODELPARAM_VALUE.BRAM_DATA_WIDTH { MODELPARAM_VALUE.BRAM_DATA_WIDTH PARAM_VALUE.BRAM_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BRAM_DATA_WIDTH}] ${MODELPARAM_VALUE.BRAM_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.AXI_ADDR_WIDTH { MODELPARAM_VALUE.AXI_ADDR_WIDTH PARAM_VALUE.AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.AXI_DATA_WIDTH { MODELPARAM_VALUE.AXI_DATA_WIDTH PARAM_VALUE.AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.AXI_STROBE_WIDTH { MODELPARAM_VALUE.AXI_STROBE_WIDTH PARAM_VALUE.AXI_STROBE_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_STROBE_WIDTH}] ${MODELPARAM_VALUE.AXI_STROBE_WIDTH}
}

proc update_MODELPARAM_VALUE.INVERT_VDMA_RESET { MODELPARAM_VALUE.INVERT_VDMA_RESET PARAM_VALUE.INVERT_VDMA_RESET } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.INVERT_VDMA_RESET}] ${MODELPARAM_VALUE.INVERT_VDMA_RESET}
}

