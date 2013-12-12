/* Pop up a color chooser widget and 
   add the chosen color to scene objects. */
unsigned char r,g,b;
r = 255;
g = 255;
b = 255;
if(fl_color_chooser("",r,g,b) != 0){
	imagePanel->sceneObjectsPushBack(new Color(r,g,b));
}

levelOfDetail->deactivate();