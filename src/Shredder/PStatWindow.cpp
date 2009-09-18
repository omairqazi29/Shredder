#include <Button.h>
#include <Slider.h>
#include <File.h>
#include <Roster.h>
#include <Entry.h>
#include <Path.h>
#include <File.h>
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <StringView.h>
#include <Alert.h>
#include <View.h>
#include <InterfaceDefs.h>
#include <Alert.h>
#include <Directory.h>
#include <Node.h>
#include "constants.h"

#include "PStatWindow.h"
#include "PAppWindow.h"

PStatWindow::PStatWindow(entry_ref ref, BMessage *msg, char showStat)
	: BWindow(BRect(64,64,600,100), "Shredding File:", B_TITLED_WINDOW,B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_CLOSABLE|B_NOT_MINIMIZABLE)
{
	BFile	file;
	char	*iterations;


	BView * MainView;
	MainView = new BView(Bounds(),"view",B_FOLLOW_ALL, 0);
	MainView->SetViewColor(ui_color (B_PANEL_BACKGROUND_COLOR));
	AddChild(MainView);

	shreddingThisFile = new BStringView(BRect(20,10,580,30), "name", "", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	shreddingThisFile->SetViewColor(ui_color (B_PANEL_BACKGROUND_COLOR));
	AddChild(shreddingThisFile);

	if (showStat != 'n') {
		Show();
	}

	// Get the config
	if (file.SetTo("/boot/home/config/settings/Shredder.conf", B_READ_ONLY) == B_OK) {
		off_t length;
		file.GetSize(&length);		// Get the file length;
		iterations = (char *) malloc(length);
		file.Read(iterations, length);
		iterations[2] = '\0';
		itpass = iterations;
		free(iterations);		
	} else {
			itpass = "10";
	}		
	// Got the config

	// determin whether entry is a directory or not.
	// test if its a symlink, if so, delete it
	BDirectory directory1;

	BEntry entry;
	entry.SetTo(&ref, false);
	
	if (entry.IsSymLink() == true) {
		entry.Remove();
	} else if (directory1.SetTo(&ref) == B_OK) {
	
		Shred(directory1);
		
		BEntry entry(&ref, false);
		entry.Remove();
	} else {
		// shred the single file
		BPath path;
		entry.GetPath(&path);
		BString com = "shred -uxz -n";	
		com << itpass;
		com << " \"";
		com << path.Path();
		com << "\"";
	
		Lock();
		shreddingThisFile->SetText(path.Path());
		Unlock();
		system(com.String());
	}

	Quit();
}

void PStatWindow::Shred(BDirectory dir)
{
	entry_ref ref;
	BDirectory tempdir;
	BEntry entry;

	while(dir.GetNextRef(&ref) == B_OK) {
		// test if its a symlink, if so, delete it
		entry.SetTo(&ref, false);
	
		if (entry.IsSymLink() == true) {
			entry.Remove();
			continue;
		} else if (tempdir.SetTo(&ref) == B_OK) {
			BEntry entry(&ref, false);
			BPath path;
			entry.GetPath(&path);
			BString temps;
			temps << path.Path();

			Shred(tempdir);
			entry.Remove();			
		}
		// shredding the file
		BPath path;
		BEntry entry(&ref, false);

		entry.GetPath(&path);

		BString com = "shred -uxz -n";	
		com << itpass;
		com << " \"";
		com << path.Path();
		com << "\"";

		Lock();
		shreddingThisFile->SetText(path.Path());
		Unlock();

		system(com.String());
	}
} 