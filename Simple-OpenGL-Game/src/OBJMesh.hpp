#ifndef OBJMESH_HPP
#define OBJMESH_HPP

#include <memory>

#include "Mesh.hpp"

struct OBJMesh : public Mesh
{
	typedef std::unique_ptr<OBJMesh> OBJMeshPtr;

					OBJMesh(std::string mName
							, std::string filename
							, ShadingType shading);

	virtual 		~OBJMesh(){};

	virtual bool 	loadMesh(std::string filename
								 , std::vector<glm::vec4> &verts
								 , std::vector<glm::vec3> &norms
								 , std::vector<glm::vec2> &uvs
								 , std::vector<GLushort> &eles);
};

#endif // OBJMESH_HPP
