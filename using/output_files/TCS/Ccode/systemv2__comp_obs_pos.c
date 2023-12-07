
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[9];
static double h = step_size;
static double* midDouble = NULL;
	static String state;
	static double entered = 0.0;
	static double c = 0.0;
	static double obs_pos = 0.0;
	static double x1 = 0.0;
	static double t = 0.0;
	static double x3 = 0.0;
	static double obs_pos_radar = 0.0;
	static double x0 = 0.0;
	static double proc_img = 0.0;
	static double x2 = 0.0;
	static double prior = 0.0;

void* comp_obs_pos (void* arg) {
    threadNumber = (int)(*((int*)arg));
        obs_pos_radar = 0.0;
    proc_img = 0.0;
    ;
    x0 = 0;
    state = *strInit("wait");
    prior = 1;
    while (1) {
        if (strEqual(state, *strInit("wait"))) { channel.channelNo = dis_lb_comp_obs_pos_rb_;
        channel.type = 0;
        channel.pos = midDouble;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_comp_obs_pos_rb__lb_obs_pos_radar_rb_;
        channel.type = 0;
        channel.pos = &obs_pos_radar;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_comp_obs_pos_rb__lb_proc_img_rb_;
        channel.type = 0;
        channel.pos = &proc_img;
        input(threadNumber, channel);
        t = 0;
        entered = 0;
        state = *strInit("ready"); } else if (strEqual(state, *strInit("ready"))) { channel.channelNo = reqProcessor_l_0_r__lb_comp_obs_pos_rb_;
        channel.type = 1;
        channel.pos = &prior;
        output(threadNumber, channel);
        channels[0].channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: obs_pos_radar = %.3f, proc_img = %.3f, obs_pos = %.3f\n", localTime[threadNumber], obs_pos_radar, proc_img, obs_pos);
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
            if (!(t < 0.097)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("running");
        }
        if ((t >= 0.097) && (strEqual(state, *strInit("ready")))) { channels[0].channelNo = exit_l_0_r__lb_comp_obs_pos_rb_;
        channels[0].type = 1;
        channels[0].pos = midDouble;
        channels[1].channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
        channels[1].type = 0;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("running");
        } } } else { if (equalInLimit(entered, 0)) { c = 0;
        x1 = (proc_img > 0 ? proc_img : obs_pos_radar);
        x2 = (obs_pos_radar > 0 ? obs_pos_radar : proc_img);
        x3 = min(2, x1, x2);
        obs_pos = (x3 > 0 ? x3 : x0);
        entered = 1; }
        if (equalInLimit(entered, 1)) { channels[0].channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: obs_pos_radar = %.3f, proc_img = %.3f, obs_pos = %.3f\n", localTime[threadNumber], obs_pos_radar, proc_img, obs_pos);
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
            if (!(c < 0.02 && t < 0.097)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("ready");
        }
        if (strEqual(state, *strInit("running"))) { if (c >= 0.02) { channel.channelNo = outputs_lb_comp_obs_pos_rb__lb_obs_pos_rb_;
        channel.type = 1;
        channel.pos = &obs_pos;
        output(threadNumber, channel);
        channels[0].channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_comp_obs_pos_rb_;
        channels[1].type = 1;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("wait");
        } } else { channels[0].channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_comp_obs_pos_rb_;
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