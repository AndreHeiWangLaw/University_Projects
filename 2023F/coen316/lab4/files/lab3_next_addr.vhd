-- coen 316 lab 3
-- Andre Hei Wang Law
-- 4017 5600

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_signed.all;

entity next_address is
port(
	-- inputs
	rt, rs:			in std_logic_vector(31 downto 0);
	pc:				in std_logic_vector(31 downto 0);
	target_address:	in std_logic_vector(25 downto 0);
	branch_type:	in std_logic_vector(1 downto 0);
	pc_sel:			in std_logic_vector(1 downto 0);
	
	-- output
	next_pc:		out std_logic_vector(31 downto 0));
end next_address;

architecture unit of next_address is

-- signal
signal tar_addr_signed_32:	std_logic_vector(31 downto 0);
signal branch_offset_signed_32:	std_logic_vector(31 downto 0);
signal branch_offset:	std_logic_vector(31 downto 0);

begin
	-- calculate "pseudo-direct" addressing to fill entire 32 bit
	-- target address for "jump" instructions
	tar_addr_signed_32(31 downto 26) <= (others => target_address(25)); -- fill with ones or zeros
	tar_addr_signed_32(25 downto 0) <= target_address; -- fill the rest 
	
	-- branch offset for no unconditional jump instructions
	branch_offset_signed_32(31 downto 16) <= (others => target_address(15)); -- fill with ones or zeros
	branch_offset_signed_32(15 downto  0) <= target_address(15 downto 0); -- fill the rest 

	-- PC_sel functionality
	process(pc_sel, rs, branch_offset, pc, tar_addr_signed_32)
	begin
		if (pc_sel = "00") then -- no unconditional jump
			next_pc <= branch_offset + pc + 1;
		elsif (pc_sel = "01") then -- jump
			next_pc <= tar_addr_signed_32;
		elsif (pc_sel = "10") then -- jump register
			next_pc <= rs;
		elsif (pc_sel = "11") then -- not used
			-- do nothing
		end if;
	end process;
	
	-- branch_type functionality
	process(branch_type, rs, rt, branch_offset, branch_offset_signed_32)
	begin
		if (branch_type = "00") then -- no branch
			branch_offset <= (others => '0');
		elsif (branch_type = "01") then -- beq (equal to 0)
			if (rs = rt) then
				branch_offset <= branch_offset_signed_32;
			else
				branch_offset <= (others => '0');
			end if;
		elsif (branch_type = "10") then -- bne (not equal to 0)
			if (rs /= rt) then
				branch_offset <= branch_offset_signed_32;
			else
				branch_offset <= (others => '0');
			end if;
		elsif (branch_type = "11") then -- bltz (less then 0)
			if (rs(31) = '1') then -- means rs is negative, or < 0
				branch_offset <= branch_offset_signed_32;
			else
				branch_offset <= (others => '0');
			end if;
		end if;
	end process;
end unit;

