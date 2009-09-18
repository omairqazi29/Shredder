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
#include "constants.h"

#include "PAppWindow.h"

PAppWindow::PAppWindow()
	: BWindow(BRect(64,64,384,220), "Shredder Preferences", B_TITLED_WINDOW,B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE)
{
	
	BView * MainView;
	MainView = new BView(Bounds(),"view",B_FOLLOW_ALL, 0);
	MainView->SetViewColor(ui_color (B_PANEL_BACKGROUND_COLOR));
	AddChild(MainView);
	
	slider = new BSlider(BRect(5,5,315,55), "const:slider1", "Number of Iterations", new BMessage(SLIDER_CHANGE), 0, 24);
	slider->SetHashMarks(B_HASH_MARKS_BOTTOM); 
	slider->SetHashMarkCount(25);
	slider->SetLimitLabels("0 - OFF","24");
	MainView->AddChild(slider);

	sliderStatusLabel = new BStringView(BRect(90,60,220,80), "sliderstatuslabel", "Number of Iterations:", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	sliderStatusLabel->SetViewColor(ui_color (B_PANEL_BACKGROUND_COLOR));	
	AddChild(sliderStatusLabel);

	sliderStatus = new BStringView(BRect(230,60,260,80), "sliderstatus", "0", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	sliderStatus->SetViewColor(ui_color (B_PANEL_BACKGROUND_COLOR));	
	AddChild(sliderStatus);

	savebutton = new BButton(BRect(20,120,100,120), "button1", "Save", new BMessage(BTN_SAVE), B_FOLLOW_LEFT | B_FOLLOW_TOP);
	MainView->AddChild(savebutton);
	
	cancelbutton = new BButton(BRect(120,120,200,120), "button2", "Cancel", new BMessage(BTN_CANCEL), B_FOLLOW_LEFT | B_FOLLOW_TOP);
	MainView->AddChild(cancelbutton);

	checkBox = new BCheckBox(BRect(10,90,159,100), "check", "Confirm Shredding", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	MainView->AddChild(checkBox);

	checkBox2 = new BCheckBox(BRect(160,90,380,100), "check", "Show Status Window", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	MainView->AddChild(checkBox2);


	// get postion
	BFile	file;
	BString temp;
	BString temp2;

	if (file.SetTo("/boot/home/config/settings/Shredder.conf", B_READ_WRITE) == B_OK) {
		char * iterations;	
		off_t length;
		file.GetSize(&length);		// Get the file length;

		iterations = (char *) malloc(length);
		file.Read(iterations, length);

		temp = iterations[2];
		temp2 = iterations[3];

		iterations[2] = '\0';

		if (temp == "n")
			checkBox->SetValue(false);
		else
			checkBox->SetValue(true);

		if (temp2 == "n")
			checkBox2->SetValue(false);
		else
			checkBox2->SetValue(true);


		// set status label
		float pos = (atof(iterations))/24;			

		slider->SetPosition(pos);
		temp = "";
		temp << (int)(slider->Position() * 24);	
		sliderStatus->SetText(temp.String());

		free(iterations);
	}
	
	Show();
}

void PAppWindow::SPrefs(void)
{
		BString tempString;
		BFile	file;

		if (file.SetTo("/boot/home/config/settings/Shredder.conf", B_READ_WRITE | B_CREATE_FILE) == B_OK) {
	
			if ((int)(slider->Position() * 24) < 10)
				tempString << "0" << (int)(slider->Position() * 24);
			else
				tempString << (int)(slider->Position() * 24);
				 
			if (checkBox->Value() == 1)
				tempString << "y";
			else
				tempString << "n";
			
			if (checkBox2->Value() == 1)
				tempString << "y";
			else
				tempString << "n";

			file.Write(tempString.String(), sizeof(tempString.String()));
		}
}

void PAppWindow::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case SLIDER_CHANGE:
			char temp2[2];
			sprintf(temp2, "%d", (int)(slider->Position() * 24));
			sliderStatus->SetText(temp2);
			break;
		case BTN_SAVE:
			SPrefs();
			Quit();
			break;
		case BTN_CANCEL:
			Quit();
			break;
		default:
		BWindow::MessageReceived(message);
		break;
	}
}

bool PAppWindow::QuitRequested()
{
	Quit();
	return 0;
}
