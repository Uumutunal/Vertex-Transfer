// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include<vector>
#include"KDTree.h"
#include <cmath> 
#include<map>

// Entry point for the DLL application.
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // A process is loading the DLL.
	case DLL_THREAD_ATTACH:  // A thread is being created in the current process.
	case DLL_THREAD_DETACH:  // A thread exits cleanly.
	case DLL_PROCESS_DETACH: // A process unloads the DLL.
		break;
	}
	return TRUE; // Successful DLL_PROCESS_ATTACH.
}

// Node class represents a point in 3D space with an associated index.
class Node {
	float co_x; // x-coordinate
	float co_y; // y-coordinate
	float co_z; // z-coordinate
	int index;  // index of the node




public:
	void add(float _co_x, float _co_y, float _co_z, int _index); // Adds coordinates and index to the node
	int get_index(); // Returns the index of the node
	std::vector<float> get_co(); // Returns the coordinates of the node
};



// Adds coordinates and index to the node.
void Node::add(float _co_x, float _co_y, float _co_z, int _index) {
	co_x = _co_x;
	co_y = _co_y;
	co_z = _co_z;
	index = _index;
}

// Returns the index of the node.
int Node::get_index() {
	return index;
}

// Returns the coordinates of the node as a vector.
std::vector<float> Node::get_co() {
	return std::vector<float> {co_x, co_y, co_z};
}



//****************


std::map<int, std::vector<Node>> indexedDict_x;
std::map<int, std::vector<Node>> indexedDict_y;

std::map<int, float> mins_x;
std::map<int, float> mins_y;
std::map<int, float> maxs_x;
std::map<int, float> maxs_y;

extern "C" __declspec(dllexport) void initDict() {


	indexedDict_x.clear();
	indexedDict_y.clear();



	/*
	for (int i = 0; i < 10000; i++)
	{
		indexedDict_x[i] = std::vector<Node>{};
	}
	for (int i = 0; i < 10000; i++)
	{
		indexedDict_y[i] = std::vector<Node>{};
	}
	*/
}


extern "C" __declspec(dllexport) void addToDict(float co_x, float co_y, int index) {


	int index_x = (int)(co_x * 10000);
	int index_y = (int)(co_y * 10000);

	Node node_x;
	node_x.add(co_x, co_y, 0, index);
	Node node_y;
	node_y.add(co_x, co_y, 0, index);

	float min = FLT_MAX;
	float max = FLT_MIN;

	mins_x[index_y];

	if (index_x >= 0 || index_x < 10000) {
		indexedDict_x[index_x].push_back(node_x);

		if (mins_x.count(index_x) == 0) {
			mins_x[index_x] = co_x;
		}
		else if (co_x < mins_x[index_x]){
			mins_x[index_x] = co_x;
		}

		if (maxs_x.count(index_x) == 0) {
			maxs_x[index_x] = co_x;
		}
		else if (co_x > maxs_x[index_x]) {
			maxs_x[index_x] = co_x;
		}


	}
	if (index_y >= 0 || index_y < 10000) {
		indexedDict_y[index_y].push_back(node_y);

		if (mins_y.count(index_y) == 0) {
			mins_y[index_y] = co_y;
		}
		else if (co_y < mins_y[index_x]) {
			mins_y[index_y] = co_y;
		}

		if (maxs_x.count(index_y) == 0) {
			maxs_x[index_y] = co_x;
		}
		else if (co_y > maxs_y[index_y]) {
			maxs_y[index_y] = co_y;
		}
	}

}

extern "C" __declspec(dllexport) int findInDict(float co_x, float co_y) {

	int index_x = (int)(co_x * 10000);
	int index_y = (int)(co_y * 10000);


	std::vector<Node> fVector_x = indexedDict_x[index_x];
	std::vector<Node> fVector_y = indexedDict_y[index_y];

	//fVector_x.push_back(mins_x[index_x - 1]);

	std::vector<Node>::iterator iter;


	std::vector<Node> vClosest;
	std::vector<float> vDist;



	float min = FLT_MAX;

	Node closestNode;

	for (iter = fVector_x.begin(); iter != fVector_x.end(); iter++)
	{
		Node node = *iter;
		if (node.get_co()[0] - co_x < 0.00001) {

			if (node.get_co()[1] - co_y < 0.00001) {

				vClosest.push_back(node);
				//vDist.push_back((node.get_co()[0] - co_x) + (node.get_co()[1] - co_y));
				vDist.push_back(std::pow(node.get_co()[0] - co_x, 2) + std::pow(node.get_co()[1] - co_y, 2));

				float dist = std::pow(node.get_co()[0] - co_x, 2) + std::pow(node.get_co()[1] - co_y, 2);

				if (dist < min) {
					min = dist;
					closestNode = node;
				}

				//return node.get_index();
			}

		}
	}

	/*

	for (size_t i = 0; i < vClosest.size(); i++)
	{
		if (vDist[i] < min) {
			min = vDist[i];
			closestNode = vClosest[i];
		}
	}
	*/

	return closestNode.get_index();

	/*
	std::vector<float>::iterator iter;

	for (iter = fVector_y.begin(); iter != fVector_y.end(); iter++)
	{
		Node node = *iter;
		if (node.get_co()[1] - co_y < 0.01) {
			return co;
		}
	}
	*/

	//return -1;

}



//****************




// Global KD-tree to store nodes.
std::vector<Node> kdTree;

// Initializes the KD-tree (clears it).
extern "C" __declspec(dllexport) void init() {
	kdTree.clear();
}

// Inserts a new node into the KD-tree.
extern "C" __declspec(dllexport) void insert(float _co_x, float _co_y, float _co_z, int index) {
	Node node;
	node.add(_co_x, _co_y, _co_z, index);
	kdTree.push_back(node);
}

// Finds and returns the index at the specified position in the KD-tree.
extern "C" __declspec(dllexport) int findAt(int index) {
	return kdTree.at(index).get_index();
}

// Finds the index of the node closest to the given coordinates (within a small threshold).
extern "C" __declspec(dllexport) int find(float _co_x, float _co_y, float _co_z) {
	std::vector<float> co = { _co_x, _co_y, _co_z };

	for (int i = 0; i < kdTree.size() - 1; i++) {
		// Calculate the distance between the current node and the given coordinates.
		float dist = sqrt(pow((kdTree.at(i).get_co().at(0) - co.at(0)), 2) + pow(kdTree.at(i).get_co().at(1) - co.at(1), 2));

		// Check if the distance is within the threshold.
		if (fabs(dist) < 0.1) {
			return i;
		}
	}
	return -1; // Return -1 if no node is found within the threshold.
}

// Finds the index of the node closest to the given coordinates.
extern "C" __declspec(dllexport) int dist(float _co_x, float _co_y, float _co_z) {
	std::vector<float> co = { _co_x, _co_y, _co_z };
	std::vector<float> dists; // Stores distances from each node to the given coordinates.
	std::vector<Node> index;  // Stores the nodes for easy access.
	int ind = -1;             // Initialize index to -1 (not found).

	// Calculate distances from each node to the given coordinates.
	for (int i = 0; i < kdTree.size(); i++) {
		float dist = sqrt(pow((kdTree.at(i).get_co().at(0) - co.at(0)), 2) + pow(kdTree.at(i).get_co().at(1) - co.at(1), 2));
		dists.push_back(dist);
		index.push_back(kdTree.at(i));
	}

	float min = FLT_MAX; // Initialize minimum distance to maximum float value.

	// Find the node with the minimum distance.
	for (int i = 0; i < dists.size(); i++) {
		if (dists.at(i) < min) {
			min = dists.at(i);
			ind = index.at(i).get_index();
		}
	}

	return ind; // Return the index of the node with the minimum distance.
}
