#pragma once

enum eBTActionState
{
	IN_PROGRESS,
	FINISHED,
	FAILED,
};
class CE_BTNode
{
public:
	virtual ~CE_BTNode() {};

	virtual eBTActionState Update() = 0;

protected:
	CE_BTNode() {};
};

