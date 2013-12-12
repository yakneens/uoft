#include <GL/glui.h>

#define X 0
#define Y 1
#define Z 2

extern void save_image(int frame);
extern void set_recording(bool newState);
extern void set_animating(bool newState);
extern void step_forward();
extern void step_back();
extern void handle_timer(int ID);
extern void set_animation_direction(int dir);

GLUI *animUI = NULL;
int myAnimating = 0;
int myRecording = 0;
int forward   = 1;

enum {ANIMATING, FORWARD,
	  STEP_FORWARD, STEP_BACK, RECORDING, SAVE, QUIT};

///////////////////////////////////////////////////////////
//
// This is called when the user interacts with a
// widget on the user interface (UI).
// The ID parameter identifies which widget was affected.
//
///////////////////////////////////////////////////////////

void handleGLUIEvent( int ID ) {
  switch (ID) {
  case ANIMATING:
    set_animating(myAnimating != 0);
    break;

  case FORWARD:
    set_animation_direction(forward);
    break;

  case RECORDING:
    set_recording(myRecording != 0);
    break;

  case STEP_FORWARD:
    myAnimating = 0;
    set_animating(false);
    step_forward();
    break;

  case STEP_BACK:
    myAnimating = 0;
    set_animating(false);
    step_back();
    break;
    
  case SAVE:
    save_image(-1);
    break;

  case QUIT:
    exit(0);

  default:
    glutPostRedisplay();
    break;

  }
}


///////////////////////////////////////////////////////////
//
// Set up the graphical user interface (UI),
// Which includes two checkboxes, two spinners,
// and two buttons.
//
///////////////////////////////////////////////////////////

void setupUI (int mainWindowID) {
  animUI = GLUI_Master.create_glui( "Animation Controls", 0 );

  GLUI_Checkbox *animatingCheckbox = animUI->add_checkbox("Animate",
							  &myAnimating,
							  ANIMATING,
							  handleGLUIEvent);

  GLUI_Checkbox *forwardCheckbox = animUI->add_checkbox("Run Forward",
							  &forward,
							  FORWARD,
							  handleGLUIEvent);

  GLUI_Checkbox *recordingCheckbox = animUI->add_checkbox("Record",
							  &myRecording,
							  RECORDING,
							  handleGLUIEvent);

  GLUI_Button *stepForwardButton = animUI->add_button("Step Forward",
						      STEP_FORWARD,
						      handleGLUIEvent);
  
  GLUI_Button *stepBackButton = animUI->add_button("Step Back",
						   STEP_BACK,
						   handleGLUIEvent);
  
  GLUI_Button *saveButton = animUI->add_button("Save Image",
					       SAVE,
					       handleGLUIEvent);
  
  GLUI_Button *quitButton = animUI->add_button("Quit",
					       QUIT,
					       handleGLUIEvent);
  
  animUI->set_main_gfx_window( mainWindowID );

  GLUI_Master.set_glutIdleFunc( NULL );

  glutMainLoop();
}
