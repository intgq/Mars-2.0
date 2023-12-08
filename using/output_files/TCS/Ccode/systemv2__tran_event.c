
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
	static String event;
	static List queue;

void* tran_event (void* arg) {
    threadNumber = (int)(*((int*)arg));
        event = *strInit("");
    midList = listInit();
    queue = *midList;
    while (1) {
        if (equalInLimit(queue.length, 0)) { channel.channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
        channel.type = 0;
        channel.pos = &event;
        input(threadNumber, channel);
        queue = *listPush(&queue, (void*)(&event), 2); } else { channels[0].channelNo = outputs_lb_user_panel_rb__lb_event_rb_;
        channels[0].type = 0;
        channels[0].pos = &event;
        midString = (String*) malloc(sizeof(String));
        *midString = *(String*)listBottom(&queue);
        channels[1].channelNo = dis_lb_pan_ctr_th_rb__lb_event_rb_;
        channels[1].type = 1;
        channels[1].pos = midString;
        midInt = externalChoice(threadNumber, 2, channels);
        if (midInt == 0) {
            queue = *listPush(&queue, (void*)(&event), 2);
        } else if (midInt == 1) {
            queue = *listPopBack(&queue);
        } }
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}