#include "XyzPad.h"

namespace Gui
{
	XyzPad::Thumb::Thumb()
	{
	}
	//XyzPad
	XyzPad::XyzPad()
	{
		setBoxSize(100, 100, 100);
		valueChanged();
	}
	void XyzPad::updateForSizeChanged()
	{
		for (int i = 0; i < 3; i++)
		{
			mSource[i] = jmap(mValue[i], minValue[i], maxValue[i], 0.0f, mBoxDimension[i]);
		}
	}
	void XyzPad::setBoxSize(float sizeX, float sizeY, float sizeZ)
	{
		mBoxDimension[0] = sizeX;
		mBoxDimension[1] = sizeY;
		mBoxDimension[2] = sizeZ;
		updateForSizeChanged();
		valueChanged();
		updateView();
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

	float XyzPad::getValue(int dimension)
	{
		return mValue[dimension];
	}
	void XyzPad::setBackgroundColor(Colour backgroundColor)
	{
		mBackgroundColor = backgroundColor;
	}
	void XyzPad::setDestination(float posX, float posY, float posZ)
	{
		mDestination[0] = posX;
		mDestination[1] = posY;
		mDestination[2] = posZ;
		for (int i = 0; i < 3; i++)
		{
			mDestinationValue[i] = jmap(mDestination[i], 0.0f, mBoxDimension[i], minValue[i], maxValue[i]);
		}
		
		updateView();
	}
	float XyzPad::getDestination(int dimension)
	{
		return mDestination[dimension];
	}
	float XyzPad::getSource(int dimension)
	{
		float source = jmap(mValue[dimension], minValue[dimension], maxValue[dimension], 0.0f, mBoxDimension[dimension]);
		return source;
	}
	 void XyzPad::registerSource(XyzPadListener* listener)
	{
		mXyzPadListenerList.push_back(listener);
		listener->sourceValueChanged(Coordinate(mSource[0], mSource[1], mSource[2]));
	}
	float XyzPad::getBoxSize(int dimension)
	{
		return mBoxDimension[dimension];
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
		mValue[whichAxis] = slider->getValue();
		mSource[whichAxis] = jmap(mValue[whichAxis], minValue[whichAxis], maxValue[whichAxis], 0.0f, mBoxDimension[whichAxis]);
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
		for (auto* listener : mXyzPadListenerList)
		{
			listener->sourceValueChanged(Coordinate(mSource[0], mSource[1], mSource[2]));
		}
		updateForSizeChanged();
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

		auto xyBound = Rectangle<float>(
			(getWidth() - (mDimension[0] + mDimension[2])) / 2,
			(getHeight() - (mDimension[1] + mDimension[2])) / 2 + mDimension[2], mDimension[0], mDimension[1]);
		float zToX = mDimension[2]*
			(z - minValue[2]) / (maxValue[2] - minValue[2]);
		float zToY = mDimension[2] *
			(z - minValue[2]) / (maxValue[2] - minValue[2]);
		float coordinateX = zToX +  xyBound.getX() + (x - minValue[0]) / (maxValue[0] - minValue[0])* (xyBound.getTopRight().getX() - xyBound.getX());
		float coordinateY = jmap(y,minValue[1],maxValue[1], xyBound.getBottom(), xyBound.getBottom()-mDimension[1]) - zToY;
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
			mValue[2] += jmap(travelVector.getX() / mDimension[0], -1.0f, 1.0f, minValue[0], maxValue[0]);
			mValue[2] += jmap(-travelVector.getY() / mDimension[1], -1.0f, 1.0f, minValue[1], maxValue[1]);
		}
		else
		{
			
			mValue[0] += jmap(travelVector.getX() / mDimension[0], -0.5f, 0.5f, minValue[0], maxValue[0]);
			mValue[1] += jmap(-travelVector.getY() / mDimension[1], -0.5f, 0.5f, minValue[1], maxValue[1]);
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
	}
		void XyzPad::updateView()
		{
		float max = -1;
		int indexOfMax = -1;
		float ratioForAll = 0;
		float ratioForZ;
		if ((mBoxDimension[0] + mBoxDimension[2] * zRatio)/ getWidth() > (mBoxDimension[1] + mBoxDimension[2] * zRatio) / getHeight())
		{
			//width is longer
			ratioForAll = getWidth() / (mBoxDimension[0] + mBoxDimension[2] * zRatio);
			mDimension[0] = mBoxDimension[0] * ratioForAll;
			mDimension[1] = mBoxDimension[1] * ratioForAll;
			mDimension[2] = getWidth() - mDimension[0];
		}
		else
		{
			//height is longer
			ratioForAll = getHeight() / (mBoxDimension[1] + mBoxDimension[2] * zRatio);
			mDimension[0] = mBoxDimension[0] * ratioForAll;
			mDimension[1] = mBoxDimension[1] * ratioForAll;
			mDimension[2] = getHeight() - mDimension[1];
		}

		thumb.setSize(thumbWidth,thumbWidth);
		Point<int> newThumbPosition = valueToCoordinate(mValue[0], mValue[1], mValue[2]) - Point<int>(thumbWidth / 2, thumbWidth / 2);
		thumb.setTopLeftPosition(newThumbPosition);

		//des thumb
		mDestinationThumb.setSize(thumbWidth, thumbWidth);
		newThumbPosition = valueToCoordinate(mDestinationValue[0], mDestinationValue[1], mDestinationValue[2]) - Point<int>(thumbWidth / 2, thumbWidth / 2);
		mDestinationThumb.setTopLeftPosition(newThumbPosition);

		repaint();
	}
	void XyzPad::paint(Graphics& g)
	{
		const auto bounds = getLocalBounds().toFloat();

		//Background
		//g.setColour(mBackgroundColor);
		//g.fillRect(bounds);
		
		auto xyBound = Rectangle<float>(
			(getWidth()-(mDimension[0] + mDimension[2]))/2, 
			(getHeight() - (mDimension[1] + mDimension[2])) / 2+ mDimension[2], mDimension[0], mDimension[1]);

		g.setColour(mBorderColor);
		float dash[2] = { mThickness * 4,mThickness * 4 };
		Point<float> corner[4] = { xyBound.getBottomLeft(),xyBound.getTopLeft(),xyBound.getTopRight(), xyBound.getBottomRight()};
		Point<float> originalCorner[4];
		std::copy(std::begin(corner), std::end(corner), std::begin(originalCorner));
		Point<float> strokeOffset(mThickness / 2, -mThickness / 2);
		//back
		Point<float> offset = Point<float>(mDimension[2], -mDimension[2]);
		for (int i = 0; i < 4; i++)
		{
			corner[i] += offset;
			corner[i] -= strokeOffset / 2;
		}
		for (int i = 0; i < 4; i++)
		{
			if (i == 2 || i == 1)
			{
				g.drawLine(Line<float> { corner[i], corner[(i + 1) % 4] }, mThickness);
			}
			else
			{
				g.drawDashedLine(Line<float> { corner[i], corner[(i + 1) % 4] }, dash, 2, mThickness);
			}
			if (i == 3)
			{
				g.drawLine(Line<float> { originalCorner[i], originalCorner[i] + offset }, mThickness);
			}
			if (i == 0)
			{
				g.drawDashedLine(Line<float> { originalCorner[i], originalCorner[i] + offset }, dash, 2, mThickness);
			}
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
		// zColor line
		g.setColour(mZColor);
		auto zLine = Line<float>{ originalCorner[0] + strokeOffset, originalCorner[0] + strokeOffset + Point<float>(mDimension[2] / 3, -mDimension[2] / 3) };
		g.drawLine(zLine, mThickness);

		if (mValue[2] > mDestinationValue[2])
		{

			//thumb
			g.setColour(Colours::white);
			auto bound = thumb.getBounds().toFloat();
			//g.drawEllipse(bound, mThickness / 2);
			g.fillEllipse(bound);

			//destination thumb
			g.setColour(mXColor);
			auto bound2 = mDestinationThumb.getBounds().toFloat();
			g.fillEllipse(bound2);
		}
		else
		{
			//destination thumb
			g.setColour(mXColor);
			auto bound2 = mDestinationThumb.getBounds().toFloat();
			g.fillEllipse(bound2);

			//thumb
			g.setColour(Colours::white);
			auto bound = thumb.getBounds().toFloat();
			//g.drawEllipse(bound, mThickness / 2);
			g.fillEllipse(bound);
		}


		g.setColour(mBorderColor.brighter(0.1f));
		//front
		for (int i = 0; i < 4; i++)
		{
			corner[i] = originalCorner[i] + strokeOffset;
		}
		for (int i = 0; i < 4; i++)
		{
			g.drawLine(Line<float> { corner[i], corner[(i + 1) % 4] }, mThickness);
			if (i>0 && i<3)
				g.drawLine(Line<float> { originalCorner[i], originalCorner[i]+offset }, mThickness);
		}
		g.setColour(mBorderColor);

		//front color
		g.setColour(mXColor);
		auto xLine = Line<float>{ originalCorner[0] + strokeOffset, originalCorner[0] + strokeOffset + Point<float>(mDimension[0] / 3, 0) };
		g.drawLine(xLine, mThickness);

		g.setColour(mYColor);
		auto yLine = Line<float>{ originalCorner[0] + strokeOffset, originalCorner[0] + strokeOffset + Point<float>(0, -mDimension[1] / 3) };
		g.drawLine(yLine, mThickness);
	}
}
