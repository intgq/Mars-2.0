/*
 * File: test.h
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

#ifndef RTW_HEADER_test_h_
#define RTW_HEADER_test_h_
#ifndef test_COMMON_INCLUDES_
#define test_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* test_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_test_T RT_MODEL_test_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T x;                            /* '<Root>/IN' */
} ExtU_test_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T OUT;                          /* '<Root>/OUT' */
} ExtY_test_T;

/* Real-time Model Data Structure */
struct tag_RTM_test_T {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU_test_T test_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_test_T test_Y;

/* Model entry point functions */
extern void test_initialize(void);
extern void test_step(void);
extern void test_terminate(void);

/* Real-time Model object */
extern RT_MODEL_test_T *const test_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'test'
 * '<S1>'   : 'test/A'
 * '<S2>'   : 'test/B'
 */
#endif                                 /* RTW_HEADER_test_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
