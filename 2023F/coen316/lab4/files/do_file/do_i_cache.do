# 3.1.2 Instructions Cache

add wave *

# 3.1.2.1 addi r1, r0, 1
force addr_in "00000"
run 2
examine -radix hex inst_out

# 3.1.2.2 addi r2, r0, 2
force addr_in "00001"
run 2
examine -radix hex inst_out

# 3.1.2.3 add r2, r2, r1
force addr_in "00010"
run 2
examine -radix hex inst_out

# 3.1.2.4 jump 00010
force addr_in "00011"
run 2
examine -radix hex inst_out

# 3.1.2.5 don’t care
force addr_in "01101"
run 2
examine -radix hex inst_out