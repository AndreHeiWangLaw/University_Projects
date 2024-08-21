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
	variable inv : unsigned(2 downto 0);
	variable temp : std_logic_vector(2 downto 0);
	variable first : std_logic;
	begin
	if sign_mag(3)  = '0' then
		first := sign_mag(3);
		temp := sign_mag(2 downto 0);
	else
		first := sign_mag(3);
		inv := unsigned(not sign_mag(2 downto 0));
		inv := inv + 1;
		temp := std_logic_vector(inv);
	end if;
	
	twos_comp <= first & temp;
	
	end process;
end if_else;
