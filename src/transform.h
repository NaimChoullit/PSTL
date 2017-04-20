#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>




class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->m_pos = pos;
		this->m_rot = rot;
		this->m_scale = scale;
    }

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(m_pos);
		glm::mat4 scaleMat = glm::scale(m_scale);
		glm::mat4 rotX = glm::rotate(m_rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(m_rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(m_rot.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;

		return posMat * rotMat * scaleMat;
	}

/*
	inline glm::mat4 GetMVP(const Camera& camera) const
	{
		glm::mat4 VP = camera.GetViewProjection();
		glm::mat4 M = GetModel();

		return VP * M;//camera.GetViewProjection() * GetModel();
	}
*/
	inline glm::vec3* GetPos() { return &m_pos; }
	inline glm::vec3* GetRot() { return &m_rot; }
	inline glm::vec3* GetScale() { return &m_scale; }

	inline void SetPos(const glm::vec3& pos) { this->m_pos = pos; }
	inline void SetRot(const glm::vec3& rot) { this->m_rot = rot; }
	inline void SetScale(const glm::vec3& scale) { this->m_scale = scale; }
protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif
