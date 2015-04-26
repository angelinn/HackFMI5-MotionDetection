/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include "KeyListener.h"
#include <fstream>

int seconds = 0;
int cnt_A = 0;
const int arr_size = 5;
const char LETTERS[3][10] = { { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' },
{ 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\n' },
{ 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.', ' ', '=' } };
#define SPEED_OFFSET 1
int last_row = 0;
const int LAST_KEY_NONE = -1;

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };
const SDL_Color white = { 255, 255, 255, 255 };
const SDL_Color red = { 255, 0, 0, 255 };

void KeyListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
	last_pressed[0] = '!';
	frame_counter = 0;
	last_pressed[1] = '!';
	lastKey = LAST_KEY_NONE;
	this->start = false ;
	CapsLock = false;
	keyboard.CreateWindow();
	keyboard.DrawKeyBoard();
}

void KeyListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void KeyListener::onDisconnect(const Controller& controller)
{
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void KeyListener::onExit(const Controller& controller) 
{
	std::cout << "Exited" << std::endl;
}

char KeyListener::onClick(const Hand& hand, int minIndex, int index, int* axisZ)
{
	std::cout << "clicked" << std::endl;
	char current = get_letter(hand, minIndex, findRow(axisZ, arr_size));
	
	if (current == last_pressed[index])
		return -100;

	if (current != '=')
	{
		line.push_back(current);
		last_pressed[index] = current;
	}
	else
	{
		if (!line.empty())
			line.pop_back();
		last_pressed[index] = current;
	}

	keyboard.changeColours(current, red);

	return current;
}

void KeyListener::onRelease(char lastKey)
{
	keyboard.RedrawEmptyBoard();
}

bool KeyListener::is_start(const Hand& hand)
{
	if(!start)
	{
		if(frame_counter % 10 != 0)
			return false;

		const FingerList fingers = hand.fingers();

		int new_z;
		int tmh_z;
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) 
		{
			const Finger finger = *fl;
			if (fingerNames[finger.type()] == "Middle")
			{
				new_z = finger.tipPosition()[2];
			}
			else if (fingerNames[finger.type()] == "Thumb")
			{
				tmh_z = finger.tipPosition()[2];
			}
		}
		std::cout << "( " << new_z << " , " << last_position << std::endl;
		if (abs(new_z-last_position) < SPEED_OFFSET)
		{
			start = true;
			int offset = abs (new_z - tmh_z);
			upper_bound = new_z +(1/8)*offset;
			lower_bound = new_z - offset - (1/8)*offset;
		}
		else
		{
			last_position = new_z;
			return false;
		}
	}
	else 
		return true;
}
int KeyListener::get_min(int* arr, int size)
{
	int f_pos = -1;
	int min = arr[0];
	int min_index = 0;
	for (int i = 1; i < size; i++) 
	{
		if (min > arr[i]) {
			min = arr[i];
			min_index = i;
		}
	}
	
	if (min_index + 1 == size &&  arr[min_index - 1] - arr[min_index] > 20)
		return min_index;
	else if (arr[min_index + 1] - arr[min_index] > 20)
		return min_index;

	return -1;
}

char KeyListener::get_letter(const Hand& hand, int index, int row)
{
	if (!hand.isLeft())
		index = index + 5;
	else
		index = 4 - index;

	return LETTERS[2 - row][index];
}
int KeyListener::findRow(int arr[], int size){
	if (arr[2]  < lower_bound)
	{
		return 2;
	}
	else if (arr[2] > upper_bound)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}

void KeyListener::print_scr(int row){
	system("cls");
	std::cout << "You are on row: "<< row << std::endl;
	for ( int i =0; i < line.size(); i++) {
		std::cout << line[i];
	}
}

void KeyListener::onFrame(const Controller& controller) {
	
	seconds++;
	seconds = seconds % 20;
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	
	HandList hands = frame.hands();

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) 
	{
		// Get the first hand
		const Hand hand = *hl;
		bool is_left = hand.isLeft();
	
			// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		if(!is_start(hand))
			return;

		const FingerList testFingers = hand.fingers();
		int fingerNumbersY[arr_size];
		int fingerNumbersX[arr_size];
		int fingerNumbersZ[arr_size];
		int cnt = 0;
		for (FingerList::const_iterator fl = testFingers.begin(); fl != testFingers.end(); ++fl) 
		{
			const Finger finger = *fl;
			fingerNumbersZ[cnt] = finger.tipPosition()[2];
			fingerNumbersY[cnt] = finger.tipPosition()[1];
			fingerNumbersX[cnt] = finger.tipPosition()[0];
			cnt++;
		}
		
	
		int row = findRow(fingerNumbersZ, arr_size);

		int index = 0;
		if (!hand.isLeft())
			index = 1;
		int minIndex = get_min(fingerNumbersY, arr_size);

		// onClick()
		if (minIndex >= 0)

			lastKey = onClick(hand, minIndex, index, fingerNumbersZ);
		else
		{	
			if (lastKey != LAST_KEY_NONE)
			{
				onRelease(lastKey);
				lastKey = LAST_KEY_NONE;
			}
			last_pressed[index] = '!';
		}
		print_scr(row);


		
		SDL_RenderPresent(keyboard.ren);
	}
}

void KeyListener::onRow(int handIndex, int row)
{
	keyboard.DrawHandPosition(handIndex, row);
}

void KeyListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void KeyListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void KeyListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void KeyListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void KeyListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}
