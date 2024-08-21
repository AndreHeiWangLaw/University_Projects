library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.numeric_std.all;

entity registers_min_max is
port (
	din:	in std_logic_vector(3 downto 0);
	reset:	in std_logic;
	clk:	in std_logic;
	sel:	in std_logic_vector(1 downto 0);
	max_out:	out std_logic_vector(3 downto 0);
	min_out:	out std_logic_vector(3 downto 0);
	reg_out:	out std_logic_vector(3 downto 0));
end registers_min_max;

architecture reg_min_max of registers_min_max is
	type reg_array is array (integer range <>) of unsigned(3 downto 0);
	signal regist: reg_array(3 downto 0);
	signal max_regist, min_regist: unsigned(3 downto 0);
	signal save_max_regist, save_min_regist:	unsigned(3 downto 0);
begin
process(regist)
	variable max:	unsigned(3 downto 0) := "0000";
	variable min:	unsigned(3 downto 0) := "1111";
	begin
		for index in regist'high downto 1 loop
			if (min > regist(index)) then
				min := regist(index);
			end if;
			if (max < regist(index)) then
				max := regist(index);
			end if;
		end loop;
	save_max_regist <= max;
	save_min_regist <= min;
end process;

process(din,reset,clk)
	begin
	if (reset = '1') then
		regist(3) <= "1000";
		regist(2) <= "1000";
		regist(1) <= "1000";
		regist(0) <= "1000";
	elsif (rising_edge(clk)) then
		regist(0) <= regist(1);
		regist(1) <= regist(2);
		regist(2) <= regist(3);
		regist(3) <= unsigned(din);
	end if;
end process;

with sel select reg_out <=
	std_logic_vector(regist(3)) when "00",
	std_logic_vector(regist(2)) when "01",
	std_logic_vector(regist(1)) when "10",
	std_logic_vector(regist(0)) when "11",
	"0000" when others;

process(clk,reset,max_regist,min_regist,save_min_regist,save_max_regist)
	begin
	if (reset = '1') then
		max_regist <= "0000";
		min_regist <= "1111";
	elsif rising_edge(clk) then
		if (save_max_regist > max_regist and max_regist(0) /= 'U') then
			max_regist <= save_max_regist; 
		end if;
		if (save_min_regist < min_regist and min_regist(0) /= 'U') then
			min_regist <= save_min_regist;
		end if;
	end if;
end process;
max_out <= std_logic_vector(max_regist);
min_out <= std_logic_vector(min_regist);
end  reg_min_max;
