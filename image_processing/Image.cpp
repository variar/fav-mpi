#include "Image.hpp"

#include <iostream>

//////////////////////



void Image::SaveImage ( std::string file_name )
{
    ilBindImage( _image_id[ 1]);

    ILuint  Width = _width_px;
    ILuint  Heigh = _height_px;
    ILuint  Depth = 1;
    ILubyte Bpp   = 3;
    ILenum  Format = IL_RGB;
    ILenum  Type = IL_FLOAT;
    int     *Data = NULL;

    ilTexImage( Width, Heigh, Depth, Bpp, Format, Type, _bitmap_out);

    std::cout << file_name.c_str( ) << std::endl;
    ilSaveImage(  file_name.c_str());

    std::cout << std::endl << ilGetError();
}
void Image::SetPixel ( int i, int j, Color &color )
{
    if ( i >= _height_px || j >= _width_px)
        return;

    {
        //boost::mutex::scoped_lock  lock ( _monitor_set );

        _bitmap_out[ ( _height_px - i - 1) * _width_px * 3 + j*3 + 0] = color.r;
        _bitmap_out[ ( _height_px - i - 1) * _width_px * 3 + j*3 + 1] = color.g;
        _bitmap_out[ ( _height_px - i - 1) * _width_px * 3 + j*3 + 2] = color.b;
    }


    //std::cout << color.r;
    //std::cout << color.g;
    //std::cout << color.b;
}

Color Image::GetPixel( int i, int j)
{
    Color col;

    col.r = _bitmap_in[ i * _width_px * 3 + j*3 + 0];
    col.g = _bitmap_in[ i * _width_px * 3 + j*3 + 1];
    col.b = _bitmap_in[ i * _width_px * 3 + j*3 + 2];

    //std::cout << col.r;
    //std::cout << col.g;
    //std::cout << col.b;

    return col;
}

int Image::GetWidth( )
{
    return _width_px;
}

int Image::GetHeight( )
{
    return _height_px;
}

void Image::LoadImage( std::string file_name)
{
    ilBindImage( _image_id[ 0]);
    ilLoadImage( file_name.c_str( ));

    int err = ilGetError( );
    if ( err  != IL_NO_ERROR)
    {
        std::cerr << "ERROR while loading :"<< err << std::endl;
    }
    _width_px  = ilGetInteger( IL_IMAGE_WIDTH);
    _height_px = ilGetInteger( IL_IMAGE_HEIGHT);

    //std::cout << "_width_px : " << _width_px << " height_px  : " << height_px << std::endl;

    _bitmap_in  = new float[ _width_px * _height_px * 3];
    _bitmap_out = new float[ _width_px * _height_px * 3];

    ilCopyPixels( 0, 0, 0, _width_px, _height_px, 1, IL_RGB, IL_FLOAT, _bitmap_in);
}

//////////////////////
Image::Image( std::string file_name)
{
    ///
    ilInit();
    _image_id = new ILuint [ 2];
    ilGenImages( 2, _image_id);

    LoadImage( file_name);
}

Image::~Image()
{
    ilDeleteImages( 2, _image_id);
    delete [] _bitmap_in;
    delete [] _bitmap_out;
    delete [] _image_id;
}


