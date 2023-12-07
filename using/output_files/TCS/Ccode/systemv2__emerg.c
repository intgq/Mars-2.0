
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[11];
static double h = step_size;
static double* midDouble = NULL;
static void en_emerg_imp_state();static void du_emerg_imp_state();static void ex_emerg_imp_state();static void entry_emerg_imp_state();static void during_emerg_imp_state();static void exit_emerg_imp_state();static void en_emerg_imp();static void du_emerg_imp();static void ex_emerg_imp();static void entry_emerg_imp();static void during_emerg_imp();static void exit_emerg_imp();static void J14_1();static void J20_1();static void init_emerg_imp();static void exec_emerg_imp();	static String emerg_imp_st;
	static double des_a = 0.0;
	static String state;
	static double v_lim = 0.0;
	static double max_d = 0.0;
	static double distance = 0.0;
	static double entered = 0.0;
	static double c = 0.0;
	static double obs_pos = 0.0;
	static double max_v = 0.0;
	static double J20_done = 0.0;
	static double next_s = 0.0;
	static double t = 0.0;
	static double min_a = 0.0;
	static double cmd = 0.0;
	static double period = 0.0;
	static double veh_v = 0.0;
	static double J14_done = 0.0;
	static double emerg_imp_state_done = 0.0;
	static double emerg_imp_done = 0.0;
	static double emerg_imp_ret = 0.0;
	static List emerg_impEL;
	static double prior = 0.0;
	static double next_v = 0.0;
	static double veh_pos = 0.0;

static void en_emerg_imp_state () {
        ;
    return;
}


static void du_emerg_imp_state () {
        ;
    return;
}


static void ex_emerg_imp_state () {
        ;
    return;
}


static void entry_emerg_imp_state () {
        emerg_imp_st = *strInit("emerg_imp_state");
    en_emerg_imp_state();
    return;
}


static void during_emerg_imp_state () {
        emerg_imp_state_done = 0;
    if (1) { next_v = veh_v + des_a * period;
    next_s = veh_pos + veh_v * period + 0.5 * des_a * period * period;
    if (strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) { J14_1();
    emerg_imp_state_done = emerg_imp_ret; } }
    if ((strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) && (equalInLimit(emerg_imp_state_done, 0))) { du_emerg_imp_state(); }
    emerg_imp_ret = emerg_imp_state_done;
    return;
}


static void exit_emerg_imp_state () {
        ex_emerg_imp_state();
    if (strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) { emerg_imp_st = *strInit(""); }
    return;
}


static void en_emerg_imp () {
        ;
    return;
}


static void du_emerg_imp () {
        ;
    return;
}


static void ex_emerg_imp () {
        ;
    return;
}


static void entry_emerg_imp () {
        en_emerg_imp();
    return;
}


static void during_emerg_imp () {
        emerg_imp_done = 0;
    if (equalInLimit(emerg_imp_done, 0)) { du_emerg_imp(); }
    emerg_imp_ret = emerg_imp_done;
    return;
}


static void exit_emerg_imp () {
        ex_emerg_imp();
    if (1) { ; }
    return;
}


static void J14_1 () {
        J14_done = 0;
    distance = obs_pos - next_s;
    max_d = max_v * max_v / -(2 * min_a);
    if ((obs_pos <= 0) || (distance >= max_d)) { v_lim = max_v; } else { if (distance >= 0) { v_lim = sqrt(-(2 * min_a * distance)); } else { v_lim = 0; } }
    if (next_v <= v_lim) { if (strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) { exit_emerg_imp_state(); }
    cmd = des_a;
    if ((strEqual(emerg_imp_st, *strInit(""))) && (1)) { entry_emerg_imp_state(); }
    emerg_imp_ret = 1;
    J14_done = emerg_imp_ret; }
    if (equalInLimit(J14_done, 0)) { next_s = veh_pos + veh_v * period;
    J20_1();
    J14_done = emerg_imp_ret; }
    emerg_imp_ret = J14_done;
    return;
}


static void J20_1 () {
        J20_done = 0;
    distance = obs_pos - next_s;
    max_d = max_v * max_v / -(2 * min_a);
    if ((obs_pos <= 0) || (distance >= max_d)) { v_lim = max_v; } else { if (distance >= 0) { v_lim = sqrt(-(2 * min_a * distance)); } else { v_lim = 0; } }
    if (veh_v <= v_lim) { if (strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) { exit_emerg_imp_state(); }
    cmd = 0;
    if ((strEqual(emerg_imp_st, *strInit(""))) && (1)) { entry_emerg_imp_state(); }
    emerg_imp_ret = 1;
    J20_done = emerg_imp_ret; }
    if (equalInLimit(J20_done, 0)) { if (strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) { exit_emerg_imp_state(); }
    cmd = min_a;
    if ((strEqual(emerg_imp_st, *strInit(""))) && (1)) { entry_emerg_imp_state(); }
    emerg_imp_ret = 1;
    J20_done = emerg_imp_ret; }
    emerg_imp_ret = J20_done;
    return;
}


static void init_emerg_imp () {
        midList = listInit();
    emerg_impEL = *midList;
    max_v = 10;
    min_a = -3;
    period = 1;
    cmd = 0;
    next_v = 0;
    next_s = 0;
    emerg_imp_st = *strInit("");
    entry_emerg_imp();
    entry_emerg_imp_state();
    return;
}


static void exec_emerg_imp () {
        during_emerg_imp();
    if (equalInLimit(emerg_imp_ret, 0)) { if (strEqual(emerg_imp_st, *strInit("emerg_imp_state"))) { during_emerg_imp_state(); } }
    return;
}


void* emerg (void* arg) {
    threadNumber = (int)(*((int*)arg));
        des_a = 0.0;
    veh_v = 0.0;
    obs_pos = 0.0;
    veh_pos = 0.0;
    init_emerg_imp();
    state = *strInit("wait");
    prior = 2;
    while (1) {
        if (strEqual(state, *strInit("wait"))) { channel.channelNo = dis_lb_emerg_rb_;
        channel.type = 0;
        channel.pos = midDouble;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_emerg_rb__lb_des_a_rb_;
        channel.type = 0;
        channel.pos = &des_a;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_emerg_rb__lb_veh_v_rb_;
        channel.type = 0;
        channel.pos = &veh_v;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_emerg_rb__lb_obs_pos_rb_;
        channel.type = 0;
        channel.pos = &obs_pos;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_emerg_rb__lb_veh_pos_rb_;
        channel.type = 0;
        channel.pos = &veh_pos;
        input(threadNumber, channel);
        t = 0;
        entered = 0;
        state = *strInit("ready"); } else if (strEqual(state, *strInit("ready"))) { channel.channelNo = reqProcessor_l_0_r__lb_emerg_rb_;
        channel.type = 1;
        channel.pos = &prior;
        output(threadNumber, channel);
        channels[0].channelNo = run_l_0_r__lb_emerg_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: des_a = %.3f, veh_v = %.3f, obs_pos = %.3f, veh_pos = %.3f, cmd = %.3f\n", localTime[threadNumber], des_a, veh_v, obs_pos, veh_pos, cmd);
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
            if (!(t < 0.005)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("running");
        }
        if ((t >= 0.005) && (strEqual(state, *strInit("ready")))) { channels[0].channelNo = exit_l_0_r__lb_emerg_rb_;
        channels[0].type = 1;
        channels[0].pos = midDouble;
        channels[1].channelNo = run_l_0_r__lb_emerg_rb_;
        channels[1].type = 0;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("running");
        } } } else { if (equalInLimit(entered, 0)) { c = 0;
        exec_emerg_imp();
        entered = 1; }
        if (equalInLimit(entered, 1)) { channels[0].channelNo = preempt_l_0_r__lb_emerg_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: des_a = %.3f, veh_v = %.3f, obs_pos = %.3f, veh_pos = %.3f, cmd = %.3f\n", localTime[threadNumber], des_a, veh_v, obs_pos, veh_pos, cmd);
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
            if (!(c < 0.001 && t < 0.005)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("ready");
        }
        if (strEqual(state, *strInit("running"))) { if (c >= 0.001) { channel.channelNo = outputs_lb_emerg_rb__lb_cmd_rb_;
        channel.type = 1;
        channel.pos = &cmd;
        output(threadNumber, channel);
        channels[0].channelNo = preempt_l_0_r__lb_emerg_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_emerg_rb_;
        channels[1].type = 1;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("wait");
        } } else { channels[0].channelNo = preempt_l_0_r__lb_emerg_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_emerg_rb_;
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