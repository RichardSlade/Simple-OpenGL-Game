#include <iostream>

#include "Mesh.hpp"

void Mesh::flatShading(std::vector<glm::vec4> &verts
						 , std::vector<glm::vec3> &norms
						 , std::vector<GLushort> &eles)
{
	std::cout << "Flat shading: " << meshName << std::endl;

	// Flat shading normals
	for (unsigned int i = 0; i < eles.size(); i++)
	{

		if ((i % 3) == 2)
		{
		  GLushort ia = eles[i-2];
		  GLushort ib = eles[i-1];
		  GLushort ic = eles[i];

		  glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(verts[ic] - verts[ia]),
			glm::vec3(verts[ib] - verts[ia])));

			norms[ia] = norms[ib] = norms[ic] = normal;
		}
	}
}

void Mesh::gouraudShading(std::vector<glm::vec4> &verts
							, std::vector<glm::vec3> &norms
							, std::vector<GLushort> &eles)
{
	std::cout << "Gouraud shading: " << meshName << std::endl;

	// Gouraud shading normals
	for (unsigned int i = 0; i < eles.size(); i += 3)
	{
		GLushort ia = eles[i];
		GLushort ib = eles[i+1];
		GLushort ic = eles[i+2];

		glm::vec3 avgNorm = glm::vec3(verts.at(ia) + verts.at(ib) + verts.at(ic));
		avgNorm *= 0.3;

		norms[ia] = norms[ib] = norms[ic] = -avgNorm;
	}

	std::vector<int> nb_seen;
	nb_seen.resize(verts.size(), 0);

	for (unsigned int i = 0; i < eles.size(); i+=3)
	{
		GLushort ia = eles[i];
		GLushort ib = eles[i+1];
		GLushort ic = eles[i+2];

		glm::vec3 normal = glm::normalize(glm::cross(
		glm::vec3(verts[ib]) - glm::vec3(verts[ia]),
		glm::vec3(verts[ic]) - glm::vec3(verts[ia])));

		glm::vec3 avgNorm = -glm::vec3(verts.at(ia) + verts.at(ib) + verts.at(ic));
		avgNorm *= 0.3;

		int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;

		for (int j = 0; j < 3; j++)
		{
			GLushort cur_v = v[j];
			nb_seen[cur_v]++;

			if(nb_seen[cur_v] == 1)
			{
				normals[cur_v] = avgNorm;
			}
			else
			{
				// average
				norms[cur_v].x = norms[cur_v].x * (1.0 - 1.0/nb_seen[cur_v]) + avgNorm.x * 1.0/nb_seen[cur_v];
				norms[cur_v].y = norms[cur_v].y * (1.0 - 1.0/nb_seen[cur_v]) + avgNorm.y * 1.0/nb_seen[cur_v];
				norms[cur_v].z = norms[cur_v].z * (1.0 - 1.0/nb_seen[cur_v]) + avgNorm.z * 1.0/nb_seen[cur_v];
				norms[cur_v] = glm::normalize(norms[cur_v]);
			}
		}
	}
}

void Mesh::phongShading(std::vector<glm::vec4> &verts
						  , std::vector<glm::vec3> &norms
						  , std::vector<GLushort> &eles)
{
	std::cout << "Phong shading: " << meshName << std::endl;

//	normals.resize(verts.size(), glm::vec3(0.0, 0.0, 0.0));

//	std::vector<int> nb_seen;
//	nb_seen.resize(verts.size(), 0);
//
//	for (int i = 0; i < eles.size(); i+=3)
//	{
//		GLushort ia = eles[i];
//		GLushort ib = eles[i+1];
//		GLushort ic = eles[i+2];
//
//		glm::vec3 normal = glm::normalize(glm::cross(
//		glm::vec3(verts[ib]) - glm::vec3(verts[ia]),
//		glm::vec3(verts[ic]) - glm::vec3(verts[ia])));
//
//		glm::vec3 avgNorm = -glm::vec3(verts.at(ia) + verts.at(ib) + verts.at(ic));
//		avgNorm *= 0.3;
//
//		int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
//
//		for (int j = 0; j < 3; j++)
//		{
//			GLushort cur_v = v[j];
//			nb_seen[cur_v]++;
//
//			if(nb_seen[cur_v] == 1)
//			{
//				normals[cur_v] = avgNorm;
//			}
//			else
//			{
//				// average
//				norms[cur_v].x = norms[cur_v].x * (1.0 - 1.0/nb_seen[cur_v]) + avgNorm.x * 1.0/nb_seen[cur_v];
//				norms[cur_v].y = norms[cur_v].y * (1.0 - 1.0/nb_seen[cur_v]) + avgNorm.y * 1.0/nb_seen[cur_v];
//				norms[cur_v].z = norms[cur_v].z * (1.0 - 1.0/nb_seen[cur_v]) + avgNorm.z * 1.0/nb_seen[cur_v];
//				norms[cur_v] = glm::normalize(norms[cur_v]);
//			}
//		}
//	}
}
