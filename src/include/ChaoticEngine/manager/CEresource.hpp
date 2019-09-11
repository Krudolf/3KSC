 
#ifndef CERESOURCE
#define CERESOURCE

#include <glew.h>
#include <vector>
#include <string>

class CEResource{
public:
	CEResource(){};
	virtual ~CEResource(){};

	std::string getName(){	return m_name;	};
	void setName(const char* p_name){	m_name = p_name;	};
	virtual bool loadFile(const char* p_name){};

protected:

private:
	std::string m_name;
};

#endif 
