#include <Application.h>
#include <Catalog.h>
#include <stdio.h>
#include <Alert.h>
#include <String.h>
#include <Roster.h>
#include <Entry.h>
#include <stdlib.h>
#include <Path.h>
#include <File.h>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

#include "PAppWindow.h"
#include "PStatWindow.h"
#include "ShredderApp.h"
#include "constants.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ShredderApp"

ShredderApp::ShredderApp()
	: BApplication(APP_SIGNATURE)
{
	ref_num = 0;
}


void ShredderApp::ReadyToRun()
{
	if (ref_num < 1) {
		prefsWindow = new PAppWindow;
	} else {
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
}

void ShredderApp::RefsReceived(BMessage *message)
{
	entry_ref 	ref;		// The entry_ref to open
	status_t 	err;		// The error code
	//int32		ref_num;	// The index into the ref list
	int32 		buttonIndex= 0; 
	BFile 		file;
	char		confirm = 'y';
	char		showStatus = 'y';
	char		*iterations;


	// Get the config
	if (file.SetTo("/boot/home/config/settings/Shredder.conf", B_READ_ONLY) == B_OK) {
		off_t length;
		file.GetSize(&length);		// Get the file length;
		iterations = (char *) malloc(length);
		file.Read(iterations, length);
		confirm = iterations[2];
		showStatus = iterations[3];	
		free(iterations);		
	}		
	// Got the config

	if (confirm != 'n')
	{
		BAlert *shredAlert = new BAlert(B_TRANSLATE_SYSTEM_NAME("Shredder"),
			B_TRANSLATE("Are you sure you want to continue shredding?"),
			B_TRANSLATE("Yes"),B_TRANSLATE("Cancel"));
		shredAlert->SetShortcut(1, B_ESCAPE);
		buttonIndex = shredAlert->Go();
	}	

	
	if (buttonIndex==1) {
		be_app->PostMessage(B_QUIT_REQUESTED);
		return;
	}
	
	// Loop through the ref list and open each one
	ref_num = 0;
	do {
		if ((err = message->FindRef("refs", ref_num, &ref)) != B_OK) {
			return;
		}
		statWindow = new PStatWindow(ref, message, showStatus);
		ref_num++;
	} while (1);
}


int main()
{
	ShredderApp app;
	app.Run();
	return 0;
}
