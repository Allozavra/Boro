std_msgs/Header header

uint8 MODE_OFF=0
uint8 MODE_POSITION=1
uint8 MODE_VELOCITY=2
uint8 MODE_FORCE=3

uint8 alpha_mode
uint8 h_mode
uint8 l_mode

float64 alpha_target
float64 alpha_velocity_target
float64 alpha_acceleration_target

float64 h_target
float64 h_velocity_target
float64 h_acceleration_target

float64 force_target

float64 l_target
float64 l_velocity_target
float64 l_acceleration_target

bool enable
bool emergency_stop
string source_state
