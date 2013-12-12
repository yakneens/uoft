CFLAGS = -I /usr/vision/include -I /u/kyros/public/include -Wno-deprecated
CC = gcc
LDFLAGS = `fltk-config --use-gl --ldflags` -lstdc++ -L/usr/vision/lib -L/u/kyros/public/lib -lvista -lnewmat
FLUID = fluid

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

%.cpp: %.fl
	$(FLUID) -c $<

BASIC_OBJ   = glutils.o Texture.o main.o imdraw-utils.o bspline.o bspline-draw.o
UI_OBJ      = ScissorUI.o
UI_CPP      = ScissorUI.cpp ScissorUI.h
STUDENT_OBJ = c-code/pqelement.o c-code/boundary.o c-code/imageprimitive.o c-code/point.o c-code/line.o c-code/circle.o c-code/rectangle.o c-code/polygon.o c-code/color.o c-code/imdraw-init.o c-code/imdraw-draw.o c-code/imdraw-handle.o c-code/readDrawing.o c-code/wavelets.o

c-code/morph:	$(UI_CPP) $(BASIC_OBJ) $(UI_OBJ) $(STUDENT_OBJ)
	$(CC) -o c-code/morph $(BASIC_OBJ) $(UI_OBJ) $(STUDENT_OBJ) $(LDFLAGS)

clean:		
	rm -rf $(BASIC_OBJ) $(UI_OBJ) $(STUDENT_OBJ)
