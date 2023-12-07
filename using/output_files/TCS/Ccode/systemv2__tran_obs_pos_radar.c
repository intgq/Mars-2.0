
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[2];
static double h = step_size;
static double* midDouble = NULL;
	static double x = 0.0;
	static double data = 0.0;

void* tran_obs_pos_radar (void* arg) {
    threadNumber = (int)(*((int*)arg));
        x = 0;
    data = 0.0;
    while (1) {
        channels[0].channelNo = outputs_lb_radar_rb__lb_obs_pos_radar_rb_;
        channels[0].type = 0;
        channels[0].pos = &data;
        channels[1].channelNo = inputs_lb_comp_obs_pos_rb__lb_obs_pos_radar_rb_;
        channels[1].type = 1;
        channels[1].pos = &data;
        interruptInit(threadNumber, 2, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            double midTime = localTime[threadNumber];
            midInt = interruptPoll(threadNumber, h, 2, channels);
            if (midInt >= 0) {
                h = localTime[threadNumber] - midTime;
            }
            double x_ori = x;
            double x_dot1 = 0;
            x = x_ori + x_dot1 * h / 2;
            double x_dot2 = 0;
            x = x_ori + x_dot2 * h / 2;
            double x_dot3 = 0;
            x = x_ori + x_dot3 * h;
            double x_dot4 = 0;
            x = x_ori + (x_dot1 + 2 * x_dot2 + 2 * x_dot3 + x_dot4) * h / 6;
            if (midInt >= 0) {
                break;
            }
            if (!(true)) {
                interruptClear(threadNumber, 2, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            ;
        }else if (midInt == 1) {
            ;
        }
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}