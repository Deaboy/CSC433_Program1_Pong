/***************************************************************************//**
 * @file File containing the declaration for the Pong class.
 *
 * @brief Contains the declaration for the Pong class, the core class of the
 *		entire program.
*******************************************************************************/
#ifndef _PONG_H_
#define _PONG_H_

/*******************************************************************************
 *                 DECLARATIONS, INCLUDES, AND NAMESPACES
*******************************************************************************/
class Pong;

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <GL/freeglut.h>
#include <time.h>
#include "PongGame.h"
#include "Drawable.h"
#include "Menu.h"

using namespace std;

/***************************************************************************//**
 * @brief The Pong Class is in charge of window management, drawing
 *		all the parts of the game, and running the other classes. It also
 *		handles the interfacing between glut and the game for keyboard and mouse
 *		management.
 *
 * @details This Class essentially provides a base for the rest of the program.
 *		It handles drawing functions, forwards OpenGL events, generates a
 *		framerate, and standardizes various aspects of the game.
*******************************************************************************/
class Pong
{
	private:
		static Pong* instance;	/*!< reference to main instance of this class */
		int view_width;			/*!< The width of the drawable region */
		int view_height;		/*!< The height of the drawable region */
		int window_width;		/*!< Width of gui window */
		int window_height;		/*!< Height of gui window */
		double scale;
		int view_x;
		int view_y;
		string window_name;		/*!< The Name of the Gui window */
		Menu* menu;				/*!< Menu object */
		
		PongGame* game;			/*!< Pointer to instance of the game manager */
		map<int, list<Drawable*>> drawables;	/*!< list of objects to draw*/
		
	public:
		static const int unit;	/*!< Unit of measurement used in calculations */

		/*!
		 * @brief The constructor. Initializes variables and classes.
		 */
		Pong();

		/*!
		 * @brief The destructor. Frees up dynamic memory.
		 */
		~Pong();

		/*!
		 * @brief Function to get the current instance of the program.
		 */
		static Pong* getInstance( );

		/*!
		 * @brief Gets the current instance of the game manager.
		 */
		PongGame* getGame();

		/*!
		 * @brief Begins running the program. Initializes OpenGL, registers
		 *		events, instantiates objects, and runs the game. Beware when
		 *		calling this function, as it enters the OpenGL main loop, only
		 *		to return at end of program execution.
		 */
		int run ( int argc, char *argv[] );

		/*!
		 * @brief Registers a Drawable object to be drawn on demand.
		 */
		void drawObject(Drawable* obj, int layer = 0);
		
		/*!
		 * @brief Checks whether an object is currently being drawn.
		 */
		bool isDrawingObject(Drawable* obj);

		/*!
		 * @brief Determines the layer given the object is drawn at.
		 */
		int getDrawingLayer(Drawable* obj);

		/*!
		 * @brief Sets the drawing layer of an already registered object.
		 */
		void setDrawingLayer(Drawable* obj, int layer);

		/*!
		 * @brief Removes an object from the drawing list and no longer draws
		 *		it. Must be done before the object becomes deallocated.
		 */
		void stopDrawingObject(Drawable* obj);

		/*!
		 * @brief Stops drawing all objects, clearing the drawing list.
		 */
		void stopDrawingAll();

		/*!
		 * @brief Gets the width of the view port in the virtual space.
		 */
		int getViewWidth();

		/*!
		 * @brief Gets the height of the view port in the virtual space.
		 */
		int getViewHeight();
		
		/*!
		 *  @brief accesses the menu entity
		 */
		Menu* getMenu();
		
		/*!
		 * @brief Builds and displays the game menu
		 */
		void displayMenu();
		
		/*!
		 * @brief Closes any currently open menus
		 */
		void closeMenu();
		
		/*!
		 * @brief Drawing callback. Executes every glut display callaback. Also
		 *		calls the draw function of all registerd Drawable objects.
		 */
		void display();

		/*!
		 * @brief Resize callback. Executes whenever the window is resized.
		 */
		void reshape( int w, int h );

		/*!
		 * @brief Key down callback. Executes whenever a typical key is pressed.
		 */
		void keyDown(unsigned char key, int x, int y);

		/*!
		 * @brief Key up callback. Executes whenever a typical key is released.
		 */
		void keyUp(unsigned char key, int x, int y);

		/*!
		 * @brief Special key down callback. Executes whenever a "special" key
		 *		such as an arrow key is pressed.
		 */
		void keySpecialDown(int key, int x, int y);

		/*!
		 * @brief Special key up callback. Executes whenever a "special" key
		 *		such as an arrow key is released.
		 */
		void keySpecialUp(int key, int x, int y);

		/*!
		 * @brief Mouse click callback. Executes whenever a mouse button is
		 *		either clicked or released.
		 */
		void mouseclick( int button, int state, int x, int y );

		/*!
		 * @brief Step callback. Called every frame of the game. Calls step
		 *		function of other classes necessary for the game. Necessary
		 *		for game elemnts that are not dependent on user interaction.
		 */
		void step();
};

/*******************************************************************************
 *                         GLUT CALLBACK FUNCTIONS
*******************************************************************************/
/*!
 * @brief Drawing callback. Executes every glut display callaback. Also
 *		calls the draw function of all registerd Drawable objects. simply
 *		forwards to Pong class' identical function.
 */
void display();

/*!
 * @brief Resize callback. Executes whenever the window is resized.
 *		Simply forwards to Pong class' identical function.
 */
void reshape( int w, int h );

/*!
 * @brief Key down callback. Executes whenever a typical key is pressed.
 *		Simply forwards to Pong class' identical function.
 */
void keyDown(unsigned char key, int x, int y);

/*!
 * @brief Key up callback. Executes whenever a typical key is released.
 *		Simply forwards to Pong class' identical function.
 */
void keyUp(unsigned char key, int x, int y);

/*!
 * @brief Special key down callback. Executes whenever a "special" key
 * 		such as an arrow key is pressed. Simply forwards to Pong class'
 *		identical function.
 */
void keySpecialDown(int key, int x, int y);

/*!
 * @brief Special key up callback. Executes whenever a "special" key
 * 		such as an arrow key is released. Simply forwards to Pong class'
 *		identical function.
 */
void keySpecialUp(int key, int x, int y);

/*!
 * @brief Mouse click callback. Executes whenever a mouse button is
 * 		either clicked or released. Simply forwards to Pong class'
 * 		identical function.
 */
void mouseclick( int button, int state, int x, int y );

/*!
 * @brief Step callback. Called every frame of the game. Calls step
 *		function of other classes necessary for the game. Necessary
 *		for game elemnts that are not dependent on user interaction.
 *		Simply to Pong class' step() function.
 */
void step(int i);

#endif
