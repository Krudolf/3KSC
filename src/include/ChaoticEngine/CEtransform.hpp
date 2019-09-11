
#ifndef CETRANSFORM
#define CETRANSFORM

#include "CEentity.hpp"

/*La API de GLM la podeis ver aqui: http://glm.g-truc.net/0.9.8/api/modules.html*/
class CETransform : public CEEntity{
public:
	CETransform();
	virtual ~CETransform();

    void        loadIdentity();
    void        loadMatrix(glm::mat4 p_matrix);
    void        showMatrix(glm::mat4 p_matrix);
    void        transpose();
    glm::mat4   transpose(glm::mat4 p_matrix);

    void        translate(float p_tx, float p_ty, float p_tz);
    void        rotate(float p_rx, float p_ry, float p_rz);
    void        scale (float p_sx, float p_sy, float p_sz);

    glm::mat4   getMatrix();

	void        beginDraw() override;
	void        endDraw() override;

private:
	glm::mat4 m_matrix;
};

#endif