CSC320
ASSIGNMENT 4
By: Sergei Iakhnin
#: 981 843 420


My implementation of the morphing algorithm has been built trhough several iterations.
The first version of the program was built to follow the pseudocode provided in the
Beier & Neely paper exactly. However, when creating morphs I realized that this 
implementation was very slow because of many repeated calculations. For that reason
I have implemented some preprocessing steps that prepare the data before a morph
takes place. You will notice that I keep an array lineData which is prefilled before
the morphing step. Later during the main morphing loop this array is referenced instead
of repeating the same calculations. This preprocessing has reduced the running time
of my program by a factor of 4. 

I have implemented antialiasing with the use of MIP mapping however a bug that I have
not been able to discover generates black colored pixels in random spots throughout
the images and for that reason I have disabled this function in my final implementation.

As a result two functions are available in my solution that perform the morph. One function
is called mipMorph() and it implements a version of MIP-mapping as suggested by Lecture 21
notes (using gaussian filters)with the minor change that the depth level is limited to 7 
so as to not produce prohibitive morph times. Currently the function is not invoked anywhere
in the code due to the bug I described above. 

Another function is called morph() and it implements the same morphing algorithm but only
uses gaussian smoothing of the source image instead of proper anti-aliasing. This function
is currently used by the program and is used in generating the images presented.

The functions have the same footprint so one can be substituted for another simply by switching
the name.

Most functions relevant to this assignment can be found in the file imdraw-utils.cpp.

My project does not implement the GUI driven version of the morphing algorithm.
