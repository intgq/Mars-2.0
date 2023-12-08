from decimal import Decimal
from fractions import Fraction
from ss2hcsp.hcsp.hcsp import HCSP

from ss2hcsp.sl.sl_block import SL_Block
from ss2hcsp.hcsp.expr import AVar, AConst, RelExpr, IfExpr, disj, conj
from ss2hcsp.hcsp import hcsp as hp


class Hitcross(SL_Block):
    """Compute the hitcross value of the dest_line wrt. the upper and lower limits."""
    def __init__(self, name: str, HC_offset, HC_direction, HC_zerocross, st=-1):
        super(Hitcross, self).__init__("hitcross", name, 1, 1, st)

        assert isinstance(HC_offset, (int, Decimal, Fraction))
        self.HC_offset = HC_offset
        self.HC_direction = HC_direction
        self.HC_zerocross = HC_zerocross

    def get_expr(self):
        in_var = AVar(self.dest_lines[0].name)
        offset = AConst(self.HC_offset)
        pre_var = AVar(self.name + "_state")
        return IfExpr(disj(conj(RelExpr("<=", pre_var, offset),
                                RelExpr(">", in_var, offset)),
                           conj(RelExpr(">", pre_var, offset),
                                RelExpr("<=", in_var, offset))), AConst(1), AConst(0))
    
    def __str__(self):
        out_var = self.src_lines[0][0].name
        return "(Hitcross) %s: out = %s, offset = %s, direction = %s, zerocross = %s" % (
            self.name, out_var, self.HC_offset, self.HC_direction, self.HC_zerocross)

    def __repr__(self):
        return "Hitcross(%s, %s, %s, %s, in = %s, out = %s)" % \
            (self.name, self.HC_offset, self.HC_direction, self.HC_zerocross,
             str(self.dest_lines), str(self.src_lines))

    def get_init_hp(self) -> HCSP:
        pre_var = AVar(self.name + "_state")
        in_var = AVar(self.dest_lines[0].name)
        return hp.Assign(pre_var, in_var)

    def get_output_hp(self) -> HCSP:
        expr = self.get_expr()
        out_var = self.src_lines[0][0].name
        return hp.Assign(out_var, expr)

    def get_var_subst(self):
        expr = self.get_expr()
        out_var = self.src_lines[0][0].name
        return {out_var: expr}
    
    def get_update_hp(self):
        in_var = self.dest_lines[0].name
        return hp.Assign(self.name + "_state", AVar(in_var))
