library ieee;
use ieee.std_logic_1164.all;

package fdiv_p is

  component fdiv is
    port (
      a : in std_logic_vector(31 downto 0);
      b : in std_logic_vector(31 downto 0);
      s : out std_logic_vector(31 downto 0));
  end component;

end package;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.finv_p.all;
use work.fmul_p.all;

entity fdiv is
    port (
      a : in std_logic_vector(31 downto 0);
      b : in std_logic_vector(31 downto 0);
      s : out std_logic_vector(31 downto 0));
end entity;

architecture behavior of fdiv is

  signal am, bm, sm: std_logic_vector(31 downto 0);
  signal ai, si: std_logic_vector(31 downto 0);

begin

  mul: fmul port map (
    a => am,
    b => bm,
    s => sm);

  inv: finv port map (
    a => ai,
    s => si);

  am <= a;
  ai <= b;
  bm <= si;
  s  <= sm;

end behavior;
