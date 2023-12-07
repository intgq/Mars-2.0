
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
	static double wheel_v = 0.0;
	static double laser_v = 0.0;
	static double wheel_valid = 0.0;
	static String state;
	static double entered = 0.0;
	static double c = 0.0;
	static double x1 = 0.0;
	static double t = 0.0;
	static double x3 = 0.0;
	static double x0 = 0.0;
	static double veh_v = 0.0;
	static double x2 = 0.0;
	static double laser_valid = 0.0;
	static double prior = 0.0;

void* velo_voter (void* arg) {
    threadNumber = (int)(*((int*)arg));
        wheel_v = 0.0;
    wheel_valid = -1.0;
    laser_v = 0.0;
    laser_valid = -1.0;
    x3 = 2;
    state = *strInit("wait");
    prior = 1;
    while (1) {
        if (strEqual(state, *strInit("wait"))) { channel.channelNo = dis_lb_velo_voter_rb_;
        channel.type = 0;
        channel.pos = midDouble;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_velo_voter_rb__lb_wheel_v_rb_;
        channel.type = 0;
        channel.pos = &wheel_v;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_velo_voter_rb__lb_wheel_valid_rb_;
        channel.type = 0;
        channel.pos = &wheel_valid;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_velo_voter_rb__lb_laser_v_rb_;
        channel.type = 0;
        channel.pos = &laser_v;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_velo_voter_rb__lb_laser_valid_rb_;
        channel.type = 0;
        channel.pos = &laser_valid;
        input(threadNumber, channel);
        t = 0;
        entered = 0;
        state = *strInit("ready"); } else if (strEqual(state, *strInit("ready"))) { channel.channelNo = reqProcessor_l_0_r__lb_velo_voter_rb_;
        channel.type = 1;
        channel.pos = &prior;
        output(threadNumber, channel);
        channels[0].channelNo = run_l_0_r__lb_velo_voter_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: wheel_v = %.3f, laser_v = %.3f, veh_v = %.3f\n", localTime[threadNumber], wheel_v, laser_v, veh_v);
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
            if (!(t < 0.008)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("running");
        }
        if ((t >= 0.008) && (strEqual(state, *strInit("ready")))) { channels[0].channelNo = exit_l_0_r__lb_velo_voter_rb_;
        channels[0].type = 1;
        channels[0].pos = midDouble;
        channels[1].channelNo = run_l_0_r__lb_velo_voter_rb_;
        channels[1].type = 0;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("running");
        } } } else { if (equalInLimit(entered, 0)) { c = 0;
        x0 = (wheel_valid > 0 ? wheel_v : laser_v);
        x1 = (laser_valid > 0 ? laser_v : wheel_v);
        x2 = x0 + x1;
        veh_v = x2 / x3;
        entered = 1; }
        if (equalInLimit(entered, 1)) { channels[0].channelNo = preempt_l_0_r__lb_velo_voter_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: wheel_v = %.3f, laser_v = %.3f, veh_v = %.3f\n", localTime[threadNumber], wheel_v, laser_v, veh_v);
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
            if (!(c < 0.001 && t < 0.008)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("ready");
        }
        if (strEqual(state, *strInit("running"))) { if (c >= 0.001) { channel.channelNo = outputs_lb_velo_voter_rb__lb_veh_v_rb_;
        channel.type = 1;
        channel.pos = &veh_v;
        output(threadNumber, channel);
        channels[0].channelNo = preempt_l_0_r__lb_velo_voter_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_velo_voter_rb_;
        channels[1].type = 1;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("wait");
        } } else { channels[0].channelNo = preempt_l_0_r__lb_velo_voter_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_velo_voter_rb_;
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