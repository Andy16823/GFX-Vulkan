#pragma once
#include "../Core/Behavior.h"
#include "../Graphics/Camera.h"
#include "../Math/Frustum.h"

class FrustumCullingBhv : public Behavior
{
private:
	Camera* m_camera;

public:
	FrustumCullingBhv(Camera* camera);

	void init(Scene* scene, Renderer* renderer) override;

	void update(Scene* scene, float dt) override;

	void destroy(Scene* scene, Renderer* renderer) override;

	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) override;

	std::string getIdentifier() override;
};

