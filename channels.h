// contains struct information for each channel. 

const int chArr[6]={1,2,3,4,5,6};

//struct {
//  int out = 41;
//} Channel1;
//
//struct {
//  int stepState[8] = {0,0,1,0,0,1,0,0};
//  int out = 43;
//} Channel2;
//
//struct {
//  int stepState[8] = {1,1,1,1,1,1,1,1};
//  int out = 45;
//} Channel3;
//
//struct {
//  int stepState[8] = {0,0,0,0,0,0,0,0};
//  int out = 47;
//} Channel4;
//
//struct {
//  int stepState[8] = {0,0,0,0,0,0,0,0};
//  int out = 49;
//} Channel5;
//
//struct {
//  int stepState[8] = {0,0,0,0,0,0,0,0};
//  int ou = 51;
//} Channel6;

int Channel[6][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int channelYpos[6] = {10,19,28,37,46,55};
