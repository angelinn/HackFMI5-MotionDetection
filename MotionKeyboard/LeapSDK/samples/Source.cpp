/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <string.h>
#include "Leap.h"
#include <cmath>

using namespace Leap;

int seconds = 0;
int cnt_A = 0;
const int arr_size = 5;
const char  letts_left[3][5]  = { { 'z', 'x' , 'c' , 'v' , 'b' } , { 'a' , 's', 'd', 'f', 'g'} , { 'q', 'w', 'd', 'e', 'r'} };   
int last_row = 0;

class SampleListener : public Listener {
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

  private:
	  char getLetter(int arr[], int size, int row);
    int findRow ( int arr[] , int size);
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

char SampleListener::getLetter(int arr[] , int size, int row ) { 
	/*

	std::cout <<"th" << arr[0] << std::endl;
	std::cout << "ind" << arr[1] << std::endl;
	std::cout << "mid"<< arr[2] << std::endl;
	std::cout << "ring" << arr[3] << std::endl;
	std::cout << "pinky" << arr[4] << std::endl;

	*/
	int f_pos = -1;
	int min = arr[0];
	int min_index = 0;
	for(int i = 1; i < size; i++) { 
		if( min > arr[i] ) {
			min = arr[i];
			min_index = i;
			
		}
	}
	//std::cout << min_index << std::endl;
	//std::cout << min << std::endl;
	if( min_index + 1 == size ) {
		if(arr[min_index - 1] - arr[min_index] > 30) {
      
      return letts_left[row][min_index];
       
		}
	}
	else{
		if(arr[min_index +1] - arr[min_index] > 30) {

      return letts_left[row][min_index];

		}

	}
	return NULL;
}


int SampleListener::findRow(int arr[], int size){


/*
  int sum = 0;

  for( int i = 0 ; i < size; i++){
    sum = sum + arr[i];
  }

  int avg = sum / size;

  */

  if(arr[2]  < -60){
    return 2;
  }
  else if(arr[2] >  -20 ){
    return 0;
  }
  else{
    return 1;
  }

}

void SampleListener::onFrame(const Controller& controller) {
	seconds ++;
	seconds = seconds % 20;
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  /*
  std::cout << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count();
			*/
  HandList hands = frame.hands();
  
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    bool is_left = hand.isLeft()
	/*
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
    std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
              << ", palm position: " << hand.palmPosition() << std::endl;
			  */
    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();
	
	/*
    // Calculate the hand's pitch, roll, and yaw angles
    std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
	
    // Get the Arm bone
    Arm arm = hand.arm();
    std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
              << " wrist position: " << arm.wristPosition()
              << " elbow position: " << arm.elbowPosition() << std::endl;
		*/	  
    // Get fingers
	/*
    const FingerList fingers = hand.fingers();

    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
      const Finger finger = *fl;
      std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
                << ", length: " << finger.length()
                //<< "mm, width: " << finger.width() 
				<<"XXXXXXXXXXXXX"<<finger.tipPosition()<< std::endl;
	}}

	*/
  // test code

  const FingerList testFingers = hand.fingers();
  int fingerNumbersY[arr_size];
  int fingerNumbersX[arr_size];
  int fingerNumbersZ[arr_size];
  int cnt = 0;
  for (FingerList::const_iterator fl = testFingers.begin(); fl != testFingers.end(); ++fl) {
    const Finger finger = *fl;
    fingerNumbersZ[cnt] = finger.tipPosition()[2];
    fingerNumbersY[cnt] = finger.tipPosition()[1];
    fingerNumbersX[cnt] = finger.tipPosition()[0];
    cnt++;
  }
  
	char letter_pressed;

	if(is_left)
  {
      letter_pressed = getLetter( , arr_size)
  }
  else
  {

  }

	
	
/*	std::cout <<"thX " << fingerNumbersX[0] << std::endl;
	std::cout << "indX"  << fingerNumbersX[1] << std::endl;
	std::cout << "midX "<< fingerNumbersX[2] << std::endl;
	std::cout << "ringX " << fingerNumbersX[3] << std::endl;
	std::cout << "pinkyX " << fingerNumbersX[4] << std::endl;

  std::cout <<"thY " << fingerNumbersY[0] << std::endl;
  std::cout << "indY " << fingerNumbersY[1] << std::endl;
  std::cout << "midY "<< fingerNumbersY[2] << std::endl;
  std::cout << "ringY " << fingerNumbersY[3] << std::endl;
  std::cout << "pinkyY " << fingerNumbersY[4] << std::endl;


  std::cout <<"thZ " << fingerNumbersZ[0] << std::endl;
  std::cout << "indZ " << fingerNumbersZ[1] << std::endl;
  std::cout << "midZ "<< fingerNumbersZ[2] << std::endl;
  std::cout << "ringZ " << fingerNumbersZ[3] << std::endl;
  std::cout << "pinkyZ " << fingerNumbersZ[4] << std::endl;*/
	
    int row = findRow(fingerNumbersZ, arr_size);
		if(seconds != 0) {
			if(last_row != row){
        last_row = row;
      }
      else{
        return;
      }
        return;
		}

		if(getLetter(fingerNumbersY , arr_size, last_row ) == NULL) {
			return;
		}

		std::cout << getLetter(fingerNumbersY , arr_size, last_row );



  }

	/*
      // Get finger bones
      for (int b = 0; b < 4; ++b) {
        Bone::Type boneType = static_cast<Bone::Type>(b);
        Bone bone = finger.bone(boneType);
        std::cout << std::string(6, ' ') <<  boneNames[boneType]
                  << " bone, start: " << bone.prevJoint()
                  << ", end: " << bone.nextJoint()
                  << ", direction: " << bone.direction() << std::endl;
      }
    }
  }
  /*
  // Get tools
  const ToolList tools = frame.tools();
  for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
    const Tool tool = *tl;
    std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
              << ", position: " << tool.tipPosition()
              << ", direction: " << tool.direction() << std::endl;
  }
  
  // Get gestures
  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];
	
    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        std::string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        std::cout << std::string(2, ' ')
                  << "Circle id: " << gesture.id()
                  << ", state: " << stateNames[gesture.state()]
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness << std::endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        std::cout << std::string(2, ' ')
          << "Swipe id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() << std::endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        std::cout << std::string(2, ' ')
          << "Key Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<< std::endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        std::cout << std::string(2, ' ')
          << "Screen Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << screentap.position()
          << ", direction: " << screentap.direction()<< std::endl;
        break;
      }
      default:
        std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
        break;
    }
  }
  
  //if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    //std::cout << std::endl;
  //}
}*/
}
void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
