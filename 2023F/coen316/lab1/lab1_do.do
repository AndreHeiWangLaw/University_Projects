-- Andre Hei Wang Law
-- 4017 5600
-- Do File
-- Coen 316 lab1


-- Testing output y					(func = "00")
force x "00000000000000001111111111111111"
force y "00000000000000000000000000001101"
force add_sub '0'
force logic_func "00"
force func "00"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000000000000000001101"
force add_sub '1'
force logic_func "00"
force func "00"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000000000000000001101"
force add_sub '1'
force logic_func "11"
force func "00"
run
examine output
examine overflow
examine zero



-- Testing output “000...MSB” of adder_subttract	(func = "01")
-- checked by performing the operationx - y
force x "00000000000000001111111111111111"
force y "00000000000000000000000000000000"
force add_sub '1'
force logic_func "00"
force func "01"
run
examine output
examine overflow
examine zero

force x "00000000000000000000000000000000"
force y "00000000000000001111111111111111"
force add_sub '1'
force logic_func "00"
force func "01"
run
examine output
examine overflow
examine zero




-- Testing output output of adder_subtract		(func = "10")
force x "00000000000000001111111111111111"
force y "00000000000000000000000000000000"
force add_sub '0'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000001111111111111111"
force add_sub '1'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero



-- Testing output of logic unit, AND, OR, XOR, NOR	(func = "11")
force x "00000000000000001111111111111111"
force y "00000000000000000000000000000000"
force add_sub '0'
force logic_func "00"
force func "11"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000000000000000000000"
force add_sub '0'
force logic_func "01"
force func "11"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000000000000000000000"
force add_sub '0'
force logic_func "10"
force func "11"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000000000000000000000"
force add_sub '0'
force logic_func "11"
force func "11"
run
examine output
examine overflow
examine zero




-- Testing overflow						(MSB = '1')
force x "00000000000000000000000000000000"
force y "01000000000000000000000000000000"
force add_sub '0'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero

force x "01000000000000000000000000000000"
force y "01000000000000000000000000000000"
force add_sub '0'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero

force x "11000000000000000000000000000000"
force y "10100000000000000000000000000000"
force add_sub '0'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero

force x "01000000000000000000000000000000"
force y "10000000000000000000000000000000"
force add_sub '1'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero

force x "10000000000000000000000000000000"
force y "01000000000000000000000000000000"
force add_sub '1'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero


-- Testing zero					(add_sub = '1' and add_sub = '0')
force x "00000000000000000000000000000000"
force y "00000000000000000000000000000000"
force add_sub '0'
force logic_func "00"
force func "11"
run
examine output
examine overflow
examine zero

force x "00000000000000001111111111111111"
force y "00000000000000001111111111111111"
force add_sub '1'
force logic_func "00"
force func "10"
run
examine output
examine overflow
examine zero





