#pragma once
/**
 * @class XyzPad
 * @brief A graphical 3D-like XYZ coordinate pad that allows manipulation of three values in a 3D space.
 * @Create by kpnn - Gyrus Space
 * The XyzPad is a custom JUCE Component that visually represents a 3D XYZ coordinate system. It provides
 * a graphical interface for manipulating three values corresponding to the X, Y, and Z axes. Sliders
 * can be registered with the XyzPad to control the individual axis values. It provides visual feedback
 * and real-time updates when the values change.
 *
 * Usage:
 * 1. Include the "XyzPad.h" header file in your project.
 * 2. Create an instance of the XyzPad in your GUI class.
 * 3. Register the sliders that control the X, Y, and Z axes using the `registerSlider()` method.
 * 4. Optionally, you can set the background and border colors using `setBackgroundColor()` and
 *    `setBorderColor()` respectively.
 * 5. You can also set custom colors for the X, Y, and Z axes using `setAxisColor()`.
 * 6. The XyzPad will automatically synchronize with the registered sliders, and their values will
 *    be updated as the user interacts with the XyzPad.
 * 7. To retrieve the current values of the X, Y, and Z axes, use the `getValue()` method.
 *
 * Standard XYZ Coordinate System:
 *
 *            y+
 *            |
 *            |
 *            |
 *            |         z-
 *            |        /
 *            |       /
 *            |      /
 *            |     /
 *            |    /
 *            |   /
 *            |  /
 *            | /
 *            +-----------------> x+
 *           /
 *          /
 *         /
 *        /
 *       /
 *      /
 *     /
 *    z+
 *
 * Mouse Interaction:
 * - Dragging the mouse within the XyzPad will update the X, Y, and Z values accordingly.
 * - Holding down the Shift key while dragging the mouse will change the Z value.
 * - Releasing the mouse button will stop the value change.
 *
 * @see Gui::XyzPad::registerSlider
 * @see Gui::XyzPad::deregisterSlider
 * @see Gui::XyzPad::setBackgroundColor
 * @see Gui::XyzPad::setBorderColor
 * @see Gui::XyzPad::setAxisColor
 * @see Gui::XyzPad::getValue
 * */
#include <JuceHeader.h>
using namespace juce;
namespace Gui
{	
	class XyzPad : public Component, Slider::Listener
	{
		class Thumb : public Component
		{
		public:
			Thumb();
		private:
		};
	private:
		float mValue[3] = {0.0f,0.0f,0.0f};
		float minValue[3] = {-1.0f, -1.0f, -1.0f};
		float maxValue[3] = { 1.0f, 1.0f, 1.0f };
		float mDimension[3] = {200,200,200};
		float zRatio = (1 / sqrt(2)) / (1 / sqrt(2) + 2);
		float boxDimension[3];
		float mThickness = 4;
		Colour mBackgroundColor = Colour(22, 22, 23); 
		Colour mBorderColor = Colour(50, 50, 50);
		Colour mXColor = Colour(66, 214, 190);
		Colour mYColor = Colour(199, 74, 153);
		Colour mZColor = Colour(199, 112, 74);
		Point<float> mDragPositionStart;
		static constexpr int thumbWidth = 20;
		Thumb thumb;
		//juce method
		Point<int> valueToCoordinate(float x, float y, float z);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;

		float getYfromBottom(float y);
		void resized() override;
		void paint(Graphics& g) override;
		void valueChanged();
		void forceSyncWithSlider();
		void sliderValueChanged(Slider* slider) override;
		void updateView();
		std::vector<Slider*> sliderList[3];
		std::mutex mutex;
	public:
		XyzPad();
		void registerSlider(Slider* slider, int axis);
		void deregisterSlider(Slider* slider);
		float getValue(int dimension);
		void setBackgroundColor(Colour backgroundColor);
		void setBorderColor(Colour borderColor);
		void setAxisColor(Colour x, Colour y, Colour z);

	};	
}