#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

class Camera {
	glm::vec3 center{ 0.0, 0.0, 0.0 };
	//// in Radians
	float hRotation = 0.0;
	//// in Radians
	float vRotation = glm::radians(20.0);
	float distance = 10.0;

	glm::vec3 generatePos();

	glm::mat4 generateViewMat();

public:
	void updateCamera() {
		const static double DIFF_RATIO = 0.01;
		const static double SCROLL_RATIO = 1.1;

		bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);

		if (!isWindowFocused) {
			if (mouseCtx.leftButton) {
				double xDiff = mouseCtx.deltaX * DIFF_RATIO;
				double yDiff = mouseCtx.deltaY * DIFF_RATIO;

				if (keyCtx.shift) {
					glm::mat4 r(1.0);
					r *= glm::rotate(
						glm::mat4(1.0),
						hRotation,
						glm::vec3(0.0, 0.0, 1.0)
					);
					r *= glm::rotate(
						glm::mat4(1.0),
						-vRotation,
						glm::vec3(0.0, 1.0, 0.0)
					);
					glm::vec4 shift(0.0, -(float)xDiff, (float)yDiff, 1.0);
					center += glm::vec3(r * shift);
				}
				else {
					hRotation = std::fmod(hRotation - xDiff, 2 * M_PI);
					vRotation = std::clamp(
						std::fmod(vRotation + yDiff, 2 * M_PI),
						-M_PI / 2.0 + 0.0001,
						M_PI / 2.0 - 0.0001
					);
				}
			}
			distance *= std::pow(SCROLL_RATIO, -mouseCtx.scrollOffsetY);
		}
	}
};
