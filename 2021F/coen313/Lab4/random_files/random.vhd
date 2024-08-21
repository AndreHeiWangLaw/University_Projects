library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
entity registers is
port( din1, din2 : in std_logic_vector(3 downto 0);
 reset : in std_logic;
 clk : in std_logic;
 mick,keith : out std_logic_vector(3 downto 0));
end registers ;
architecture rtl of registers is
begin
 
process(clk, reset)
begin
if reset = '1' then
 mick <= "0000";
 keith <= "0000";
elsif clk'event and clk = '1' then
 mick <= din1;
end if;
end process;
process(clk, reset)
begin
if reset = '1' then
 mick <= "0000";
 keith <= "0000";
elsif clk'event and clk = '1' then
 keith <= din2;
end if;
end process;
end rtl;