library IEEE;
use IEEE.std_logic_1164.all;

package fsub_p is

  component fsub is
    port (
      a, b : in std_logic_vector(31 downto 0);
      s    : out std_logic_vector(31 downto 0));
  end component;

end package;


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_misc.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;

library work;
use work.fadd_p.all;
use work.fneg_p.all;

entity fsub is
  Port ( A,B : in std_logic_vector(31 downto 0);
         S   : out std_logic_vector(31 downto 0));
end entity fsub;

architecture blackbox of FSUB is

  signal i1 : std_logic_vector(31 downto 0);
  signal i2 : std_logic_vector(31 downto 0);
  signal n2 : std_logic_vector(31 downto 0);
  signal o1 : std_logic_vector(31 downto 0);

begin
  -- Component Instantiation
  fadd_connect: fadd port map(
    A => i1,
    B => n2,
    S => o1);

  fneg_connect: fneg port map(
    A => i2,
    S => n2);

  i1 <= A;
  i2 <= B;
  S  <= o1;
end blackbox;

