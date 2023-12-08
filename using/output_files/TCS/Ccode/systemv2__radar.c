
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[1];
static double h = step_size;
static double* midDouble = NULL;
	static double slope = 0.0;
	static double t = 0.0;
	static double data = 0.0;

void* radar (void* arg) {
    threadNumber = (int)(*((int*)arg));
        t = 0;
    data = 0.0;
    while (1) {
        if (t < 10) { data = 0.0; } else if (t < 20) { slope = (55 - 35) / (20 - 10);
        data = 35 + slope * (t - 10); } else { data = 0.0; }
        channel.channelNo = outputs_lb_radar_rb__lb_obs_pos_radar_rb_;
        channel.type = 1;
        channel.pos = &data;
        output(threadNumber, channel);
        delay(threadNumber, 0.01);
        t = t + 0.01;
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}