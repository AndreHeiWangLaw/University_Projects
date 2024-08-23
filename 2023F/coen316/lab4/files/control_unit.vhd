library IEEE;
use IEEE.std_logic_1164.all;

entity control_unit is
port(
	op:				in std_logic_vector(5 downto 0);
	func_in:		in std_logic_vector(5 downto 0);
	reg_write:		out std_logic;
	reg_dst:		out std_logic;
	reg_in_src:		out std_logic;
	alu_src:		out std_logic;
	add_sub:		out std_logic;
	data_write:		out std_logic;
	logic_func:		out std_logic_vector(1 downto 0);
	func:			out std_logic_vector(1 downto 0);
	branch_type:	out std_logic_vector(1 downto 0);
	pc_sel:			out std_logic_vector(1 downto 0));
end control_unit;

architecture control_unit_arch of control_unit is

signal control_signals: std_logic_vector(13 downto 0);



begin
	process(op, func_in, control_signals)
	begin		
		case(op) is
		when "000000" =>
			case(func_in) is
			when "100000" => control_signals <= "11100000100000"; -- add
			when "100010" => control_signals <= "11101000100000"; -- sub
			when "101010" => control_signals <= "11101000010000"; -- slt
			when "100100" => control_signals <= "11100000110000"; -- and
			when "100101" => control_signals <= "11100001110000"; -- or
			when "100110" => control_signals <= "11100010110000"; -- xor
			when "100111" => control_signals <= "11100011110000"; -- nor
			when "001000" => control_signals <= "00000000000010"; -- jr
			when others   => control_signals <= "00000000000000";
			end case;
		when "001111" => control_signals <= "10110000000000"; -- lui
		when "001000" => control_signals <= "10110000100000"; -- addi
		when "001010" => control_signals <= "10111000010000"; -- slti
		when "001100" => control_signals <= "10110000110000"; -- andi
		when "001101" => control_signals <= "10110001110000"; -- ori
		when "001110" => control_signals <= "10110010110000"; -- xori
		when "100011" => control_signals <= "10010010100000"; -- lw
		when "101011" => control_signals <= "00010100100000"; -- sw
		when "000010" => control_signals <= "00000000000001"; -- j
		when "000001" => control_signals <= "00000000001100"; -- bltz
		when "000100" => control_signals <= "00000000000100"; -- beq
		when "000101" => control_signals <= "00000000001000"; -- bne
		when others   => control_signals <= "00000000000000";
		end case;
	
		reg_write 	<= control_signals(13);
		reg_dst 	<= control_signals(12);
		reg_in_src 	<= control_signals(11);
		alu_src 	<= control_signals(10);
		add_sub 	<= control_signals(9);
		data_write 	<= control_signals(8);
		logic_func 	<= control_signals(7 downto 6);
		func 		<= control_signals(5 downto 4);
		branch_type	<= control_signals(3 downto 2);
		pc_sel		<= control_signals(1 downto 0);
			
	end process;
	
end control_unit_arch;