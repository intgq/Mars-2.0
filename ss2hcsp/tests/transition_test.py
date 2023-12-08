import unittest

from ss2hcsp.matlab import parser

import html


class MatlabTest(unittest.TestCase):
    def testParser(self):
        s = """
        [chance(a.b) == 10 && i > 1]{y = chance(E,A.B); i = i+1;}/{i = 1; y = 8;}
        """
        func = parser.parse_transition(html.unescape(s))
        
        self.assertEqual(str(func.cond), "chance(a.b) == 10 && i > 1")
        self.assertTrue(str(func.cond_act), "y = chance(E,A.B); i = i+1;")


if __name__ == "__main__":
    unittest.main()