// storage of integers and the likes

const int outArr[6]={41,43,45,47,49,51};
const int ledArr[8]={2,3,4,5,6,7,14,15};
//const int btnArr[8]={23,25,27,29,31,33,35,37};
int ledStp = 0;
int currentCh = 0;
int step = 0; 

// LED Pins for DirectIO
const int step1L = 2;
const int step2L = 3;
const int step3L = 4;
const int step4L = 5;
const int step5L = 6;
const int step6L = 7;
const int step7L = 14;
const int step8L = 15;

// Buttons for DirectIO
const int step1B = 23;
const int step2B = 25;
const int step3B = 27;
const int step4B = 29;
const int step5B = 31;
const int step6B = 33;
const int step7B = 35;
const int step8B = 37;

// setup DirectIO outputs
//LEDs
volatile Output<step1L> l_1;
volatile Output<step2L> l_2;
volatile Output<step3L> l_3;
volatile Output<step4L> l_4;
volatile Output<step5L> l_5;
volatile Output<step6L> l_6;
volatile Output<step7L> l_7;
volatile Output<step8L> l_8;
//Buttons
volatile Input<step1B> b_1;
volatile Input<step2B> b_2;
volatile Input<step3B> b_3;
volatile Input<step4B> b_4;
volatile Input<step5B> b_5;
volatile Input<step6B> b_6;
volatile Input<step7B> b_7;
volatile Input<step8B> b_8;
const char btnArr[8] = {b_1,b_2,b_3,b_4,b_5,b_6,b_7,b_8};
const int chBtn = 53;

// Tempo and duty cycle parameters
bool running;  // clock is running?
float BPM = 120.0;     // Beats per minute
float max_BPM = 208.0;
float min_BPM = 7.5;
int PPB = 4;         // pulses per beat
int max_time = 60000 / min_BPM;
int min_time = 60000 / max_BPM;
int duty_cycle = 50;  // in percent
int min_duty = 5;
int max_duty = 95;
unsigned long period = 0; // period in microseconds
unsigned long ontime = 0; // on time in microseconds

// For the timer interrupt
volatile unsigned long tickcount;  // interrupt counter
volatile unsigned long cycle_start_time;  // time (in microsec) when this cycle started
volatile bool clock_state;              // true when clock pulse high
bool ch_press;

// Channel button debounce 
const unsigned long debounceDelay = 50;    // the debounce time milliseconds; increase if the output flickers
