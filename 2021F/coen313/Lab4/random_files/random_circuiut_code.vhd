library ieee;
use ieee.std_logic_1164.all;
use IEEE.numeric_std.all;

entity registers_min_max is
      port(         
		clk: in std_logic;
		reset: in std_logic;
		din: in std_logic_vector(3 downto 0);
		sel:  in std_logic_vector(1 downto 0);
		reg_out : out std_logic_vector(3 downto 0);
		max_out : out std_logic_vector(3 downto 0);
		min_out : out std_logic_vector(3 downto 0));
end entity;  

architecture shifter_arch of registers_min_max is     
	 type sr is array(integer range <>) of unsigned(3 downto 0);
     signal q_reg : sr(3 downto 0);
     signal q_next : unsigned(3 downto 0);
	 signal max_in : unsigned(3 downto 0);
     signal min_in : unsigned(3 downto 0);
	 signal load_max : unsigned(3 downto 0);
     signal load_min : unsigned(3 downto 0);
begin
     in_reg : process(clk, reset)
     begin         
		if (reset = '1') then
			for index in 3 downto 0 loop
				q_reg(index) <= "1000";
			end loop;
		elsif (rising_edge(clk)) then
			q_reg(3) <= unsigned(din);
			q_reg(2) <= q_reg(3);
			q_reg(1) <= q_reg(2);
			q_reg(0) <= q_reg(1);
	 -- for index in q_reg'high - 1 downto 1 loop
	 --     q_reg(index - 1) <= q_reg(index);
	 -- end loop;
	 end if;

	end process in_reg;
	WITH sel SELECT reg_out <=
		std_logic_vector(q_reg(0)) WHEN "00",
		std_logic_vector(q_reg(1)) WHEN "01",
		std_logic_vector(q_reg(2)) WHEN "10",
		std_logic_vector(q_reg(3)) WHEN "11",
		"0000" WHEN OTHERS;

	max_min_logic : process(q_reg)
	variable max : unsigned(3 downto 0) := "0000";
	variable min : unsigned(3 downto 0) := "1111";
	begin         
		if (q_reg(0)(0) /= 'U') then
			for index in q_reg'high downto 1 loop
				if (max < q_reg(index)) then 
					if q_reg(index) > q_reg(index - 1) then
						max := q_reg(index);
					else 
						max := q_reg(index - 1);
					end if;
				end if; 

				if min > q_reg(index) then
					if q_reg(index) < q_reg(index - 1) then
						min := q_reg(index);
					else 
						min := q_reg(index - 1); 
					end if;
				end if;
			end loop;
		end if;
		load_max <= max;
		load_min <= min;
	end process max_min_logic;
	max_min_registers : process(clk, reset)
	begin 
		if reset = '1' then
			max_in <= "0000";
			min_in <= "1111";
		elsif rising_edge(clk) then 
			if (load_max > max_in and max_in(0) /= 'U')  then
				max_in <= load_max;
			end if;
			if (load_min < min_in and min_in(0) /= 'U') then
				min_in <= load_min;
			end if;
		end if;
	end process max_min_registers;
	max_out <= std_logic_vector(max_in);
	min_out <= std_logic_vector(min_in);
end;