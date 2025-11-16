#include "Canvas.h"
#include "Widget.h"

void Canvas::init(Renderer* renderer)
{
	m_renderTargetIndex = renderer->createRenderTarget(false);
	m_cameraIndex = renderer->createCamera();
	for(const auto& widget : m_widgets) {
		widget->init(this, renderer);
	}
}

void Canvas::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	auto renderTarget = renderer->getRenderTarget(m_renderTargetIndex);
	renderer->updateCamera(m_cameraIndex, currentFrame, m_camera->getViewProjection());
	renderer->setActiveCamera(m_cameraIndex);
	renderer->beginnRenderPass(commandBuffer, renderTarget->getFramebuffer(), m_clearColor, renderer->getOffscreenRenderPass());
	for (const auto& widget : m_widgets) {
		widget->draw(this, renderer, commandBuffer, currentFrame);
	}
	renderer->endRenderPass(commandBuffer);
}

void Canvas::update(GLFWwindow* window, float deltaTime)
{
	for (const auto& widget : m_widgets) {
		widget->update(this, window, deltaTime);
	}
}

void Canvas::destroy(Renderer* renderer)
{
	for (const auto& widget : m_widgets) {
		widget->destroy(this, renderer);
	}
}

void Canvas::beforeSwapchainRecreation(Renderer* renderer)
{
	renderer->cleanupRenderTarget(m_renderTargetIndex);
	for (const auto& widget : m_widgets) {
		widget->beforeSwapchainRecreation(this, renderer);
	}
}

void Canvas::afterSwapchainRecreation(Renderer* renderer, const glm::ivec2& newSize)
{
	renderer->recreateRenderTarget(m_renderTargetIndex);
	for (const auto& widget : m_widgets) {
		widget->afterSwapchainRecreation(this, renderer, newSize);
	}
}