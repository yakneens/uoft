#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "models.h"
#include "anim.cpp"

void drawWalls(){
	glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
 	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	set_colour(0.4f, 0.4f, 0.4f, 0.4f, 0.7f, 0.1f);
	
	glPushMatrix();
	glTranslatef(-12,0,0);
	
	//Left wall
	glBegin(GL_QUADS);
	
	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,30);
	
	glNormal3f(1,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0,30, -30);
	
	glNormal3f(1,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0,0, -30); 
	
	glEnd();
	
	
	//Back wall
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,30);
	
	glNormal3f(0,0,-1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(24,30, 30);
	
	glNormal3f(0,0,-1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(24,0, 30); 
	glEnd();
	
	glTranslatef(24,0,0);
	
	//Right wall
	glBegin(GL_QUADS);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,30);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0,30, -30);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0,0, -30); 
	
	glEnd();
	
	//Front wall
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, -30);
	
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,-30);
	
	glNormal3f(0,0,1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-24,30, -30);
	
	glNormal3f(0,0,1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-24,0, -30); 
	glEnd();
	
	//Floor
	glBegin(GL_QUADS);
	
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,0,-30);
	
	glNormal3f(0,1,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(24,0, -30);
	
	glNormal3f(0,1,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(24,0,30); 
	
	
	glEnd();
	
	//Ceiling
	glBegin(GL_QUADS);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,30, 30);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,-30);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(24,30, -30);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(24,30,30); 
	
	
	glEnd();
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void drawColumnShaft(){

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluCylinder(aardvark, 1.2, 1.2, 1.0, 32, 32);
	glPopMatrix();

}


void drawColumn(float shaftHeight){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	
	glPushMatrix();
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glRotatef(90,0,1,0);
	gluCylinder(aardvark, 2.0, 2.0, 2.0, 32, 32);
	gluDisk(aardvark, 0, 2, 32, 32);
	glTranslatef(0, 0, 2);
	gluDisk(aardvark, 0, 2, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glScalef(1.0, shaftHeight, 1.0);
	drawColumnShaft();
	glPopMatrix();
	
	glTranslatef(0,shaftHeight, 0);
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glRotatef(90,0,1,0);
	gluCylinder(aardvark, 2.0, 2.0, 2.0, 32, 32);
	gluDisk(aardvark, 0, 2, 32, 32);
	glTranslatef(0, 0, 2);
	gluDisk(aardvark, 0, 2, 32, 32);
	glPopMatrix();
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}


void drawStairs(){
	
	glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
 	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	//set_colour(1.0f, 1.0f, 1.0f, 0.2f, 0.7f, 0.1f);
	set_colour(0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.1f);
	
	for(int i = 1; i <= 6; i++){
		
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-12.0f, i - 1 , -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(12.0f, i - 1, -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(12.0f, i, -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-12.0f, i, -2 * (i - 1));
		glEnd();
		
		
		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-12.0f, i, -2 * (i-1));
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(12.0f, i, -2 * (i-1));
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(12.0f, i, -2 * i);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-12.0f, i, -2 * i);
		
		glEnd();
	
	}
	
	glDisable(GL_TEXTURE_2D);
}
