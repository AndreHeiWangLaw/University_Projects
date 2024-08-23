# 3.1.1 Data Cache

add wave *

# 3.2.1.1) -- add
force op "000000"
force func_in "100000"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.2) -- sub
force op "000000"
force func_in "100010"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.3) -- slt
force op "000000"
force func_in "101010"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.4) -- and
force op "000000"
force func_in "100100"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.5) -- or
force op "000000"
force func_in "100101"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.6) -- xor
force op "000000"
force func_in "100110"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.7) -- nor
force op "000000"
force func_in "100111"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.8) -- jr
force op "000000"
force func_in "001000"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.9) -- lui
force op "001111"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.10) -- addi
force op "001000"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.11) -- slti
force op "001010"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.12) -- andi
force op "001100"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.13) -- ori
force op "001101"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.14) -- xori
force op "001110"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.15) -- lw
force op "100011"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.16) -- sw
force op "101011"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.17) -- j
force op "000010"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.18) -- bltz
force op "000001"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.19) -- beq
force op "000100"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel

# 3.2.1.20) -- bne
force op "000101"
run 2
examine reg_write reg_dst reg_in_src alu_src add_sub data_write logic_func func branch_type pc_sel