#pragma once

#include "CN_Node.h"

class CN_Position1Node : public CN_Node
{
	CN_DEFINE_NODE(CN_Position1Node);

public:
	CN_Position1Node();

	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myOutputPin;
};

class CN_Position2Node : public CN_Node
{
	CN_DEFINE_NODE(CN_Position2Node);

public:
	CN_Position2Node();

	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myOutputPin;
};

class CN_Position3Node : public CN_Node
{
	CN_DEFINE_NODE(CN_Position3Node);

public:
	CN_Position3Node();

	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myOutputPin;
};

class CN_Position4Node : public CN_Node
{
	CN_DEFINE_NODE(CN_Position4Node);

public:
	CN_Position4Node();

	void Execute(const CN_Pin& anExecutingPin) override;

private:
	CN_Pin* myOutputPin;
};