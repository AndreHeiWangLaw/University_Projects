library IEEE;
use IEEE.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity converter is
port (	sign_mag :	in std_logic_vector (3 downto 0);
	twos_comp :	out std_logic_vector (3 downto 0));
end converter;

architecture if_else of converter is
--signal temp, inv :	std_logic_vector(3 downto 0);
signal temp, inv : unsigned(3 downto 0);

begin
	process (sign_mag)
	begin
	if sign_mag(3)  = '0' then
		twos_comp <= sign_mag;
	else
		--inverter
		--temp <= not sign_mag;
		
		----temp <= unsigned(not sign_mag);
		----inv <= temp + 1;
		----twos_comp <= std_logic_vector(inv);
		
		--twos_comp <= std_logic_vector(unsigned(temp + 1));	

		--combiner
		--twos_comp <= "1" & inv;
		
		
		twos_comp <= std_logic_vector(unsigned((not sign_mag) + 1));
		twos_comp(3) <= '1';
		
		
		
	end if;
	end process;
end if_else;
