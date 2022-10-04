#include "GraphicRenderPipeLine.h"
#include "Tools.h"
#include <iostream>
#include<chrono>
int main()
{
	auto PipeLine = GraphicRenderPipeLine::CreateInstance();
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
