# XyzPad - A Graphical 3D-like XYZ Coordinate Pad

The `XyzPad` is a custom JUCE (Jules' Utility Class Extensions) component that provides a graphical interface for manipulating three values corresponding to the X, Y, and Z axes in a 3D space. It is designed to be used in GUI applications and allows users to interactively manipulate the X, Y, and Z values using a graphical representation of a standard XYZ coordinate system.

## Features

- Allows manipulation of three values (X, Y, and Z) in a 3D space.
- Visual representation of a standard XYZ coordinate system.
- Register sliders to control individual axis values and provide real-time updates.
- Mouse interaction: Dragging the mouse within the XyzPad updates the values accordingly.
- Holding down the Shift key while dragging the mouse changes the Z value.
- Customizable background and border colors.
- Customizable colors for the X, Y, and Z axes.

## Usage

1. Include the "XyzPad.h" header file in your project.
2. Create an instance of the `XyzPad` in your GUI class.
3. Register sliders that control the X, Y, and Z axes using the `registerSlider()` method.
4. Optionally, set the background and border colors using `setBackgroundColor()` and `setBorderColor()`, respectively.
5. Optionally, set custom colors for the X, Y, and Z axes using `setAxisColor()`.
6. The XyzPad will automatically synchronize with the registered sliders, and their values will be updated as the user interacts with the XyzPad.
7. To retrieve the current values of the X, Y, and Z axes, use the `getValue()` method.

## Standard XYZ Coordinate System

```
            y+
            |
            |
            |
            |         z+
            |        /
            |       /
            |      /
            |     /
            |    /
            |   /
            |  /
            | /
            +-----------------> x+
           /
          /
         /
        /
       /
      /
     /
    z-
```

## Mouse Interaction

- Dragging the mouse within the XyzPad will update the X, Y, and Z values accordingly.
- Holding down the Shift key while dragging the mouse will change the Z value.
- Releasing the mouse button will stop the value change.

## Class Structure

The `XyzPad` class has the following member classes:

1. `Thumb`: Represents the draggable thumb component used to interact with the XyzPad.
   - Constructor: Creates an instance of the Thumb component.

2. `XyzPad`: The main class representing the 3D XYZ coordinate pad.
   - Constructor: Creates an instance of the XyzPad component.
   - Methods:
     - `setBoxSize(float sizeX, float sizeY, float sizeZ)`: Sets the size of the XyzPad box in 3D space.
     - `registerSlider(Slider* slider, int axis)`: Registers a slider to control an axis.
     - `deregisterSlider(Slider* slider)`: Deregisters a previously registered slider.
     - `getValue(int dimension)`: Retrieves the current value of the specified dimension (0 for X, 1 for Y, and 2 for Z).
     - `setBackgroundColor(Colour backgroundColor)`: Sets the background color of the XyzPad.
     - `setBorderColor(Colour borderColor)`: Sets the border color of the XyzPad.
     - `setAxisColor(Colour x, Colour y, Colour z)`: Sets the custom colors for the X, Y, and Z axes.
   - Mouse Event Methods:
     - `mouseDown(const MouseEvent& event)`: Handles the mouse-down event on the XyzPad.
     - `mouseDrag(const MouseEvent& event)`: Handles the mouse-drag event on the XyzPad.
   - Helper Methods:
     - `valueToCoordinate(float x, float y, float z)`: Converts X, Y, and Z values to screen coordinates.
     - `getYfromBottom(float y)`: Calculates the Y coordinate from the bottom of the component.
     - `updateView()`: Updates the view of the XyzPad component.
     - `paint(Graphics& g)`: Draws the XyzPad and its components on the screen.

## Example

```cpp
#include "XyzPad.h"

// Create an instance of the XyzPad
Gui::XyzPad xyzPad;

// Register sliders that control the X, Y, and Z axes
Slider sliderX, sliderY, sliderZ;
xyzPad.registerSlider(&sliderX, 0); // Register the X-axis slider
xyzPad.registerSlider(&sliderY, 1); // Register the Y-axis slider
xyzPad.registerSlider(&sliderZ, 2); // Register the Z-axis slider

// Optionally, set custom colors for the axes
xyzPad.setAxisColor(Colour(66, 214, 190), Colour(199, 74, 153), Colour(199, 112, 74));

// Optionally, set the background and border colors
xyzPad.setBackgroundColor(Colour(22, 22, 23));
xyzPad.setBorderColor(Colour(50, 50, 50));
```

For further details on the usage and methods, refer to the code comments and the provided documentation for the `XyzPad` class.

(Note: This example assumes you have included the required JUCE headers and set up the GUI components accordingly.)