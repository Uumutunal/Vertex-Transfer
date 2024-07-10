#pragma once
extern "C" __declspec(dllexport) void insert(float _co_x, float _co_y, float _co_z, int index);
extern "C" __declspec(dllexport) int find(float _co_x, float _co_y, float _co_z);
extern "C" __declspec(dllexport) int findAt(int index);
extern "C" __declspec(dllexport) int dist(float _co_x, float _co_y, float _co_z);
extern "C" __declspec(dllexport) void init();


extern "C" __declspec(dllexport) void initDict();
extern "C" __declspec(dllexport) void addToDict(float co_x, float co_y, int index);
extern "C" __declspec(dllexport) int findInDict(float co_x, float co_y);
