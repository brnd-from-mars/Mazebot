#define RIGHT 0
#define BACK  1
#define LEFT  2
#define FRONT 3

#define EAST  0
#define SOUTH 1
#define WEST  2
#define NORTH 3

#define SERVO_MIDDLE       80
#define SERVO_LEFT          3
#define SERVO_SHORT_LEFT   85
#define SERVO_RIGHT       170
#define SERVO_SHORT_RIGHT  75

#define NAVIGATION_ACTION_NOTHING     -1
#define NAVIGATION_ACTION_TURN_RIGHT   0
#define NAVIGATION_ACTION_TURN_180     1
#define NAVIGATION_ACTION_TURN_LEFT    2
#define NAVIGATION_ACTION_DRIVE_FWD    3
#define NAVIGATION_ACTION_ESC_BLACK    4
#define NAVIGATION_ACTION_RESTORE_BKUP 5
#define NAVIGATION_ACTION_DRIVE_RAMP   6

#define MAP_MAX_FIELDS_PER_FLOOR 40

#define BLACK_RECOGNITION

//#define DRIVE_ENC_STEPS_PER_CM     4.70
//#define DRIVE_ENC_STEPS_PER_90_ROT 70
#define DRIVE_ENC_STEPS_PER_CM      4.70
#define DRIVE_ENC_STEPS_PER_90_ROT 70

// #define BLACK_MIN  750
//#define SILVER_MAX 450
#define BLACK_MIN  900
#define SILVER_MAX 450

#define TIMER_STOP  TCCR5B &= ~((1<<CS50) | (1<<CS51))
#define TIMER_START TCCR5B |=   (1<<CS50) | (1<<CS51)

#define VICTIM_TEMPERATURE 35

#define RAMP_UP   340
#define RAMP_DOWN 325

#define COM_SCAN_LEFT       0xE00 
#define COM_SCAN_RIGHT      0xE01
#define COM_MAP_RESET       0xE10
#define COM_MAP_SEND_START  0xE11
#define COM_MAP_SEND_STOP   0xE12
#define COM_MAP_POSITION    0xE13
#define COM_VICT_SEND_START 0xE14
#define COM_VICT_SEND_STOP  0xE15

#define COM_LETTER_LEFT     0xD00
#define COM_LETTER_RIGHT    0xD10
#define COM_LETTER_H        0xD00
#define COM_LETTER_S        0xD01
#define COM_LETTER_U        0xD02
#define COM_LETTER_E        0xD03

#define SIGNUM(x) ((x==0) ? 0 : x/abs(x))
