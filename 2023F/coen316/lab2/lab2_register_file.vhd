-- coen 316 lab
-- Andre Hei Wang Law
-- 4017 5600

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity regfile is
	port (
	din: 	in std_logic_vector(31 downto 0);	-- data written to specific register
	reset: 	in std_logic;	-- asynchronous active-high reset input
	clk: 	in std_logic;	-- clock input
	write: 	in std_logic;	-- write control signal
	read_a: in std_logic_vector(4 downto 0);	-- address for read port A
	read_b: in std_logic_vector(4 downto 0);	-- address for read port B
	write_address: in std_logic_vector(4 downto 0);	-- address for write port
	out_a: 	out std_logic_vector(31 downto 0);	-- data output for read port A
	out_b: 	out std_logic_vector(31 downto 0));	-- data output for read port B
end regfile;

architecture reg_arch of regfile is
	type reg_array is array (0 to 31) of std_logic_vector(31 downto 0); -- 32 reg, each 32 bits
	signal registers : reg_array;	-- register file, all zeros
	
begin
	process(reset, clk)
	begin
		if (reset = '1') then -- reset (asynchronous) 
			registers <= (others => (others => '0'));	-- clear all reg. zeros
		elsif (clk'event and clk='1') then	-- click (synchronous), rising edge
			if (write = '1') then		-- check for active write signal
				registers(conv_integer(write_address)) <= din;	-- write 
			end if;
		end if;
	end process;

	-- reading (asynchronous)
	out_a <= registers(conv_integer(read_a));	-- read data from read_a
	out_b <= registers(conv_integer(read_b));	-- read data from read_b

end architecture reg_arch;
