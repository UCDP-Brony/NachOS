#include "frameprovider.h"
#include "system.h"
#include "syscall.h"

bool frameRandom;


//----------------------------------------------------------------------
// Constructor
//		Initialises the BitMap to track the physical frames.
//----------------------------------------------------------------------
FrameProvider::FrameProvider(int nbFrames){
	bMap = new BitMap(nbFrames);
	frameRandom = false;
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
FrameProvider::~FrameProvider(){
	delete bMap;
}

//----------------------------------------------------------------------
// GetEmptyFrames
//		returns a free frame. If frameRandom is true, it will return 
//		a random frame. Marks the frame as used.
//----------------------------------------------------------------------
unsigned int FrameProvider::GetEmptyFrame(){
	unsigned int frameIndex;
	if(frameRandom){ 
		//random
		frameIndex = bMap->RandomFind();
	}
	else 
	{
		//vanilla
		frameIndex = bMap->Find();
	}
	if((int)frameIndex != -1){
		bzero( machine->mainMemory + (frameIndex*PageSize), PageSize);
	}
	return frameIndex;
}

//----------------------------------------------------------------------
// ReleaseFrame (int frame)
//		Marks the frame "frame" as free.
//----------------------------------------------------------------------
void FrameProvider::ReleaseFrame(unsigned int frame){
	bMap->Clear(frame);
}

//----------------------------------------------------------------------
// NumAvailFrame
//		Returns the amount of free frames.
//----------------------------------------------------------------------
int FrameProvider::NumAvailFrame(){
	return bMap->NumClear();
}

//----------------------------------------------------------------------
// setFrameRandom (bool b)
//		Sets the allocation politics. True for randomness.
//----------------------------------------------------------------------
void setFrameRandom(bool b){
	frameRandom = b;
}