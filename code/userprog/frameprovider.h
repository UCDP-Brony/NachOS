#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "bitmap.h"

class FrameProvider{
private:
	BitMap *bMap;
public:
	FrameProvider(int);
	~FrameProvider();
	unsigned int GetEmptyFrame();
	void ReleaseFrame(unsigned int);
	int NumAvailFrame();
};


#endif //FRAMEPROVIDER_H