# Vivado does not support old UCF syntax
# must use XDC syntax

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets clk]

##Switches
set_property -dict { PACKAGE_PIN J15 IOSTANDARD LVCMOS33 } [ get_ports { rt_in[0] } ];
set_property -dict { PACKAGE_PIN L16 IOSTANDARD LVCMOS33 } [ get_ports { rt_in[1] } ];

set_property -dict { PACKAGE_PIN M13 IOSTANDARD LVCMOS33 } [ get_ports { rs_in[0] } ];
set_property -dict { PACKAGE_PIN R15 IOSTANDARD LVCMOS33 } [ get_ports { rs_in[1] } ];

set_property -dict { PACKAGE_PIN R17 IOSTANDARD LVCMOS33 } [ get_ports { pc_in[0] } ];
set_property -dict { PACKAGE_PIN T18 IOSTANDARD LVCMOS33 } [ get_ports { pc_in[1] } ];
set_property -dict { PACKAGE_PIN U18 IOSTANDARD LVCMOS33 } [ get_ports { pc_in[2] } ];

set_property -dict { PACKAGE_PIN R13 IOSTANDARD LVCMOS33 } [ get_ports { target_address_in[0] } ];
set_property -dict { PACKAGE_PIN T8 IOSTANDARD LVCMOS33 } [ get_ports { target_address_in[1] } ];
set_property -dict { PACKAGE_PIN U8 IOSTANDARD LVCMOS33 } [ get_ports { target_address_in[2] } ];

set_property -dict { PACKAGE_PIN R16 IOSTANDARD LVCMOS33 } [ get_ports { branch_type[0] }];
set_property -dict { PACKAGE_PIN T13 IOSTANDARD LVCMOS33 } [ get_ports { branch_type[1] } ];

set_property -dict { PACKAGE_PIN H6 IOSTANDARD LVCMOS33 } [ get_ports { pc_sel[0] } ];
set_property -dict { PACKAGE_PIN U12 IOSTANDARD LVCMOS33 } [ get_ports { pc_sel[1] } ];

## LEDs
set_property -dict { PACKAGE_PIN H17 IOSTANDARD LVCMOS33 } [ get_ports { next_pc_out[0] } ];
set_property -dict { PACKAGE_PIN K15 IOSTANDARD LVCMOS33 } [ get_ports { next_pc_out[1] } ];
set_property -dict { PACKAGE_PIN J13 IOSTANDARD LVCMOS33 } [ get_ports { next_pc_out[2] } ];

