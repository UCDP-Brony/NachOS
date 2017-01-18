#include "frameprovider.h"
#include "system.h"

FrameProvider::FrameProvider(int nbFrames){
	bMap = new BitMap(nbFrames);
}

FrameProvider::~FrameProvider(){
	delete bMap;
}

unsigned int FrameProvider::GetEmptyFrame(){
	unsigned int frameIndex = bMap->Find();
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