# 3.1.4 Sign extended

add wave *

# 3.1.4.1 load upper immediate
force func "00"
force sign_in x"ABCD"
run 2
examine -radix hex sign_out

# 3.1.4.2 set less immediate
force func "01"
force sign_in x"ABCD"
run 2
examine -radix hex sign_out

# 3.1.4.3 arithmetic
force func "10"
force sign_in x"0BCD"
run 2
examine -radix hex sign_out

# 3.1.4.4 logical
force func "11"
force sign_in x"ABCD"
run 2
examine -radix hex sign_out
