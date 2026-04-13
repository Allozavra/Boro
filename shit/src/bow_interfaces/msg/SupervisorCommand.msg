std_msgs/Header header

uint8 CMD_NONE=0
uint8 CMD_GOTO_HOME=1
uint8 CMD_SELECT_STRING=2
uint8 CMD_LOWER_BOW=3
uint8 CMD_START_STROKE=4
uint8 CMD_RAISE_BOW=5
uint8 CMD_FINISH=6
uint8 CMD_RESET_ERROR=7
uint8 CMD_ABORT=8

uint8 command
string selected_string
float64 desired_alpha
float64 desired_force
float64 bow_velocity
float64 bow_range_min
float64 bow_range_max
float64 h_approach_velocity
float64 h_raise_velocity
bool auto_continue
