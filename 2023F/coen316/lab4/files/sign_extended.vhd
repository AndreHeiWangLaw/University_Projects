library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_signed.all;

entity sign_extended is
port(
	func:		in std_logic_vector(1 downto 0);
	sign_in:	in std_logic_vector(15 downto 0);
	sign_out:	out std_logic_vector(31 downto 0));
end sign_extended;

architecture sign_extended_arch of sign_extended is

begin
	-- mux (func)
	process(sign_in, func)
	begin
		case(func) is
		when "00" => 
			sign_out(31 downto 16) <= sign_in; -- load upper immediate
			sign_out(15 downto  0) <= (others => '0');
		when "01" => 
			sign_out(31 downto 16) <= (others => sign_in(15)); -- sign extend
			sign_out(15 downto  0) <= sign_in; -- fill rest
		when "10" => 
			sign_out(31 downto 16) <= (others => sign_in(15)); -- sign extend
			sign_out(15 downto  0) <= sign_in; -- fill rest
		when others => 
			sign_out(31 downto 16) <= (others => '0'); -- logical
			sign_out(15 downto  0) <= sign_in;
		end case;
	end process;
	
end sign_extended_arch;