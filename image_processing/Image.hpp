#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include <string>
#include <vector>

#include <IL/il.h>

typedef struct _Color
{
    float r,g,b;
} Color;


class Image
{
private:
    ILuint         *_image_id;
    float          *_bitmap;

    int             _width_px;
    int             _height_px;

    void   LoadImage ( std::string file_name);
    void   RotateImage ( );
public:

    //////////////////////
    // i - строка
    // j - столбец
    void   SetPixel    ( int i, int j, Color &color);
    Color  GetPixel    ( int i, int j);

    int    GetWidth  ( );
    int    GetHeight ( );


    void   SaveImage ( std::string file_name);
    //////////////////////
    Image ( std::string);
    ~Image();
};

#endif  //_IMAGE_HPP_
