library IEEE;
use IEEE.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity converter is
port (	sign_mag :	in std_logic_vector (3 downto 0);
	twos_comp :	out std_logic_vector (3 downto 0));
end converter;

architecture if_else of converter is
begin
	process (sign_mag)
	begin
	if sign_mag(3)  = '0' then
		twos_comp <= sign_mag;
	else
		twos_comp <= std_logic_vector(unsigned((not sign_mag) + 1));
		twos_comp(3) <= '1';
	end if;
	end process;
end if_else;
