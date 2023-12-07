# hhlprover
## usage
   1. Install Isabelle2023 from:<br>
        https://isabelle.in.tum.de/  <br>
   2. Download the afp package from :<br>
        https://www.isa-afp.org/ <br>
        we need to make the whole AFP available to Isabelle (we use the version afp-2023-12-06) <br>
   3. Launch Isabelle2023 and then change logic session to "Ordinary_Differential_Equations" <br>
      Session change requires restart and then waiting for the session to be proved for the first time. <br>
      If the target session is not found, it means step 2 was not completed. <br> 
   4. Add the relative mars package path ".../mars/lunarlander_sl" into the ROOTS file in Isabelle2023 <br>
      If an error dialog box is displayed while launching Isabelle2023, it means this step was not completed.
   5. Open the following files in this package in Isabelle2023 <br>
        "mars\Examples\lander\Lander1.thy" <br>
	      "mars\Examples\lander\Lander2.thy" <br>
        "mars\Examples\CTCS3\C.thy" <br>
      If step 4 secceeds, Isabelle2023 will automatically import the dependent files in HHLprover. <br>


## simple intrudction of theories
### Analysis_more.thy
  * some results about derivatives   
  * some lemmas in real functions like MVT and IVT  
  * definitions of state, ode and ode solution
      
### BigStepSimple.thy
  * Big-step semantics 
  * definition of valid
  * Hoare rules for discrete process
      
### BigStepContinuous.thy
  * Hoare rules for ode and ode interrupt
      
### BigStepParallel.thy
  * combination of traces
  * Hoare rules for parallel hcsp
      
### ContinuousInv.thy and Complementlemma.thy
  * Diffinv rule, Dbx rule and Dbarrier rule and their variants

### Lander1.thy and Lander2.thy
  * Lander1: the example of lander with no parallel
  * Lander2: the example of lander with as described in article
  * "sorry" in the proof correspond to properties of "inv" which have been verified in Mathematica

### C.thy 
  * C: the example of TCS
  
   
