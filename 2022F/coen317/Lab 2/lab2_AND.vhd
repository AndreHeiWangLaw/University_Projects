-- 8-bit AND gate


library ieee;
use ieee.std_logic_1164.all;

--------------------------------------------------

entity and_gate8 is
port(	x: in std_logic_vector(0 to 7);
	    y: in std_logic_vector(0 to 7);
	    F: out std_logic_vector(0 to 7)
);
end and_gate8;


architecture Behavioral of and_gate8 is
begin
	F <= x and y;

end Behavioral;
