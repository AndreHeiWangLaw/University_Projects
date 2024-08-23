# coen 316 
# Andre Hei Wang Law
# 4017 5600

# 3.1 Setup
add wave reset
add wave clk
add wave write
add wave din
add wave read_a
add wave read_b
add wave write_address
add wave registers
add wave out_a
add wave out_b

# 3.2 Initialize Values
# 88888888 Hex is 1000 x 8 times in Binary
force reset 1
force clk 0
force write 0
force din x"88888888"
force read_a "00101"
force read_b "01111"
force write_address "00000"
run
examine -radix hex out_a 
examine -radix hex out_b

# 3.3) Test Writing Into “registers” (reset = 1, write = 1, rising edge clk)
examine -radix hex registers(0)
force write 1
run
force clk 1
run
examine -radix hex registers(0)

# 3.4) Test Writing Into “registers” (reset = 0, write = 0, rising edge clk)
examine -radix hex registers(0)
force write 0
force reset 0
force clk 0
run
force clk 1
run
examine -radix hex registers(0)

# 3.5) Test Writing Into “registers” (reset = 0, write = 1, rising edge clk)
examine -radix hex registers(0)
force write 1
force clk 0
run
force clk 1
run
examine -radix hex registers(0)

# 3.6) Test Reset (reset = 1, no rising edge clk)
examine -radix hex registers(0)
force reset 1
run
examine -radix hex registers(0)

# 3.7) Test Writing Into “registers” (reset = 0, write = 1, independent of rising edge clk)
examine -radix hex registers(0)
force reset 1
run
force reset 0
force write 1
force clk 0
run
examine -radix hex registers(0)

# 3.8) Rising Edge (clk = 1)
examine -radix hex registers(0)
force clk 1
run
examine -radix hex registers(0)

# 3.9) Display Output (read_a "00000" and  read_b "01111")
examine -radix hex out_a
examine -radix hex out_b
force read_a "00000"
run
examine -radix hex out_a
examine -radix hex out_b

# 3.10) Display Output (read_a "00000" and  read_b "00000")
examine -radix hex out_a
examine -radix hex out_b
force read_b "00000"
run
examine -radix hex out_a
examine -radix hex out_b

# 3.11) Test Write on Different Address (write_address "00101", read_a "00101")
force reset 1
force clk 0
force write 1
run
examine -radix hex out_a 
examine -radix hex out_b
force reset 0
force din x"CCCCCCCC"
force read_a "00101"
force read_b "01111"
force write_address "00101"
force clk 1
run
examine -radix hex out_a 
examine -radix hex out_b


