library ieee;
use ieee.std_logic_1164.all;

package fneg_p is

  component fneg is
    port (
      a: in  std_logic_vector(31 downto 0);
      s: out std_logic_vector(31 downto 0));
  end component;

end package;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_util_p.all;
use work.fneg_p.all;

entity fneg is
  port (
    a: in std_logic_vector(31 downto 0);
    s: out std_logic_vector(31 downto 0));
end entity;

architecture behavior of fneg is
begin

  process (a)
    variable b: unsigned(31 downto 0);
  begin

    if is_metavalue(a) then
      s <= (others => '0');
    else
      b := unsigned(a);
      s <= std_logic_vector((not b(31 downto 31)) & b(30 downto 0));
    end if;

  end process;

end behavior;
