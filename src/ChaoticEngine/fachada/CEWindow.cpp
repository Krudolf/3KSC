
#include <iostream>

#include "../../include/ChaoticEngine/fachada/CEWindow.hpp"

/*
Creamos una ventana con GLFW
*/
CEWindow::CEWindow(int p_width, int p_height, const char* p_title, bool p_fullscreen){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(p_fullscreen){
    	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    	const GLFWvidmode* mode    = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    	m_window = glfwCreateWindow(mode->width, mode->height, p_title, monitor, NULL);
    }
    else
    	m_window = glfwCreateWindow(p_width, p_height, p_title, NULL, NULL);

	if(m_window == NULL){
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	}
	glfwMakeContextCurrent(m_window);
	
	//start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(GLEW_OK != err){
		//Problem: glewInit failed, something is seriously wrong.
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	//fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glfwSetFramebufferSizeCallback(m_window, windows_size_callback);

	std::cout << "VERSION OPENGL: " << glGetString(GL_VERSION) << std::endl;

	enableCulling();
	enableZBuffer();

	m_time		= glfwGetTime();
	m_lastTime	= 0;
}

CEWindow::~CEWindow(){}

void CEWindow::enableCulling(){
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void CEWindow::enableZBuffer(){
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

bool CEWindow::isOpen(){
	return !glfwWindowShouldClose(m_window);
}

void CEWindow::close(){
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void CEWindow::setCursorVisible(bool p_visible){ 
    if (p_visible){ 
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
    } 
 
    else{ 
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 
    } 
}

void CEWindow::clear(float p_red, float p_green, float p_blue, float p_alpha){
	glClearColor(p_red, p_green, p_blue, p_alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CEWindow::swapBuffers(){
	glfwSwapBuffers(m_window);
}

void CEWindow::pollEvents(){
	glfwPollEvents();
}

void CEWindow::processInput(){
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

GLFWwindow* CEWindow::getWindow(){
	return m_window;
}

CEPosition CEWindow::getPosition(){
    CEPosition t_position;
    glfwGetWindowPos(m_window, &t_position.x, &t_position.y);
    return t_position;
}

CESize CEWindow::getSize(){
    CESize t_size;
    glfwGetWindowSize(m_window, &t_size.width, &t_size.height);
    return t_size;
}

double CEWindow::getTimer(){
	return glfwGetTime();
}

double CEWindow::getElapsedTime(){
	m_lastTime = m_time;
	m_time = glfwGetTime();

	return (m_time-m_lastTime);
}

/*  CALLBACKS  */
void CEWindow::windows_size_callback(GLFWwindow* p_window, int p_width, int p_height){
	glViewport(0, 0, p_width, p_height);
}