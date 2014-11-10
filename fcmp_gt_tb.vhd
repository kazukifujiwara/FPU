library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library work;
use work.fpu_common_p.all;
use work.fcmp_p.all;

entity fcmp_gt_tb is
end fcmp_gt_tb;

architecture test of fcmp_gt_tb is
  signal a, b, s : fpu_data_t;
begin

  process
    variable l, m: line;
    variable i: integer := 0;
    variable j: integer := 0;
    variable a, b, s, t: std_logic_vector(31 downto 0);
  begin

    while endfile(input) = false loop
      readline(input, l);
      hread(l, a);
      hread(l, b);
      hread(l, s);

      t := std_logic_vector(fcmp_gt(unsigned(a), unsigned(b)));

      if t /= s then
        hwrite(m, a);
        write(m, string'(" "));
        hwrite(m, b);
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
