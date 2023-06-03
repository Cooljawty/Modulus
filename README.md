# Modulus

A modular game engine library 

# Building

In order to build the Modulus library you rwquire the following:
 * SDL2
 * SDL2_image
 * GLEW
 * GLM
 * Freetype
 * Assimp
 * Lua 
 
To build Modulus run the following commands from the source directory:

> mkdir build

> cmake -S ./ -B ./build

The library can then be found at `build/src/libmodulus.a`

# Example Programs

The following example program should open a 800x800px window with a triangle in the center.
```
#include "modulus.h"

using namespace Modulus;

int main(){
	GameManager gGameManager( false, 800, 800);
	
	gGameManager.init();
	gGameManager.initOGL();
	
	Button quitButton("Escape");

	FrameBuffer mainFrameBuffer;
	mainFrameBuffer.init( gGameManager.getScreenWidth(), gGameManager.getScreenHeight());
	
	FBOShader gFBOShader;
	gFBOShader.loadProgram();

	gFBOShader.bind();
	gFBOShader.setTexture(0);
	gFBOShader.unbind();
	
	Mesh triangle(  {  0.0,0.5,  1,0,0,1,  
			  -0.5,-0.5, 0,1,0,1,  
			   0.5,-0.5, 0,0,1,1 }, 
			   {0,1,2}, std::vector<Material>(), {{2, GL_FLOAT}, {4, GL_FLOAT}} );
	
	struct TriangleShader : public Shader{
		public:
		bool loadProgram(){ 
			return compileShaders( { { GL_VERTEX_SHADER, "shaders/PolygonShader2D.vs" },
					       { GL_FRAGMENT_SHADER, "shaders/PolygonShader2D.fs" }});
		}
	} triangleShader;
	triangleShader.loadProgram();

	triangleShader.bind();

	glm::mat4 identitymatrix(1.f);
	triangleShader.setParameter("ProjectionMatrix", GL_FLOAT_MAT4, &identitymatrix);
	triangleShader.setParameter("ViewMatrix", GL_FLOAT_MAT4, &identitymatrix);
	triangleShader.setParameter("ModelMatrix", GL_FLOAT_MAT4, &identitymatrix);

	triangleShader.unbind();

	while( gGameManager.getRunning() ){
		gGameManager.pollEvents();
		if( quitButton.getState() ) gGameManager.toggleRunning();

		mainFrameBuffer.clear( GL_FRAMEBUFFER );
		
		mainFrameBuffer.bind( GL_FRAMEBUFFER );

		gGameManager.draw( triangleShader, triangle, mainFrameBuffer);
	
		gGameManager.drawToScreen( gFBOShader, mainFrameBuffer );
	}

	gGameManager.close();

	return 0;
}

```
To compile it do the following:
 1. Copy the library file `libmodulus.a` from the `build/src` directory.
 2. Set the include path to or copy the `include` directory
 3. Copy the `shaders` directory
 4. Compile the example program

The following program does the same, but using the Lua language binding.
```
#include "modulus.h"
#include "parser/lua/lua.h"

using namespace Modulus;

GameManager gGameManager(  false, 800, 800 );
Parse::Lua::Context gLuaContext( gGameManager );

using Parse::Lua::gLuaFrameBuffers;
using Parse::Lua::gLuaShaders;

int main(){

	gGameManager.init();
	gGameManager.initOGL();
	
	FBOShader gFBOShader;
	gFBOShader.loadProgram();

	gFBOShader.bind();
	gFBOShader.setTexture(0);
	gFBOShader.unbind();
	
	gLuaContext.init();

	if( !gLuaContext.loadFile("scripts/init.lua") ) gGameManager.toggleRunning();

	while( gGameManager.getRunning() ){
		gGameManager.pollEvents();
		
		gLuaContext.loadFile( "scripts/update.lua" );

		for( auto fb : gLuaFrameBuffers){
			fb->clear(GL_FRAMEBUFFER);
			glEnable(GL_DEPTH_TEST);
		}

		gLuaContext.loadFile("scripts/render.lua");

		gGameManager.drawToScreen( gFBOShader, *gLuaFrameBuffers.front() );
	}

	gGameManager.close();

	return 0;
}
```

`scripts/init.lua`:
```
package.path = package.path .. ";./scripts/?.lua;"

mainFrameBuffer = framebuffer.new( gameManager.getWindow() )

triangleShader = shader.new { name = "TriangleShader", 
			      vertex = "shaders/PolygonShader2D.vs", 
			      fragment = "shaders/PolygonShader2D.fs" }

triangle = mesh.new( { 
			{ { 0.0,0.5,  }, { 1, 0, 0, 1, } },  
			{ {-0.5,-0.5, }, { 0, 1, 0, 1, } },  
			{ { 0.5,-0.5, }, { 0, 0, 1, 1  } }
		      }, 
		      {})
					 
local width, height = gameManager.getWindow()

identityMatrix = { 
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1}
}

triangleShader:set("ProjectionMatrix", identityMatrix)
triangleShader:set("ViewMatrix", identityMatrix)
triangleShader:set("ModelMatrix", identityMatrix)

quitButton  = button.new("Escape")
```
Where the Lua scripts are in their own directory `scripts`.

`scripts/update.lua`:
```
if quitButton:pressed() then gameManager.quit() end
```

`scripts/render.lua`:
```
gameManager.draw( triangleShader, triangle, mainFrameBuffer )
```
