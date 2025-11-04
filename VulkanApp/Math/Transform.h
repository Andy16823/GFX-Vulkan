#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/// <summary>
/// Transform class for position, rotation, and scale
/// </summary>
class Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform()	: position(0.0f, 0.0f, 0.0f), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f, 1.0f, 1.0f)
	{
	};

	/// <summary>
	/// Sets the position of the transform
	/// </summary>
	/// <param name="pos"></param>
	void setPosition(const glm::vec3& pos) {
		position = pos;
	}

	/// <summary>
	/// Sets the rotation of the transform
	/// </summary>
	/// <param name="rot"></param>
	void setRotation(const glm::quat& rot) {
		rotation = rot;
	}

	/// <summary>
	/// Sets the scale of the transform
	/// </summary>
	/// <param name="scl"></param>
	void setScale(const glm::vec3& scl) {
		scale = scl;
	}

	/// <summary>
	/// Translates the transform by the given delta
	/// </summary>
	/// <param name="delta"></param>
	void translate(const glm::vec3& delta) {
		position += delta;
	}

	/// <summary>
	/// Rotates the transform by the given delta quaternion
	/// </summary>
	/// <param name="delta"></param>
	void rotate(const glm::quat& delta) {
		rotation = delta * rotation;
	}

	/// <summary>
	/// Rotates the transform by the given euler angles (in degrees)
	/// </summary>
	/// <param name="eulerAngles"></param>
	void rotate(glm::vec3 eulerAngles) {
		glm::quat delta = glm::quat(glm::radians(eulerAngles));
		rotation = delta * rotation;
	}

	/// <summary>
	/// Rotates the transform by the given pitch, yaw, and roll (in degrees)
	/// </summary>
	/// <param name="pitch"></param>
	/// <param name="yaw"></param>
	/// <param name="roll"></param>
	void rotate(float pitch, float yaw, float roll) {
		glm::quat delta = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
		rotation = delta * rotation;
	}

	/// <summary>
	/// Scales the transform by the given factor
	/// </summary>
	/// <param name="factor"></param>
	void scaleBy(const glm::vec3& factor) {
		scale *= factor;
	}

	/// <summary>
	/// Gets the forward direction vector of the transform
	/// </summary>
	/// <returns></returns>
	glm::vec3 getForward() const {
		return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	/// <summary>
	/// Gets the up direction vector of the transform
	/// </summary>
	/// <returns></returns>
	glm::vec3 getUp() const {
		return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	/// <summary>
	/// Gets the right direction vector of the transform
	/// </summary>
	/// <returns></returns>
	glm::vec3 getRight() const {
		return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Gets the transformation matrix of the transform
	/// </summary>
	/// <returns></returns>
	glm::mat4 getMatrix() const {
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotationMat = glm::toMat4(rotation);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
		return translationMat * rotationMat * scaleMat;
	}
	
	/// <summary>
	/// Point the transform towards a target position
	/// </summary>
	/// <param name="target"></param>
	void towards(glm::vec3 target);
};