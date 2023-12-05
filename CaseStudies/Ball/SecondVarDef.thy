theory SecondVarDef
	imports "assertionDef"
begin

(*Define channel names.*)
definition BC_1 :: cname where
"BC_1 == ''BC_1''"
definition BR_1 :: cname where
"BR_1 == ''BR_1''"
definition BO_1 :: cname where
"BO_1 == ''BO_1''"
definition VO1 :: cname where
"VO1 == ''VO1''"
definition VI1 :: cname where
"VI1 == ''VI1''"
definition vBO1 :: exp where
"vBO1 == SVar ''vBO1''"
definition Ch_Second_1 :: cname where
"Ch_Second_1 == ''Ch_Second_1''"

(*Define event variables assistent.*)
consts eL :: "exp list"
consts nL :: "exp list"
(*Define event variables.*)
definition E1 :: exp where
"E1 == SVar ''E1''"
definition done1 :: exp where
"done1 == RVar ''done1''"
definition E :: exp where
"E == SVar ''E''"
definition num :: exp where
"num == RVar ''num''"
definition EL :: exp where
"EL == List eL"
definition NL :: exp where
"NL == List nL"
(*Define local and sending variables.*)
definition sfTemp1 :: exp where
"sfTemp1 == RVar ''sfTemp1''"
definition x :: exp where
"x == RVar ''x''"
definition y :: exp where
"y == RVar ''y''"
definition actOn :: exp where
"actOn == RVar ''actOn''"
definition actOff :: exp where
"actOff == RVar ''actOff''"
(*Define input variables.*)
definition x1 :: exp where
"x1 == RVar ''x1''"
(*Define output variables.*)
definition y1 :: exp where
"y1 == RVar ''y1''"
(*Define local variables.*)

end
