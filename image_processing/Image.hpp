#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <string>
#include <vector>
#include <cmath>


#include <IL/il.h>

class Color
{
public:
    float r,g,b;
};


class Camera
{
private:
    ILuint         *_image_id;
    float          *_bitmap_in,
                   *_bitmap_out;

    int             _width_px;
    int             _height_px;
public:

    //////////////////////
    void   SetPixel    ( int i, int j, Color &color);
    Color  GetPixel    ( int i, int j);

    int    GetWidth  ( );
    int    GetHeight ( );

    void   LoadImage ();
    void   LoadImage ( std::string file_name);

    void   SaveImage ();
    void   SaveImage ( std::string file_name);
    //////////////////////
    Camera ( std::string);
    ~Camera();
};

#endif  //_CAMERA_HPP_
