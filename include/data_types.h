#pragma once
#include "SDL_GL.h"
#include "texture.h"

namespace Modulus{
	
	struct Vertex{
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) :
			Position(p), Normal(n), TexCoordinate(t) {}
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoordinate;
	};

	struct Material{
		Material(Texture* tx, std::string ty, std::string pa) :
			texture(tx), type(ty), path(pa) {}
		Texture* texture;
		std::string type;
		std::string path;
	};	
}
