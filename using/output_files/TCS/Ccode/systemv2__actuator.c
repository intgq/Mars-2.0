
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
	static double cmd = 0.0;

void* actuator (void* arg) {
    threadNumber = (int)(*((int*)arg));
        while (1) {
        channel.channelNo = inputs_lb_actuator_rb__lb_cmd_rb_;
        channel.type = 0;
        channel.pos = &cmd;
        input(threadNumber, channel);
        channel.channelNo = outputs_lb_PHY_vehicle_imp_rb__lb_veh_a_rb_;
        channel.type = 1;
        channel.pos = &cmd;
        output(threadNumber, channel);
        delay(threadNumber, 0.002);
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}