#ifndef _SHREDAPP_H
#define _SHREDAPP_H

#include <Application.h>

class ShredderApp
	: public BApplication
{
public:
	ShredderApp();
	virtual void RefsReceived(BMessage *message);
	virtual void ReadyToRun();
	int32 ref_num;
private:
	PAppWindow * prefsWindow;
	PStatWindow * statWindow;
};

#endif