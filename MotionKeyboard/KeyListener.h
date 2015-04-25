#include <iostream>
#include <string.h>
#include "LeapSDK/include/Leap.h"
#include <cmath>
#include <vector>
#include "KeyBoardGUI.h"

using std::vector;

using namespace Leap;



class KeyListener : public Listener
{
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);
	virtual char onClick(const Hand &, int, int, int *);
	virtual void onRelease(char);

private:
	bool is_start(const Hand& hand);
	char getLetter(int arr[], int size, int row, int offset);
	int findRow(int arr[], int size);
	int get_min(int * arr, int size);
	std::vector< std::pair<int, int> > sorted(int* arr, int size);
	char get_letter(const Hand& hand, int index, int row);
	void print_scr(int row);
	char last_pressed[2];
	unsigned int frame_counter;
private:
	vector<char> line;
	bool start;
	int last_position;
	int upper_bound;
	int lower_bound;
	char lastKey;
	bool CapsLock;
	KeyBoardGUI keyboard;
};