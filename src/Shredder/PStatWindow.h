#ifndef _PSTATW_H
#define _PSTATW_H

#include <Window.h>
#include <StringView.h>
#include <String.h>

class PStatWindow
	: public BWindow
{
public:
	PStatWindow(entry_ref ref, BMessage *msg, char showStat);
	virtual void Shred(BDirectory dir);
private:
	BStringView * shreddingThisFile;
	BString	itpass;
};

#endif