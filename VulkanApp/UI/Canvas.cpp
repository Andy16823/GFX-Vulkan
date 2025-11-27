#include "Canvas.h"
#include "Widget.h"
#include "../Core/Input.h"

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

void Canvas::update(GLFWwindow* window, float deltaTime, const glm::vec2& viewportSize)
{
	auto mousePos = Input::getMousePosition(window);
	mousePos = mousePos - (viewportSize / 2.0f);
	mousePos.y = -mousePos.y;
	if (this->containsPoint(mousePos)) {
		// Canvas is being interacted with
	}

	for (const auto& widget : m_widgets) {
		if (widget->containsPoint(mousePos)) {
			widget->mouseOver(this, window, mousePos);
		}
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

bool Canvas::containsPoint(const glm::vec2& point)
{
	glm::vec3 canvasPos = m_transform.position;
	glm::vec3 canvasScale = m_transform.scale;
	return (point.x >= canvasPos.x && point.x <= canvasPos.x + canvasScale.x &&
		point.y >= canvasPos.y && point.y <= canvasPos.y + canvasScale.y);
}
