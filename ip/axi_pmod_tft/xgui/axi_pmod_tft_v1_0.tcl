# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0" -display_name {Parameters}]
  set_property tooltip {Configuration} ${Page_0}
  set INVERT_AXI_RESET [ipgui::add_param $IPINST -name "INVERT_AXI_RESET" -parent ${Page_0}]
  set_property tooltip {If set the AXI bus is active low} ${INVERT_AXI_RESET}
  set INVERT_AXIS_RESET [ipgui::add_param $IPINST -name "INVERT_AXIS_RESET" -parent ${Page_0}]
  set_property tooltip {True = AXI Stream Reset Low, False = AXI Stream Reset High} ${INVERT_AXIS_RESET}
  ipgui::add_static_text $IPINST -name "Configure the Polarity of the Reset" -parent ${Page_0} -text {Checked: reset line is active low
Unchecked: reset line is active high
}

  set BUFFER_SIZE [ipgui::add_param $IPINST -name "BUFFER_SIZE"]
  set_property tooltip {This sets the size of the internal FIFO, 10 is fine} ${BUFFER_SIZE}

}

proc update_PARAM_VALUE.ADDR_WIDTH { PARAM_VALUE.ADDR_WIDTH } {
	# Procedure called to update ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ADDR_WIDTH { PARAM_VALUE.ADDR_WIDTH } {
	# Procedure called to validate ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.AXIS_WIDTH { PARAM_VALUE.AXIS_WIDTH } {
	# Procedure called to update AXIS_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXIS_WIDTH { PARAM_VALUE.AXIS_WIDTH } {
	# Procedure called to validate AXIS_WIDTH
	return true
}

proc update_PARAM_VALUE.BUFFER_SIZE { PARAM_VALUE.BUFFER_SIZE } {
	# Procedure called to update BUFFER_SIZE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.BUFFER_SIZE { PARAM_VALUE.BUFFER_SIZE } {
	# Procedure called to validate BUFFER_SIZE
	return true
}

proc update_PARAM_VALUE.DATA_WIDTH { PARAM_VALUE.DATA_WIDTH } {
	# Procedure called to update DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DATA_WIDTH { PARAM_VALUE.DATA_WIDTH } {
	# Procedure called to validate DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.IMAGE_HEIGHT { PARAM_VALUE.IMAGE_HEIGHT } {
	# Procedure called to update IMAGE_HEIGHT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IMAGE_HEIGHT { PARAM_VALUE.IMAGE_HEIGHT } {
	# Procedure called to validate IMAGE_HEIGHT
	return true
}

proc update_PARAM_VALUE.IMAGE_WIDTH { PARAM_VALUE.IMAGE_WIDTH } {
	# Procedure called to update IMAGE_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IMAGE_WIDTH { PARAM_VALUE.IMAGE_WIDTH } {
	# Procedure called to validate IMAGE_WIDTH
	return true
}

proc update_PARAM_VALUE.INVERT_AXIS_RESET { PARAM_VALUE.INVERT_AXIS_RESET } {
	# Procedure called to update INVERT_AXIS_RESET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.INVERT_AXIS_RESET { PARAM_VALUE.INVERT_AXIS_RESET } {
	# Procedure called to validate INVERT_AXIS_RESET
	return true
}

proc update_PARAM_VALUE.INVERT_AXI_RESET { PARAM_VALUE.INVERT_AXI_RESET } {
	# Procedure called to update INVERT_AXI_RESET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.INVERT_AXI_RESET { PARAM_VALUE.INVERT_AXI_RESET } {
	# Procedure called to validate INVERT_AXI_RESET
	return true
}

proc update_PARAM_VALUE.STROBE_WIDTH { PARAM_VALUE.STROBE_WIDTH } {
	# Procedure called to update STROBE_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.STROBE_WIDTH { PARAM_VALUE.STROBE_WIDTH } {
	# Procedure called to validate STROBE_WIDTH
	return true
}

proc update_PARAM_VALUE.USE_BOARD_FLOW { PARAM_VALUE.USE_BOARD_FLOW } {
	# Procedure called to update USE_BOARD_FLOW when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.USE_BOARD_FLOW { PARAM_VALUE.USE_BOARD_FLOW } {
	# Procedure called to validate USE_BOARD_FLOW
	return true
}


proc update_MODELPARAM_VALUE.ADDR_WIDTH { MODELPARAM_VALUE.ADDR_WIDTH PARAM_VALUE.ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ADDR_WIDTH}] ${MODELPARAM_VALUE.ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.DATA_WIDTH { MODELPARAM_VALUE.DATA_WIDTH PARAM_VALUE.DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DATA_WIDTH}] ${MODELPARAM_VALUE.DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.STROBE_WIDTH { MODELPARAM_VALUE.STROBE_WIDTH PARAM_VALUE.STROBE_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.STROBE_WIDTH}] ${MODELPARAM_VALUE.STROBE_WIDTH}
}

proc update_MODELPARAM_VALUE.BUFFER_SIZE { MODELPARAM_VALUE.BUFFER_SIZE PARAM_VALUE.BUFFER_SIZE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BUFFER_SIZE}] ${MODELPARAM_VALUE.BUFFER_SIZE}
}

proc update_MODELPARAM_VALUE.INVERT_AXI_RESET { MODELPARAM_VALUE.INVERT_AXI_RESET PARAM_VALUE.INVERT_AXI_RESET } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.INVERT_AXI_RESET}] ${MODELPARAM_VALUE.INVERT_AXI_RESET}
}

proc update_MODELPARAM_VALUE.IMAGE_WIDTH { MODELPARAM_VALUE.IMAGE_WIDTH PARAM_VALUE.IMAGE_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IMAGE_WIDTH}] ${MODELPARAM_VALUE.IMAGE_WIDTH}
}

proc update_MODELPARAM_VALUE.IMAGE_HEIGHT { MODELPARAM_VALUE.IMAGE_HEIGHT PARAM_VALUE.IMAGE_HEIGHT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IMAGE_HEIGHT}] ${MODELPARAM_VALUE.IMAGE_HEIGHT}
}

proc update_MODELPARAM_VALUE.AXIS_WIDTH { MODELPARAM_VALUE.AXIS_WIDTH PARAM_VALUE.AXIS_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXIS_WIDTH}] ${MODELPARAM_VALUE.AXIS_WIDTH}
}

proc update_MODELPARAM_VALUE.INVERT_AXIS_RESET { MODELPARAM_VALUE.INVERT_AXIS_RESET PARAM_VALUE.INVERT_AXIS_RESET } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.INVERT_AXIS_RESET}] ${MODELPARAM_VALUE.INVERT_AXIS_RESET}
}

