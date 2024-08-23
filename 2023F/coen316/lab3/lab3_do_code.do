# DO File
# 3.1 setup
add wave rt
add wave rs
add wave pc
add wave target_address
add wave branch_type
add wave pc_sel
add wave next_pc
force rt x"00000000"
force rs x"00000000"
force pc x"00000000"
force target_address "00000000000000000000000000"
force branch_type "00"
force pc_sel "00"
run

# 3.2 pc_sel "00" and branch_type "00"
force pc x"00000000"
force branch_type "00"
force pc_sel "00"
run
examine -radix hex next_pc

# 3.3 pc_sel "00" and branch_type "01", false
force pc x"00000100"
force branch_type "01"
force pc_sel "00"
force rt x"00000000"
force rs x"AAAAAAAA"
run
examine -radix hex next_pc

# 3.4 pc_sel "00" and branch_type "10", false
force pc x"00001000"
force branch_type "10"
force pc_sel "00"
force rt x"AAAAAAAA"
force rs x"AAAAAAAA"
run
examine -radix hex next_pc

# 3.5 pc_sel "00" and branch_type "11", false
force pc x"00100000"
force branch_type "11"
force pc_sel "00"
force rt x"AAAAAAAA"
force rs x"00001111"
run
examine -radix hex next_pc

# 3.6 pc_sel "00" and branch_type "01", true
force pc x"00100000"
force branch_type "01"
force pc_sel "00"
force target_address "00000000000000000000000011"
force rt x"AAAAAAAA"
force rs x"AAAAAAAA"
run
examine -radix hex next_pc

# 3.7 pc_sel "00" and branch_type "10", true
force pc x"00001000"
force branch_type "10"
force pc_sel "00"
force target_address "00000000000000000000001111"
force rt x"00000000"
force rs x"AAAAAAAA"
run
examine -radix hex next_pc

# 3.8 pc_sel "00" and branch_type "11", true
force pc x"00000011"
force branch_type "11"
force pc_sel "00"
force target_address "00000000000000000000001100"
force rt x"00000000"
force rs x"FFFFFFFF"
run
examine -radix hex next_pc

# 3.9 pc_sel "01" 
force rt x"00000011"
force rs x"00001100"
force pc x"00111100"
force target_address "00000000000000000000001111"
force branch_type "00"
force pc_sel "01"
run
examine -radix hex next_pc

# 3.10 pc_sel "10" 
force rt x"00000011"
force rs x"00001100"
force pc x"00111100"
force target_address "00000000000000000000001111"
force branch_type "00"
force pc_sel "10"
run
examine -radix hex next_pc

# 3.11 pc_sel "11" 
force rt x"00000011"
force rs x"00001100"
force pc x"00111100"
force target_address "00000000000000000000001111"
force branch_type "00"
force pc_sel "11"
run
examine -radix hex next_pc