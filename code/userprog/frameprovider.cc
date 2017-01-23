#include "frameprovider.h"
#include "system.h"
#include "syscall.h"

bool frameRandom;

FrameProvider::FrameProvider(int nbFrames){
	bMap = new BitMap(nbFrames);
	frameRandom = false;
}

FrameProvider::~FrameProvider(){
	delete bMap;
}

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

void FrameProvider::ReleaseFrame(unsigned int frame){
	bMap->Clear(frame);
}

int FrameProvider::NumAvailFrame(){
	return bMap->NumClear();
}

//possiblity to enable/disable random mode
void setFrameRandom(bool b){
	frameRandom = b;
}