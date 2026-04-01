std_msgs/Header header

uint32 NONE=0
uint32 READY=1
uint32 HOME_REACHED=2
uint32 STRING_READY=3
uint32 CONTACT_DETECTED=4
uint32 FORCE_STABLE=5
uint32 STROKE_DONE=6
uint32 BOW_RAISED=7
uint32 FINISHED=8
uint32 ERROR=100
uint32 CONTACT_LOST=101
uint32 NO_CONTACT=102
uint32 FORCE_OVERLIMIT=103
uint32 LIMIT_REACHED=104
uint32 TIMEOUT=105

uint32 event_code
string state_name
string description
bool level_error
