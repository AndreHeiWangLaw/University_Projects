library IEEE;
use IEEE.std_logic_1164.all;

entity sum_of_minterms is
port(	A,B,C:	in BIT;
	
	D:	out BIT);
end sum_of_minterms;

architecture circuit_arch of sum_of_minterms is

component and_gate
port(	A,B,C:	in BIT;
	OUTPUT: out BIT);
end component;

component or_gate
port(	A,B,C:	in BIT;
	OUTPUT:	out BIT);
end component;

signal s1, s2, s3:	BIT;
signal not_A, not_B:	BIT;

for U1,U2,U3:	and_gate use entity WORK.and_gate(and_arch);
for U4:		or_gate use entity WORK.or_gate(or_arch);

begin
not_A <= not A;
not_B <= not B;

U1:	and_gate port map(A=>not_A, B=> not_B, C=> C, OUTPUT=> s1);
U2:	and_gate port map(A=> not_A, B=> B, C=> C, OUTPUT=> s2);
U3:	and_gate port map(A=> A, B=> B, C=> C, OUTPUT=> s3);
U4:	or_gate port map(A=> s1, B=> s2, C=> s3, OUTPUT => D);
end circuit_arch;
