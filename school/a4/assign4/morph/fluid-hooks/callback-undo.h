/* If there are objects in the scene. */
if(!imagePanel->sceneIsEmpty()){

	/* Remove the most recently added object. */
	imagePanel->sceneObjectsUndo();

	/* Redraw the scene. */
	imagePanel->redraw();
}
