#include <Application.h>
#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
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

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PAppWindow"

PAppWindow::PAppWindow()
	: BWindow(BRect(64,64,384,220), B_TRANSLATE("Shredder preferences"), B_TITLED_WINDOW,B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE|B_AUTO_UPDATE_SIZE_LIMITS)
{
	slider = new BSlider("const:slider1", "", new BMessage(SLIDER_CHANGE), 0, 24, B_HORIZONTAL);
	slider->SetModificationMessage(new BMessage(SLIDER_CHANGE));
	slider->SetHashMarks(B_HASH_MARKS_BOTTOM); 
	slider->SetHashMarkCount(25);
	slider->SetLimitLabels(B_TRANSLATE("0 - OFF"), B_TRANSLATE("24"));

	savebutton = new BButton("button1", B_TRANSLATE("Save"), new BMessage(BTN_SAVE));
	cancelbutton = new BButton("button2", B_TRANSLATE("Cancel"), new BMessage(BTN_CANCEL));
	checkBox = new BCheckBox("check", B_TRANSLATE("Confirm shredding"), NULL);
	checkBox2 = new BCheckBox("check", B_TRANSLATE("Show status window"), NULL);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(slider)
		.AddGroup(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
			.Add(checkBox)
			.Add(checkBox2)
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(cancelbutton)
			.Add(savebutton);

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
		slider->SetValue(atoi(iterations));
		UpdateNumerOfIterations(slider->Value());

		free(iterations);
	}
	
	Show();
}

void PAppWindow::SPrefs(void)
{
		BString tempString;
		BFile	file;

		if (file.SetTo("/boot/home/config/settings/Shredder.conf", B_READ_WRITE | B_CREATE_FILE) == B_OK) {
			if (slider->Value() < 10)
				tempString << "0" << slider->Value();
			else
				tempString << slider->Value();
				 
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
			UpdateNumerOfIterations(slider->Value());
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

void PAppWindow::UpdateNumerOfIterations(uint32 iterations)
{
	BString stringLabel = B_TRANSLATE("Number of iterations: %iterations%");
	BString stringIterations;
	stringIterations << iterations;
	stringLabel.ReplaceFirst("%iterations%", stringIterations);
	slider->SetLabel(stringLabel);
}

bool PAppWindow::QuitRequested()
{
	Quit();
	return 0;
}
