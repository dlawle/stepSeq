// contains struct information for each channel. 
int outArr[6]={41,43,45,47,49,51};
int currentCh = 1;
int step = 0; 

struct {
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int out = 41;
} Channel1;

struct {
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int out = 43;
} Channel2;

struct {
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int out = 45;
} Channel3;

struct {
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int out = 47;
} Channel4;

struct {
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int out = 49;
} Channel5;

struct {
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int ou = 51;
} Channel6;
