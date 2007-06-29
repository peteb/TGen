//
//  main.cpp
//  tgen texturing
//
//  Created by Peter Backman on 6/29/07.
//  Copyright Peter Backman 2007. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <iostream>

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	
	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("TGen texturing");
	
	glutMainLoop();
	
	return EXIT_SUCCESS;
}
