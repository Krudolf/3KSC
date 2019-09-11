 
#ifndef CESCENENODE
#define CESCENENODE

#include <iostream>
#include <vector>

#include "CEentity.hpp"

class CESceneNode{
public:
	CESceneNode();
	CESceneNode(CESceneNode* p_father);
	virtual ~CESceneNode();

	void 			addChild(CESceneNode* p_child);
	bool			removeChild(CESceneNode* p_child);
	void 			removeAllChilds();

	void 			setEntity(CEEntity* p_entity);
	
	CEEntity*		getEntity();
	CESceneNode* 	getFather();
	void 			setFather(CESceneNode* p_father);

	void 			draw();
	void 			release();

	int 			getTotalChilds();


private:
	CEEntity* 					m_entity;
	std::vector<CESceneNode*>	m_childs;
	CESceneNode* 				m_father;
	
};

#endif