// Callback specifying what happens when a user chooses
// "File/Open Image" from the UI menu 

{
  int OK=0;
  VAttrList attrs1;
  VImage *im1;
  FILE *fd;
  char *fname;

  fname = fl_file_chooser("Open Image...", "*.v", "");
  if (fname == NULL)
	  OK = 0;
  else {
#ifdef _WIN32
	  fd = fopen(fname, "rb");
#else
	  fd = fopen(fname, "r");
#endif
	  if (fd)
		if (VReadImages(fd, &attrs1, &im1)) 
			OK = 1;
  }
  if (!OK)
	 fl_alert("VReadImages: Error opening image file. Image(s) not displayed.");
  else {
     if (im != NULL) 
           VDestroyImage(im[0]);
     if (attrs != NULL)
	       VDestroyAttrList(attrs);
	 im = im1;
	 attrs = attrs1;
	 set(im[0]);
  
	imagePanel->computeLinkCosts(*im1);

  }

}
