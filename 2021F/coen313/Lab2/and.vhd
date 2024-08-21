entity and_gate is
port (	A,B,C:	in BIT;
	OUTPUT:	out BIT);
end and_gate;

architecture and_arch of and_gate is
begin
	OUTPUT <= A and B and C after 5 ns;
end and_arch;

