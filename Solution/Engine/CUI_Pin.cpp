#include "stdafx.h"
#include "CUI_Pin.h"
#include "CE_RendererProxy.h"

#include "CE_DebugDraw.h"
#include "CUI_Message.h"

CUI_Pin::CUI_Pin(bool aIsInput, const CE_Vector2f& aSize, const CE_Vector4f& aColor)
	: myColor(aColor)
	, myIsInput(aIsInput)
	, myHasStartedDrag(false)
{
	mySize = aSize;
}

void CUI_Pin::Render(CE_RendererProxy& anRendererProxy)
{
	CE_Vector4f color = myColor;
	if (myIsFocused)
		color.x += 0.2f;
	else if (myHasLongPress)
		color.y += 0.1f;
	else if (myIsHovered)
		color.z += 0.1f;

	anRendererProxy.AddSprite(myPosition, mySize, color);

	if (myHasStartedDrag)
	{
		CE_Vector2f position = myPosition;
		position += mySize * 0.5f;
		RenderSteppedLine(anRendererProxy, position, myMousePosition, 0.5f);
	}
}

bool CUI_Pin::OnDragMessage(CUI_DragMessage& aMessage)
{
	if (aMessage.myType == CUI_DragMessage::DRAG_START)
	{
		if (myIsInput && myConnections.Size() > 0)
		{
			myConnections[0]->myConnections.RemoveCyclic(this);
			myConnections[0]->OnDragMessage(aMessage);
			myConnections.RemoveAll();
		}
		else
		{
			aMessage.myData.Set(this);
			myHasStartedDrag = true;
		}

		return true;
	}

	if (aMessage.myType == CUI_DragMessage::DRAG_END)
	{
		CE_Any& data = aMessage.myData;

		if (data.Is<CUI_Pin*>())
		{
			CUI_Pin* pin = data.Get<CUI_Pin*>();

			if (myIsInput && !pin->myIsInput)
			{
				if (myConnections.Size() == 0)
				{
					myConnections.Add(pin);
					pin->myConnections.Add(this);
				}
			}
			else if (pin->myIsInput && !myIsInput)
			{
				if (pin->myConnections.Size() == 0)
				{
					pin->myConnections.Add(this);
					myConnections.Add(pin);
				}
			}
		}

		myHasStartedDrag = false;
		return true;
	}

	return false;
}

bool CUI_Pin::OnMouseMessage(const CUI_MouseMessage& aMessage)
{
	myMousePosition = aMessage.myNewPosition;
	return false;
}

void CUI_Pin::OnLostFocus()
{
	myHasStartedDrag = false;
}

void CUI_Pin::RenderSteppedLine(CE_RendererProxy& anRendererProxy, const CE_Vector2f& aStartPos, const CE_Vector2f& aEndPos, float aCutPoint)
{
	float cutPoint = aStartPos.x + (aEndPos.x - aStartPos.x) * aCutPoint;

	CE_Vector2f cutPointA = aStartPos;
	cutPointA.x = cutPoint;

	CE_Vector2f cutPointB = aEndPos;
	cutPointB.x = cutPoint;

	anRendererProxy.Add2DLine(aStartPos, cutPointA);
	anRendererProxy.Add2DLine(cutPointA, cutPointB);
	anRendererProxy.Add2DLine(cutPointB, aEndPos);
}
