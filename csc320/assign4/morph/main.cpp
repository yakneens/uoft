// main.cpp : Defines the entry point for the console application.
//
// This is the top-level function of an example program that displays a Vista
// image on an fltk OpenGL panel

#include "stdafx.h"
#include <stdlib.h>
#include <iostream.h>
#include <stdio.h>
#include <vista/file.h>
#include <vista/VImage.h>
#include <vista/option.h>
#include <vista/mu.h>
#include "ScissorUI.h"

// Routine for processing command-line arguments (defined below)
void processArgs(int* argc, char** argv, ScissorUI& window);
// Routine for processing the '-im' command line argument
void openImage(char* progname, char* filename, ScissorUI& window);
// Routine for processing the '-cmd' command line argument
void processDrawingCommands(char* progname, char* filename, 
			      ScissorUI& window);
//Routine for processing the -pts command line argument
void processSeedPoints(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -bdry command line argument
void processBoundary(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -matte command line argument
void processMatte(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -grad command line argument
void processGrad(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -zero command line argument
void processZeroCross(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -lcost command line argument
void processLinkCosts(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -bspl command line argument
void processSpline(char* progname, char* filename, ScissorUI& window);

//Routine for processing the -reduce command line argument
void reduceSpline(char* progname, int level, ScissorUI& window);

//Routine for processing the -bout command line argument
void saveSpline(char* progname, char* filename, ScissorUI& window);

//Routine for processing the morphing command line arguments
void processMorph(char* progname, char* src_fname, char* dst_fname, char* src_cmd_fname, 
				  char* dst_cmd_fname, char* morph_base_fname, int frames, ScissorUI& window);

// Program entry point
int main(int argc, char **argv){
	// Initialize fluid
	Fl::visual(FL_INDEX || FL_DOUBLE);

	// Create the UI windows for the interface
	ScissorUI *window = new ScissorUI;

	// Display the UI windows
	window->show();
	
	// Process command-line arguments (if any)  
	processArgs(&argc, argv, *window);

	return Fl::run();
}

// Routine for processing command-line arguments
void processArgs(int* argc, char** argv, ScissorUI& window){
	// Variables that will hold values of command-line arguments
	static VArgVector im_filenames, cmd_filenames,
		pts_filenames, matte_filenames, bdry_filenames,
		grad_filenames, zero_filenames, lcost_filenames,
		bspl_filenames, reduce_level, bout_filenames,
		mS1_filenames, mS2_filenames, msrcl_filenames,
		mdestl_filenames, frames_num, base_fname;
	static VBoolean im_found, cmd_found,
		pts_found, matte_found, bdry_found,
		grad_found, zero_found, lcost_found,
		bspl_found, reduce_found, bout_found,
		mS1_found, mS2_found, msrcl_found,
		mdestl_found, frames_found, base_found;

     // Below is a vista structure that specifies the valid 
     // command-line options for this program
     // The structure specifies that there is only one command-line 
     // argument, which is optional, and takes a list of vista images. 
     // Command-line invocation of the program will be:
     //     imdraw -in <image-1> <image-2> ...
     // Run 'man VParseCommand' and 'man Voption' for more information

	static VOptionDescRec options[] = {
		{ "im", VStringRepn, 0, & im_filenames, & im_found, NULL,
		"Image to display in OpenGL panel" },
		{ "cmd", VStringRepn, 0, & cmd_filenames, & cmd_found, NULL,
		"File that contains drawing commands" },
		{ "pts", VStringRepn, 0, &pts_filenames, &pts_found, NULL,
		"File that contains seed points for scissor" },
		{ "matte", VStringRepn, 0, &matte_filenames, &matte_found, NULL,
		"File that contains the image alpha matte" },
		{ "bdry", VStringRepn, 0, &bdry_filenames, &bdry_found, NULL,
		"File that contains the alpha matte boundary" },
		{"grad", VStringRepn, 0, &grad_filenames, &grad_found, NULL,
		"File that contains the image gradient"},
		{"zero", VStringRepn, 0, &zero_filenames, &zero_found, NULL,
		"File that contains the image gradient zero crossings"},
		{"lcost", VStringRepn, 0, &lcost_filenames, &lcost_found, NULL,
		"File that contains image link costs"},
		{"bspl", VStringRepn, 0, &bspl_filenames, &bspl_found, NULL,
		"File that contains image spline control points"},
		{"reduce", VDoubleRepn, 0, &reduce_level, &reduce_found, NULL,
		"Level to reduce to"},
		{"bout", VStringRepn, 0, &bout_filenames, &bout_found, NULL,
		"Control points of reduced spline"},
		{"mS1", VStringRepn, 0, &mS1_filenames, &mS1_found, NULL,
		"Morph source image"},
		{"mS2", VStringRepn, 0, &mS2_filenames, &mS2_found, NULL,
		"Morph destination image"},
		{"msrcl", VStringRepn, 0, &msrcl_filenames, &msrcl_found, NULL,
		"Source image control lines drawing"},
		{"mdestl", VStringRepn, 0, &mdestl_filenames, &mdestl_found, NULL,
		"Destination image control lines drawing"},
		{"frames", VDoubleRepn, 0, &frames_num, &frames_found, NULL,
		"Number of morph frames to generate"},
		{"mD", VStringRepn, 0, &base_fname, &base_found, NULL,
		"Base filename for morph frames"},
	};

	//
	// Parse the command line options
	//

	if (!VParseCommand(VNumber(options), options, argc, argv)) {
		// Parsing failed, so print a command usage string
		// This also happens if the '-help' option is given
		VReportUsage(argv[0], VNumber(options), options, NULL);
		exit(0);
	}

	// If there are any unrecognized command line arguments, return
	// the usage string
	if (*argc > 1) {
		// Print all unrecognized command-line arguments
		VReportBadArgs(*argc, argv);
		// Print a command usage string
		VReportUsage(argv[0], VNumber(options), options, NULL);
		exit(0);
	}

	//
	// Process the input arguments
	//

	// 
	//  1. Perform error checking and read and display the image (if supplied)
	//

	if(im_found){
		// We can only display one image at a time, so if more than one 
		// filename is supplied after '-im', we ignore all of them
		// except the first one
		if(im_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -im option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, read the image(s) it contains
		if(im_filenames.number > 0){
			char *fname;
			fname = ((char **) im_filenames.vector)[0];
			openImage(argv[0], fname, window);
		}
	}

	if(pts_found){
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(pts_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -pts option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if (pts_filenames.number > 0) {
			char *fname;
			fname = ((char **) pts_filenames.vector)[0];
			processSeedPoints(argv[0], fname, window);
		}
	}

	
	if(bdry_found){
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(bdry_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -bdry option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(bdry_filenames.number > 0){
			char *fname;
			fname = ((char **) bdry_filenames.vector)[0];
			processBoundary(argv[0], fname, window);
		}
	}

	if(matte_found){
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if (matte_filenames.number > 1) {
			cerr << argv[0] << ": Multiple filenames after -matte option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(matte_filenames.number > 0){
			char *fname;
			fname = ((char **) matte_filenames.vector)[0];
			processMatte(argv[0], fname, window);
		}
	}

	if(grad_found){
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(grad_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -grad option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(grad_filenames.number > 0){
			char *fname;
			fname = ((char **) grad_filenames.vector)[0];
			processGrad(argv[0], fname, window);
		}
	}

	if(zero_found){
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(zero_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -zero option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if (zero_filenames.number > 0) {
			char *fname;
			fname = ((char **) zero_filenames.vector)[0];
			processZeroCross(argv[0], fname, window);
		}
	}

	if(lcost_found){
		// We will only execute commands from one file at a time, so
		 // if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(lcost_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -lcost option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(lcost_filenames.number > 0){
			char *fname;
			fname = ((char **) lcost_filenames.vector)[0];
			processLinkCosts(argv[0], fname, window);
		}
	}
	
	if(bspl_found){
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(bspl_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -bspl option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(bspl_filenames.number > 0){
			char *fname;
			fname = ((char **) bspl_filenames.vector)[0];
			processSpline(argv[0], fname, window);
		
			if(reduce_found){
			// We will only execute commands from one file at a time, so
			// if more than one filename is supplied after '-cmd', we
			// ignore all of them except the first one
				if(reduce_level.number > 1){
					cerr << argv[0] << ": Multiple level after -reduce option: ignoring all but 1st\n";
					cerr.flush();
				}
				// If a filename was supplied, process the commands it contains
				if(reduce_level.number > 0){
					int level;
					level = (int)((double*)reduce_level.vector)[0];
					reduceSpline(argv[0], level, window);
				
					if(bout_found){
						// We will only execute commands from one file at a time, so
						 // if more than one filename is supplied after '-cmd', we
						// ignore all of them except the first one
						if(bout_filenames.number > 1){
							cerr << argv[0] << ": Multiple filenames after -bout option: ignoring all but 1st\n";
							cerr.flush();
						}
						// If a filename was supplied, process the commands it contains
						if(bout_filenames.number > 0){
							char *fname;
							fname = ((char **) bout_filenames.vector)[0];
							saveSpline(argv[0], fname, window);
						}
					}
				}
			}
		}
	}

	if(mS1_found && mS2_found && msrcl_found && 
		mdestl_found && frames_found && base_found){
		char *src_fname, *dst_fname, *scmd_fname, *dcmd_fname, *fname_base;
		int num_frames;
		
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		// ignore all of them except the first one
		if(mS1_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -mS1 option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(mS1_filenames.number > 0){
			src_fname = ((char **) mS1_filenames.vector)[0];
		}
			

		if(mS2_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -mS2 option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(mS2_filenames.number > 0){
			dst_fname = ((char **) mS2_filenames.vector)[0];
		}

		if(msrcl_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -msrcl option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(msrcl_filenames.number > 0){
			scmd_fname = ((char **) msrcl_filenames.vector)[0];
		}

		if(mdestl_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -mdestl option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(mdestl_filenames.number > 0){
			dcmd_fname = ((char **) mdestl_filenames.vector)[0];
		}
		
		if(frames_num.number > 1){
			cerr << argv[0] << ": Multiple frame numbers -frames option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(frames_num.number > 0){
			num_frames = (int)((double*) frames_num.vector)[0];
		}

		if(base_fname.number > 1){
			cerr << argv[0] << ": Multiple filenames after -mdestl option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if(base_fname.number > 0){
			fname_base = ((char **) base_fname.vector)[0];
		}

		processMorph(argv[0], src_fname, dst_fname, scmd_fname, dcmd_fname, fname_base, 
			num_frames, window);
	}
	// 
	//  2. Perform error checking and execute drawing commands from
	//  the file (if supplied)
	//
	if (cmd_found) {
		// We will only execute commands from one file at a time, so
		// if more than one filename is supplied after '-cmd', we
		 // ignore all of them except the first one
		if (cmd_filenames.number > 1){
			cerr << argv[0] << ": Multiple filenames after -cmd option: ignoring all but 1st\n";
			cerr.flush();
		}
		// If a filename was supplied, process the commands it contains
		if (cmd_filenames.number > 0) {
			char *fname;
			fname = ((char **) cmd_filenames.vector)[0];
			processDrawingCommands(argv[0], fname, window);
		}
	}
}


// Function for displaying a Vista image in the GL panel
// This is done by the set() method in the last line of this function
void openImage(char* progname, char* filename, ScissorUI& window)
{
	int OK=0;
	VAttrList attrs1;
	VImage *im1;
	FILE *fd;
	char *fname;


#ifdef _WIN32
	fd = fopen(filename, "rb");
#else
	fd = fopen(filename, "r");
#endif
	if (fd)
		if (VReadImages(fd, &attrs1, &im1)) 
			OK = 1;
	if (!OK)
		fl_alert("VReadImages: Error opening image file. Image(s) not displayed.");
	else {
		window.set(*im1);
		window.imagePanel->computeLinkCosts(*im1);
	}
}


