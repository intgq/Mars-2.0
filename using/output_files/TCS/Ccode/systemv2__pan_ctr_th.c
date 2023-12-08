
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[7];
static double h = step_size;
static double* midDouble = NULL;
static void en_pan_ctr_th_imp_update();static void du_pan_ctr_th_imp_update();static void ex_pan_ctr_th_imp_update();static void entry_pan_ctr_th_imp_update();static void during_pan_ctr_th_imp_update();static void exit_pan_ctr_th_imp_update();static void en_pan_ctr_th_imp();static void du_pan_ctr_th_imp();static void ex_pan_ctr_th_imp();static void entry_pan_ctr_th_imp();static void during_pan_ctr_th_imp();static void exit_pan_ctr_th_imp();static void init_pan_ctr_th_imp();static void exec_pan_ctr_th_imp();	static String event;
	static double des_v = 0.0;
	static double pan_ctr_th_imp_done = 0.0;
	static double t = 0.0;
	static double entered = 0.0;
	static double pan_ctr_th_imp_ret = 0.0;
	static double c = 0.0;
	static double pan_ctr_th_imp_update_done = 0.0;
	static String pan_ctr_th_imp_st;
	static double prior = 0.0;
	static String state;
	static List pan_ctr_th_impEL;

static void en_pan_ctr_th_imp_update () {
        ;
    return;
}


static void du_pan_ctr_th_imp_update () {
        ;
    return;
}


static void ex_pan_ctr_th_imp_update () {
        ;
    return;
}


static void entry_pan_ctr_th_imp_update () {
        pan_ctr_th_imp_st = *strInit("pan_ctr_th_imp_update");
    en_pan_ctr_th_imp_update();
    return;
}


static void during_pan_ctr_th_imp_update () {
        pan_ctr_th_imp_update_done = 0;
    if ((!equalInLimit(pan_ctr_th_impEL.length, 0)) && (strEqual(*(String*)listTop(&pan_ctr_th_impEL), *strInit("inc")))) { ;
    if (strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) { if (strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) { exit_pan_ctr_th_imp_update(); }
    des_v = des_v + 1;
    if ((strEqual(pan_ctr_th_imp_st, *strInit(""))) && (1)) { entry_pan_ctr_th_imp_update(); }
    pan_ctr_th_imp_ret = 1;
    pan_ctr_th_imp_update_done = pan_ctr_th_imp_ret; } }
    if ((strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) && (((equalInLimit(pan_ctr_th_imp_update_done, 0)) && (((!equalInLimit(pan_ctr_th_impEL.length, 0)) && (strEqual(*(String*)listTop(&pan_ctr_th_impEL), *strInit("dec")))))))) { ;
    if (strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) { if (strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) { exit_pan_ctr_th_imp_update(); }
    des_v = des_v - 1;
    if ((strEqual(pan_ctr_th_imp_st, *strInit(""))) && (1)) { entry_pan_ctr_th_imp_update(); }
    pan_ctr_th_imp_ret = 1;
    pan_ctr_th_imp_update_done = pan_ctr_th_imp_ret; } }
    if ((strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) && (equalInLimit(pan_ctr_th_imp_update_done, 0))) { du_pan_ctr_th_imp_update(); }
    pan_ctr_th_imp_ret = pan_ctr_th_imp_update_done;
    return;
}


static void exit_pan_ctr_th_imp_update () {
        ex_pan_ctr_th_imp_update();
    if (strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) { pan_ctr_th_imp_st = *strInit(""); }
    return;
}


static void en_pan_ctr_th_imp () {
        ;
    return;
}


static void du_pan_ctr_th_imp () {
        ;
    return;
}


static void ex_pan_ctr_th_imp () {
        ;
    return;
}


static void entry_pan_ctr_th_imp () {
        en_pan_ctr_th_imp();
    return;
}


static void during_pan_ctr_th_imp () {
        pan_ctr_th_imp_done = 0;
    if (equalInLimit(pan_ctr_th_imp_done, 0)) { du_pan_ctr_th_imp(); }
    pan_ctr_th_imp_ret = pan_ctr_th_imp_done;
    return;
}


static void exit_pan_ctr_th_imp () {
        ex_pan_ctr_th_imp();
    if (1) { ; }
    return;
}


static void init_pan_ctr_th_imp () {
        midList = listInit();
    pan_ctr_th_impEL = *midList;
    des_v = 0;
    pan_ctr_th_imp_st = *strInit("");
    entry_pan_ctr_th_imp();
    des_v = 0;
    entry_pan_ctr_th_imp_update();
    return;
}


static void exec_pan_ctr_th_imp () {
        during_pan_ctr_th_imp();
    if (equalInLimit(pan_ctr_th_imp_ret, 0)) { if (strEqual(pan_ctr_th_imp_st, *strInit("pan_ctr_th_imp_update"))) { during_pan_ctr_th_imp_update(); } }
    return;
}


void* pan_ctr_th (void* arg) {
    threadNumber = (int)(*((int*)arg));
        event = *strInit(" ");
    ;
    init_pan_ctr_th_imp();
    state = *strInit("wait");
    prior = 0;
    while (1) {
        if (strEqual(state, *strInit("wait"))) { channel.channelNo = dis_lb_pan_ctr_th_rb__lb_event_rb_;
        channel.type = 0;
        channel.pos = &event;
        input(threadNumber, channel);
        t = 0;
        entered = 0;
        state = *strInit("ready"); } else if (strEqual(state, *strInit("ready"))) { channel.channelNo = reqProcessor_l_0_r__lb_pan_ctr_th_rb_;
        channel.type = 1;
        channel.pos = &prior;
        output(threadNumber, channel);
        channels[0].channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: des_v = %.3f\n", localTime[threadNumber], des_v);
            }
            double midTime = localTime[threadNumber];
            midInt = interruptPoll(threadNumber, h, 1, channels);
            if (midInt >= 0) {
                h = localTime[threadNumber] - midTime;
            }
            double t_ori = t;
            double t_dot1 = 1;
            t = t_ori + t_dot1 * h / 2;
            double t_dot2 = 1;
            t = t_ori + t_dot2 * h / 2;
            double t_dot3 = 1;
            t = t_ori + t_dot3 * h;
            double t_dot4 = 1;
            t = t_ori + (t_dot1 + 2 * t_dot2 + 2 * t_dot3 + t_dot4) * h / 6;
            if (midInt >= 0) {
                break;
            }
            if (!(t < 0.1)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("running");
        }
        if ((t >= 0.1) && (strEqual(state, *strInit("ready")))) { channels[0].channelNo = exit_l_0_r__lb_pan_ctr_th_rb_;
        channels[0].type = 1;
        channels[0].pos = midDouble;
        channels[1].channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
        channels[1].type = 0;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("running");
        } } } else { if (equalInLimit(entered, 0)) { c = 0;
        pan_ctr_th_impEL = *listPush(&pan_ctr_th_impEL, (void*)(&event), 2);
        exec_pan_ctr_th_imp();
        pan_ctr_th_impEL = *listPop(&pan_ctr_th_impEL);
        entered = 1; }
        if (equalInLimit(entered, 1)) { channels[0].channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: des_v = %.3f\n", localTime[threadNumber], des_v);
            }
            double midTime = localTime[threadNumber];
            midInt = interruptPoll(threadNumber, h, 1, channels);
            if (midInt >= 0) {
                h = localTime[threadNumber] - midTime;
            }
            double t_ori = t;
            double c_ori = c;
            double t_dot1 = 1;
            double c_dot1 = 1;
            t = t_ori + t_dot1 * h / 2;
            c = c_ori + c_dot1 * h / 2;
            double t_dot2 = 1;
            double c_dot2 = 1;
            t = t_ori + t_dot2 * h / 2;
            c = c_ori + c_dot2 * h / 2;
            double t_dot3 = 1;
            double c_dot3 = 1;
            t = t_ori + t_dot3 * h;
            c = c_ori + c_dot3 * h;
            double t_dot4 = 1;
            double c_dot4 = 1;
            t = t_ori + (t_dot1 + 2 * t_dot2 + 2 * t_dot3 + t_dot4) * h / 6;
            c = c_ori + (c_dot1 + 2 * c_dot2 + 2 * c_dot3 + c_dot4) * h / 6;
            if (midInt >= 0) {
                break;
            }
            if (!(c < 0.005 && t < 0.1)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("ready");
        }
        if (strEqual(state, *strInit("running"))) { if (c >= 0.005) { channel.channelNo = outputs_lb_pan_ctr_th_rb__lb_des_v_rb_;
        channel.type = 1;
        channel.pos = &des_v;
        output(threadNumber, channel);
        channels[0].channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_pan_ctr_th_rb_;
        channels[1].type = 1;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("wait");
        } } else { channels[0].channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_pan_ctr_th_rb_;
        channels[1].type = 1;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("wait");
        } } } } }
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}