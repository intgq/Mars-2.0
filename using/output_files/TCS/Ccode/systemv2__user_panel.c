
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

void* user_panel (void* arg) {
    threadNumber = (int)(*((int*)arg));
        midString = strInit("inc");
    channel.channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
    channel.type = 1;
    channel.pos = midString;
    output(threadNumber, channel);
    delay(threadNumber, 0.5);
    midString = strInit("inc");
    channel.channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
    channel.type = 1;
    channel.pos = midString;
    output(threadNumber, channel);
    delay(threadNumber, 0.5);
    midString = strInit("inc");
    channel.channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
    channel.type = 1;
    channel.pos = midString;
    output(threadNumber, channel);
    delay(threadNumber, 29);
    midString = strInit("dec");
    channel.channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
    channel.type = 1;
    channel.pos = midString;
    output(threadNumber, channel);
    midString = strInit("dec");
    channel.channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
    channel.type = 1;
    channel.pos = midString;
    output(threadNumber, channel);
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}