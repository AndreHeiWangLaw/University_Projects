library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_signed.all;

entity pc is
port(
	reset:		in std_logic;
	clk:		in std_logic;
	d_in:		in std_logic_vector(31 downto 0);
	q_out:		out std_logic_vector(31 downto 0));
end pc;

architecture pc_arch of pc is

begin
	process(reset, clk)
	begin
		if (reset = '1') then -- reset (asynchronous) 
			q_out <= (others => '0');	-- clear all 
		elsif (clk'event and clk='1') then	-- click (synchronous), rising edge
			q_out <= d_in;
		end if;
	end process;
	
end pc_arch;