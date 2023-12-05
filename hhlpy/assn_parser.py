"""Parser for assertions."""

from typing import List, Tuple
from lark import Lark, Transformer, v_args, exceptions, Token

from hhlpy import parallel
from hhlpy.parallel import EExpr, Assertion, ParamAssertion, PathInvariant

grammar = r"""
    ?atom_expr: CNAME -> var_expr
        | INT -> int_expr
        | atom_expr "[" CNAME ":=" expr "]" -> var_subst_expr
        | "(" expr ")"
        
    ?power_expr: power_expr "^" atom_expr -> power_expr    // priority 85
        | atom_expr

    ?uminus: "-" uminus -> uminus_expr                     // priority 80
        | power_expr

    ?times_expr: times_expr "*" uminus -> times_expr       // priority 70
        | times_expr "/" uminus -> divide_expr
        | times_expr "%" uminus -> mod_expr
        | uminus

    ?plus_expr: plus_expr "+" times_expr -> plus_expr      // priority 65
        | plus_expr "-" times_expr -> minus_expr
        | times_expr

    ?if_expr: "if" expr "then" if_expr "else" if_expr      // priority 40
        | plus_expr

    ?rel_expr: if_expr "==" if_expr -> eq_cond             // priority 50
        | if_expr "!=" if_expr -> ineq_cond
        | if_expr "<=" if_expr -> less_eq_cond
        | if_expr "<" if_expr -> less_cond
        | if_expr ">=" if_expr -> greater_eq_cond
        | if_expr ">" if_expr -> greater_cond
        | if_expr

    ?neg_expr: "!" neg_expr -> not_cond          // priority 40
        | rel_expr

    ?conj: neg_expr "&&" conj -> conj_expr       // Conjunction: priority 35
        | neg_expr

    ?disj: conj "||" disj -> disj_expr           // Disjunction: priority 30
        | conj

    ?equiv: disj "<->" equiv -> equiv_expr       // Equivalence: priority 25
        | disj

    ?imp: equiv "->" imp -> imp_expr             // Implies: priority 20
        | equiv

    ?bound_var: CNAME -> bound_var

    ?bound_vars: bound_vars ("," bound_var)* -> bound_vars

    ?quant: "exists" bound_vars "." quant -> exists_expr  // priority 10
        | "forall" bound_vars "." quant -> forall_expr
        | imp

    ?expr: quant
    
    ?path_inv: "id_inv" -> id_path_inv
        | expr -> expr_path_inv

    ?param_assn1: "{" bound_var "=>" assn "}" -> param_assn1

    ?param_assn2: "{" "(" bound_var "," bound_var ")" "=>" assn "}" -> param_assn2

    ?rec_var: CNAME -> rec_var

    ?atom_assn: "init" -> init_assn
        | atom_assn "[" CNAME ":=" expr "]" -> var_subst_assn
        | CNAME -> rec_var_assn
        | "rec" rec_var "." assn -> rec_assn
        | "wait_in" "(" path_inv "," CNAME "," param_assn2 ")" -> wait_in_assn
        | "wait_out" "(" path_inv "," CNAME "," param_assn2 ")" -> wait_out_assn
        | "wait_outv" "(" path_inv "," CNAME "," expr "," param_assn1 ")" -> wait_outv_assn
        | "wait_in0" "(" path_inv "," CNAME "," param_assn1 ")" -> wait_in0_assn
        | "wait" "(" path_inv "," expr "," param_assn1 ")" -> wait_assn
        | "(" assn ")"

    ?disj_assn: atom_assn "||" disj_assn -> disj_assn
        | atom_assn

    ?assn: disj_assn

    %import common.CNAME
    %import common.WS
    %import common.INT

    %ignore WS
"""

@v_args(inline=True)
class AssnTransformer(Transformer):
    def __init__(self):
        # Initialize list of pre-set bound variables
        self.bound_vars: List[str] = ["s0", "s", "t", "tr"]

        # List of recursion variables in assertions
        self.rec_vars: List[str] = []

    def var_expr(self, s: Token) -> EExpr:
        s = str(s)
        if s in self.bound_vars:
            return parallel.BoundVar(s)
        else:
            return parallel.VarExpr(s)
    
    def int_expr(self, n: Token) -> EExpr:
        return parallel.ConstExpr(int(n))

    def var_subst_expr(self, start_expr: EExpr, var: Token, expr: EExpr) -> EExpr:
        return parallel.VarSubstExpr(start_expr, str(var), expr)
    
    def power_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("^", expr1, expr2)
    
    def uminus_expr(self, expr: EExpr) -> EExpr:
        return parallel.OpExpr("-", expr)
    
    def times_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("*", expr1, expr2)
    
    def divide_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("/", expr1, expr2)
    
    def mod_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("%", expr1, expr2)
    
    def plus_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("+", expr1, expr2)
    
    def minus_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("-", expr1, expr2)
    
    def if_expr(self, cond: EExpr, if_branch: EExpr, else_branch: EExpr) -> EExpr:
        return parallel.IfExpr(cond, if_branch, else_branch)
    
    def eq_cond(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("==", expr1, expr2)
    
    def ineq_cond(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("!=", expr1, expr2)

    def less_eq_cond(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("<=", expr1, expr2)

    def less_cond(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("<", expr1, expr2)

    def greater_eq_cond(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr(">=", expr1, expr2)

    def greater_cond(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr(">", expr1, expr2)

    def not_cond(self, expr: EExpr) -> EExpr:
        return parallel.OpExpr("!", expr)
    
    def conj_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("&&", expr1, expr2)
    
    def disj_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("||", expr1, expr2)

    def equiv_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("<->", expr1, expr2)

    def imp_expr(self, expr1: EExpr, expr2: EExpr) -> EExpr:
        return parallel.OpExpr("->", expr1, expr2)

    def bound_var(self, name: Token) -> str:
        name = str(name)
        if name in self.bound_vars:
            raise AssertionError("repeated bound variable: %s" % name)
        self.bound_vars.append(name)
        return name
    
    def bound_vars(self, *vars: str) -> Tuple[str]:
        return tuple(vars)

    def exists_expr(self, bound_vars: Tuple[str], expr: EExpr) -> EExpr:
        for bound_var in bound_vars:
            self.bound_vars.remove(bound_var)
        return parallel.QuantExpr("exists", bound_vars, expr)
    
    def forall_expr(self, bound_vars: Tuple[str], expr: EExpr) -> EExpr:
        for bound_var in bound_vars:
            self.bound_vars.remove(bound_var)
        return parallel.QuantExpr("forall", bound_vars, expr)
        
    def id_path_inv(self) -> PathInvariant:
        return parallel.IdPathInvariant()
    
    def expr_path_inv(self, expr: EExpr) -> PathInvariant:
        return parallel.ExprPathInvariant(expr)

    def param_assn1(self, bound_var: str, assn: Assertion) -> ParamAssertion:
        self.bound_vars.remove(bound_var)
        return parallel.ParamAssertion((bound_var,), assn)
    
    def param_assn2(self, bound_var1: str, bound_var2: str, assn: Assertion) -> ParamAssertion:
        self.bound_vars.remove(bound_var1)
        self.bound_vars.remove(bound_var2)
        return parallel.ParamAssertion((bound_var1, bound_var2), assn)

    def init_assn(self):
        return parallel.InitAssertion()
    
    def var_subst_assn(self, start_assn: Assertion, var: Token, expr: EExpr) -> Assertion:
        return parallel.VarSubstAssertion(start_assn, str(var), expr)
    
    def rec_var(self, name: Token) -> str:
        name = str(name)
        self.rec_vars.append(name)
        return name

    def rec_var_assn(self, name: Token) -> Assertion:
        name = str(name)
        if name not in self.rec_vars:
            raise AssertionError("recursion variable %s not found" % name)
        return parallel.RecursionVar(name)

    def rec_assn(self, rec_var: str, assn: Assertion) -> Assertion:
        self.rec_vars.remove(rec_var)
        return parallel.RecAssertion(rec_var, assn)
    
    def wait_in_assn(self, path_inv: PathInvariant, ch_name: Token, param_assn: ParamAssertion):
        return parallel.WaitInAssertion(path_inv, ch_name, param_assn)

    def wait_out_assn(self, path_inv: PathInvariant, ch_name: Token, param_assn: ParamAssertion):
        return parallel.WaitOutAssertion(path_inv, ch_name, param_assn)

    def wait_outv_assn(self, path_inv: PathInvariant, ch_name: Token, expr: EExpr, param_assn: ParamAssertion):
        return parallel.WaitOutvAssertion(path_inv, ch_name, expr, param_assn)

    def wait_in0_assn(self, path_inv: PathInvariant, ch_name: Token, param_assn: ParamAssertion):
        return parallel.WaitIn0Assertion(path_inv, ch_name, param_assn)
    
    def wait_assn(self, path_inv: PathInvariant, delay_expr: EExpr, param_assn: ParamAssertion):
        return parallel.WaitAssertion(path_inv, delay_expr, param_assn)

    def disj_assn(self, assn1: Assertion, assn2: Assertion) -> Assertion:
        return parallel.OpAssn("||", assn1, assn2)

expr_parser = Lark(grammar, start="expr", parser="lalr", transformer=AssnTransformer())
path_inv_parser = Lark(grammar, start="path_inv", parser="lalr", transformer=AssnTransformer())
assn_parser = Lark(grammar, start="assn", parser="lalr", transformer=AssnTransformer())


def parse_expr(s: str) -> EExpr:
    return expr_parser.parse(s)

def parse_path_inv(s: str) -> PathInvariant:
    return path_inv_parser.parse(s)

def parse_assn(s: str) -> Assertion:
    return assn_parser.parse(s)
