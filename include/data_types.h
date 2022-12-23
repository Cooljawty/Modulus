#pragma once
#include "SDL_GL.h"
#include "texture.h"

namespace Modulus{
	
	struct Vertex{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoordinate;
	};

	struct Material{
		Texture* texture;
		std::string type;
		std::string path;
	};

	struct Parameter{
		std::string name;
		GLenum type;
		void* value;
	};
}
