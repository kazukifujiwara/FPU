library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package fcmp_p is

  function fcmp_eq(a, b: fpu_data_t) return fpu_data_t;
  function fcmp_gt(a, b: fpu_data_t) return fpu_data_t;

end package;

package body fcmp_p is

  type chk_result_t is (
    INVALID,
    EQ,
    GT,
    LT);

  function fcmp_chk(a, b: fpu_data_t)
    return chk_result_t is

    variable c, d: float_t;
    variable tc, td: float_type_t;

  begin

    c := float(a);
    d := float(b);

    tc := float_type(c);
    td := float_type(d);

    if tc = NAN or td = NAN then
      return INVALID;
    end if;

    case tc is
      when FORMAL =>
        case td is
          when PLUS_INF => return GT;
          when MINUS_INF => return LT;
          when others => null;
        end case;

      when PLUS_ZERO | MINUS_ZERO =>
        case td is
          when PLUS_ZERO | MINUS_ZERO => return EQ;
          when others => null;
        end case;

      when PLUS_INF =>
        case td is
          when PLUS_INF => return EQ;
          when others => return LT;
        end case;

      when MINUS_INF =>
        case td is
          when MINUS_INF => return EQ;
          when others => return GT;
        end case;

      when others => null;
    end case;

    if a = b then
      return EQ;
    elsif a > b then
      return GT;
    else
      return LT;
    end if;

  end function;

  function fcmp_eq(a, b: fpu_data_t)
    return fpu_data_t is
  begin

    case fcmp_chk(a, b) is
      when EQ => return to_unsigned(1, 32);
      when others => return to_unsigned(0, 32);
    end case;

  end function;

  function fcmp_gt(a, b: fpu_data_t)
    return fpu_data_t is
    -- return 1 if a > b
  begin

    case fcmp_chk(a, b) is
      when GT => return to_unsigned(1, 32);
      when others => return to_unsigned(0, 32);
    end case;

  end function;

end package body;

