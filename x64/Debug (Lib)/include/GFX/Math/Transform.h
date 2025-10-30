#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
class Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform()	: position(0.0f, 0.0f, 0.0f), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f, 1.0f, 1.0f)
	{
	};

	void setPosition(const glm::vec3& pos) {
		position = pos;
	}

	void setRotation(const glm::quat& rot) {
		rotation = rot;
	}

	void setScale(const glm::vec3& scl) {
		scale = scl;
	}

	void translate(const glm::vec3& delta) {
		position += delta;
	}

	void rotate(const glm::quat& delta) {
		rotation = delta * rotation;
	}

	void rotate(glm::vec3 eulerAngles) {
		glm::quat delta = glm::quat(glm::radians(eulerAngles));
		rotation = delta * rotation;
	}

	void rotate(float pitch, float yaw, float roll) {
		glm::quat delta = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
		rotation = delta * rotation;
	}

	void scaleBy(const glm::vec3& factor) {
		scale *= factor;
	}

	glm::vec3 getForward() const {
		return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	glm::vec3 getUp() const {
		return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::vec3 getRight() const {
		return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::mat4 getMatrix() const {
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotationMat = glm::toMat4(rotation);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
		return translationMat * rotationMat * scaleMat;
	}
	
	void towards(glm::vec3 target);

	static glm::vec3 worldToNDC(const glm::vec3& localCoords, glm::mat4 projection, glm::mat4 view);
	static glm::vec3 ndcToWorld(const glm::vec3& ndcCoords, glm::mat4 projection, glm::mat4 view);
};