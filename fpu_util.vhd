library ieee;
use ieee.std_logic_1164.all;

package fpu_util_p is

  function is_metavalue(v: std_logic_vector) return boolean;

end package;

package body fpu_util_p is

  function is_metavalue(v: std_logic_vector)
    return boolean is
  begin

    for i in 0 to v'length-1 loop
      if v(i) /= '0' and v(i) /= '1' then
        return true;
      end if;
    end loop;

    return false;

  end function;

end fpu_util_p;
