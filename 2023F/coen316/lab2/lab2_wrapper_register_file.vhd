-- coen 316 lab
-- Andre Hei Wang Law
-- 4017 5600

-- board wrapper version

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity regfile is
	port (
	-- (original)
	din: 	in std_logic_vector(3 downto 0);	-- data written to specific register
	reset: 	in std_logic;	-- asynchronous active-high reset input
	clk: 	in std_logic;	-- clock input
	write: 	in std_logic;	-- write control signal
	read_a: in std_logic_vector(1 downto 0);	-- address for read port A
	read_b: in std_logic_vector(1 downto 0);	-- address for read port B
	write_address: in std_logic_vector(1 downto 0);	-- address for write port
	out_a: 	out std_logic_vector(3 downto 0);	-- data output for read port A
	out_b: 	out std_logic_vector(3 downto 0));	-- data output for read port B
end regfile;

architecture reg_arch of regfile is
	type reg_array is array (0 to 31) of std_logic_vector(31 downto 0); -- 32 reg, each 32 bits
	signal registers : reg_array;	-- register file, all zeros
	
	-- board wrapper (new)
	signal din_wrap, out_a_wrap, out_b_wrap : std_logic_vector(31 downto 0);
	signal read_a_wrap, read_b_wrap, write_address_wrap : std_logic_vector(4 downto 0);

begin
	-- board wrapper (left side new, right side original)
	din_wrap(31 downto 4) <= (others => '0');
	din_wrap(3 downto 0) <= din(3) & din(2) & din(1) & din(0);
	read_a_wrap(4 downto 2) <= (others => '0');
	read_a_wrap(1 downto 0) <= read_a(1) & read_a(0);
	read_b_wrap(4 downto 2) <= (others => '0');
	read_b_wrap(1 downto 0) <= read_b(1) & read_b(0);
	write_address_wrap(4 downto 2) <= (others => '0');
	write_address_wrap(1 downto 0) <= write_address(1) & write_address(0);
	
	-- board wrapper (left side original, right side new)
	-- output (notice it's 3 downto 0 and not 31 downto 0)
	out_a(3 downto 0) <= out_a_wrap(3 downto 0);
	out_b(3 downto 0) <= out_b_wrap(3 downto 0);	

	process(reset, clk)
	begin
		if (reset = '1') then -- reset (asynchronous) 
			registers <= (others => (others => '0'));	-- clear all reg. zeros
		elsif (clk'event and clk='1') then	-- click (synchronous), rising edge
			if (write = '1') then		-- check for active write signal
				registers(conv_integer(write_address_wrap)) <= din_wrap;	-- write 
			end if;
		end if;
	end process;

	-- reading (asynchronous)
	out_a_wrap <= registers(conv_integer(read_a_wrap));	-- read data from read_a
	out_b_wrap <= registers(conv_integer(read_b_wrap));	-- read data from read_b

end architecture reg_arch;
