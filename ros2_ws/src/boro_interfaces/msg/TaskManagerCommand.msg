std_msgs/Header header

uint8 CMD_NONE=0
uint8 CMD_INIT=1
uint8 CMD_HOME=2
uint8 CMD_SELECT_CONFIG=3
uint8 CMD_ROTATE=4
uint8 CMD_LOWER_BOW=5
uint8 CMD_BOWING=6
uint8 CMD_RAISE_BOW=7
uint8 CMD_FINISH=8
uint8 CMD_ERROR=9

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
