#pragma once
#include <memory>
class GraphicRenderPipeLine
{
	using GraphicRenderPipeLinePtr = std::shared_ptr<GraphicRenderPipeLine>;
public:
	static GraphicRenderPipeLinePtr CreateInstance();

private:
	GraphicRenderPipeLine();
	GraphicRenderPipeLine operator=(GraphicRenderPipeLine& e) = delete;
	GraphicRenderPipeLine(GraphicRenderPipeLine& e) = delete;
private:
	
};

