#include <stdio.h>
#include <Alert.h>
#include <Message.h>
#include <String.h>
#include <Node.h>
#include <Roster.h>
#include <Entry.h>
#include <stdlib.h>
#include <Path.h>
#include <File.h>
#include <fstream>
#include <StorageKit.h>
#include <SupportKit.h>
#include <AppKit.h>
#include <TrackerAddOn.h>
#include <Application.h>

#include "PAppWindow.h"
#include "PStatWindow.h"

void
process_refs(entry_ref ref, BMessage* msg, void* reserved)
{
	int32 	buttonIndex= 0; 
	int32	prefsset=0;

	BFile 		file;
	char		confirm = 'y';
	char		showStatus = 'y';
	char		*iterations;

	int32 entryCount = 0;
	entry_ref fref;


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

	
	if ((confirm != 'n') && (msg->FindRef("refs", 0, &fref) == B_OK)) {
		// ok or not!
		BAlert *shredAlert;
		BString alertText;
		alertText.SetTo("Are you sure you want to continue shredding?");
		shredAlert = new BAlert("shredalert",alertText.String(), "Yes","OpenPrefs","Cancel"); //,
		// B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT);
		shredAlert->SetShortcut(2, B_ESCAPE);

		buttonIndex = shredAlert->Go();
	}	

	if (buttonIndex==2) {
		return;
	}
	
	if (buttonIndex==1) {
		PAppWindow * prefsWindow = new PAppWindow(/*ref, msg*/);
	
		thread_id thread = prefsWindow->Thread();
		//wait for the window thread to return
		status_t win_status = B_OK;
		wait_for_thread(thread, &win_status);
		prefsset=1;
	}

	if ((prefsset==0) && (msg->FindRef("refs", 0, &fref) == B_OK)) {

		for(int32 count = 0 ; msg->FindRef("refs", count, &fref) == B_OK ; count++) {
			PStatWindow * statWindow = new PStatWindow(fref, msg, showStatus);
			thread_id thread = statWindow->Thread();
			status_t win_status = B_OK;	
			wait_for_thread(thread, &win_status);

			entryCount++;
		}
	}

	if ((entryCount == 0) && (prefsset==0)) {
		PAppWindow * prefsWindow = new PAppWindow(/*ref, msg*/);
		thread_id thread = prefsWindow->Thread();
		//wait for the window thread to return
		status_t win_status = B_OK;
		wait_for_thread(thread, &win_status);
	}

}

int
main(int /*argc*/, char **/*argv*/)
{
	fprintf(stderr, "This can only be used as a Tracker add-on.\n");
	return -1; 
}
