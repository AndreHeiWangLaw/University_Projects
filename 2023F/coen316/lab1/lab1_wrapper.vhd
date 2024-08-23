-- coen 316 lab1
-- Andre Hei Wang Law
-- 4017 5600

-- board wrapper version

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.std_logic_signed.all;

entity alu_board  is
	port (
    x_in, y_in: in std_logic_vector(3 downto 0);  -- Input ports for low-order 4 bits of x and y
	add_sub: 	in std_logic;				-- 0=add, 1=sub
	logic_func:	in std_logic_vector(1 downto 0);	-- 00=AND, 01=OR, 10=XOR, 11=NOR
	func:		in std_logic_vector(1 downto 0);	-- 00=lui, 01=setless, 10=arith, 11=logic
    output_out: out std_logic_vector(3 downto 0); -- Output port for low-order 4 bits of output
	overflow:	out std_logic;
	zero:		out std_logic);
end alu_board ;

architecture board_wrapper of alu_board  is

-- signal 
signal adder_subtract_output, logic_unit_output: std_logic_vector(31 downto 0);
signal x, y, output : std_logic_vector(31 downto 0);

begin
	-- board wrapper 
	-- assign port inputs to internal signals x and y
	x(3 downto 0) <= x_in(3) & x_in(2) & x_in(1) & x_in(0) ;
	y(3 downto 0) <= y_in(3) & y_in(2) & y_in(1) & y_in(0) ;
	x(31 downto 4) <= (others => '0');
	y(31 downto 4) <= (others => '0');
	output_out(3 downto 0) <= output(3 downto 0);

	-- adder_subtract (add_sub)
	process(x, y, add_sub)
	begin
		-- check for addition or substraction
		if add_sub = '0' then
			adder_subtract_output <= x + y;		-- addition
		elsif add_sub = '1' then
			adder_subtract_output <= x - y;		-- substraction
		end if;
	end process;

	-- LINE 40
	-- logic unit (logic_func)
	process(x, y, logic_func)
	begin
		-- check for logic (AND, OR, XOR, NOR)
		if logic_func = "00" then
			logic_unit_output <= x AND y;	-- AND
		elsif logic_func = "01" then
			logic_unit_output <= x OR y;	-- OR
		elsif logic_func = "10" then
			logic_unit_output <= x XOR y;	-- XOR
		elsif logic_func = "11" then
			logic_unit_output <= x NOR y;	-- NOR
		end if;
	end process;

	-- mux (func)
	process(y, adder_subtract_output, logic_unit_output, func)
	begin
		-- determines output
		if func = "00" then
			output <= y;				-- y
		elsif func = "01" then
			-- output = 1 if x < y 
			-- output = 0 otherwise
			-- “000...MSB” of adder_subttract
			output <= "0000000000000000000000000000000" & adder_subtract_output(31);
		elsif func = "10" then
			output <= adder_subtract_output;	-- output of adder_subtract
		elsif func = "11" then
			output <= logic_unit_output;		-- output of logic unit
		end if;
	end process;

	-- zero
	process(adder_subtract_output)
	begin
		-- zero = 1 when output of the adder_subtract unit is all 0s
		-- zero = 0 otherwise
		if adder_subtract_output = "00000000000000000000000000000000" then
			zero <= '1';
		else
			zero <= '0';
		end if;
	end process;

	-- overflow
	process(adder_subtract_output, add_sub, x, y)
	begin
		-- overflow = 1 when: 
			-- 1. Adding two positive num (x + y) and a negative result is obtained
			-- 2. Adding two negative numbers (-x + -y) and a possitive result is obtained
			-- 3. Substracting two operands of opposite sign (-x - y) 
			-- 4. Substracting two operands of opposite sign (x - -y)
		
		-- 1. and 2. Addition
		if add_sub = '0' then
			if (x(3) = '0' and y(3) = '0' and adder_subtract_output(3) = '1') or
			   (x(3) = '1' and y(3) = '1' and adder_subtract_output(3) = '0') then
				overflow <= '1'; -- Overflow detected
			else
				overflow <= '0'; -- No overflow
			end if;
			--overflow <= (not(x(31)) AND not(y(31)) AND adder_subtract_output(31)) OR (x(31) AND y(31) AND not(adder_subtract_output(31)));
				
		-- 3. and 4. Substraction
		elsif add_sub = '1' then
			if (x(3) = '0' and y(3) = '1' and adder_subtract_output(3) = '1') or
			   (x(3) = '1' and y(3) = '0' and adder_subtract_output(3) = '0') then
				overflow <= '1'; -- Overflow detected
			else
				overflow <= '0'; -- No overflow
			end if;
			--overflow <= (x(31) AND not(y(31)) AND not(adder_subtract_output(31))) OR (not(x(31)) AND y(31) AND adder_subtract_output(31));
		end if;
	end process;

end board_wrapper;
	
