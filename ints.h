// storage of integers and the likes

const int outArr[6]={41,43,45,47,49,51};
int ledArr[8]={2,3,4,5,6,7,14,15};
const int btnArr[8]={23,25,27,29,31,33,35,37};
int ledStp = 0;
int currentCh = 0;
int step = 0; 

// setup DirectIO outputs
//LEDs
Output<2> l_1;
Output<3> l_2;
Output<4> l_3;
Output<5> l_4;
Output<6> l_5;
Output<7> l_6;
Output<14> l_7;
Output<15> l_8;

//Buttons
Input<23> b_1;
Input<25> b_2;
Input<27> b_3;
Input<29> b_4;
Input<31> b_5;
Input<33> b_6;
Input<35> b_7;
Input<37> b_8;

//char btnArr[8] = {b_1,b_2,b_3,b_4,b_5,b_6,b_7,b_8};
//char ledArr[8]={l_1,l_2,l_3,l_4,l_5,l_6,l_7,l_8};
const int chBtn = 53;

//const int step_1test = 23;   // this was done for testing a single button

// Tempo and duty cycle parameters
bool running;  // clock is running?
float BPM = 90.0;     // Beats per minute
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
