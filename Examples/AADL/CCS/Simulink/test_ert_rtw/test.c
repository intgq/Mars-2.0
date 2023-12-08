/*
 * File: test.c
 *
 * Code generated for Simulink model 'test'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Wed Nov  2 23:12:51 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "test.h"

/* External inputs (root inport signals with default storage) */
ExtU_test_T test_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_test_T test_Y;

/* Real-time model */
static RT_MODEL_test_T test_M_;
RT_MODEL_test_T *const test_M = &test_M_;

/* Model step function */
void test_step(void)
{
  /* Outport: '<Root>/OUT' incorporates:
   *  Bias: '<S1>/Bias'
   *  Gain: '<S2>/Gain'
   *  Inport: '<Root>/IN'
   */
  test_Y.OUT = (test_U.x + 1.0) * 2.0;
}

/* Model initialize function */
void test_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void test_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
