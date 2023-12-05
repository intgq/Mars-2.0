from ss2hcsp.sl.sl_block import SL_Block
from ss2hcsp.hcsp.expr import AVar, AConst, RelExpr


class Relation(SL_Block):
    """Relation of two dest lines."""
    def __init__(self, name: str, op: str, *, st=-1):
        super(Relation, self).__init__(type="relation", name=name, num_src=1, num_dest=2, st=st)

        if op not in ("<", ">", "==", "!=", ">=", "<="):
            raise AssertionError("Relation: unknown operator %s" % op)
        self.op = op

    def __str__(self):
        return "%s: Relation[in = %s, out = %s, op = %s, st = %s]" % \
               (self.name, str(self.dest_lines), str(self.src_lines), self.op, str(self.st))

    def __repr__(self):
        return str(self)
    
    def get_var_subst(self):
        out_var = self.src_lines[0][0].name
        in_var1 = self.dest_lines[0].name
        in_var2 = self.dest_lines[1].name
        return {out_var: RelExpr(self.op, AVar(in_var1), AVar(in_var2))}
