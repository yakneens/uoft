// WARNING: DO NOT MODIFY THIS FILE EXECPT WHERE EXPLICITLY NOTED

#include "../stdafx.h"
#include <stdlib.h>
#include <iostream.h>
#include <stdio.h>
#include <vista/file.h>
#include <vista/VImage.h>
#include <vista/option.h>
#include <vista/mu.h>
#include "../ScissorUI.h"
#include "point.h"
#include "line.h"
#include "rectangle.h"
#include "boundary.h"
#include "circle.h"
#include "polygon.h"

// Routine for reading and executing drawing commands from a file
void processDrawingCommands(char* progname, char* filename, ScissorUI& window)
{
	FILE *cmdfile;
	char cmd;

	if((cmdfile = fopen(filename, "rwb")) == NULL)
		cerr << progname << ": Error opening file " 
			<< filename << "\n";
	else
		while ((cmd = fgetc(cmdfile)) != EOF) {
			switch (cmd) {

/////////// ADD YOUR CODE BETWEEN THESE TWO LINES ///////////////////
			
			/* Reading a rectangle. */
			case 'r':
			{
				int r1, c1, r2, c2;
				
				/* Read in coordinates of the rectangle and insert rectangle into the scene. */
				fscanf(cmdfile, "%d %d %d %d", &r1, &c1, &r2, &c2);
				window.imagePanel->sceneObjectsPushBack(new Rect(r1, c1, r2, c2, window.imagePanel));
				break;
			}
			/* Reading a circle. */
			case 'c':
			{
				int c1,c2,r;
				
				/* Read in coordinates of the circle and insert circle into the scene. */
				fscanf(cmdfile, "%d %d %d", &c1, &c2, &r);
				window.imagePanel->sceneObjectsPushBack(new Circle(c1, c2, c1 + r, c2, window.imagePanel));
				break;
			}
			/* Reading a line. */
			case 'l':
			{
				int x1, x2, y1, y2;
				
				/* Read in coordinates of the line and insert line into the scene. */
				fscanf(cmdfile, "%d %d %d %d", &x1, &y1, &x2, &y2);
				window.imagePanel->sceneObjectsPushBack(new Line(x1, y1, x2, y2, window.imagePanel));
				break;
			}
			/* Reading a color. */
			case 'C':
			{
				int r, g, b;

				/* Read in r,g,b values of the color and insert color into the scene. */
				fscanf(cmdfile, "%d %d %d", &r, &g, &b);
				window.imagePanel->sceneObjectsPushBack(new Color(r, g, b));
				break;
			}
			/* Reading a point. */
			case 'P':
			{
				int x,y;
				
				/* Read in coordinates of the point and insert point into the scene. */
				fscanf(cmdfile, "%d %d", &x, &y);
				window.imagePanel->sceneObjectsPushBack(new Point(x, y, window.imagePanel));
				
				break;
			}
			/* Reading a polygon. */
			case 'p':
			{
				int x,y;
				char delim;
				bool first = true;
				Poly *myPoly;
				
				/* While not at the end of the line. */
				while((delim = fgetc(cmdfile)) != '\n'){

/* scanf on windows reads in a space as an integer
   so need to leave out this line. */
#ifndef WIN32
					fputc(delim, cmdfile);
#endif

					/* Read in coordinates of the current vertex. */
					fscanf(cmdfile, "%d %d", &x, &y);
					
					/* If this is the first vertex. Create a new
					   polygon and add it to the scene. */
					if(first){
						myPoly = new Poly(x, y, window.imagePanel);
						window.imagePanel->sceneObjectsPushBack(myPoly);
						first = false;
					/* If this is not the first vertex, add this vertex
					   to the polygon. */
					}else{
						myPoly->addPoint(x,y);
					
					}
				}

				break;
			}
			default:
				break;
			}
 
//////////////////////////////////////////////////////////////////////

		}
}

void processSeedPoints(char* progname, char* filename, ScissorUI& window)
{
	FILE *cmdfile;
	char cmd;
	bool first = true;
	Boundary *boundary;

	if((cmdfile = fopen(filename, "rwb")) == NULL){
		cerr << progname << ": Error opening file " 
			<< filename << "\n";
	}else{
		while ((cmd = fgetc(cmdfile)) != EOF) {
			switch (cmd) {
				
				/* Reading a point. */
				case 'P':
				{
					int x,y;
					
					/* Read in coordinates of the point and insert point into the scene. */
					fscanf(cmdfile, "%d %d", &x, &y);
					
					if(first){
						boundary = new Boundary(y,x, window.imagePanel);
						first = false;
					}else{
						boundary->addSeedPoint(y, x);
					}

					
					
					break;
				}
	
			}
		}
	//boundary->completeBoundary();

	window.imagePanel->sceneObjectsPushBack(boundary);
	}
}

void processBoundary(char* progname, char* filename, ScissorUI& window){
	string s = window.imagePanel->writeBoundaryVertices();

	FILE *fp;
	
	/* If a file has been successfully chosen, write the boundary
	to the file. */
	fp = fopen(filename, "w");
	fputs(s.c_str(), fp);
	fflush(fp);
	fclose(fp);
}

void processSpline(char* progname, char* filename, ScissorUI& window){
	FILE *cmdfile;
	char cmd;
	bool first = true;
	
	if((cmdfile = fopen(filename, "rwb")) == NULL){
		cerr << progname << ": Error opening file " 
			<< filename << "\n";
	}else{
		window.imagePanel->deleteSpline();
		while ((cmd = fgetc(cmdfile)) != EOF) {
			switch (cmd) {
				
				/* Reading a point. */
				case 'P':
				{
					int x,y;
					
					/* Read in coordinates of the point and insert point into the scene. */
					fscanf(cmdfile, "%d %d", &x, &y);
					
					window.imagePanel->addSplineKnot(x, y);
					break;
				}
	
			}
		}
	
		window.imagePanel->completeSpline();
	}
}

void reduceSpline(char* progname, int level, ScissorUI& window){
	window.imagePanel->reduceSplineTo(level);
}

void saveSpline(char* progname, char* filename, ScissorUI& window){
	window.imagePanel->writeSplineToFile(filename);
}

void processMatte(char* progname, char* filename, ScissorUI& window){
	window.imagePanel->writeMatteToFile(filename);
}

void processGrad(char* progname, char* filename, ScissorUI& window){
	window.imagePanel->writeGradToFile(filename);
}

void processZeroCross(char* progname, char* filename, ScissorUI& window){
	window.imagePanel->writeZeroCrossToFile(filename);
}

void processLinkCosts(char* progname, char* filename, ScissorUI& window){
	window.imagePanel->writeLinkCostsToFile(filename);
}

/* Process the morph specified by src_fname, dst_fname, src_cmd_fname. dst_cmd_fname, morph_bas_name,'
frames. Calls the prepMorph() function of the current ImDraw instance. */
void processMorph(char* progname, char* src_fname, char* dst_fname, char* src_cmd_fname, 
				  char* dst_cmd_fname, char* morph_base_fname, int frames, ScissorUI& window){
	window.imagePanel->prepMorph(src_fname, dst_fname, src_cmd_fname, dst_cmd_fname, morph_base_fname, frames);
}