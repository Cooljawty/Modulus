#pragma once
template<class Obj>
class ObjManager{
	public:
		ObjManager();
		~ObjManager();
		
		//Craate a new instance of the object
		void addInstance(Obj*);

		//Delete instance from manager
		void freeInstance(Obj*);
		
		//Accesss instance from list
		Obj* getInstance(Obj*);
		
	private:
		std::vector<Obj*> Instances;
};

template <typename T>
class MeshManager : public ObjManager<Mesh>{
	public:
		//Add an instance of a mesh
		void addMesh(Mesh*);

		void render();
};
