library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity twos_complementer is
port(	din:	in std_logic_vector(7 downto 0);
		reset:	in std_logic;
		clk:	in std_logic;
		done_out:	out std_logic;
		reg_out:	out std_logic_vector(7 downto 0));
end twos_complementer;

architecture arch_twos_comp of twos_comp is

type states is (FILL STATES~~!~~~);
signal state: states;
signal d, shift, load, inc, clr, initial, clr_done, id_done: std_logic;
signal count: std_logic_vector(2 downto 0);
signal shift_reg, temp_reg: std_logic_vector(7 downto 0);

begin
process()
begin
	if (reset = '1') then
		state <= initial;
		counter <= "000";
	elsif (clk'event and clk = '1') then
		case state is
			when start =>
				counter <= "000";
				shift <= '0';
				load <= '1';
				inc <= '0';
				clr <= '1';
				clr_done <= '1';
				ld_done <= '0';
				state <= loading;
			when loading =>
				load <= '0';
				clr <= '0';
				clr_done <= '0';


end process;
end arch_twos_comp;