#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "./sphere.hpp"

std::vector<Vertex> Sphere::getVertices() const {
	std::vector<Vertex> result;

	// Top vertex (n = 0)
	result.push_back({ {pos.x, pos.y, pos.z + r}, {0.0, 1.0, 0.0}, {0.0, 0.0} });

	for (uint32_t n = 1; n < numSplitZ; n++) {
		for (uint32_t m = 0; m < numSplitXY; m++) {
			float degXY = ((M_PI * 2) / numSplitXY) * m;
			float degZ = M_PI / 2 - (M_PI / numSplitZ) * n;

			float x = r * std::cos(degXY) * std::cos(degZ);
			float y = r * std::sin(degXY) * std::cos(degZ);
			float z = r * std::sin(degZ);

			result.push_back({
				{pos.x + x, pos.y + y, pos.z + z},
				{n / (float)numSplitZ, 0.2, 0.2},
				{0.0, 0.0}
				});
		}
	}

	// Bottom vertex (n = numSplitZ)
	result.push_back({ {pos.x, pos.y, pos.z - r}, {1.0, 0.0, 0.0}, {0.0, 0.0} });

	return result;
}

std::vector<uint32_t> Sphere::getIndices() const {
	std::vector<uint32_t> result;

	// Top index (n = 1)
	for (uint32_t m = 0; m < numSplitXY; m++) {
		result.insert(result.end(), {
			0,
			m + 1,
			(m + 1) % numSplitXY + 1
			});
	}

	for (uint32_t n = 2; n < numSplitZ; n++) {
		for (uint32_t m = 0; m < numSplitXY; m++) {
			result.insert(result.end(), {
				numSplitXY * (n - 1) + m + 1,
				numSplitXY * (n - 1) + (m + 1) % numSplitXY + 1,
				numSplitXY * (n - 2) + m + 1
				});
			result.insert(result.end(), {
				numSplitXY * (n - 1) + (m + 1) % numSplitXY + 1,
				numSplitXY * (n - 2) + (m + 1) % numSplitXY + 1,
				numSplitXY * (n - 2) + m + 1
				});
		}
	}

	// Bottom index (n = numSplitZ)
	for (uint32_t m = 0; m < numSplitXY; m++) {
		result.insert(result.end(), {
			(numSplitZ - 1) * numSplitXY + 1,
			(numSplitZ - 2) * numSplitXY + (m + 1) % numSplitXY + 1,
			(numSplitZ - 2) * numSplitXY + m + 1
			});
	}

	std::for_each(result.begin(), result.end(), [=](uint32_t& index) {
		index += baseIndex;
	});

	return result;
}