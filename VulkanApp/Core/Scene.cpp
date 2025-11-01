#include "Scene.h"

void Scene::init(Renderer* renderer)
{
	m_renderTargetIndex = renderer->createRenderTarget(true);
}