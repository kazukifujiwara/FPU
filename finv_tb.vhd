-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use IEEE.std_logic_textio.all;

use std.textio.all;

use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

ENTITY testbench IS
END testbench;

ARCHITECTURE behavior OF testbench IS

  -- Component Declaration
  COMPONENT finv
    PORT(A : IN std_logic_vector(31 downto 0);
         S : OUT std_logic_vector(31 downto 0));
  END COMPONENT;

  SIGNAL i1 :  std_logic_vector(31 downto 0);
  SIGNAL o1 :  std_logic_vector(31 downto 0);

BEGIN

  -- Component Instantiation
  uut: finv PORT MAP(
    A => i1,
    S => o1);

  --  Test Bench Statements
  tb : PROCESS
    file infile : text is in "/home/kazuki/CPU/FPU/finv_test/testcase.txt";
    file outfile : text is out "/home/kazuki/CPU/FPU/finv_test/result.txt";
    variable my_line, out_line : LINE;
    variable a, b : std_logic_vector(31 downto 0);
  BEGIN
    
    wait for 100 ns; -- wait until global set/reset completes

    -- Add user defined stimulus here
    
    while not endfile(infile) loop
      readline(infile, my_line);
      read(my_line, a);

      i1 <= a;

      wait for 2 ns;

      b := o1;

      write(out_line, b);
      writeline(outfile, out_line);
    end loop;

  END PROCESS;

  --  End Test Bench

end behavior;
