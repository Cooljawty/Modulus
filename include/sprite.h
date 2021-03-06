#pragma once

class Sprite{
	public:
		Sprite();
		~Sprite();
	
	private:
		
		//Pointer to sprites associated shader
		static SpriteShader* mSpriteShader;
};
