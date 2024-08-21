library IEEE;
use IEEE.std_logic_1164.all;

entity csa_rewrite is
port (	a: in std_logic_vector(2 downto 0);
	output: out std_logic);
end csa_rewrite;

architecture circuit_rewrite of csa_rewrite is
	signal s1, s2, s3: std_logic;
begin
	s1 <= (not a(0)) and (not a(1)) and a(2) after 12 ns;
	s2 <= (not a(0)) and a(1) and a(2) after 12 ns;
	s3 <= a(0) and a(1) and a(2) after 12 ns;
	output <= s1 or s2 or s3 afer 20 ns;
end circuit_rewrite;

