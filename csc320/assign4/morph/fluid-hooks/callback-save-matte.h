// Callback specifying what happens when a user chooses
// "File/Save Drawing" from the UI menu 

#include <errno.h>

{
	// This menu choice should allow the user to
	// save all interactively-performed drawing 
	// operations into a file with extension .cmd
	// It should pop up a dialog box for choosing 
	// the filename where the commands will be saved,
	// just like in callback-open-image.h

	// The format of this file should be IDENTICAL
	// to the format of the file read by the
	// ProcessArgs() routine in main.cpp
	// 
	// Once such a file is saved, it should be possible
	// to re-load the file (and thus execute all previously-
	// executed drawing commands) by including it as a
	// command-line option or by loading it via the menu
	//
	// In particular, if you perform the following:
	//    1. create a file with drawing commands called qq.cmd
	//    2. run your program with the option -cmd qq.cmd
	//    3. choose 'Save Drawing' from the menu
	//    4. save your drawing into a file qq1.cmd
	// the files qq.cmd and qq1.cmd should be identical
	int OK=0;
	char *fname;

	/* Pop up a file chooser for selecting a file to save the scene to. */
	fname = fl_file_chooser("Save Alpha Matte to file...", "*.v", "");
	if (fname == NULL){
		OK = 0;
	}else{
		imagePanel->writeMatteToFile(fname);
	}

}
