#define RIGHT 0
#define BACK 1
#define LEFT 2
#define FRONT 3

#define EAST 0
#define SOUTH 1
#define WEST 2
#define NORTH 3

#define SERVO_MIDDLE 80
#define SERVO_LEFT 3
#define SERVO_SHORT_LEFT 85
#define SERVO_RIGHT 170
#define SERVO_SHORT_RIGHT 75

#define BLACK_RECOGNITION

#define TIMER_STOP TCCR5B &= ~((1<<CS50)|(1<<CS51));
#define TIMER_START TCCR5B |= (1<<CS50)|(1<<CS51);

#define FLOOR_SIZE 6
#define FLOOR_COUNT 1

#define VICTIM_TEMPERATURE 30

#define SIGNUM(x) (x==0 ? 0 : x/abs(x))
