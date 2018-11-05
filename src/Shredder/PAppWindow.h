#ifndef _PAPPW_H
#define _PAPPW_H

#include <Window.h>
#include <StringView.h>
#include <Application.h>
#include <Button.h>
#include <Slider.h>
#include <File.h>
#include <Roster.h>
#include <Entry.h>
#include <stdlib.h>
#include <Path.h>
#include <File.h>
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <StringView.h>
#include <Alert.h>
#include <View.h>
#include <InterfaceDefs.h>
#include <CheckBox.h>

class PAppWindow
	: public BWindow
{
public:
	PAppWindow();
	virtual bool QuitRequested();
	virtual void SPrefs(void);
	virtual void MessageReceived(BMessage *message);
private:
	void UpdateNumerOfIterations(uint32 iterations);
	BSlider *slider;
	BButton * savebutton;
	BButton * cancelbutton;	
	BCheckBox * checkBox;
	BCheckBox * checkBox2;
};

#endif
