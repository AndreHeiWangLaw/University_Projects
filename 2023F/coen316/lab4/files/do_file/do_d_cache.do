# 3.1.1 Data Cache

add wave *

# 3.1.1.1 initialize value
force d_in x"AAAAAAAA"
force reset 0
force clk 1
force data_write 1
force addr "00000"
run 2
examine -radix hex d_out

# 3.1.1.2 reset 1, clk 0 and data_write 0
force d_in x"AAAAAAAA"
force reset 1
force clk 0
force data_write 0
force addr "00000"
run 2
examine -radix hex d_out

# 3.1.1.3 data_write 1, reset 0 and clk 0
force d_in x"AAAAAAAA"
force reset 0
force clk 0
force data_write 1
force addr "00000"
run 2
examine -radix hex d_out

# 3.1.1.4 data_write 0, reset 0 and clk 1
force clk 0
run 2
force d_in x"AAAAAAAA"
force reset 0
force clk 1
force data_write 0
force addr "00000"
run 2
examine -radix hex d_out

# 3.1.1.5 data_write 1, reset 0 and clk 1
force clk 0
run 2
force d_in x"AAAAAAAA"
force reset 0
force clk 1
force data_write 1
force addr "00000"
run 2
examine -radix hex d_out