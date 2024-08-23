-- coen 316 lab 4
-- Andre Hei Wang Law
-- 4017 5600

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_signed.all;

entity cpu is
port(reset:			in std_logic;
	clk: 			in std_logic;
	rs_out, rt_out: out std_logic_vector(3 downto 0) := (others => '0'); -- output ports from reg. file
	pc_out: 		out std_logic_vector(3 downto 0) := (others => '0');
	overflow, zero: out std_logic); 
end cpu;

architecture datapath_cpu of cpu is

-- initialize components
	-- next address
	component next_address
	port(
		rt, rs:			in std_logic_vector(31 downto 0);
		pc:				in std_logic_vector(31 downto 0);
		target_address:	in std_logic_vector(25 downto 0);
		branch_type:	in std_logic_vector(1 downto 0);
		pc_sel:			in std_logic_vector(1 downto 0);
		next_pc:		out std_logic_vector(31 downto 0));
	end component;

	-- pc
	component pc
	port(
		reset:		in std_logic;
		clk:		in std_logic;
		d_in:		in std_logic_vector(31 downto 0) := (others => '0');
		q_out:		out std_logic_vector(31 downto 0):= (others => '0'));
	end component;

	-- i_cache
	component i_cache
	port(
		addr_in:	in std_logic_vector(4 downto 0);
		inst_out:	out std_logic_vector(31 downto 0));
	end component;

	-- register file
	component reg_file
	port(
		din: 	in std_logic_vector(31 downto 0);	-- data written to specific register
		reset: 	in std_logic;	-- asynchronous active-high reset input
		clk: 	in std_logic;	-- clock input
		write: 	in std_logic;	-- write control signal
		read_a: in std_logic_vector(4 downto 0);	-- address for read port A
		read_b: in std_logic_vector(4 downto 0);	-- address for read port B
		write_address: in std_logic_vector(4 downto 0);	-- address for write port
		out_a: 	out std_logic_vector(31 downto 0);	-- data output for read port A
		out_b: 	out std_logic_vector(31 downto 0));	-- data output for read port B
	end component;

	-- alu
	component alu
	port (
		x, y: in std_logic_vector(31 downto 0);		-- two input operands
		add_sub: 	in std_logic;				-- 0=add, 1=sub
		logic_func:	in std_logic_vector(1 downto 0);	-- 00=AND, 01=OR, 10=XOR, 11=NOR
		func:		in std_logic_vector(1 downto 0);	-- 00=lui, 01=setless, 10=arith, 11=logic
		output:		out std_logic_vector(31 downto 0);
		overflow:	out std_logic;
		zero:		out std_logic);
	end component;

	-- d_cache
	component d_cache
	port(
		d_in:		in std_logic_vector(31 downto 0);
		reset:		in std_logic;
		clk:		in std_logic;
		data_write:	in std_logic;
		addr:		in std_logic_vector(4 downto 0);
		d_out:		out std_logic_vector(31 downto 0));
	end component;

	-- sign extend
	component sign_ext
	port(
		func:		in std_logic_vector(1 downto 0);
		sign_in:	in std_logic_vector(15 downto 0);
		sign_out:	out std_logic_vector(31 downto 0));
	end component;

	-- control unit
	component control_unit
	port(
		op:				in std_logic_vector(5 downto 0);
		func_in:		in std_logic_vector(5 downto 0);
		reg_write:		out std_logic;
		reg_dst:		out std_logic;
		reg_in_src:		out std_logic;
		alu_src:		out std_logic;
		add_sub:		out std_logic;
		data_write:		out std_logic;
		logic_func:		out std_logic_vector(1 downto 0);
		func:			out std_logic_vector(1 downto 0);
		branch_type:	out std_logic_vector(1 downto 0);
		pc_sel:			out std_logic_vector(1 downto 0));
	end component;

-- signals
	-- control unit
	--signal s_op:			std_logic_vector(5 downto 0);
	--signal s_func_in:		std_logic_vector(5 downto 0);
	signal s_reg_write:		std_logic;
	signal s_reg_dst:		std_logic;
	signal s_reg_in_src:	std_logic;
	signal s_alu_src:		std_logic;
	signal s_add_sub:		std_logic;
	signal s_data_write:	std_logic;
	signal s_logic_func:	std_logic_vector(1 downto 0);
	signal s_func:			std_logic_vector(1 downto 0);
	signal s_branch_type:	std_logic_vector(1 downto 0);
	signal s_pc_sel:		std_logic_vector(1 downto 0);
	
	signal s_inst: 			std_logic_vector(31 downto 0);
	
	-- next address
	signal s_out_a:		std_logic_vector(31 downto 0);
	signal s_out_b:		std_logic_vector(31 downto 0);
	signal s_pc:		std_logic_vector(31 downto 0);
	signal s_next_pc:	std_logic_vector(31 downto 0);
	
	-- pc

	-- i_cache


	-- register file
	signal s_reg_in_src_out:	std_logic_vector(31 downto 0);
	signal s_reg_dst_out:	std_logic_vector(4 downto 0);

	-- alu
	signal s_alu_src_out:	std_logic_vector(31 downto 0);
	signal s_alu:			std_logic_vector(31 downto 0);


	-- d_cache
	signal s_d_out:			std_logic_vector(31 downto 0);

	-- sign extend
	signal s_sign_out:		std_logic_vector(31 downto 0);


	-- 	FOR pm_pc : "component_name" USE ENTITY work."entity_name" ("architecutre_name");
	FOR pm_pc : pc USE ENTITY work.pc (pc_arch);
	FOR pm_i_cache : i_cache USE ENTITY work.i_cache(i_cache_arch);
	FOR pm_reg_file : reg_file  USE ENTITY work.regfile (reg_arch);
	FOR pm_alu : alu USE ENTITY work.alu(arith_logic_unit);
	FOR pm_d_cache : d_cache USE ENTITY work.d_cache(d_cache_arch);
	FOR pm_sign_ext : sign_ext USE ENTITY work.sign_extended(sign_extended_arch);
	FOR pm_next_address : next_address USE ENTITY work.next_address(unit);
	FOR pm_control_unit : control_unit USE ENTITY work.control_unit(control_unit_arch);

begin
	-- port map
	pm_pc: pc port map( -- pc
		reset => reset,
		clk => clk,
		d_in => s_next_pc,
		q_out => s_pc
	);
	
	pm_i_cache: i_cache port map( -- i_cache
		addr_in => s_pc(4 downto 0),
		inst_out => s_inst
	);
	
	pm_reg_file: reg_file port map( -- reg_file
		din => s_reg_in_src_out,
		reset => reset,
		clk => clk,
		write => s_reg_write,
		read_a => s_inst(25 downto 21),
		read_b => s_inst(20 downto 16),
		write_address => s_reg_dst_out,
		out_a => s_out_a,
		out_b => s_out_b
	);
	
	pm_alu: alu port map( -- alu
		x => s_out_a,
		y => s_alu_src_out,
		add_sub => s_add_sub,
		logic_func => s_logic_func,
		func => s_func,
		output => s_alu,
		overflow => overflow,
		zero => zero
	);
	
	pm_d_cache: d_cache port map( -- d_cache
		d_in => s_out_b,
		reset => reset,
		clk => clk,
		data_write => s_data_write,
		addr => s_alu(4 downto 0),
		d_out => s_d_out
	);
	
	pm_sign_ext: sign_ext port map( -- sign_ext
		func => s_func,
		sign_in => s_inst(15 downto 0),
		sign_out => s_sign_out
	);
	
	pm_next_address: next_address port map( -- next address
		rt => s_out_b,
		rs => s_out_a,
		pc => s_pc,
		target_address => s_inst(25 downto 0),
		branch_type => s_branch_type,
		pc_sel => s_pc_sel,
		next_pc => s_next_pc
	);
	
	pm_control_unit: control_unit port map( -- control_unit
		op => s_inst(31 downto 26),
		func_in => s_inst(5 downto 0),
		reg_write => s_reg_write,
		reg_dst => s_reg_dst,
		reg_in_src => s_reg_in_src,
		alu_src => s_alu_src,
		add_sub => s_add_sub,
		data_write => s_data_write,
		logic_func => s_logic_func,
		func => s_func,
		branch_type => s_branch_type,
		pc_sel => s_pc_sel
	);
	
	s_reg_dst_out <= 	s_inst(20 downto 16) when (s_reg_dst = '0') else
						s_inst(15 downto 11) when (s_reg_dst = '1');
	
	s_alu_src_out <=	s_out_b when (s_alu_src = '0') else
						s_sign_out when (s_alu_src = '1');

	s_reg_in_src_out <= s_d_out when (s_reg_in_src = '0') else
						s_alu when (s_reg_in_src = '1');
	
	rs_out <= s_out_a(3) & s_out_a(2) & s_out_a(1) & s_out_a(0);
	rt_out <= s_out_b(3) & s_out_b(2) & s_out_b(1) & s_out_b(0);
	pc_out <= s_pc(3) & s_pc(2) & s_pc(1) & s_pc(0);
	
end datapath_cpu;


