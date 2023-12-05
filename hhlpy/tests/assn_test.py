"""Unit test for parallel assertions."""

import unittest

from hhlpy.assn_parser import parse_expr, parse_path_inv, parse_assn
from hhlpy.parallel import seq_hcsp_to_assn
from ss2hcsp.hcsp.parser import parse_hp_with_meta
from wolframclient.evaluation import WolframLanguageSession
from wolframclient.language import wl, wlexpr
from wolframclient.language.expression import WLFunction, WLSymbol
from wolframclient.exception import WolframKernelException
from hhlpy.wolframengine_wrapper import solveODE, wl_prove
from hhlpy.wolframengine_wrapper import wl_simplify, wl_polynomial_div, wl_is_polynomial, found_wolfram
from hhlpy.wolframengine_wrapper import session


class AssnTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            session.start()
        except Exception as e:
            session.start()

    @classmethod
    def tearDownClass(cls):
        session.terminate()

    def testParsePathInv(self):
        test_case = [
            "id_inv",
            "s == s0",
            "s == s0[x := v]",
        ]

        for s in test_case:
            path_inv = parse_path_inv(s)
            print(path_inv)

    def testParseAssn(self):
        test_case = [
            "init",

            # Example 1
            "wait_in(id_inv, ch1, {(d, v) =>"
            "  wait_outv(s == s0[x := v], ch2, v + 1, {d2 => init[x := v]})})",

            # Example 2
            # { P } *  ==  rec Q. skip || P; Q
            "rec Q. init || wait_in(id_inv, ch1, {(d, v) => Q[y := y + x][x := v]})",

            # Example 3
            "wait_in0(id_inv, ch2, {v =>"
            "  wait(s == s0[x := v], 1, {d1 =>"
            "    wait_outv(s == s0[x := v], ch1, v, {d2 => init[x := v]})})})",

            # Example 4
            "wait_outv(s == s0[x := 0], ch1, 1, {d1 =>"
            "  wait_in(s == s0[x := 2 * t], ch2, {(d2, v) =>"
            "    init[y := v][x := x + 2 * d2][x := 0]"
            "})}) ||"
            "wait_outv(s == s0[x := 0], ch1, 2, {d1 =>"
            "  wait_in(s == s0[x := t], ch2, {(d2, v) =>"
            "    init[y := v][x := x + d2][x := 0]"
            "})})",

            # Example 5
            "rec Q. init ||"
            "wait_outv(s == s0[x := 0], ch1, 1, {d1 =>"
            "  wait_in(s == s0[x := 2 * t], ch2, {(d2, v) =>"
            "    Q[y := v][x := x + 2 * d2][x := 0]"
            "})}) ||"
            "wait_outv(s == s0[x := 0], ch1, 2, {d1 =>"
            "  wait_in(s == s0[x := t], ch2, {(d2, v) =>"
            "    Q[y := v][x := x + d2][x := 0]"
            "})})"
        ]

        for s in test_case:
            assn = parse_assn(s)
            print(assn)

    def testSeqHcspToAssn(self):
        test_case = [
            # # Example 1
            # "ch1?x; ch2!(x+1);",

            # # Example 2
            # "{ ch1?x; y := y + x; }*",

            # # Example 3
            # "ch1!x; { ch1?x; y := y + x; }* ch1?x;",

            # # Example 4
            # "{ x := 0; { ch1?x; y := y + x; }* }*",

            # # Example 5
            # "wait (x); { ch1?x; y := y + x; }*",

            # # Example 6
            # "{x_dot=1 , y_dot=2 & x < 6 && y < 8} |> [] (ch1!x --> ch!y;) wait (5);",

            # # Example 7
            # "{x_dot=1 , y_dot=x & true} |> [] (ch1!x --> skip;, ch1?x --> skip;)"

            # Example 8
            "{x_dot=1 , y_dot=2 & x < 6 && y < 8}",

            # Example 9
            "{x_dot=1 , y_dot=x & y<8} wait (5);"
        ]

        for s in test_case:
            hp = parse_hp_with_meta(s)
            print(hp)
            print(seq_hcsp_to_assn(hp))



if __name__ == "__main__":
    unittest.main()
