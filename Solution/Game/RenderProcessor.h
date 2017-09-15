#pragma once

#include <CE_BaseProcessor.h>
class CE_RendererProxy;
class RenderProcessor : public CE_BaseProcessor
{
public:
	RenderProcessor(CE_World& aWorld, CE_RendererProxy& aRendererProxy);
	~RenderProcessor();

	void Update(float aDelta) override;

private:
	CE_RendererProxy& myRendererProxy;
};

