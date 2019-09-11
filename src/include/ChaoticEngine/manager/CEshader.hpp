#ifndef CEGLSHADER_H
#define CEGLSHADER_H

#include <glew.h>
#include <string>

class CEShader{
public:
	CEShader();
	virtual ~CEShader();
    GLuint 	LoadShader(const char *vertex_path, const char *fragment_path);
    void	useShader(GLuint p_shader);

private:
	std::string readFile(const char *filePath);
	std::string	getFileError(const char* p_file);

};

#endif