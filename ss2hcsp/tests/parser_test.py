"""Unit test for parsing files."""

import unittest

from ss2hcsp.hcsp.hcsp import HCSPInfo, Procedure
from ss2hcsp.hcsp import module
from ss2hcsp.hcsp.parser import hp_parser, module_parser, parse_file, parse_module_file, \
                                expr_parser


class ParserTest(unittest.TestCase):
    def testParseFile(self):
        infos = parse_file("""
            P0 ::= x := 0; { {x_dot = 1 & true} |> [](p2c!x --> skip;) c2p?x; }*
            P1 ::= { wait(2); p2c?x; c2p!x-1; }*
        """)

        self.assertEqual(infos, [
            HCSPInfo("P0", hp_parser.parse("x := 0; { {x_dot = 1 & true} |> [](p2c!x --> skip;) c2p?x; }*")),
            HCSPInfo("P1", hp_parser.parse("{ wait(2); p2c?x; c2p!x-1; }*"))
        ])

    def testParseModuleFile(self):
        infos = parse_module_file("""
            %type: module

            module P0():
            output x;
            begin
                x := 0;
                { {x_dot = 1 & true} |> [](p2c!x --> skip; ) c2p?x; }*
            end
            endmodule

            module P1():
            begin
                { wait(2); p2c?x; c2p!x-1; }*
            end
            endmodule

            system
            P0() ||
            P1()
            endsystem
        """)

        self.assertEqual(infos, [
            HCSPInfo("P0", hp_parser.parse("x := 0; { {x_dot = 1 & true} |> [](p2c!x --> skip;) c2p?x; }*")),
            HCSPInfo("P1", hp_parser.parse("{ wait(2); p2c?x; c2p!x-1; }*"))
        ])

    def testParseModuleFile2(self):
        infos = parse_module_file("""
            %type: module

            module P0(p2c,c2p):
            output x;
            begin
              x := 0;
              {{x_dot = 1 & true} |> [](p2c!x --> skip;) c2p?x; }*
            end
            endmodule

            module P1(p2c,c2p):
            begin
              { wait(2); p2c?x; c2p!x-1; }*
            end
            endmodule

            system
              P0a = P0(ch1,ch2) ||
              P1a = P1(ch1,ch2) ||
              P0b = P0(ch3,ch4) ||
              P1b = P1(ch3,ch4)
            endsystem
        """)

        self.assertEqual(infos, [
            HCSPInfo("P0a", hp_parser.parse("x := 0; { {x_dot = 1 & true} |> [](ch1!x --> skip;) ch2?x; }*")),
            HCSPInfo("P1a", hp_parser.parse("{ wait(2); ch1?x; ch2!x-1; }*")),
            HCSPInfo("P0b", hp_parser.parse("x := 0; { {x_dot = 1 & true} |> [](ch3!x --> skip;) ch4?x; }*")),
            HCSPInfo("P1b", hp_parser.parse("{ wait(2); ch3?x; ch4!x-1; }*")),
        ])

    def testParseModule3(self):
        infos = parse_module_file("""
            %type: module

            module P0(i):
            output x;
            begin
              x := 0;
              { {x_dot = 1 & true} |> [](p2c[i]!x --> skip; ) c2p[i]?x; }*
            end
            endmodule

            module P1(i):
            begin
              { wait(2); p2c[i]?x; c2p[i]!x-1; }*
            end
            endmodule

            system
              P0a=P0($0) || P1a=P1($0) ||
              P0b=P0($1) || P1b=P1($1)
            endsystem
        """)

        self.assertEqual(infos, [
            HCSPInfo("P0a", hp_parser.parse("x := 0; { {x_dot = 1 & true} |> [](p2c[0]!x --> skip;) c2p[0]?x; }*")),
            HCSPInfo("P1a", hp_parser.parse("{ wait(2); p2c[0]?x; c2p[0]!x-1; }*")),
            HCSPInfo("P0b", hp_parser.parse("x := 0; { {x_dot = 1 & true} |> [](p2c[1]!x --> skip;) c2p[1]?x; }*")),
            HCSPInfo("P1b", hp_parser.parse("{ wait(2); p2c[1]?x; c2p[1]!x-1; }*")),
        ])

    def testParseProcedure(self):
        mod = module_parser.parse("""
            module P0():
            procedure incr begin
              x := x+1;
            end
            begin
              x := 0; @incr; @incr; ch!x;
            end
            endmodule
        """)

        self.assertEqual(mod, module.HCSPModule(
            "P0", "x := 0; @incr; @incr; ch!x;",
            procedures=[Procedure("incr", "x := x+1;")],
        ))

    def testQuantifierExpr(self):
        test_data = [
            "forall x. x != 0 -> 1 / x != 0",
            "forall {x, y}. x^2 + y^2 >= 0",
            "exists x. x > 0 -> x * y^2 == 1",
            "exists {x, y}. x^2 + y^2 == 0",
            "forall x. exists y. x > 0 -> x * y^2 == 1",
            "exists y. forall x. x > 0 -> x * y^2 == 1",
            "forall x. forall y. x^2 + y^2 >= 0",
        ]

        for s in test_data:
            e = expr_parser.parse(s)
            self.assertEqual(str(e), s)

    def testNotExpr(self):
        test_data = [
            "!x >= 0",
        ]

        for s in test_data:
            e = expr_parser.parse(s)
            self.assertEqual(str(e), s)

    def testODE(self):
        # TODO: print solution tactic
        ode = "{x_dot = 1 & true} solution;"

        hp = hp_parser.parse(ode)
        self.assertEqual(str(hp), "{x_dot = 1 & true}")

    def testArrayIdxExpr(self):
        test_data = [
            "[1, 2, 3][0]",
        ]

        for s in test_data:
            e = expr_parser.parse(s)
            self.assertEqual(str(e), s)


if __name__ == "__main__":
    unittest.main()
