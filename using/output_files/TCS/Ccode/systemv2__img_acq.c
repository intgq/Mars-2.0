
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[8];
static double h = step_size;
static double* midDouble = NULL;
	static double processing_state = 0.0;
	static double entered = 0.0;
	static double new_image = 0.0;
	static double c = 0.0;
	static double prior = 0.0;
	static String state;
	static double image = 0.0;
	static double t = 0.0;

void* img_acq (void* arg) {
    threadNumber = (int)(*((int*)arg));
        image = 0.0;
    ;
    processing_state = 0;
    state = *strInit("wait");
    prior = 1;
    while (1) {
        if (strEqual(state, *strInit("wait"))) { channel.channelNo = dis_lb_img_acq_rb_;
        channel.type = 0;
        channel.pos = midDouble;
        input(threadNumber, channel);
        channel.channelNo = inputs_lb_img_acq_rb__lb_img_rb_;
        channel.type = 0;
        channel.pos = &image;
        input(threadNumber, channel);
        t = 0;
        entered = 0;
        state = *strInit("ready"); } else if (strEqual(state, *strInit("ready"))) { channel.channelNo = reqProcessor_l_0_r__lb_img_acq_rb_;
        channel.type = 1;
        channel.pos = &prior;
        output(threadNumber, channel);
        channels[0].channelNo = run_l_0_r__lb_img_acq_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: image = %.3f, new_image = %.3f\n", localTime[threadNumber], image, new_image);
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
            if (!(t < 0.045)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("running");
        }
        if ((t >= 0.045) && (strEqual(state, *strInit("ready")))) { channels[0].channelNo = exit_l_0_r__lb_img_acq_rb_;
        channels[0].type = 1;
        channels[0].pos = midDouble;
        channels[1].channelNo = run_l_0_r__lb_img_acq_rb_;
        channels[1].type = 0;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("running");
        } } } else { if (equalInLimit(entered, 0)) { c = 0;
        new_image = processing_state;
        processing_state = image;
        entered = 1; }
        if (equalInLimit(entered, 1)) { channels[0].channelNo = preempt_l_0_r__lb_img_acq_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: image = %.3f, new_image = %.3f\n", localTime[threadNumber], image, new_image);
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
            if (!(c < 0.01 && t < 0.045)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            state = *strInit("ready");
        }
        if (strEqual(state, *strInit("running"))) { if (c >= 0.01) { channel.channelNo = outputs_lb_img_acq_rb__lb_proc_img_rb_;
        channel.type = 1;
        channel.pos = &new_image;
        output(threadNumber, channel);
        channels[0].channelNo = preempt_l_0_r__lb_img_acq_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_img_acq_rb_;
        channels[1].type = 1;
        channels[1].pos = midDouble;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            state = *strInit("wait");
        } else if (midInt == 1) {
            state = *strInit("wait");
        } } else { channels[0].channelNo = preempt_l_0_r__lb_img_acq_rb_;
        channels[0].type = 0;
        channels[0].pos = midDouble;
        channels[1].channelNo = free_l_0_r__lb_img_acq_rb_;
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