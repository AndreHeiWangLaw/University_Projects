# 3.1.3 Pc

add wave *

# 3.1.3.1 initialize value
force reset 0
force clk 1
force d_in x"AAAAAAAA"
run 2
examine -radix hex q_out

# 3.1.3.2 reset 1, clk 0
force clk 0
run 2
force reset 1
force d_in x"AAAAAAAA"
run 2
examine -radix hex q_out

# 3.1.3.3 reset 0, clk 1
force clk 0
run 2
force reset 0
force clk 1
force d_in x"AAAAAAAA"
run 2
examine -radix hex q_out

