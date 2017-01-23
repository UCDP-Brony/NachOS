#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "bitmap.h"


//----------------------------------------------------//
//					  FrameProvider 				  //
//	A utility class that manages the physical frames. //
//  This class is part of the virtual memory system.  //
//----------------------------------------------------//

class FrameProvider{
private:
	BitMap *bMap;		//The real frame manager
public:
	FrameProvider(int);
	~FrameProvider();
	unsigned int GetEmptyFrame();		// returns the id of an empty physical frame.
	void ReleaseFrame(unsigned int);	// releases the physical frame, it can be allocated again.
	int NumAvailFrame();				// returns the amount of available frames.
	void setFrameRandom(bool);			// Sets the allocation politic (true for random).
};


#endif //FRAMEPROVIDER_H