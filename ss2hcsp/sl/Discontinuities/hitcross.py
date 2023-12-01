from ss2hcsp.sl.sl_block import SL_Block
from ss2hcsp.hcsp.expr import AVar, AConst, RelExpr, IfExpr,disj,conj
from ss2hcsp.hcsp import hcsp as hp


class Hitcross(SL_Block):
    """Compute the hitcross value of the dest_line wrt. the upper and lower limits."""
    def __init__(self, name, HC_offset, HC_direction, HC_zerocross, st=-1):
        super(Hitcross, self).__init__("hitcross", name, 1, 1, st)

        assert isinstance(HC_offset, (int, float))
        self.HC_offset = HC_offset
        self.HC_direction = HC_direction
        self.HC_zerocross = HC_zerocross

    def get_expr(self):
        in_var = AVar(self.dest_lines[0].name)
        offset = AConst(self.HC_offset)
        pre_var = AVar(self.name+"_state")
        return IfExpr(disj(RelExpr("==",offset,in_var),conj(RelExpr("<",pre_var,offset),RelExpr("<",offset,in_var)),conj(RelExpr(">",pre_var,offset),RelExpr(">",offset,in_var))),AConst(1),AConst(0))
    
    def __str__(self):
        out_var = self.src_lines[0][0].name
        expr = self.get_expr()
        return "(Hitcross)%s: %s = %s" % (self.name, out_var, expr)

    def __repr__(self):
        return "Hitcross(%s, %s, %s, %s, in = %s, out = %s)" % \
            (self.name, self.HC_offset, self.HC_direction, self.HC_zerocross, str(self.dest_lines), str(self.src_lines))

    def get_output_hp(self):
        expr = self.get_expr()
        out_var = self.src_lines[0][0].name
        return hp.Assign(out_var, expr)

    def get_var_subst(self):
        expr = self.get_expr()
        out_var = self.src_lines[0][0].name
        return {out_var: expr}
    
    def get_update_hp(self):
        in_var = self.dest_lines[0].name
        return hp.Assign(self.name+"_state", AVar(in_var))
