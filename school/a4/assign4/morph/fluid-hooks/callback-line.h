/* Set the drawing state. */
imagePanel->setDrawingState(D_LINE);

/* Check if there is an unfinished polygon in the scene.
   If so, complete the polygon. */
imagePanel->checkForPoly();
levelOfDetail->deactivate();