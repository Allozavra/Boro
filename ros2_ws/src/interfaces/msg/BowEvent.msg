std_msgs/Header header

uint32 NONE=0
uint32 READY=1
uint32 HOME_REACHED=2
uint32 CONFIG_SELECTED=3
uint32 ANGLE_REACHED=4
uint32 CONTACT_REACHED=5
uint32 CONFIG_PLAYED=6
uint32 BOW_RAISED=7
uint32 SHUTDOWN_SELECTED=8
uint32 ERROR=100
uint32 CONTACT_LOST=101
uint32 NO_CONTACT_DETECTED=102
uint32 TIMEOUT=105

uint32 event_code
string state_name
string description
bool level_error
