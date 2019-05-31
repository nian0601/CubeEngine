#include "stdafx.h"
#include "CUI_Dropbox.h"
#include "CUI_Button.h"
#include "CUI_Label.h"
#include "CUI_Image.h"
#include "CUI_Message.h"

CUI_Dropbox::CUI_Dropbox(const CE_String& aString)
	: myIsExpanded(false)
	, myFirstListIndex(2)
	, myMaxVisibleListCount(10)
{
	CUI_Button* button = new CUI_Button(aString);
	button->myOnClick = std::bind(&CUI_Dropbox::OnToggleExpansionClick, this);

	//CUI_Image* background = new CUI_Image({ 0.17f, 0.17f, 0.17f, 1.f });
	CUI_Image* background = new CUI_Image("panel");

	AddWidget(button);
	AddWidget(background);
}

void CUI_Dropbox::PrepareLayout()
{
	CE_Vector2f position = GetPosition();
	mySize = CE_Vector2f(0.f);

	CUI_Button* button = static_cast<CUI_Button*>(myWidgets[0]);
	CUI_Image* background = static_cast<CUI_Image*>(myWidgets[1]);

	button->SetPosition(position);
	button->PrepareLayout();

	mySize = button->GetSize();

	background->Hide();

	if (IsEmpty())
		return;
	
	position.y += mySize.y;
	background->SetPosition(position);
	position.x += 10.f;

	int iterations = myWidgets.Size() < myMaxVisibleListCount ? myWidgets.Size() : myMaxVisibleListCount;
	iterations += myFirstListIndex;

	CE_Vector2f totalChildSize;
	totalChildSize.x = mySize.x;
	for (int i = myFirstListIndex; i < iterations; ++i)
	{
		CUI_Widget* child = myWidgets[i];
		child->Show();
		if (!myIsExpanded)
			child->Hide();

		if (child->IsVisible())
		{
			child->SetPosition(position);
			child->PrepareLayout();

			position.y += child->GetSize().y;
			ExpandSize(child->GetSize(), totalChildSize);
		}
	}

	if (myIsExpanded && !IsEmpty())
	{
		background->Show();
		background->SetSize(totalChildSize);
	}
}

void CUI_Dropbox::Render(CE_RendererProxy& anRendererProxy)
{
	myWidgets[0]->Render(anRendererProxy);

	if (myIsExpanded)
	{
		myWidgets[1]->Render(anRendererProxy);

		int iterations = myWidgets.Size() < myMaxVisibleListCount ? myWidgets.Size() : myMaxVisibleListCount;
		iterations += myFirstListIndex;
		for (int i = myFirstListIndex; i < iterations; ++i)
			myWidgets[i]->Render(anRendererProxy);
	}
}

bool CUI_Dropbox::OnMouseUp(const CUI_MouseMessage& aMessage)
{
	if (!IsVisible())
		return false;

	if (myWidgets[0]->OnMouseUp(aMessage))
		return true;

	if (IsEmpty())
		return false;

	if (aMessage.myMouseButton == CUI_MouseMessage::MOUSE_LEFT)
	{
		CUI_Widget* clickedWidget = nullptr;
		int index = 0;

		int iterations = myWidgets.Size() < myMaxVisibleListCount ? myWidgets.Size() : myMaxVisibleListCount;
		iterations += myFirstListIndex;
		for (int i = myFirstListIndex; i < iterations; ++i)
		{
			CUI_Widget* widget = myWidgets[i];
			if (widget->OnMouseUp(aMessage) || widget->Contains(aMessage.myNewPosition))
			{
				clickedWidget = widget;
				index = i;
			}
		}

		myIsExpanded = false;
		if (clickedWidget != nullptr && myOnSelection != nullptr)
		{
			myOnSelection(clickedWidget, index - 2);
			return true;
		}
	}

	return CUI_Widget::OnMouseUp(aMessage);
}

bool CUI_Dropbox::OnMouseWheel(const CUI_MouseMessage& aMessage)
{
	if (myIsExpanded == false)
		return false;

	if (!myIsVisible && myIsFocused)
		return false;

	if (myWidgets[1]->Contains(aMessage.myNewPosition))
	{
		OnMouseMessage(aMessage);
		return true;
	}

	return false;
}

bool CUI_Dropbox::OnMouseMessage(const CUI_MouseMessage& aMessage)
{
	if (aMessage.myType != CUI_MouseMessage::MOUSE_WHEEL)
		return false;

	if (aMessage.myMouseWheelDelta < 0)
		--myFirstListIndex;
	else if (aMessage.myMouseWheelDelta > 0)
		++myFirstListIndex;

	if (myFirstListIndex < 2)
		myFirstListIndex = 2;
	
	if (myFirstListIndex + myMaxVisibleListCount > GetLabelCount())
		myFirstListIndex = GetLabelCount() - myMaxVisibleListCount + 2;

	return true;
}

void CUI_Dropbox::DeleteAllChildren()
{
	while (myWidgets.Size() > 2)
		myWidgets.DeleteCyclicAtIndex(2);

	myFirstListIndex = 2;
}

void CUI_Dropbox::AddLabel(const char* aString)
{
	CUI_Label* label = new CUI_Label(aString);
	AddWidget(label);
}

void CUI_Dropbox::ExpandSize(const CE_Vector2f& aNewSize, CE_Vector2f& aSizeOut) const
{
	if (aNewSize.x > aSizeOut.x)
		aSizeOut.x = aNewSize.x;

	aSizeOut.y += aNewSize.y;
}

void CUI_Dropbox::OnToggleExpansionClick()
{
	myIsExpanded = !myIsExpanded;
}
