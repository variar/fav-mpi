/*
 * Пример работы с цветным изображением в формате TIFF
 * Программа меняет местами цветовые каналы
 * 
 * Для сборки используется команда:
 * gcc -std=c99 -ltiff -lm rgb_tiff.c
 */

#include <stdio.h>
#include <tiffio.h>

#include <stdlib.h>
#include <sys/time.h>

typedef struct _Color
{
  uint8 r;
  uint8 g;
  uint8 b;
} Color;

typedef struct _ImageRGB
{
  int width;
  int height;
  uint32* raster;
} ImageRGB;

// Функция чтения данных из TIFF-файла в массив
void readImage(char* filename, ImageRGB* image)
{
  size_t npixels;
  
  TIFF* in = TIFFOpen(filename, "r");
  
  if (in) {
    printf("TIFF opened\n");
    
    int width = 0;
    int height = 0;
    
    // Запрашиваем из файла ширину и высоту изображения
    TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
    
    npixels = width*height; // полное число пикселей = ширина*высота
    
    printf("width - %d, length - %d\n", width, height);
    
    // выделяем память под все пиксели изображния
    image->raster = (uint32*)_TIFFmalloc(npixels*sizeof(uint32));
    image->width = width;
    image->height = height;
    
    // Считываем данные о пикселях в наш массив raster.
    // После вызова этой функции в массиве raster лежат все пиксели
    // по порядку. Т.е. сначала первая строка, потом вторая строка и т.д.
    // В массиве получается цепочка RGBARGBARGBA..., где каждый пиксель представлен в виде 
    // четрые чисел: R красный, G зеленый, B синий, A прозрачность.
    TIFFReadRGBAImageOriented(in, width, height, image->raster, ORIENTATION_TOPLEFT, 0);
    
    TIFFClose(in);
  }
}

void writeImage(char* newFile, ImageRGB* image)
{
  TIFF* out = TIFFOpen(newFile, "w");
  
  // Выставляем теги, чтобы изображение правильно записалось
  TIFFSetField(out, TIFFTAG_IMAGEWIDTH, image->width);
  TIFFSetField(out, TIFFTAG_IMAGELENGTH, image->height);
  
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
  TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  
  TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  
  TIFFSetField(out, TIFFTAG_SOFTWARE, TIFFGetVersion());
  TIFFSetField(out, TIFFTAG_DOCUMENTNAME, newFile);
  
  TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  
  // Запись растра в файл
  TIFFWriteEncodedStrip(out, 0, image->raster, image->width * image->height * 4);
  
  TIFFClose(out);
}

Color color(int r, int g, int b)
{
  Color c;
  c.r = r;
  c.g = g;
  c.b = b;
  return c;
}

// возвращяет индекс в растре. i - строка, j - столбец
int index(int i, int j, int width)
{
  return i*width + j;
}

Color getColor(ImageRGB* image, int i, int j)
{
  Color c;
  int pos = index(i,j,image->width);
  return color(TIFFGetR(image->raster[pos]), 
	       TIFFGetG(image->raster[pos]),
	       TIFFGetB(image->raster[pos]));
}

void setColor(ImageRGB* image, int i, int j, Color c)
{
  int pos = index(i,j,image->width);
  // Значения нужно положить в память в порядке ABGR
  image->raster[pos] = 0xff<<24; // alpha
  image->raster[pos]+=((uint32)c.b)<<16; // B
  image->raster[pos]+=((uint32)c.g)<<8; // G
  image->raster[pos]+=((uint32)c.r); // R
}

Color colorDiff(Color c1, Color c2)
{
  Color c;
  c.r = abs(c1.r-c2.r);
  c.g = abs(c1.g-c2.g);
  c.b = abs(c1.b-c2.b);
  return c;
}

// Функция меняет местами цветовые каналы G->R,B->G,R->B
void processPixels(ImageRGB* input, ImageRGB* output)
{
  for (int i=0; i<input->height; ++i)
  {
    for (int j=0; j<input->width; ++j)
    {
      Color c = getColor(input,i,j);
            
      setColor(output, i, j, color(c.g,c.b,c.r));
    }
  }
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printf("Usage: prog input.tiff output.tiff\n");
    return 0;
  }
  
  // Читаем картинку в массив
  ImageRGB input;
  readImage(argv[1], &input);
  
  ImageRGB output;
  output.width = input.width;
  output.height = input.height;
  output.raster = (uint32*)_TIFFmalloc(output.width * output.height * sizeof(uint32));
    
  struct timeval start_time, end_time;
  gettimeofday( &start_time, NULL);
  
  processPixels(&input, &output);
  
  gettimeofday( &end_time, NULL);
  double time = end_time.tv_sec - start_time.tv_sec + 0.000001*(end_time.tv_usec - start_time.tv_usec);
  printf("Process time: %f\n", time);
    
  writeImage(argv[2], &output);
  
  _TIFFfree(input.raster);
  _TIFFfree(output.raster);
}
