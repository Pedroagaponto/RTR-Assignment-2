#include "calc.h"
#include "io.h"

#include "stdio.h"

glm::vec3 computeLighting(glm::vec3 & rEC, glm::vec3 & nEC)
{
	// Perform lighting calculations in eye coordinates (EC)

	if (debug[d_computeLighting]) {
		printf("modelViewMatrix\n");
		printMatrixColumnMajor(&modelViewMatrix[0][0], 4);
		printf("normalMatrix\n");
		printMatrixColumnMajor(&normalMatrix[0][0], 3);
	}

	glm::vec3 lEC( 0.0, 0.0, 1.0 );
	nEC = glm::normalize(nEC);

	if (debug[d_computeLighting]) {
		printf("rEC ");
		printVec(&rEC[0], 3);
		printf("nEC ");
		printVec(&nEC[0], 3);
		printf("lEC ");
		printVec(&lEC[0], 3);
	}

	// Ambient
	// Default fixed pipeline ambient light and material are both (0.2, 0.2, 0.2)
	glm::vec3 ambient(0.2 * 0.2);
	// Default diffuse light for GL_LIGHT0 is (1.0, 1.0, 1.0)
	// and default diffuse material is (0.8, 0.8, 0.8);
	// Lambertian diffuse reflection: L.V
	float dp = glm::dot(nEC, lEC);
	glm::vec3 diffuse(0.0);
	if (dp > 0.0)
		diffuse = glm::vec3(dp * 1.0 * 0.8);

	// Color is ambient + diffuse
	glm::vec3 color = ambient + diffuse;

	return color;
}
