
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

void* camera (void* arg) {
    threadNumber = (int)(*((int*)arg));
        while (1) {
        midDouble = (double*) malloc(sizeof(double));
        *midDouble = -1;
        channel.channelNo = outputs_lb_camera_rb__lb_img_rb_;
        channel.type = 1;
        channel.pos = midDouble;
        output(threadNumber, channel);
        delay(threadNumber, 0.2);
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}