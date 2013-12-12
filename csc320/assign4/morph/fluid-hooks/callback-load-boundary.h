
/* Define the processDrawingCommands() function. */
void processBoundary(char* progname, char* filename, ScissorUI& window);
  
int OK=0;

char *fname;

/* Pop up a file chooser for selecting an input drawing file. */
fname = fl_file_chooser("Open Boundary...", "*.cmd", "");
if (fname == NULL){
	OK = 0;
}else{
	OK = 1;

	/* Clear out the scene objects. */
	imagePanel->clearScene();

	/* Insert objects from file named fname into the scene. */
	processBoundary("bob", fname, *this);

	/* Redraw the scene. */
	imagePanel->redraw();
}

/* If user did not select a valid drawing file print an error message. */
if(!OK){
	fl_alert("Error opening drawing!");
}
