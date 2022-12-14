// Code generated by the Lingua Franca compiler from:
// file://home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf
#define LOG_LEVEL 2
#define TARGET_FILES_DIRECTORY "/home/jk/Documents/lab_test/src-gen/controllers/boomer/lfc-src/webotsClient"
#include "include/ctarget/ctarget.h"
#include "core/threaded/reactor_threaded.c"
#include "core/threaded/scheduler.h"
#include "core/mixed_radix.h"
#include <stdio.h>
#include <string.h>
#include <webots/keyboard.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/inertial_unit.h>
#include <webots/position_sensor.h>
#include <webots/compass.h>

// to be used as array indices
enum { X, Y, Z };

// This needs to be changed if the .wbt model changes
#define FRONT_WHEEL_RADIUS 0.38
#define ENCODER_UNIT 0.25

// devices
WbDeviceTag left_steer, right_steer;
WbDeviceTag left_front_wheel, right_front_wheel;
WbDeviceTag left_position_sensor, right_position_sensor;
WbDeviceTag compass;

// misc variables
int time_step = -1;
double speed = 0.0;
double steering_angle = 0.0;
double manual_steering = 0.0;

void print_help() {
printf("You can drive this vehicle!\n");
printf("Select the 3D window and then use the cursor keys to:\n");
printf("[LEFT]/[RIGHT] - steer\n");
printf("[UP]/[DOWN] - accelerate/slow down\n");
}

// positive: turn right, negative: turn left
void set_steering_angle(double wheel_angle) {
steering_angle = wheel_angle;
wb_motor_set_position(left_steer, steering_angle);
wb_motor_set_position(right_steer, steering_angle);
}

void change_manual_steer_angle(double inc) {
double new_manual_steering = manual_steering + inc;
printf("steer %f\n", new_manual_steering);
if (new_manual_steering <= 0.94 && new_manual_steering >= -0.94) {
    manual_steering = new_manual_steering;
    set_steering_angle(manual_steering);
}

if (manual_steering == 0)
    printf("going straight\n");
else
    printf("turning %.2f rad (%s)\n", steering_angle, steering_angle < 0 ? "left" : "right");
}
int main(int argc, char* argv[]) {
    return lf_reactor_c_main(argc, argv);
}
void _lf_set_default_command_line_options() {}
// =============== START reactor class webotsClient
typedef struct {
    struct self_base_t base;
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    reaction_t _lf__reaction_0;
    trigger_t _lf__startup;
    reaction_t* _lf__startup_reactions[1];
} webotsclient_self_t;
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/ctarget/set.h"
void webotsclientreaction_function_0(void* instance_args) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    webotsclient_self_t* self = (webotsclient_self_t*)instance_args;
    #pragma GCC diagnostic pop
    #line 65 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    wb_robot_init();
    time_step = (int)wb_robot_get_basic_time_step();
    
    WbDeviceTag imu = wb_robot_get_device("inertial unit");
    wb_inertial_unit_enable(imu, time_step);
    
    // find wheels
    left_front_wheel = wb_robot_get_device("left wheel motor");
    right_front_wheel = wb_robot_get_device("right wheel motor");
    
    wb_motor_set_position(left_front_wheel, INFINITY);
    wb_motor_set_position(right_front_wheel, INFINITY);
    wb_motor_set_velocity(left_front_wheel, 3.1);
    wb_motor_set_velocity(right_front_wheel, 3.1);
    printf("You are driving this vehicle!\n");
    
    //encoder + compass
    left_position_sensor = wb_robot_get_device("left wheel sensor");
    right_position_sensor = wb_robot_get_device("right wheel sensor");
    wb_position_sensor_enable(left_position_sensor, time_step);
    wb_position_sensor_enable(right_position_sensor, time_step);
    double left_encoder_offset = 0.0;
    double right_encoder_offset = 0.0;
    
    while (wb_robot_step(time_step) != -1) {
    double encoder_value[2];
    encoder_value[0] = ENCODER_UNIT * (wb_position_sensor_get_value(left_position_sensor) - left_encoder_offset);
    encoder_value[1] = ENCODER_UNIT * (wb_position_sensor_get_value(right_position_sensor) - right_encoder_offset);
    const double roll = wb_inertial_unit_get_roll_pitch_yaw(imu)[0]*100;
    const double pitch = wb_inertial_unit_get_roll_pitch_yaw(imu)[1]*100;
    const double yaw = wb_inertial_unit_get_roll_pitch_yaw(imu)[2]*100;
    
    printf("roll: %f, pitch: %f, yaw: %f \n", roll, pitch, yaw);
    printf("encode1: %f, encode2: %f \n", encoder_value[0], encoder_value[1]);
    //encode1: 298, encode2: 303roll: 2.503876, pitch: 30.529904, yaw: -303.518054 
    if (encoder_value[0]>=298 && encoder_value[1]>=303.39){
        wb_motor_set_velocity(left_front_wheel, 10);
        wb_motor_set_velocity(right_front_wheel, 10);
    }
    
    else if (encoder_value[0]>=298){
        wb_motor_set_velocity(left_front_wheel, 0);
        wb_motor_set_velocity(right_front_wheel, 10); 
    }
    //roll: -23.625994, pitch: -6.759791, yaw: 156.173631 
    else if (roll>=-23.625994 && pitch >= -6.7597 && yaw >= 156.173){
        wb_motor_set_velocity(left_front_wheel, 10);
        wb_motor_set_velocity(right_front_wheel, 10);
    }
    
    else if (encoder_value[0]>=295){
        wb_motor_set_velocity(left_front_wheel, 0);
        wb_motor_set_velocity(right_front_wheel, 10); 
    }
    else{
        wb_motor_set_velocity(left_front_wheel, speed);
        wb_motor_set_velocity(right_front_wheel, speed);
        speed=speed+1;
        if(speed>10){
        speed=10;
        }
    }
    }
    
    wb_robot_cleanup();
        
}
#include "include/ctarget/set_undef.h"
webotsclient_self_t* new_webotsClient() {
    webotsclient_self_t* self = (webotsclient_self_t*)_lf_new_reactor(sizeof(webotsclient_self_t));
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.number = 0;
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.function = webotsclientreaction_function_0;
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.self = self;
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.STP_handler = NULL;
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.name = "?";
    #line 64 "file:/home/jk/Documents/lab_test/controllers/boomer/lfc-src/webotsClient.lf"
    self->_lf__reaction_0.mode = NULL;
    self->_lf__startup_reactions[0] = &self->_lf__reaction_0;
    self->_lf__startup.last = NULL;
    self->_lf__startup.reactions = &self->_lf__startup_reactions[0];
    self->_lf__startup.number_of_reactions = 1;
    self->_lf__startup.is_timer = false;
    return self;
}
// =============== END reactor class webotsClient
// Array of pointers to timer triggers to be scheduled in _lf_initialize_timers().
trigger_t** _lf_timer_triggers = NULL;
int _lf_timer_triggers_size = 0;
// Array of pointers to startup triggers.
reaction_t* _lf_startup_reactions[1];
int _lf_startup_reactions_size = 1;
// Array of pointers to shutdown triggers.
reaction_t** _lf_shutdown_reactions = NULL;
int _lf_shutdown_reactions_size = 0;
// Array of pointers to reset triggers.
reaction_t** _lf_reset_reactions = NULL;
int _lf_reset_reactions_size = 0;
trigger_t* _lf_action_for_port(int port_id) {
        return NULL;
}
void _lf_initialize_trigger_objects() {
    // Initialize the _lf_clock
    lf_initialize_clock();
    int _lf_startup_reactions_count = 0;
    int _lf_shutdown_reactions_count = 0;
    int _lf_reset_reactions_count = 0;
    int _lf_timer_triggers_count = 0;
    int _lf_tokens_with_ref_count_count = 0;
    webotsclient_self_t* webotsclient_self[1];
    // ***** Start initializing webotsClient of class webotsClient
    webotsclient_self[0] = new_webotsClient();
    int bank_index = 0;
    _lf_startup_reactions[_lf_startup_reactions_count++] = &webotsclient_self[0]->_lf__reaction_0;
    webotsclient_self[0]->_lf__reaction_0.deadline = NEVER;
    //***** End initializing webotsClient
    // **** Start deferred initialize for webotsClient
    {
        // Total number of outputs (single ports and multiport channels)
        // produced by reaction_0 of webotsClient.
        webotsclient_self[0]->_lf__reaction_0.num_outputs = 0;
        {
            int count = 0;
        }
        
        // ** End initialization for reaction 0 of webotsClient
    }
    // **** End of deferred initialize for webotsClient
    // **** Start non-nested deferred initialize for webotsClient
    // **** End of non-nested deferred initialize for webotsClient
    // Connect inputs and outputs for reactor webotsClient.
    // Set reaction priorities for ReactorInstance webotsClient
    {
        webotsclient_self[0]->_lf__reaction_0.chain_id = 1;
        // index is the OR of level 0 and 
        // deadline 140737488355327 shifted left 16 bits.
        webotsclient_self[0]->_lf__reaction_0.index = 0x7fffffffffff0000LL;
    }
    // Initialize the scheduler
    size_t num_reactions_per_level[1] = 
        {1};
    sched_params_t sched_params = (sched_params_t) {
                            .num_reactions_per_level = &num_reactions_per_level[0],
                            .num_reactions_per_level_size = (size_t) 1};
    lf_sched_init(
        (size_t)_lf_number_of_workers,
        &sched_params
    );
}
void _lf_trigger_startup_reactions() {
    for (int i = 0; i < _lf_startup_reactions_size; i++) {
        if (_lf_startup_reactions[i] != NULL) {
            _lf_trigger_reaction(_lf_startup_reactions[i], -1);
        }
    }
}
void _lf_initialize_timers() {

}
void logical_tag_complete(tag_t tag_to_send) {

}
bool _lf_trigger_shutdown_reactions() {
    return false;
}
void terminate_execution() {}
