#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Mesh
{
	enum ShadingType{Flat, Gouraud, Phong};

	std::string 			meshName;
	GLuint 			vertNum;
	GLuint 			normalNum;
	GLuint 			texCordNum;
	GLuint 			elementNum;

	std::vector<glm::vec4> 	vertices;
	std::vector<glm::vec3> 	normals;
	std::vector<glm::vec2> 	texCords;
	std::vector<GLushort> 	elements;

							Mesh(std::string mName)
							: meshName(mName)
							{};

	virtual bool 			loadMesh(std::string filename
									, std::vector<glm::vec4> &vert
									, std::vector<glm::vec3> &norm
									, std::vector<glm::vec2> &uv
									, std::vector<GLushort> &elements) = 0;

	void					flatShading(std::vector<glm::vec4> &verts
										, std::vector<glm::vec3> &norms
										, std::vector<GLushort> &eles);

	void					gouraudShading(std::vector<glm::vec4> &verts
										, std::vector<glm::vec3> &norms
										, std::vector<GLushort> &eles);

	void					phongShading(std::vector<glm::vec4> &verts
										, std::vector<glm::vec3> &norms
										, std::vector<GLushort> &eles);
};

#endif // MESH_HPP
