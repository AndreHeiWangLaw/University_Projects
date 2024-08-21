entity or_gate is
port(	A,B,C:	in BIT;
	OUTPUT: out BIT);
end or_gate;

architecture or_arch of or_gate is
begin
	OUTPUT <= A or B or C;
end or_arch;
