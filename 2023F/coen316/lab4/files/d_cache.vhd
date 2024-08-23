library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity d_cache is
port(
	d_in:		in std_logic_vector(31 downto 0);
	reset:		in std_logic;
	clk:		in std_logic;
	data_write:	in std_logic;
	addr:		in std_logic_vector(4 downto 0);
	d_out:		out std_logic_vector(31 downto 0));
end d_cache;

architecture d_cache_arch of d_cache is
	
	type reg_array is array (0 to 31) of std_logic_vector(31 downto 0); -- 32 reg, each 32 bits
	signal registers : reg_array;	
	
begin
	process(reset, clk)
	begin
		if (reset = '1') then -- reset (asynchronous) 
			registers <= (others => (others => '0'));	-- clear all reg. zeros
		elsif (rising_edge(clk) and data_write ='1') then	-- click (synchronous), rising edge
			registers(conv_integer(addr)) <= d_in;	-- write 
		end if;
	end process;

	process(addr, registers)
	begin
		d_out <= registers(conv_integer(addr));	-- read data from addr
	end process;
	
end d_cache_arch;