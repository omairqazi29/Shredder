#include <stdio.h>
#include <Alert.h>
#include <Catalog.h>
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

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ShredderAppAddon"

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
		BAlert *shredAlert = new BAlert(B_TRANSLATE_SYSTEM_NAME("Shredder"),
			B_TRANSLATE("Are you sure you want to continue shredding?"),
			B_TRANSLATE("Yes"), B_TRANSLATE("Preferences" B_UTF8_ELLIPSIS), B_TRANSLATE("Cancel"));
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
