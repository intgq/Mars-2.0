"""Assertions for reasoning about parallel programs."""

from typing import Iterable, List, Tuple, Union, Set
from ss2hcsp.hcsp import hcsp, expr
from ss2hcsp.hcsp.parser import parse_hp_with_meta
from wolframclient.evaluation import WolframLanguageSession
from wolframclient.language import wl, wlexpr
from wolframclient.language.expression import WLFunction, WLSymbol
from wolframclient.exception import WolframKernelException
from hhlpy.wolframengine_wrapper import solveODE, wl_prove, wl_reduce
from hhlpy.wolframengine_wrapper import wl_simplify, wl_polynomial_div, wl_is_polynomial, found_wolfram
from hhlpy.hhlpy import constraint_examination, compute_boundary

class EExpr:
    """Base class of extended expression used on assertions."""
    def substVar(self, var: str, expr: "EExpr") -> "EExpr":
        """Substitution of a variable by an expression.
        
        Default behavior is to return the corresponding VarSubstExpr.
        
        """
        return VarSubstExpr(self, var, expr)

class BoundVar(EExpr):
    """Bound variables.
    
    The following are special bound variables for assertions:
    s0 : State - starting state
    s : State - ending state
    tr : Trace - accumulated trace

    The following are special bound variables for path invariants:
    s0 : State - starting state
    t : Real - time
    s : State - state at time t

    For parameterized assertions and quantifier expressions, name
    of bound variables are indicated within the assertion or expression.
    
    """
    def __init__(self, name: str):
        self.name = name

    def __str__(self):
        return self.name

class VarExpr(EExpr):
    """Represents a variable in a state."""
    def __init__(self, name: str):
        self.name = name

    def __str__(self):
        return self.name

    def substVar(self, var: str, expr: EExpr) -> EExpr:
        if var == self.name:
            return expr
        else:
            return self

class ConstExpr(EExpr):
    """Represents a constant number."""
    def __init__(self, val: int):
        self.val = val

    def __str__(self):
        return str(self.val)
    
    def substVar(self, var: str, expr: EExpr) -> EExpr:
        return self

class VarSubstExpr(EExpr):
    """Substitution of a state on a variable."""
    def __init__(self, start_expr: EExpr, var: str, expr: EExpr):
        self.start_expr = start_expr
        self.var = var
        self.expr = expr

    def __str__(self):
        return "%s[%s := %s]" % (self.start_expr, self.var, self.expr)

class OpExpr(EExpr):
    """Operator."""
    def __init__(self, op: str, *exprs: EExpr):
        self.op = op
        self.exprs = tuple(exprs)

    def __str__(self):
        if len(self.exprs) == 1:
            return "%s%s" % (self.op, self.exprs[0])
        else:
            return "%s %s %s" % (self.exprs[0], self.op, self.exprs[1])

    def substVar(self, var: str, expr: EExpr) -> EExpr:
        return OpExpr(self.op, *(sube.substVar(var, expr) for sube in self.exprs))

class IfExpr(EExpr):
    """If expressions."""
    def __init__(self, cond: EExpr, if_branch: EExpr, else_branch: EExpr):
        self.cond = cond
        self.if_branch = if_branch
        self.else_branch = else_branch

    def __str__(self):
        return "if %s then %s else %s" % (self.cond, self.if_branch, self.else_branch)

class QuantExpr(EExpr):
    """Quantifier expressions."""
    def __init__(self, quantifier: str, bound_names: Iterable[str], expr: EExpr):
        self.quantifier = quantifier
        self.bound_names = bound_names,
        self.expr = expr

    def __str__(self):
        return "%s%s. %s" % (self.quantifier, " ".join(self.bound_names), self.expr)

class Assertion:
    """Base class for assertion for parallel programs.
    
    Each assertion is interpreted as a predicate on (s0, s, tr), where
    s0 is the starting state, s is the ending state, and tr is the
    accumulated trace.
    
    """
    def substVar(self, var: str, expr: EExpr) -> "Assertion":
        """Substitute given variable for an expression.
        
        The default implementation returns the VarSubstAssersion. Concrete
        assertions can choose to simplify the substitution.
        
        """
        return VarSubstAssertion(self, var, expr)

class InitAssertion(Assertion):
    """Assertion representing initial state.
    
    The interpretation is: s == s0 && tr = [].
    
    """
    def __init__(self):
        pass

    def __str__(self):
        return "init"
    
class VarSubstAssertion(Assertion):
    """Obtaining a new assertion by substitution of a variable.
    
    The interpretation is to perform the substitution on the starting
    state.
    
    """
    def __init__(self, start_assn: Assertion, var: str, expr: EExpr):
        self.start_assn = start_assn
        self.var = var
        self.expr = expr

    def __str__(self):
        return "%s[%s := %s]" % (self.start_assn, self.var, self.expr)

class OpAssn(Assertion):
    """Operators acting on assertions."""
    def __init__(self, op: str, *assns: Assertion):
        self.op = op
        self.assns = tuple(assns)

    def __str__(self):
        if len(self.assns) == 1:
            return "%s%s" % (self.op, self.assns[0])
        else:
            return "%s %s %s" % (self.assns[0], self.op, self.assns[1])
        
    def substVar(self, var: str, expr: EExpr) -> Assertion:
        return OpAssn(self.op, *(assn.subst_var(var, expr) for assn in self.assns))

class RecursionVar(Assertion):
    """Recursion variable."""
    def __init__(self, name: str):
        self.name = name

    def __str__(self):
        return self.name

class RecAssertion(Assertion):
    """Assertion defined by recursion."""
    def __init__(self, rec_var: str, assn: Assertion):
        self.rec_var = rec_var
        self.assn = assn

    def __str__(self):
        return "rec %s. %s" % (self.rec_var, self.assn)

class ParamAssertion:
    """Parameterized assertions.
    
    Possible parameters include:
    d : Real - delay time
    v : Real -> communicated value.
    
    """
    def __init__(self, bound_names: Iterable[str], assn: Assertion):
        self.bound_names = bound_names
        self.assn = assn

    def __str__(self):
        if len(self.bound_names) == 1:
            return "{%s => %s}" % (self.bound_names[0], self.assn)
        else:
            return "{(%s) => %s}" % (', '.join(self.bound_names), self.assn)
        
    def substVar(self, var: str, expr: EExpr) -> "ParamAssertion":
        """Substitution of a variable by an expression."""
        return ParamAssertion(self.bound_names, self.assn.substVar(var, expr))

class PathInvariant:
    """Base class of properties satisfied by a path.
    
    Each path invariant is interpreted as a predicate on (s0, t, s),
    where s0 is the starting state, t is the time, and s is the state
    reached at time t.
    
    """
    def substVar(self, var: str, expr: EExpr) -> "PathInvariant":
        return SubstVarPathInvariant(self, var, expr)

class IdPathInvariant(PathInvariant):
    """Identity path invariant.
    
    The interpretation is: s = s0 (for all time t).
    """
    def __init__(self):
        pass

    def __str__(self):
        return "id_inv"
    
    def substVar(self, var: str, expr: EExpr) -> PathInvariant:
        return ExprPathInvariant(OpExpr("==", BoundVar("s"), BoundVar("s0").substVar(var, expr)))
    
class ExprPathInvariant(PathInvariant):
    """Path invariant specified by an expression.
    
    The expression contains special variables: s0, s and t.
    
    """
    def __init__(self, expr: EExpr):
        self.expr = expr

    def __str__(self):
        return str(self.expr)
    
class SubstVarPathInvariant(PathInvariant):
    """Substituting a variable in a path invariant."""
    def __init__(self, start_path_inv: PathInvariant, var: str, expr: EExpr):
        self.start_path_inv = start_path_inv
        self.var = var
        self.expr = expr

    def __str__(self):
        return "%s[%s := %s]" % (self.start_path_inv, self.var, self.expr)

class WaitInAssertion(Assertion):
    """Wait-in assertion.
    
    Syntax for wait-in assertion is:

        wait_in(path_inv, ch, {(d, v) => assn})

    The components are path invariant, channel of communication, and the
    ensuing assertion, respectively.

    """
    def __init__(self, path_inv: PathInvariant, ch_name: str, param_assn: ParamAssertion):
        self.path_inv = path_inv
        self.ch_name = ch_name
        assert len(param_assn.bound_names) == 2
        self.param_assn = param_assn

    def __str__(self):
        return "wait_in(%s, %s, %s)" % (self.path_inv, self.ch_name, self.param_assn)

class WaitOutAssertion(Assertion):
    """Wait-out assertion.
    
    Syntax for wait-out assertion is:

        wait_out(path_inv, ch, {(d, v) => assn})

    The components are path invariant, channel of communication, and the
    ensuing assertion, respectively.
    
    """
    def __init__(self, path_inv: PathInvariant, ch_name: str, param_assn: ParamAssertion):
        self.path_inv = path_inv
        self.ch_name = ch_name
        assert len(param_assn.bound_names) == 2
        self.param_assn = param_assn

    def __str__(self):
        return "wait_out(%s, %s, %s)" % (self.path_inv, self.ch_name, self.param_assn)

class WaitOutvAssertion(Assertion):
    """Wait-outv assertion.
    
    Syntax for wait-outv assertion is:

        wait_outv(path_inv, ch, e, {d => assn})

    The components are path invariant, channel of communication, expression
    for the value sent, and the ensuing assertion.
    
    """
    def __init__(self, path_inv: PathInvariant, ch_name: str, expr: EExpr, param_assn: ParamAssertion):
        self.path_inv = path_inv
        self.ch_name = ch_name
        self.expr = expr
        assert len(param_assn.bound_names) == 1
        self.param_assn = param_assn

    def __str__(self):
        return "wait_outv(%s, %s, %s, %s)" % (self.path_inv, self.ch_name, self.expr, self.param_assn)
    
    def substVar(self, var: str, expr: EExpr) -> Assertion:
        return WaitOutvAssertion(self.path_inv.substVar(var, expr), self.ch_name,
                                 self.expr.substVar(var, expr), self.param_assn.substVar(var, expr))

class WaitIn0Assertion(Assertion):
    """Wait-in0 assertion.

    Version of wait-in assuming immediate communication. The syntax is:

        wait_in0(path_inv, ch, {v => assn})

    The components are path invariant, channel of communication, and the
    ensuing assertion.

    TODO: investigate whether path_inv is necessary in this case.
    
    """
    def __init__(self, path_inv: PathInvariant, ch_name: str, param_assn: ParamAssertion):
        self.path_inv = path_inv
        self.ch_name = ch_name
        assert len(param_assn.bound_names) == 1
        self.param_assn = param_assn

    def __str__(self):
        return "wait_in0(%s, %s, %s)" % (self.path_inv, self.ch_name, self.param_assn)

class WaitAssertion(Assertion):
    """Wait assertion.
    
    Syntax for wait assertion is:

        wait(path_inv, e, {(d => assn)})

    The components are path invariant, expression for delay time, and the
    ensuing assertion.

    """
    def __init__(self, path_inv: PathInvariant, delay_expr: EExpr, param_assn: ParamAssertion):
        self.path_inv = path_inv
        self.delay_expr = delay_expr
        assert len(param_assn.bound_names) == 1
        self.param_assn = param_assn

    def __str__(self):
        return "wait(%s, %s, %s)" % (self.path_inv, self.delay_expr, self.param_assn)
    

class InterruptAssertion(Assertion):
    """Wait assertion.
    
    Syntax for wait assertion is:

        wait(path_inv, e, {(d => assn)})

    The components are path invariant, expression for delay time, and the
    ensuing assertion.

    """
    def __init__(self, path_inv: PathInvariant, delay_expr: expr.Expr, param_assn: ParamAssertion, comm_specs):
        self.path_inv = path_inv
        self.delay_expr = delay_expr
        assert len(param_assn.bound_names) == 1
        self.param_assn = param_assn
        for comm_spec in comm_specs:
            assert isinstance(comm_spec, tuple) and len(comm_spec) == 2
            assert hcsp.is_comm_channel(comm_spec[0]) and isinstance(comm_spec[1], ParamAssertion)
        self.comm_specs: List[Tuple[hcsp.HCSP, Assertion]] = comm_specs

    def __str__(self):
        str_comms = ", ".join(str(comm_hp)[:-1] + " --> " + str(out_assn) for comm_hp, out_assn in self.comm_specs)
        return "interrupt(%s, %s, %s, (%s))" % (self.path_inv, self.delay_expr, self.param_assn, str_comms)


init = InitAssertion()
id_inv = IdPathInvariant()

def expr_to_eexpr(e: expr.Expr) -> EExpr:
    """Convert expr in HCSP to EExpr."""
    if isinstance(e, expr.AVar):
        return VarExpr(e.name)
    elif isinstance(e, expr.AConst):
        return ConstExpr(e.value)
    elif isinstance(e, expr.OpExpr):
        return OpExpr(e.op, *(expr_to_eexpr(sube) for sube in e.exprs))
    else:
        raise NotImplementedError

def variant_name(vars: List[str], prefix: str) -> str:
    i = 1
    while prefix + str(i) in vars:
        i += 1
    vars.append(prefix + str(i))
    return prefix + str(i)
 
def seq_hcsp_to_assn_gen(hp: hcsp.HCSP, remain: Assertion, bound_vars: List[str]) -> Assertion:
    """Obtain assertion for sequential HCSP programs."""
    if isinstance(hp, hcsp.Skip):
        return remain
    elif isinstance(hp, hcsp.Assign):
        return remain.substVar(hp.var_name.name, expr_to_eexpr(hp.expr))
    elif isinstance(hp, hcsp.InputChannel):
        dname, vname = variant_name(bound_vars, "d"), variant_name(bound_vars, "v")
        return WaitInAssertion(id_inv, hp.ch_name.name,
                               ParamAssertion([dname, vname],
                                              remain.substVar(hp.var_name.name, VarExpr(vname))))
    elif isinstance(hp, hcsp.OutputChannel):
        dname = variant_name(bound_vars, "d")
        return WaitOutvAssertion(id_inv, hp.ch_name.name, expr_to_eexpr(hp.expr),
                                 ParamAssertion([dname], remain))
    elif isinstance(hp, hcsp.Wait):
        dname = variant_name(bound_vars, "d")
        return WaitAssertion(id_inv, expr_to_eexpr(hp.delay),
                             ParamAssertion([dname], remain))
    elif isinstance(hp, hcsp.Sequence):
        raise AssertionError
    else:
        raise NotImplementedError

# def seq_hcsp_to_assn(hp: hcsp.HCSP) -> Assertion:
#     bound_vars = []

#     def rec(hp):
#         nonlocal bound_vars
#         if isinstance(hp, hcsp.Sequence):
#             if len(hp.hps) == 1:
#                 return rec(hp.hps[0])
#             else:
#                 Q = rec(hcsp.Sequence(*(hp.hps[1:])))
#                 return seq_hcsp_to_assn_gen(hp.hps[0], Q, bound_vars)
#         else:
#             return seq_hcsp_to_assn_gen(hp, init, bound_vars)

#     return rec(hp)


# def seq_hcsp_to_assn(hp: hcsp.HCSP) -> Assertion:
#     bound_vars = []

#     def rec(hp, remain: Assertion):
#         nonlocal bound_vars
#         if isinstance(hp, hcsp.Sequence):
#             if len(hp.hps) == 1:
#                 return rec(hp.hps[0], remain)
#             else:
#                 Q = rec(hcsp.Sequence(*(hp.hps[1:])), remain)
#                 return seq_hcsp_to_assn_gen(hp.hps[0], Q, bound_vars)
#         elif isinstance(hp, hcsp.Loop):
#             Rname = variant_name(bound_vars, "R")
#             Q = rec(hp.hp, RecursionVar(Rname))
#             return RecAssertion(Rname,Q)
#         else:
#             return seq_hcsp_to_assn_gen(hp, remain, bound_vars)

#     return rec(hp, init)

def seq_hcsp_to_assn(hp: hcsp.HCSP) -> Assertion:
    bound_vars = []

    def rec(hp, remain: Assertion):
        nonlocal bound_vars
        if isinstance(hp, hcsp.Sequence):
            if len(hp.hps) == 1:
                return rec(hp.hps[0], remain)
            else:
                Q = rec(hcsp.Sequence(*(hp.hps[1:])), remain)
                return rec(hp.hps[0], Q)
        elif isinstance(hp, hcsp.Loop):
            Rname = variant_name(bound_vars, "R")
            Q = rec(hp.hp, RecursionVar(Rname))
            P = OpAssn("||", init, Q)
            return RecAssertion(Rname,P)
        elif isinstance(hp, hcsp.Skip):
            return remain
        elif isinstance(hp, hcsp.Assign):
            return remain.substVar(hp.var_name.name, expr_to_eexpr(hp.expr))
        elif isinstance(hp, hcsp.InputChannel):
            dname, vname = variant_name(bound_vars, "d"), variant_name(bound_vars, "v")
            return WaitInAssertion(id_inv, hp.ch_name.name,
                               ParamAssertion([dname, vname],
                                              remain.substVar(hp.var_name.name, VarExpr(vname))))
        elif isinstance(hp, hcsp.OutputChannel):
            dname = variant_name(bound_vars, "d")
            return WaitOutvAssertion(id_inv, hp.ch_name.name, expr_to_eexpr(hp.expr),
                                 ParamAssertion([dname], remain))
        elif isinstance(hp, hcsp.Wait):
            dname = variant_name(bound_vars, "d")
            return WaitAssertion(id_inv, expr_to_eexpr(hp.delay),
                             ParamAssertion([dname], remain))
        elif isinstance(hp, hcsp.ODE_Comm):
            time_var = variant_name(bound_vars, "t")
            solution_dict = solveODE(hp, set(bound_vars), time_var)
            # print(solution_dict)
            inv = id_inv
            vlist = []
            dname, vname = variant_name(bound_vars, "d"), variant_name(bound_vars, "v")
            ddic = dict()
            ddic[time_var] = expr.AVar(dname)
            P = remain
            for fun_name, sol in solution_dict.items():
                # print(sol)
                inv = inv.substVar(fun_name,expr_to_eexpr(sol))
                vlist.append(str(fun_name))
                P = P.substVar(fun_name,expr_to_eexpr(sol.subst(ddic)))
            vlist.append(time_var)

            cons = hp.constraint.subst(solution_dict)
            if cons == expr.true_expr:
                waite = cons
            else:
                # consall = expr.LogicExpr('&&', hp.constraint, compute_boundary(cons))
                wait = wl_reduce(compute_boundary(cons),[time_var])
                waite = wait
            comm_specs = []
            for comm_hp, out_hp in hp.io_comms:
                Q = rec(out_hp,remain)
                for fun_name, sol in solution_dict.items():
                    Q = Q.substVar(fun_name,expr_to_eexpr(sol.subst(ddic)))
                if comm_hp.type == "input_channel":
                    comm_specs.append((comm_hp,ParamAssertion([dname, vname],Q.substVar(comm_hp.var_name.name, VarExpr(vname)))))
                else:
                    comm_specs.append((comm_hp,ParamAssertion([dname],Q)))


            return InterruptAssertion(inv, waite, ParamAssertion([dname],P), comm_specs)
        elif isinstance(hp, hcsp.ODE):
            time_var = variant_name(bound_vars, "t")
            solution_dict = solveODE(hp, set(bound_vars), time_var)
            # print(solution_dict)
            inv = id_inv
            vlist = []
            dname, vname = variant_name(bound_vars, "d"), variant_name(bound_vars, "v")
            ddic = dict()
            ddic[time_var] = expr.AVar(dname)
            P = remain
            for fun_name, sol in solution_dict.items():
                # print(sol)
                inv = inv.substVar(fun_name,expr_to_eexpr(sol))
                vlist.append(str(fun_name))
                P = P.substVar(fun_name,expr_to_eexpr(sol.subst(ddic)))
            vlist.append(time_var)

            cons = hp.constraint.subst(solution_dict)
            if cons == expr.true_expr:
                waite = cons
            else:
                consall = expr.LogicExpr('&&', hp.constraint, compute_boundary(cons))
                consall = expr.LogicExpr('&&', expr.RelExpr('>=', expr.AVar(time_var), expr.AConst(0)), consall)
                wait = wl_reduce(consall, [time_var])
                waite = wait
            return WaitAssertion(inv, waite, ParamAssertion([dname],P))
            
        else:
            raise NotImplementedError

    return rec(hp, init)
