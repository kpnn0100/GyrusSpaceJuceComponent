#include "XyzPad.h"

namespace Gui
{
	XyzPad::Thumb::Thumb()
	{
	}
	//XyzPad
	XyzPad::XyzPad()
	{

	}
	void XyzPad::registerSlider(Slider* slider, int axis)
	{
		slider->addListener(this);
		const std::lock_guard<std::mutex> lock(mutex);
		sliderList[axis].push_back(slider);
		forceSyncWithSlider();
	}
	void XyzPad::deregisterSlider(Slider* slider)
	{
		slider->removeListener(this);
		const std::lock_guard<std::mutex> lock(mutex);
		for (int i = 0; i < 3; i++)
		{
			sliderList[i].erase(std::remove(sliderList[i].begin(), sliderList[i].end(), slider), sliderList[i].end());
		}
	}
	void XyzPad::updateView()
	{
		boxDimension[0] = (1 - zRatio) * getWidth();
		boxDimension[1] = (1 - zRatio) * getHeight();
		boxDimension[2] = zRatio * getHeight();
		thumb.setSize(thumbWidth,thumbWidth);
		Point<int> newThumbPosition = valueToCoordinate(mValue[0], mValue[1], mValue[2]) - Point<int>(thumbWidth / 2, thumbWidth / 2);
		thumb.setTopLeftPosition(newThumbPosition);
		repaint();
	}
	float XyzPad::getValue(int dimension)
	{
		return mValue[dimension];
	}
	void XyzPad::setBackgroundColor(Colour backgroundColor)
	{
		mBackgroundColor = backgroundColor;
	}
	void XyzPad::setBorderColor(Colour borderColor)
	{
		mBorderColor = borderColor;
	}
	void XyzPad::setAxisColor(Colour x, Colour y, Colour z)
	{
		mXColor = x;
		mYColor = y;
		mZColor = z;
	}
	void XyzPad::sliderValueChanged(Slider* slider)
	{
		if (this->isMouseOverOrDragging(false))
			return;
		int whichAxis;
		for (int i = 0; i < 3; i++)
		{
			if (std::find(sliderList[i].begin(), sliderList[i].end(), slider) != sliderList[i].end())
			{
				whichAxis = i;
				break;
			}
		}
		mValue[whichAxis] = jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), (double)minValue[whichAxis], (double)maxValue[whichAxis]);
		
		updateView();
	}
	void XyzPad::valueChanged()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto* slider : sliderList[i])
			{
				slider->setValue(jmap(mValue[i], minValue[i], maxValue[i], (float)slider->getMinimum(), (float)slider->getMaximum()));
			}
		}
		updateView();
	}
	void XyzPad::forceSyncWithSlider()
	{
		for (int i = 0; i < 3; i++)
		{
			if (!sliderList[i].empty())
				sliderValueChanged(sliderList[i][0]);
		}
	}
	Point<int> XyzPad::valueToCoordinate(float x, float y, float z)
	{

		auto xyBound = Rectangle<float>(0, getYfromBottom(boxDimension[1]), boxDimension[0], boxDimension[1]);
		float zToX = boxDimension[2]*
			(z - minValue[2]) / (maxValue[2] - minValue[2]);
		float zToY = boxDimension[2] *
			(z - minValue[2]) / (maxValue[2] - minValue[2]);
		float coordinateX = zToX +  xyBound.getX() + (x - minValue[0]) / (maxValue[0] - minValue[0])* (xyBound.getTopRight().getX() - xyBound.getX());
		float coordinateY = jmap(y,minValue[1],maxValue[1], xyBound.getBottom(), xyBound.getBottom()-boxDimension[1]) - zToY;
		return Point<int>((int)coordinateX,(int)coordinateY);
	}

	void XyzPad::mouseDown(const MouseEvent& event)
	{
		mDragPositionStart = event.getMouseDownPosition().toFloat();
	}

	void XyzPad::mouseDrag(const MouseEvent& event)
	{
		Point<float> newPosition = event.getMouseDownPosition().toFloat() + event.getOffsetFromDragStart().toFloat();
		Point<float> travelVector = newPosition - mDragPositionStart;
		if (ModifierKeys::getCurrentModifiers().isShiftDown())
		{
			mValue[2] += jmap(travelVector.getX() / boxDimension[0], -1.0f, 1.0f, minValue[0], maxValue[0]);
			mValue[2] += jmap(-travelVector.getY() / boxDimension[1], -1.0f, 1.0f, minValue[1], maxValue[1]);
		}
		else
		{
			
			mValue[0] += jmap(travelVector.getX() / boxDimension[0], -0.5f, 0.5f, minValue[0], maxValue[0]);
			mValue[1] += jmap(-travelVector.getY() / boxDimension[1], -0.5f, 0.5f, minValue[1], maxValue[1]);
		}
		for (int i = 0; i < 3; i++)
		{
			mValue[i] = std::min(mValue[i], 1.0f);
			mValue[i] = std::max(mValue[i], -1.0f);
		}
		valueChanged();

		mDragPositionStart = newPosition;
	}

	float XyzPad::getYfromBottom(float y)
	{
		return getHeight()-y;
	}

	void XyzPad::resized()
	{
		forceSyncWithSlider();
		updateView();
	}
	void XyzPad::paint(Graphics& g)
	{
		const auto bounds = getLocalBounds().toFloat();

		//Background
		g.setColour(mBackgroundColor);
		g.fillRect(bounds);
		auto xyBound = Rectangle<float>(0.0f, getYfromBottom(boxDimension[1]), boxDimension[0], boxDimension[1]);
		g.setColour(mBorderColor);
		float dash[2] = { mThickness * 4,mThickness * 4 };
		Point<float> corner[4] = { xyBound.getBottomLeft(),xyBound.getTopLeft(),xyBound.getTopRight(), xyBound.getBottomRight()};
		Point<float> originalCorner[4];
		std::copy(std::begin(corner), std::end(corner), std::begin(originalCorner));
		Point<float> strokeOffset(mThickness / 2, -mThickness / 2);
		//back
		Point<float> offset = Point<float>(boxDimension[2], -boxDimension[2]);
		for (int i = 0; i < 4; i++)
		{
			corner[i] += offset;
			corner[i] -= strokeOffset / 2;
		}
		for (int i = 0; i < 4; i++)
		{
			g.drawDashedLine(Line<float> { corner[i], corner[(i + 1) % 4] }, dash, 2, mThickness);
			if (i ==0 || i ==3)
				g.drawDashedLine(Line<float> { originalCorner[i], originalCorner[i] + offset }, dash, 2, mThickness);
		}
		//tracking line
		auto trackingLine = Line<int>(valueToCoordinate(mValue[0], mValue[1], mValue[2]), valueToCoordinate(mValue[0], mValue[1], minValue[2])).toFloat();
		g.drawLine(trackingLine, mThickness/2);

		trackingLine = Line<int>(valueToCoordinate(mValue[0], mValue[1], mValue[2]), valueToCoordinate(mValue[0], mValue[1], maxValue[2])).toFloat();
		g.drawLine(trackingLine, mThickness / 2);

		trackingLine = Line<int>(valueToCoordinate(mValue[0], mValue[1], mValue[2]), valueToCoordinate(minValue[0], mValue[1], mValue[2])).toFloat();
		g.drawLine(trackingLine, mThickness / 2);

		trackingLine = Line<int>(valueToCoordinate(mValue[0], mValue[1], mValue[2]), valueToCoordinate(maxValue[0], mValue[1], mValue[2])).toFloat();
		g.drawLine(trackingLine, mThickness / 2);


		trackingLine = Line<int>(valueToCoordinate(mValue[0], mValue[1], mValue[2]), valueToCoordinate(mValue[0], minValue[1], mValue[2])).toFloat();
		g.drawLine(trackingLine, mThickness / 2);

		trackingLine = Line<int>(valueToCoordinate(mValue[0], mValue[1], mValue[2]), valueToCoordinate(mValue[0], maxValue[1], mValue[2])).toFloat();
		g.drawLine(trackingLine, mThickness / 2);
		//Shadow for thumb
		//thumb
		g.setColour(Colours::white);
		auto bound = thumb.getBounds().toFloat();
		g.fillEllipse(bound);
		

		g.setColour(mBorderColor.brighter(0.1f));
		//front
		for (int i = 0; i < 4; i++)
		{
			corner[i] = originalCorner[i] + strokeOffset;
		}
		for (int i = 0; i < 4; i++)
		{
			g.drawDashedLine(Line<float> { corner[i], corner[(i + 1) % 4] }, dash, 2, mThickness);
			if (i>0 && i<3)
				g.drawDashedLine(Line<float> { originalCorner[i], originalCorner[i]+offset }, dash, 2, mThickness);
		}


		g.setColour(mBorderColor);

		g.setColour(mZColor);
		auto zLine = Line<float>{ originalCorner[0] + strokeOffset, originalCorner[0] + strokeOffset + Point<float>(boxDimension[2] / 3, -boxDimension[2] / 3) };
		g.drawLine(zLine, mThickness);

		g.setColour(mXColor);
		auto xLine = Line<float>{ originalCorner[0] + strokeOffset, originalCorner[0] + strokeOffset + Point<float>(boxDimension[0] / 3, 0) };
		g.drawLine(xLine, mThickness);

		g.setColour(mYColor);
		auto yLine = Line<float>{ originalCorner[0] + strokeOffset, originalCorner[0] + strokeOffset + Point<float>(0, -boxDimension[1] / 3) };
		g.drawLine(yLine, mThickness);


	}



}
