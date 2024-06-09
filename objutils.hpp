#pragma once
#include "vec2.hpp"
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct MeshFace {
	vector<unsigned int> vertexIndices;
};

class MeshObject {
public:
	string name;
	vector<unsigned int> vertexIndices;
	vector<MeshFace*> faces; //edges are faces

	MeshObject() {
		name = "MESH";
		vertexIndices = vector<unsigned int>();
		faces = vector<MeshFace*>();
	}

	~MeshObject() {
		for (MeshFace* face : faces) delete face;
		faces.clear();
	}

	void makeSingleFace() {
		for (MeshFace* face : faces) delete face;
		faces.clear();
		MeshFace* newFace = new MeshFace();
		newFace->vertexIndices = vector<unsigned int>(this->vertexIndices);
		faces.push_back(newFace);
	}
};

class Mesh {
public:
	string name;
	vector<vec2> vertices;
	vector<MeshObject*> objects; //edges are faces

	Mesh() {}
	Mesh(string nameIn) { name = nameIn; }
	Mesh(vector<vec2>* points) {
		objects = vector<MeshObject*>();
		vertices = vector<vec2>(*points);
		MeshObject* newObject = new MeshObject();
		for (int i = 0; i < points->size(); i++) {
			newObject->vertexIndices.push_back(i);
		}
		objects.push_back(newObject);
		//cout << "objects: ";
		//for (auto i : objects)
		//	std::cout << i << ' ';
		//cout << endl;
	}

	~Mesh() {
		for (MeshObject* object : objects) delete object;
		objects.clear();
	}

	vector<vec2>* getVertices() { return &vertices; }
	void makeObjectsSingleFace() {
		for (MeshObject* object : objects) {
			object->makeSingleFace();
		}
	}
};

class ObjUtils {
public:

	// filename é formatado como "<nome>.obj"
	static Mesh* loadMesh(string filename) {
		string filepath = "assets/" + filename;
		cout << "loading obj from filepath: " << filepath << endl;
		Mesh* mesh = new Mesh();

		ifstream inFile(filepath);
		if (!inFile.is_open()) {
			cerr << "Erro ao abrir o arquivo." << endl;
			return mesh;
		}

		string line;
		MeshObject* currentObject = nullptr;
		unsigned int vertexCounter = 1; //vertices are 1-indexed. oof.
		// toda vez ao salvar um indice, subtrair um para tornar 0-indexed.

		while (std::getline(inFile, line)) {
			cout << "line: " << line << endl;
			if (line.empty() || line[0] == '#') continue;

			istringstream iss(line);
			string token;
			iss >> token;

			if (token == "o") {
				string name = line.substr(2); // exclui
				currentObject = new MeshObject();
				mesh->objects.push_back(currentObject);
				// pegar nome
				
				currentObject->name = name;
				cout << currentObject->name << endl;
				continue;
			}
			if (currentObject == nullptr) continue;

			if (token == "v") {
				double x, y;
				iss >> x; iss >> y;
				vec2 point(x, y);
				//currentMesh->addVertex(point);
				mesh->vertices.push_back(point);
				currentObject->vertexIndices.push_back(vertexCounter - 1);
				vertexCounter += 1;
			}

			if (token == "f") {
				MeshFace* face = new MeshFace();
				while (!iss.eof()) { //while not reached End of File
					string faceStr;
					iss >> faceStr;
					string vertexIndexStr = faceStr.substr(0, faceStr.find('/'));
					istringstream viiss(vertexIndexStr);
					int vertexIndex;
					viiss >> vertexIndex;
					face->vertexIndices.push_back(vertexIndex - 1);
				}
				currentObject->faces.push_back(face);
			}

			continue;

		}
		inFile.close();
		return mesh;
	}

	// filename é formatado como "<nome>.obj"
	static int saveMesh(Mesh* mesh, string filename) {
		string filepath = "output/" + filename;
		cout << "saving obj to filepath: " << filepath << endl;

		ofstream outFile(filepath);
		if (!outFile.is_open()) {
			cerr << "Erro ao abrir o arquivo. Talvez a pasta não exista ou esteja inacessível." << endl;
			return 1;
		}

		for (MeshObject* currentObject : mesh->objects) {
			outFile << "o " << currentObject->name << endl;

			for (unsigned int vertexIndex : currentObject->vertexIndices) {
				vec2 vertex = mesh->vertices.at(vertexIndex);
				outFile << "v " << vertex.x << " " << vertex.y << " 0.0" << endl;
			}

			for (MeshFace* face : currentObject->faces) {
				outFile << "f ";
				for (int vertexIndex : face->vertexIndices) {
					outFile << vertexIndex + 1 << " ";
				}
				outFile << endl;
			}
		}

		outFile.close();
		return 0;
	}

};
