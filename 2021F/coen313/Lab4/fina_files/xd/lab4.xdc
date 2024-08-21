set_property -dict {PACKAGE_PIN J15 IOSTANDARD LVCMOS33} [get_ports {din[0]}];
set_property -dict {PACKAGE_PIN L16 IOSTANDARD LVCMOS33} [get_ports {din[1]}];
set_property -dict {PACKAGE_PIN M13 IOSTANDARD LVCMOS33} [get_ports {din[2]}];
set_property -dict {PACKAGE_PIN R15 IOSTANDARD LVCMOS33} [get_ports {din[3]}];

set_property -dict {PACKAGE_PIN R17 IOSTANDARD LVCMOS33} [get_ports {sel[0]}];
set_property -dict {PACKAGE_PIN T18 IOSTANDARD LVCMOS33} [get_ports {sel[1]}];

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets clk];
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets reset];

set_property -dict {PACKAGE_PIN U18 IOSTANDARD LVCMOS33} [get_ports {reset}];
set_property -dict {PACKAGE_PIN R13 IOSTANDARD LVCMOS33} [get_ports {clk}];

set_property -dict {PACKAGE_PIN H17 IOSTANDARD LVCMOS33} [get_ports {reg_out[0]}];
set_property -dict {PACKAGE_PIN K15 IOSTANDARD LVCMOS33} [get_ports {reg_out[1]}];
set_property -dict {PACKAGE_PIN J13 IOSTANDARD LVCMOS33} [get_ports {reg_out[2]}];
set_property -dict {PACKAGE_PIN N14 IOSTANDARD LVCMOS33} [get_ports {reg_out[3]}];

set_property -dict {PACKAGE_PIN R18 IOSTANDARD LVCMOS33} [get_ports {max_out[0]}];
set_property -dict {PACKAGE_PIN V17 IOSTANDARD LVCMOS33} [get_ports {max_out[1]}];
set_property -dict {PACKAGE_PIN U17 IOSTANDARD LVCMOS33} [get_ports {max_out[2]}];
set_property -dict {PACKAGE_PIN U16 IOSTANDARD LVCMOS33} [get_ports {max_out[3]}];

set_property -dict {PACKAGE_PIN V16 IOSTANDARD LVCMOS33} [get_ports {min_out[0]}];
set_property -dict {PACKAGE_PIN T15 IOSTANDARD LVCMOS33} [get_ports {min_out[1]}];
set_property -dict {PACKAGE_PIN U14 IOSTANDARD LVCMOS33} [get_ports {min_out[2]}];
set_property -dict {PACKAGE_PIN T16 IOSTANDARD LVCMOS33} [get_ports {min_out[3]}];
