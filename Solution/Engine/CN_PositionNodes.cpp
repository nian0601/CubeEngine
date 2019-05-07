#include "stdafx.h"
#include "CN_PositionNodes.h"


CN_Position1Node::CN_Position1Node()
{
	myOutputPin = AddPin<CE_Vector2f>(3, false, "Out");
}

void CN_Position1Node::Execute(const CN_Pin& /*anExecutingPin*/)
{
	myOutputPin->Write(CE_Vector2f(0.f, 0.f));
}

CN_Position2Node::CN_Position2Node()
{
	myOutputPin = AddPin<CE_Vector2f>(3, false, "Out");
}

void CN_Position2Node::Execute(const CN_Pin& /*anExecutingPin*/)
{
	myOutputPin->Write(CE_Vector2f(10.f, 0.f));
}

CN_Position3Node::CN_Position3Node()
{
	myOutputPin = AddPin<CE_Vector2f>(3, false, "Out");
}

void CN_Position3Node::Execute(const CN_Pin& /*anExecutingPin*/)
{
	myOutputPin->Write(CE_Vector2f(10.f, 10.f));
}

CN_Position4Node::CN_Position4Node()
{
	myOutputPin = AddPin<CE_Vector2f>(3, false, "Out");
}

void CN_Position4Node::Execute(const CN_Pin& /*anExecutingPin*/)
{
	myOutputPin->Write(CE_Vector2f(0.f, 10.f));
}