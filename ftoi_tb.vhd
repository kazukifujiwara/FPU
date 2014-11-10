library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library work;
use work.fpu_common_p.all;
use work.ftoi_p.all;

entity ftoi_tb is
end ftoi_tb;

architecture test of ftoi_tb is
  signal a, b, s : fpu_data_t;
begin

  process
    variable l, m: line;
    variable i: integer := 0;
    variable j: integer := 0;
    variable a, s, t: std_logic_vector(31 downto 0);
  begin

    while endfile(input) = false loop
      readline(input, l);
      hread(l, a);
      hread(l, s);

      t := std_logic_vector(ftoi(unsigned(a)));

      if t /= s then
        hwrite(m, a);
        write(m, string'(" "));
        hwrite(m, s);
        write(m, string'(" "));
        hwrite(m, t);
        writeline(output, m);
      elsif j = 0 then
        write(m, i);
        writeline(output, m);
        j := 1000;
      end if;

      i := i+1;
      j := j-1;
    end loop;

    wait;
  end process;
end test;
