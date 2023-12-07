
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[4];
static double h = step_size;
static double* midDouble = NULL;
	static double PHY_vehicle_imp_veh_a = 0.0;
	static double PHY_vehicle_imp_veh_s = 0.0;
	static double s = 0.0;
	static double PHY_vehicle_imp_veh_v_l = 0.0;
	static double a = 0.0;
	static double PHY_vehicle_imp_veh_v_w = 0.0;
	static double v = 0.0;

void* vehicle_imp (void* arg) {
    threadNumber = (int)(*((int*)arg));
        v = 0;
    s = 0;
    PHY_vehicle_imp_veh_a = 0;
    PHY_vehicle_imp_veh_s = 0;
    PHY_vehicle_imp_veh_v_l = 0;
    PHY_vehicle_imp_veh_v_w = 0;
    a = 0.0;
    while ((equalInLimit(PHY_vehicle_imp_veh_s, 0)) || (((equalInLimit(PHY_vehicle_imp_veh_v_l, 0)) || (equalInLimit(PHY_vehicle_imp_veh_v_w, 0))))) {
        channels[0].channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_s_rb_;
        channels[0].type = 1;
        channels[0].pos = &s;
        channels[1].channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_l_rb_;
        channels[1].type = 1;
        channels[1].pos = &v;
        channels[2].channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_w_rb_;
        channels[2].type = 1;
        channels[2].pos = &v;
        midInt = externalChoice(threadNumber, 3, channels);
        if (midInt == 0) {
            PHY_vehicle_imp_veh_s = 1;
        } else if (midInt == 1) {
            PHY_vehicle_imp_veh_v_l = 1;
        } else if (midInt == 2) {
            PHY_vehicle_imp_veh_v_w = 1;
        }
    }
    channel.channelNo = outputs_lb_PHY_vehicle_imp_rb__lb_veh_a_rb_;
    channel.type = 0;
    channel.pos = &a;
    input(threadNumber, channel);
    while (1) {
        channels[0].channelNo = outputs_lb_PHY_vehicle_imp_rb__lb_veh_a_rb_;
        channels[0].type = 0;
        channels[0].pos = &a;
        channels[1].channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_s_rb_;
        channels[1].type = 1;
        channels[1].pos = &s;
        channels[2].channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_l_rb_;
        channels[2].type = 1;
        channels[2].pos = &v;
        channels[3].channelNo = inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_w_rb_;
        channels[3].type = 1;
        channels[3].pos = &v;
        interruptInit(threadNumber, 4, channels);
        h = step_size;
        is_comm = 0;
        while (1) {
            if ((int) (localTime[threadNumber] * 1000 + 0.5) % (int) (output_step_size * 1000 + 0.5) == 0) {
                printf("%.3f: v = %.3f, s = %.3f, a = %.3f\n", localTime[threadNumber], v, s, a);
            }
            double midTime = localTime[threadNumber];
            midInt = interruptPoll(threadNumber, h, 4, channels);
            if (midInt >= 0) {
                h = localTime[threadNumber] - midTime;
            }
            double v_ori = v;
            double s_ori = s;
            double v_dot1 = a;
            double s_dot1 = v;
            v = v_ori + v_dot1 * h / 2;
            s = s_ori + s_dot1 * h / 2;
            double v_dot2 = a;
            double s_dot2 = v;
            v = v_ori + v_dot2 * h / 2;
            s = s_ori + s_dot2 * h / 2;
            double v_dot3 = a;
            double s_dot3 = v;
            v = v_ori + v_dot3 * h;
            s = s_ori + s_dot3 * h;
            double v_dot4 = a;
            double s_dot4 = v;
            v = v_ori + (v_dot1 + 2 * v_dot2 + 2 * v_dot3 + v_dot4) * h / 6;
            s = s_ori + (s_dot1 + 2 * s_dot2 + 2 * s_dot3 + s_dot4) * h / 6;
            if (midInt >= 0) {
                break;
            }
            if (!(true)) {
                interruptClear(threadNumber, 4, channels);
                midInt = -1;
                break;
            }
            
        }
        if (midInt == 0) {
            ;
        }else if (midInt == 1) {
            ;
        }else if (midInt == 2) {
            ;
        }else if (midInt == 3) {
            ;
        }
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}