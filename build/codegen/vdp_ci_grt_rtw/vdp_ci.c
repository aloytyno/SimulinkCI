/*
 * vdp_ci.c
 *
 * Code generation for model "vdp_ci".
 *
 * Model version              : 13.2
 * Simulink Coder version : 26.1 (R2026a) 20-Nov-2025
 * C source code generated on : Fri Sep 25 10:50:05 2026
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "vdp_ci.h"
#include "vdp_ci_private.h"
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_vdp_ci_T vdp_ci_B;

/* Continuous states */
X_vdp_ci_T vdp_ci_X;

/* Disabled State Vector */
XDis_vdp_ci_T vdp_ci_XDis;

/* External outputs (root outports fed by signals with default storage) */
ExtY_vdp_ci_T vdp_ci_Y;

/* Real-time model */
static RT_MODEL_vdp_ci_T vdp_ci_M_;
RT_MODEL_vdp_ci_T *const vdp_ci_M = &vdp_ci_M_;

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  vdp_ci_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  vdp_ci_output();
  vdp_ci_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  vdp_ci_output();
  vdp_ci_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  vdp_ci_output();
  vdp_ci_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void vdp_ci_output(void)
{
  if (rtmIsMajorTimeStep(vdp_ci_M)) {
    /* set solver stop time */
    if (!(vdp_ci_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&vdp_ci_M->solverInfo,
                            ((vdp_ci_M->Timing.clockTickH0 + 1) *
        vdp_ci_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&vdp_ci_M->solverInfo, ((vdp_ci_M->Timing.clockTick0
        + 1) * vdp_ci_M->Timing.stepSize0 + vdp_ci_M->Timing.clockTickH0 *
        vdp_ci_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(vdp_ci_M)) {
    vdp_ci_M->Timing.t[0] = rtsiGetT(&vdp_ci_M->solverInfo);
  }

  /* Integrator: '<Root>/x1' */
  vdp_ci_B.x1 = vdp_ci_X.x1_CSTATE;

  /* Outport: '<Root>/Out1' */
  vdp_ci_Y.Out1 = vdp_ci_B.x1;

  /* Integrator: '<Root>/x2' */
  vdp_ci_B.x2 = vdp_ci_X.x2_CSTATE;

  /* Outport: '<Root>/Out2' */
  vdp_ci_Y.Out2 = vdp_ci_B.x2;
  if (rtmIsMajorTimeStep(vdp_ci_M) &&
      vdp_ci_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* Sum: '<Root>/Sum' incorporates:
   *  Constant: '<Root>/Constant'
   *  Gain: '<S1>/Slider Gain'
   *  Math: '<Root>/Square'
   *  Product: '<Root>/Product'
   *  Sum: '<Root>/Sum1'
   */
  vdp_ci_B.Sum = (vdp_ci_P.Constant_Value - vdp_ci_B.x1 * vdp_ci_B.x1) *
    vdp_ci_B.x2 * vdp_ci_P.Mu_gain - vdp_ci_B.x1;
}

/* Model update function */
void vdp_ci_update(void)
{
  if (rtmIsMajorTimeStep(vdp_ci_M)) {
    rt_ertODEUpdateContinuousStates(&vdp_ci_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++vdp_ci_M->Timing.clockTick0)) {
    ++vdp_ci_M->Timing.clockTickH0;
  }

  vdp_ci_M->Timing.t[0] = rtsiGetSolverStopTime(&vdp_ci_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.01s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++vdp_ci_M->Timing.clockTick1)) {
      ++vdp_ci_M->Timing.clockTickH1;
    }

    vdp_ci_M->Timing.t[1] = vdp_ci_M->Timing.clockTick1 *
      vdp_ci_M->Timing.stepSize1 + vdp_ci_M->Timing.clockTickH1 *
      vdp_ci_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void vdp_ci_derivatives(void)
{
  XDot_vdp_ci_T *_rtXdot;
  _rtXdot = ((XDot_vdp_ci_T *) vdp_ci_M->derivs);

  /* Derivatives for Integrator: '<Root>/x1' */
  _rtXdot->x1_CSTATE = vdp_ci_B.x2;

  /* Derivatives for Integrator: '<Root>/x2' */
  _rtXdot->x2_CSTATE = vdp_ci_B.Sum;
}

/* Model initialize function */
void vdp_ci_initialize(void)
{
  /* InitializeConditions for Integrator: '<Root>/x1' */
  vdp_ci_X.x1_CSTATE = vdp_ci_P.x1_IC;

  /* InitializeConditions for Integrator: '<Root>/x2' */
  vdp_ci_X.x2_CSTATE = vdp_ci_P.x2_IC;
}

/* Model terminate function */
void vdp_ci_terminate(void)
{
  /* (no terminate code required) */
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  vdp_ci_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  vdp_ci_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  vdp_ci_initialize();
}

void MdlTerminate(void)
{
  vdp_ci_terminate();
}

/* Registration function */
RT_MODEL_vdp_ci_T *vdp_ci(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)vdp_ci_M, 0,
                sizeof(RT_MODEL_vdp_ci_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&vdp_ci_M->solverInfo, &vdp_ci_M->Timing.simTimeStep);
    rtsiSetTPtr(&vdp_ci_M->solverInfo, &rtmGetTPtr(vdp_ci_M));
    rtsiSetStepSizePtr(&vdp_ci_M->solverInfo, &vdp_ci_M->Timing.stepSize0);
    rtsiSetdXPtr(&vdp_ci_M->solverInfo, &vdp_ci_M->derivs);
    rtsiSetContStatesPtr(&vdp_ci_M->solverInfo, (real_T **)
                         &vdp_ci_M->contStates);
    rtsiSetNumContStatesPtr(&vdp_ci_M->solverInfo,
      &vdp_ci_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&vdp_ci_M->solverInfo,
      &vdp_ci_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&vdp_ci_M->solverInfo,
      &vdp_ci_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&vdp_ci_M->solverInfo,
      &vdp_ci_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&vdp_ci_M->solverInfo, (boolean_T**)
      &vdp_ci_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&vdp_ci_M->solverInfo, (&rtmGetErrorStatus(vdp_ci_M)));
    rtsiSetRTModelPtr(&vdp_ci_M->solverInfo, vdp_ci_M);
  }

  rtsiSetSimTimeStep(&vdp_ci_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&vdp_ci_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&vdp_ci_M->solverInfo, false);
  vdp_ci_M->intgData.y = vdp_ci_M->odeY;
  vdp_ci_M->intgData.f[0] = vdp_ci_M->odeF[0];
  vdp_ci_M->intgData.f[1] = vdp_ci_M->odeF[1];
  vdp_ci_M->intgData.f[2] = vdp_ci_M->odeF[2];
  vdp_ci_M->intgData.f[3] = vdp_ci_M->odeF[3];
  vdp_ci_M->contStates = ((real_T *) &vdp_ci_X);
  vdp_ci_M->contStateDisabled = ((boolean_T *) &vdp_ci_XDis);
  vdp_ci_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&vdp_ci_M->solverInfo, (void *)&vdp_ci_M->intgData);
  rtsiSetSolverName(&vdp_ci_M->solverInfo,"ode4");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = vdp_ci_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    vdp_ci_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    vdp_ci_M->Timing.sampleTimes = (&vdp_ci_M->Timing.sampleTimesArray[0]);
    vdp_ci_M->Timing.offsetTimes = (&vdp_ci_M->Timing.offsetTimesArray[0]);

    /* task periods */
    vdp_ci_M->Timing.sampleTimes[0] = (0.0);
    vdp_ci_M->Timing.sampleTimes[1] = (0.01);

    /* task offsets */
    vdp_ci_M->Timing.offsetTimes[0] = (0.0);
    vdp_ci_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(vdp_ci_M, &vdp_ci_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = vdp_ci_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    vdp_ci_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(vdp_ci_M, 20.0);
  vdp_ci_M->Timing.stepSize0 = 0.01;
  vdp_ci_M->Timing.stepSize1 = 0.01;
  vdp_ci_M->solverInfoPtr = (&vdp_ci_M->solverInfo);
  vdp_ci_M->Timing.stepSize = (0.01);
  rtsiSetFixedStepSize(&vdp_ci_M->solverInfo, 0.01);
  rtsiSetSolverMode(&vdp_ci_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  vdp_ci_M->blockIO = ((void *) &vdp_ci_B);

  {
    vdp_ci_B.x1 = 0.0;
    vdp_ci_B.x2 = 0.0;
    vdp_ci_B.Sum = 0.0;
  }

  /* parameters */
  vdp_ci_M->defaultParam = ((real_T *)&vdp_ci_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &vdp_ci_X;
    vdp_ci_M->contStates = (x);
    (void) memset((void *)&vdp_ci_X, 0,
                  sizeof(X_vdp_ci_T));
  }

  /* disabled states */
  {
    boolean_T *xdis = (boolean_T *) &vdp_ci_XDis;
    vdp_ci_M->contStateDisabled = (xdis);
    (void) memset((void *)&vdp_ci_XDis, 0,
                  sizeof(XDis_vdp_ci_T));
  }

  /* external outputs */
  vdp_ci_M->outputs = (&vdp_ci_Y);
  vdp_ci_Y.Out1 = 0.0;
  vdp_ci_Y.Out2 = 0.0;

  /* Initialize Sizes */
  vdp_ci_M->Sizes.numContStates = (2); /* Number of continuous states */
  vdp_ci_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  vdp_ci_M->Sizes.numY = (2);          /* Number of model outputs */
  vdp_ci_M->Sizes.numU = (0);          /* Number of model inputs */
  vdp_ci_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  vdp_ci_M->Sizes.numSampTimes = (2);  /* Number of sample times */
  vdp_ci_M->Sizes.numBlocks = (11);    /* Number of blocks */
  vdp_ci_M->Sizes.numBlockIO = (3);    /* Number of block outputs */
  vdp_ci_M->Sizes.numBlockPrms = (4);  /* Sum of parameter "widths" */
  return vdp_ci_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
