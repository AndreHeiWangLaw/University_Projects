# Vivado does not support old UCF syntax
# must use XDC syntax
set_property -dict { PACKAGE_PIN J15 IOSTANDARD LVCMOS33 } [ get_ports { x_in[0] } ];
set_property -dict { PACKAGE_PIN L16 IOSTANDARD LVCMOS33 } [ get_ports { x_in[1] } ];
set_property -dict { PACKAGE_PIN M13 IOSTANDARD LVCMOS33 } [ get_ports { x_in[2] } ];
set_property -dict { PACKAGE_PIN R15 IOSTANDARD LVCMOS33 } [ get_ports { x_in[3] } ];

set_property -dict { PACKAGE_PIN R17 IOSTANDARD LVCMOS33 } [ get_ports { y_in[0] } ];
set_property -dict { PACKAGE_PIN T18 IOSTANDARD LVCMOS33 } [ get_ports { y_in[1] } ];
set_property -dict { PACKAGE_PIN U18 IOSTANDARD LVCMOS33 } [ get_ports { y_in[2] } ];
set_property -dict { PACKAGE_PIN R13 IOSTANDARD LVCMOS33 } [ get_ports { y_in[3] } ];

set_property -dict { PACKAGE_PIN T8 IOSTANDARD LVCMOS33 } [ get_ports { func[1] } ];
set_property -dict { PACKAGE_PIN U8 IOSTANDARD LVCMOS33 } [ get_ports { func[0] } ];

set_property -dict { PACKAGE_PIN R16 IOSTANDARD LVCMOS33 } [ get_ports { logic_func[0] }];
set_property -dict { PACKAGE_PIN T13 IOSTANDARD LVCMOS33 } [ get_ports { logic_func[1] } ];
set_property -dict { PACKAGE_PIN H6 IOSTANDARD LVCMOS33 } [ get_ports { add_sub } ];

set_property -dict { PACKAGE_PIN H17 IOSTANDARD LVCMOS33 } [ get_ports { output_out[0] } ];
set_property -dict { PACKAGE_PIN K15 IOSTANDARD LVCMOS33 } [ get_ports { output_out[1] } ];
set_property -dict { PACKAGE_PIN J13 IOSTANDARD LVCMOS33 } [ get_ports { output_out[2] } ];
set_property -dict { PACKAGE_PIN N14 IOSTANDARD LVCMOS33 } [ get_ports { output_out[3] } ];

set_property -dict { PACKAGE_PIN R18 IOSTANDARD LVCMOS33 } [ get_ports { overflow } ];
set_property -dict { PACKAGE_PIN V17 IOSTANDARD LVCMOS33 } [ get_ports { zero } ];
