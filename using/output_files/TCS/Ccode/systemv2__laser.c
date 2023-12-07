
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[3];
static double h = step_size;
static double* midDouble = NULL;
	static double data = 0.0;
	static double error = 0.0;
	static double t = 0.0;
	static double x = 0.0;

void* laser (void* arg) {
    threadNumber = (int)(*((int*)arg));
        while (1) {
        channel.channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_l_rb_;
        channel.type = 0;
        channel.pos = &data;
        input(threadNumber, channel);
        t = 0;
        midDouble = (double*) malloc(sizeof(double));
        *midDouble = 1.0;
        channels[0].channelNo = outputs_lb_laser_rb__lb_laser_valid_rb_;
        channels[0].type = 1;
        channels[0].pos = midDouble;
        interruptInit(threadNumber, 1, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
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
            if (!(t < 0.01)) {
                interruptClear(threadNumber, 1, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            error = 0;
            x = data + error;
            midDouble = (double*) malloc(sizeof(double));
            *midDouble = data + error;
            channel.channelNo = outputs_lb_laser_rb__lb_laser_v_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel);
        }
        h = step_size;
        while (1) {
            double t_ori = t;
            double t_dot1 = 1;
            t = t_ori + t_dot1 * h / 2;
            double t_dot2 = 1;
            t = t_ori + t_dot2 * h / 2;
            double t_dot3 = 1;
            t = t_ori + t_dot3 * h;
            double t_dot4 = 1;
            t = t_ori + (t_dot1 + 2 * t_dot2 + 2 * t_dot3 + t_dot4) * h / 6;
            delay(threadNumber, h);
            if (!(t < 0.01)) {
            	break;
            }
        }
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}