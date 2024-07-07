// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include<vector>
#include"KDTree.h"
#include <cmath> 


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}




class Node {

	float co_x;
	float co_y;
	float co_z;
	int index;

	public:
		void add(float _co_x, float _co_y, float _co_z, int _index);
		int get_index();
		std::vector<float> get_co();

};





void Node::add(float _co_x, float _co_y, float _co_z, int _index) {
	co_x = _co_x;
	co_y = _co_y;
	co_z = _co_z;
	index = _index;
}


int Node::get_index() {

	return index;
}

std::vector<float> Node::get_co() {
	return std::vector<float> {co_x, co_y, co_z};
}



std::vector<Node> kdTree;

extern "C" __declspec(dllexport) void init() {

	kdTree.clear();
}

extern "C" __declspec(dllexport) void insert(float _co_x, float _co_y, float _co_z, int index) {

	Node node;
	node.add(_co_x, _co_y, _co_z, index);
	kdTree.push_back(node);
}

extern "C" __declspec(dllexport) int findAt(int index) {

	return kdTree.at(index).get_index();
}


extern "C" __declspec(dllexport) int find(float _co_x, float _co_y, float _co_z) {

	std::vector<float> co = { _co_x, _co_y, _co_z };

	for (int i = 0; i < kdTree.size() - 1; i++)
	{
		
		float dist = sqrt(pow((kdTree.at(i).get_co().at(0) - co.at(0)), 2) + pow(kdTree.at(i).get_co().at(1) - co.at(1), 2));

		if (fabs(dist) < 0.1)
		{
			return i;
		}

	}
	return -1;

}

extern "C" __declspec(dllexport) int dist(float _co_x, float _co_y, float _co_z) {

	std::vector<float> co = { _co_x, _co_y, _co_z };

	std::vector<float> dists;
	std::vector<Node> index;
	int ind = -1;

	for (int i = 0; i < kdTree.size(); i++)
	{

		float dist = sqrt(pow((kdTree.at(i).get_co().at(0) - co.at(0)), 2) + pow(kdTree.at(i).get_co().at(1) - co.at(1), 2));
		dists.push_back(dist);
		index.push_back(kdTree.at(i));
	}


	float min = FLT_MAX;

	for (int i = 0; i < dists.size(); i++)
	{
		if (dists.at(i) < min) {
			min = dists.at(i);
			ind = index.at(i).get_index();
		}
	}

	return ind;

}



