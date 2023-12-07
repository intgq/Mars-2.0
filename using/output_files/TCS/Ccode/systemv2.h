
#ifndef systemv2_h
#define systemv2_h

#define step_size 0.0001
#define output_step_size 0.01
#define reqProcessor_l_0_r__lb_PI_ctr_rb_ 0
#define preempt_l_0_r__lb_PI_ctr_rb_ 1
#define preempt_l_0_r__lb_comp_obs_pos_rb_ 2
#define preempt_l_0_r__lb_emerg_rb_ 3
#define preempt_l_0_r__lb_img_acq_rb_ 4
#define preempt_l_0_r__lb_pan_ctr_th_rb_ 5
#define preempt_l_0_r__lb_velo_voter_rb_ 6
#define run_l_0_r__lb_PI_ctr_rb_ 7
#define run_l_0_r__lb_comp_obs_pos_rb_ 8
#define run_l_0_r__lb_emerg_rb_ 9
#define run_l_0_r__lb_img_acq_rb_ 10
#define run_l_0_r__lb_pan_ctr_th_rb_ 11
#define run_l_0_r__lb_velo_voter_rb_ 12
#define reqProcessor_l_0_r__lb_comp_obs_pos_rb_ 13
#define reqProcessor_l_0_r__lb_emerg_rb_ 14
#define reqProcessor_l_0_r__lb_img_acq_rb_ 15
#define reqProcessor_l_0_r__lb_pan_ctr_th_rb_ 16
#define reqProcessor_l_0_r__lb_velo_voter_rb_ 17
#define free_l_0_r__lb_PI_ctr_rb_ 18
#define free_l_0_r__lb_comp_obs_pos_rb_ 19
#define free_l_0_r__lb_emerg_rb_ 20
#define free_l_0_r__lb_img_acq_rb_ 21
#define free_l_0_r__lb_pan_ctr_th_rb_ 22
#define free_l_0_r__lb_velo_voter_rb_ 23
#define exit_l_0_r__lb_PI_ctr_rb_ 24
#define exit_l_0_r__lb_comp_obs_pos_rb_ 25
#define exit_l_0_r__lb_emerg_rb_ 26
#define exit_l_0_r__lb_img_acq_rb_ 27
#define exit_l_0_r__lb_pan_ctr_th_rb_ 28
#define exit_l_0_r__lb_velo_voter_rb_ 29
#define inputs_lb_PHY_vehicle_imp_rb__lb_veh_s_rb_ 30
#define inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_l_rb_ 31
#define inputs_lb_PHY_vehicle_imp_rb__lb_veh_v_w_rb_ 32
#define outputs_lb_PHY_vehicle_imp_rb__lb_veh_a_rb_ 33
#define outputs_lb_camera_rb__lb_img_rb_ 34
#define outputs_lb_radar_rb__lb_obs_pos_radar_rb_ 35
#define inputs_lb_actuator_rb__lb_cmd_rb_ 36
#define outputs_lb_GPS_rb__lb_veh_pos_rb_ 37
#define outputs_lb_laser_rb__lb_laser_valid_rb_ 38
#define outputs_lb_laser_rb__lb_laser_v_rb_ 39
#define outputs_lb_wheel_rb__lb_wheel_valid_rb_ 40
#define outputs_lb_wheel_rb__lb_wheel_v_rb_ 41
#define outputs_lb_user_panel_rb__lb_event_rb_ 42
#define dis_lb_img_acq_rb_ 43
#define inputs_lb_img_acq_rb__lb_img_rb_ 44
#define outputs_lb_img_acq_rb__lb_proc_img_rb_ 45
#define dis_lb_comp_obs_pos_rb_ 46
#define inputs_lb_comp_obs_pos_rb__lb_obs_pos_radar_rb_ 47
#define inputs_lb_comp_obs_pos_rb__lb_proc_img_rb_ 48
#define outputs_lb_comp_obs_pos_rb__lb_obs_pos_rb_ 49
#define dis_lb_emerg_rb_ 50
#define inputs_lb_emerg_rb__lb_des_a_rb_ 51
#define inputs_lb_emerg_rb__lb_veh_v_rb_ 52
#define inputs_lb_emerg_rb__lb_obs_pos_rb_ 53
#define inputs_lb_emerg_rb__lb_veh_pos_rb_ 54
#define outputs_lb_emerg_rb__lb_cmd_rb_ 55
#define dis_lb_velo_voter_rb_ 56
#define inputs_lb_velo_voter_rb__lb_wheel_v_rb_ 57
#define inputs_lb_velo_voter_rb__lb_wheel_valid_rb_ 58
#define inputs_lb_velo_voter_rb__lb_laser_v_rb_ 59
#define inputs_lb_velo_voter_rb__lb_laser_valid_rb_ 60
#define outputs_lb_velo_voter_rb__lb_veh_v_rb_ 61
#define dis_lb_pan_ctr_th_rb__lb_event_rb_ 62
#define outputs_lb_pan_ctr_th_rb__lb_des_v_rb_ 63
#define inputs_lb_PI_ctr_rb__lb_des_v_rb_ 64
#define inputs_lb_PI_ctr_rb__lb_veh_v_rb_ 65
#define outputs_lb_PI_ctr_rb__lb_des_a_rb_ 66
#define dis_lb_PI_ctr_rb_ 67
void* scheduler (void* arg);
void* vehicle_imp (void* arg);
void* camera (void* arg);
void* radar (void* arg);
void* actuator (void* arg);
void* GPS (void* arg);
void* laser (void* arg);
void* wheel (void* arg);
void* user_panel (void* arg);
void* ACT_img_acq (void* arg);
void* img_acq (void* arg);
void* ACT_comp_obs_pos (void* arg);
void* comp_obs_pos (void* arg);
void* ACT_emerg (void* arg);
void* emerg (void* arg);
void* ACT_velo_voter (void* arg);
void* velo_voter (void* arg);
void* pan_ctr_th (void* arg);
void* tran_img (void* arg);
void* tran_obs_pos_radar (void* arg);
void* tran_cmd (void* arg);
void* tran_veh_pos (void* arg);
void* tran_laser_valid (void* arg);
void* tran_laser_v (void* arg);
void* tran_wheel_valid (void* arg);
void* tran_wheel_v (void* arg);
void* tran_event (void* arg);
void* tran_des_v (void* arg);
void* tran_proc_img (void* arg);
void* tran_obs_pos (void* arg);
void* tran_veh_v (void* arg);
void* tran_des_a (void* arg);
void* ACT_PI_ctr (void* arg);
void* PI_ctr (void* arg);
#endif