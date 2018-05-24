#include "stdafx.h"
#include "CUI_Dropbox.h"
#include "CUI_Button.h"
#include "CUI_Label.h"
#include "CUI_Image.h"
#include "CUI_Message.h"

CUI_Dropbox::CUI_Dropbox(const CE_Font& aFont, const CE_String& aString)
	: myIsExpanded(false)
	, myFont(aFont)
{
	CUI_Button* button = new CUI_Button(myFont, aString);
	button->myOnClick = std::bind(&CUI_Dropbox::OnToggleExpansionClick, this);

	CUI_Image* background = new CUI_Image({ 0.17f, 0.17f, 0.17f, 1.f });

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
	
	position.x += 10.f;
	position.y += mySize.y;
	background->SetPosition(position);

	CE_Vector2f totalChildSize;
	for (int i = 2; i < myWidgets.Size(); ++i)
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
		myWidgets[1]->Show();
		myWidgets[1]->SetSize(totalChildSize);
	}
}

void CUI_Dropbox::Render(CE_RendererProxy& anRendererProxy)
{
	myWidgets[0]->Render(anRendererProxy);

	if (myIsExpanded)
	{
		for (int i = 1; i < myWidgets.Size(); ++i)
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
		for (int i = 2; i < myWidgets.Size(); ++i)
		{
			CUI_Widget* widget = myWidgets[i];
			if (widget->OnMouseUp(aMessage) || widget->Contains(aMessage.myNewPosition))
				clickedWidget = widget;
		}

		myIsExpanded = false;
		if (clickedWidget != nullptr && myOnSelection != nullptr)
		{
			myOnSelection(clickedWidget);
			return true;
		}
	}

	return CUI_Widget::OnMouseUp(aMessage);
}

void CUI_Dropbox::DeleteAllChildren()
{
	while (myWidgets.Size() > 2)
		myWidgets.DeleteCyclicAtIndex(2);
}

void CUI_Dropbox::AddLabel(const char* aString)
{
	CUI_Label* label = new CUI_Label(myFont, aString);
	myWidgets.Add(label);
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
