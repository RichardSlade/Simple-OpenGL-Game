#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <map>

#include "OBJMesh.hpp"

typedef std::tuple<int, int, int> vertexTuple;
typedef std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> vertexTupleKey;

OBJMesh::OBJMesh(std::string mName
				, std::string filename
				, ShadingType shading)
: Mesh(mName)
{
	if(!loadMesh(filename
				, vertices
				, normals
				, texCords
				, elements))
	{throw std::runtime_error("Error loading mesh: " + mName);}

	switch(shading)
	{
		case ShadingType::Flat: flatShading(vertices, normals, elements); break;
		case ShadingType::Gouraud: gouraudShading(vertices, normals, elements); break;
		case ShadingType::Phong: phongShading(vertices, normals, elements); break;
		default: break;
	}

	vertNum = vertices.size();
	elementNum = elements.size();
	normalNum = normals.size();
	texCordNum = texCords.size();
}

bool OBJMesh::loadMesh(std::string filename
					  , std::vector<glm::vec4> &verts
					  , std::vector<glm::vec3> &norms
					  , std::vector<glm::vec2> &uvs
					  , std::vector<GLushort> &eles)
{
	std::ifstream in(filename, std::ios::in);

	if(!in.is_open())
	{
		std::cerr << "Error opening " << filename << std::endl;
		return false;
	}

	std::vector<vertexTuple> vertexTuples;

	std::vector<GLushort> tmpNormElements;
	std::vector<GLushort> tmpUVElements;

	std::vector<glm::vec4> sharedVertices;
	std::vector<glm::vec3> sharedNorms;
	std::vector<glm::vec2> sharedUVs;

	std::string line;

	while(std::getline(in, line))
	{
		std::string dataType = line.substr(0, 2);

		if(dataType == "v ") // If vertex
		{
			std::istringstream s(line.substr(2));
			glm::vec4 vert;

			s >> vert.x;
			s >> vert.y;
			s >> vert.z;
			vert.w = 1.0f;

			sharedVertices.push_back(vert);
		}
		else if(dataType == "vt")
		{
			std::istringstream ss(line.substr(2));
			glm::vec2 uv;

			ss >> uv.x;
			ss >> uv.y;

			sharedUVs.push_back(uv);
		}
		else if(dataType == "vn")
		{
			std::istringstream ss(line.substr(2));
			glm::vec3 norm;

			ss >> norm.x;
			ss >> norm.y;
			ss >> norm.z;

			sharedNorms.push_back(norm);
		}
		else if(dataType == "f ") // If element
		{
			std::string parseStr = line.substr(2);

			for(int i = 0; i < 3; i++)
			{
				int pos = parseStr.find(" ");
				char delim = '/';

				std::vector<GLushort> values;
				std::string item;
				std::stringstream ss(parseStr.substr(0, pos));

				while(std::getline(ss, item, delim))
					values.push_back(std::atoi(item.c_str()));

				for(int i = 0; i < 3; i++)
					values.at(i) --;

				vertexTuple tup(values.at(0)
								  , values.at(1)
								  , values.at(2));

				vertexTuples.push_back(tup);

				parseStr = parseStr.substr(pos + 1);
			}
		}
	}

	if(elements.size() % 3 != 0)
	{
		throw std::runtime_error("Mesh elements must be a multiple of 3\nCurrently:" + elements.size());
		return false;
	}

	std::map<vertexTupleKey, int> vertexMap;
	std::map<vertexTupleKey, int>::iterator it;

	int nextVertex = 0;
	int newIndex;

	for(unsigned int i = 0; i < vertexTuples.size(); i++)
	{
		glm::vec4 tmpV = sharedVertices.at(std::get<0>(vertexTuples.at(i)));
		glm::vec2 tmpT = sharedUVs.at(std::get<1>(vertexTuples.at(i)));
		glm::vec3 tmpN = sharedNorms.at(std::get<2>(vertexTuples.at(i)));

		std::vector<float> v = {tmpV.x, tmpV.y, tmpV.z, tmpV.w}
							, t = {tmpT.x, tmpT.y}
							, n = {tmpN.x, tmpN.y, tmpN.z};

		vertexTupleKey key(v, t, n);

		it = vertexMap.find(key);

		if(it != vertexMap.end())
		{
			newIndex = it->second;
		}
		else
		{
			verts.push_back(tmpV);
			uvs.push_back(tmpT);
			norms.push_back(-tmpN);

			newIndex = nextVertex;
			vertexMap[key] = newIndex;

			nextVertex ++;
		}

		eles.push_back(newIndex);
	}

	return true;
}


