library IEEE;
use IEEE.std_logic_1164.all;

entity i_cache is
port(
	addr_in:	in std_logic_vector(4 downto 0);
	inst_out:	out std_logic_vector(31 downto 0));
end i_cache;

architecture i_cache_arch of i_cache is

begin

	process(addr_in)
	begin
		case(addr_in) is
			when "00000" => inst_out <= "00100000000000110000000000000000"; -- addi r3, r0, 0
			when "00001" => inst_out <= "00100000000000010000000000000000"; -- addi r1, r0, 0
			when "00010" => inst_out <= "00100000000000100000000000000101"; -- addi r2,r0,5
			when "00011" => inst_out <= "00000000001000100000100000100000"; -- add r1,r1,r2
			when "00100" => inst_out <= "00100000010000101111111111111111"; -- addi r2, r2, -1
			when "00101" => inst_out <= "00010000010000110000000000000001"; -- beq r2,r3 (+1) THERE
			when "00110" => inst_out <= "00001000000000000000000000000011"; -- jump 3  (LOOP)
			when "00111" => inst_out <= "10101100000000010000000000000000"; -- sw r1, 0(r0)  
			when "01000" => inst_out <= "10001100000001000000000000000000"; -- lw r4, 0(r0)
			when "01001" => inst_out <= "00110000100001000000000000001010"; -- andi r4,r4, 0x000A
			when "01010" => inst_out <= "00110100100001000000000000000001"; -- ori r4,r4, 0x0001
			when "01011" => inst_out <= "00111000100001000000000000001011"; -- xori r4,r4, 0xB
			when "01100" => inst_out <= "00111000100001000000000000000000"; -- xori r4,r4, 0x0000
			when others => 	inst_out <= "00000000000000000000000000000000"; -- dont care
		end case;
	end process;

end i_cache_arch;