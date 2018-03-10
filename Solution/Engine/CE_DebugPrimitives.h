#pragma once

struct CE_Line
{
	CE_Line() {}
	CE_Line(const CE_Vector3f& aStart, const CE_Vector3f& aEnd)
		: CE_Line(aStart, aEnd, CE_Vector4f(1.f, 1.f, 1.f, 1.f))
	{}

	CE_Line(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, float aHeight)
		: CE_Line(aStart, aEnd, aHeight, CE_Vector4f(1.f, 1.f, 1.f, 1.f))
	{}

	CE_Line(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, const CE_Vector4f& aColor)
		: CE_Line(aStart, aEnd, aColor, aColor)
	{}

	CE_Line(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, float aHeight, const CE_Vector4f& aColor)
		: CE_Line(aStart, aEnd, aHeight, aColor, aColor)
	{}

	CE_Line(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, const CE_Vector4f& aStartColor, const CE_Vector4f& aEndColor)
		: myStart(aStart)
		, myEnd(aEnd)
		, myStartColor(aStartColor)
		, myEndColor(aEndColor)
	{}

	CE_Line(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, float aHeight, const CE_Vector4f& aStartColor, const CE_Vector4f& aEndColor)
		: myStart(aStart)
		, myEnd(aEnd)
		, myStartColor(aStartColor)
		, myEndColor(aEndColor)
	{
		myStart.y += aHeight;
		myEnd.y += aHeight;
	}

	CE_Vector3f myStart;
	CE_Vector3f myEnd;

	CE_Vector4f myStartColor;
	CE_Vector4f myEndColor;
};