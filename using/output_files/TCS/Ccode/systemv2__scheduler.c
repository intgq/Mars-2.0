
#include <math.h>
#include "hcsp2c.h"
#include "systemv2.h"

static int threadNumber = 0;
static int midInt = 0;
static int is_comm = 0;
static String* midString = NULL;
static List* midList = NULL;
static Channel channel;
static Channel channels[30];
static double h = step_size;
static double* midDouble = NULL;
	static double del_index = 0.0;
	static List GetMaxList;
	static double idx = 0.0;
	static double run_prior = 0.0;
	static double now_prior = 0.0;
	static List Pool_prior;
	static String run_now;
	static double max_index = 0.0;
	static String now_now;
	static List Pool_now;
	static double prior = 0.0;

void* scheduler (void* arg) {
    threadNumber = (int)(*((int*)arg));
        midList = listInit();
    Pool_prior = *midList;
    midList = listInit();
    Pool_now = *midList;
    midList = listInit();
    GetMaxList = *midList;
    prior = -1;
    run_now = *strInit("");
    run_prior = -1;
    while (1) {
        channels[0].channelNo = reqProcessor_l_0_r__lb_PI_ctr_rb_;
        channels[0].type = 0;
        channels[0].pos = &prior;
        channels[1].channelNo = reqProcessor_l_0_r__lb_comp_obs_pos_rb_;
        channels[1].type = 0;
        channels[1].pos = &prior;
        channels[2].channelNo = reqProcessor_l_0_r__lb_emerg_rb_;
        channels[2].type = 0;
        channels[2].pos = &prior;
        channels[3].channelNo = reqProcessor_l_0_r__lb_img_acq_rb_;
        channels[3].type = 0;
        channels[3].pos = &prior;
        channels[4].channelNo = reqProcessor_l_0_r__lb_pan_ctr_th_rb_;
        channels[4].type = 0;
        channels[4].pos = &prior;
        channels[5].channelNo = reqProcessor_l_0_r__lb_velo_voter_rb_;
        channels[5].type = 0;
        channels[5].pos = &prior;
        channels[6].channelNo = free_l_0_r__lb_PI_ctr_rb_;
        channels[6].type = 0;
        channels[6].pos = midDouble;
        channels[7].channelNo = free_l_0_r__lb_comp_obs_pos_rb_;
        channels[7].type = 0;
        channels[7].pos = midDouble;
        channels[8].channelNo = free_l_0_r__lb_emerg_rb_;
        channels[8].type = 0;
        channels[8].pos = midDouble;
        channels[9].channelNo = free_l_0_r__lb_img_acq_rb_;
        channels[9].type = 0;
        channels[9].pos = midDouble;
        channels[10].channelNo = free_l_0_r__lb_pan_ctr_th_rb_;
        channels[10].type = 0;
        channels[10].pos = midDouble;
        channels[11].channelNo = free_l_0_r__lb_velo_voter_rb_;
        channels[11].type = 0;
        channels[11].pos = midDouble;
        channels[12].channelNo = exit_l_0_r__lb_PI_ctr_rb_;
        channels[12].type = 0;
        channels[12].pos = midDouble;
        channels[13].channelNo = exit_l_0_r__lb_comp_obs_pos_rb_;
        channels[13].type = 0;
        channels[13].pos = midDouble;
        channels[14].channelNo = exit_l_0_r__lb_emerg_rb_;
        channels[14].type = 0;
        channels[14].pos = midDouble;
        channels[15].channelNo = exit_l_0_r__lb_img_acq_rb_;
        channels[15].type = 0;
        channels[15].pos = midDouble;
        channels[16].channelNo = exit_l_0_r__lb_pan_ctr_th_rb_;
        channels[16].type = 0;
        channels[16].pos = midDouble;
        channels[17].channelNo = exit_l_0_r__lb_velo_voter_rb_;
        channels[17].type = 0;
        channels[17].pos = midDouble;
        midInt = externalChoice(threadNumber, 18, channels);
        if (midInt == 0) {
            if (run_prior >= prior) { Pool_prior = *listPush(&Pool_prior, (void*)(&prior), 1);
            Pool_now = *listPush(&Pool_now, (void*)strInit("PI_ctr"), 2); } else { if (!strEqual(run_now, *strInit(""))) { if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = preempt_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = preempt_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = preempt_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = preempt_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
            run_now = *strInit("PI_ctr");
            run_prior = prior;
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
        } else if (midInt == 1) {
            if (run_prior >= prior) { Pool_prior = *listPush(&Pool_prior, (void*)(&prior), 1);
            Pool_now = *listPush(&Pool_now, (void*)strInit("comp_obs_pos"), 2); } else { if (!strEqual(run_now, *strInit(""))) { if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = preempt_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = preempt_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = preempt_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = preempt_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
            run_now = *strInit("comp_obs_pos");
            run_prior = prior;
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
        } else if (midInt == 2) {
            if (run_prior >= prior) { Pool_prior = *listPush(&Pool_prior, (void*)(&prior), 1);
            Pool_now = *listPush(&Pool_now, (void*)strInit("emerg"), 2); } else { if (!strEqual(run_now, *strInit(""))) { if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = preempt_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = preempt_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = preempt_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = preempt_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
            run_now = *strInit("emerg");
            run_prior = prior;
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
        } else if (midInt == 3) {
            if (run_prior >= prior) { Pool_prior = *listPush(&Pool_prior, (void*)(&prior), 1);
            Pool_now = *listPush(&Pool_now, (void*)strInit("img_acq"), 2); } else { if (!strEqual(run_now, *strInit(""))) { if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = preempt_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = preempt_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = preempt_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = preempt_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
            run_now = *strInit("img_acq");
            run_prior = prior;
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
        } else if (midInt == 4) {
            if (run_prior >= prior) { Pool_prior = *listPush(&Pool_prior, (void*)(&prior), 1);
            Pool_now = *listPush(&Pool_now, (void*)strInit("pan_ctr_th"), 2); } else { if (!strEqual(run_now, *strInit(""))) { if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = preempt_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = preempt_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = preempt_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = preempt_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
            run_now = *strInit("pan_ctr_th");
            run_prior = prior;
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
        } else if (midInt == 5) {
            if (run_prior >= prior) { Pool_prior = *listPush(&Pool_prior, (void*)(&prior), 1);
            Pool_now = *listPush(&Pool_now, (void*)strInit("velo_voter"), 2); } else { if (!strEqual(run_now, *strInit(""))) { if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = preempt_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = preempt_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = preempt_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = preempt_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = preempt_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = preempt_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
            run_now = *strInit("velo_voter");
            run_prior = prior;
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } }
        } else if (midInt == 6) {
            
            if (Pool_prior.length > 0) { run_prior = -1;
            run_now = *strInit("");
            max_index = 0;
            idx = 0;
            while (idx < Pool_prior.length) {
                now_prior = 0;
                now_prior = *(double*)listGet(&Pool_prior, idx);
                if (now_prior > run_prior) { run_prior = *(double*)listGet(&Pool_prior, idx);
                run_now = *(String*)listGet(&Pool_now, idx);
                max_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, max_index);
            Pool_now = *listDel(&Pool_now, max_index);
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } } else { run_prior = -1;
            run_now = *strInit(""); }
        } else if (midInt == 7) {
            
            if (Pool_prior.length > 0) { run_prior = -1;
            run_now = *strInit("");
            max_index = 0;
            idx = 0;
            while (idx < Pool_prior.length) {
                now_prior = 0;
                now_prior = *(double*)listGet(&Pool_prior, idx);
                if (now_prior > run_prior) { run_prior = *(double*)listGet(&Pool_prior, idx);
                run_now = *(String*)listGet(&Pool_now, idx);
                max_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, max_index);
            Pool_now = *listDel(&Pool_now, max_index);
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } } else { run_prior = -1;
            run_now = *strInit(""); }
        } else if (midInt == 8) {
            
            if (Pool_prior.length > 0) { run_prior = -1;
            run_now = *strInit("");
            max_index = 0;
            idx = 0;
            while (idx < Pool_prior.length) {
                now_prior = 0;
                now_prior = *(double*)listGet(&Pool_prior, idx);
                if (now_prior > run_prior) { run_prior = *(double*)listGet(&Pool_prior, idx);
                run_now = *(String*)listGet(&Pool_now, idx);
                max_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, max_index);
            Pool_now = *listDel(&Pool_now, max_index);
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } } else { run_prior = -1;
            run_now = *strInit(""); }
        } else if (midInt == 9) {
            
            if (Pool_prior.length > 0) { run_prior = -1;
            run_now = *strInit("");
            max_index = 0;
            idx = 0;
            while (idx < Pool_prior.length) {
                now_prior = 0;
                now_prior = *(double*)listGet(&Pool_prior, idx);
                if (now_prior > run_prior) { run_prior = *(double*)listGet(&Pool_prior, idx);
                run_now = *(String*)listGet(&Pool_now, idx);
                max_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, max_index);
            Pool_now = *listDel(&Pool_now, max_index);
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } } else { run_prior = -1;
            run_now = *strInit(""); }
        } else if (midInt == 10) {
            
            if (Pool_prior.length > 0) { run_prior = -1;
            run_now = *strInit("");
            max_index = 0;
            idx = 0;
            while (idx < Pool_prior.length) {
                now_prior = 0;
                now_prior = *(double*)listGet(&Pool_prior, idx);
                if (now_prior > run_prior) { run_prior = *(double*)listGet(&Pool_prior, idx);
                run_now = *(String*)listGet(&Pool_now, idx);
                max_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, max_index);
            Pool_now = *listDel(&Pool_now, max_index);
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } } else { run_prior = -1;
            run_now = *strInit(""); }
        } else if (midInt == 11) {
            
            if (Pool_prior.length > 0) { run_prior = -1;
            run_now = *strInit("");
            max_index = 0;
            idx = 0;
            while (idx < Pool_prior.length) {
                now_prior = 0;
                now_prior = *(double*)listGet(&Pool_prior, idx);
                if (now_prior > run_prior) { run_prior = *(double*)listGet(&Pool_prior, idx);
                run_now = *(String*)listGet(&Pool_now, idx);
                max_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, max_index);
            Pool_now = *listDel(&Pool_now, max_index);
            if (strEqual(run_now, *strInit("PI_ctr"))) { channel.channelNo = run_l_0_r__lb_PI_ctr_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("comp_obs_pos"))) { channel.channelNo = run_l_0_r__lb_comp_obs_pos_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("emerg"))) { channel.channelNo = run_l_0_r__lb_emerg_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("img_acq"))) { channel.channelNo = run_l_0_r__lb_img_acq_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("pan_ctr_th"))) { channel.channelNo = run_l_0_r__lb_pan_ctr_th_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } else if (strEqual(run_now, *strInit("velo_voter"))) { channel.channelNo = run_l_0_r__lb_velo_voter_rb_;
            channel.type = 1;
            channel.pos = midDouble;
            output(threadNumber, channel); } } else { run_prior = -1;
            run_now = *strInit(""); }
        } else if (midInt == 12) {
            idx = 0;
            del_index = 0;
            while (idx < Pool_prior.length) {
                now_now = *strInit("");
                now_now = *(String*)listGet(&Pool_now, idx);
                if (strEqual(now_now, *strInit("PI_ctr"))) { del_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, del_index);
            Pool_now = *listDel(&Pool_now, del_index);
        } else if (midInt == 13) {
            idx = 0;
            del_index = 0;
            while (idx < Pool_prior.length) {
                now_now = *strInit("");
                now_now = *(String*)listGet(&Pool_now, idx);
                if (strEqual(now_now, *strInit("comp_obs_pos"))) { del_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, del_index);
            Pool_now = *listDel(&Pool_now, del_index);
        } else if (midInt == 14) {
            idx = 0;
            del_index = 0;
            while (idx < Pool_prior.length) {
                now_now = *strInit("");
                now_now = *(String*)listGet(&Pool_now, idx);
                if (strEqual(now_now, *strInit("emerg"))) { del_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, del_index);
            Pool_now = *listDel(&Pool_now, del_index);
        } else if (midInt == 15) {
            idx = 0;
            del_index = 0;
            while (idx < Pool_prior.length) {
                now_now = *strInit("");
                now_now = *(String*)listGet(&Pool_now, idx);
                if (strEqual(now_now, *strInit("img_acq"))) { del_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, del_index);
            Pool_now = *listDel(&Pool_now, del_index);
        } else if (midInt == 16) {
            idx = 0;
            del_index = 0;
            while (idx < Pool_prior.length) {
                now_now = *strInit("");
                now_now = *(String*)listGet(&Pool_now, idx);
                if (strEqual(now_now, *strInit("pan_ctr_th"))) { del_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, del_index);
            Pool_now = *listDel(&Pool_now, del_index);
        } else if (midInt == 17) {
            idx = 0;
            del_index = 0;
            while (idx < Pool_prior.length) {
                now_now = *strInit("");
                now_now = *(String*)listGet(&Pool_now, idx);
                if (strEqual(now_now, *strInit("velo_voter"))) { del_index = idx; }
                idx = idx + 1;
            }
            Pool_prior = *listDel(&Pool_prior, del_index);
            Pool_now = *listDel(&Pool_now, del_index);
        }
    }
    threadState[threadNumber] = STATE_STOPPED;
    updateCurrentTime(threadNumber);
    return NULL;
}