/*
 * Пример работы с цветным изображением в формате TIFF
 * Программа меняет местами цветовые каналы
 * 
 * Для сборки используется команда:
 * gcc -std=c99 -ltiff rgb_tiff.c
 */

#include <stdio.h>
#include <tiffio.h>

#include <sys/time.h>

// Функция чтения данных из TIFF-файла в массив
void readImage(char* filename, uint32** raster, uint32* width, uint32* height)
{
  size_t npixels;
  
  TIFF* in = TIFFOpen(filename, "r");
  
  if (in) {
    printf("TIFF opened\n");
    
    // Запрашиваем из файла ширину и высоту изображения
    TIFFGetField(in, TIFFTAG_IMAGEWIDTH, width);
    TIFFGetField(in, TIFFTAG_IMAGELENGTH, height);
    
    npixels = *width*(*height); // полное число пикселей = ширина*высота
    
    printf("width - %d, length - %d\n", *width,*height);
    
    // выделяем память под все пиксели изображния
    *raster = (uint32*)_TIFFmalloc(npixels*sizeof(uint32));
    
    // Считываем данные о пикселях в наш массив raster.
    // После вызова этой функции в массиве raster лежат все пиксели
    // по порядку. Т.е. сначала первая строка, потом вторая строка и т.д.
    // В массиве получается цепочка RGBARGBARGBA..., где каждый пиксель представлен в виде 
    // четрые чисел: R красный, G зеленый, B синий, A прозрачность.
    TIFFReadRGBAImageOriented(in, *width, *height, *raster, ORIENTATION_TOPLEFT, 0);
    
    TIFFClose(in);
  }
}

void writeImage(char* newFile, uint32* raster, uint32 width, uint32 height)
{
  TIFF* out = TIFFOpen(newFile, "w");
  
  // Выставляем теги, чтобы изображение правильно записалось
  TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
  
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
  TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  
  TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  
  TIFFSetField(out, TIFFTAG_SOFTWARE, TIFFGetVersion());
  TIFFSetField(out, TIFFTAG_DOCUMENTNAME, newFile);
  
  TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  
  // Запись растра в файл
  TIFFWriteEncodedStrip(out, 0, raster, width*height*4);
  
  TIFFClose(out);
}

// возвращяет индекс в растре. i - строка, j - столбец
int index(int i, int j, int width)
{
  return i*width + j;
}

void setColor(uint32* raster, int pos, uint8 r, uint8 g, uint8 b)
{
  // Значения нужно положить в память в порядке ABGR
  raster[pos] = 0xff<<24; // alpha
  raster[pos]+=((uint32)b)<<16; // B
  raster[pos]+=((uint32)g)<<8; // G
  raster[pos]+=((uint32)r); // R
}

// Функция меняет местами цветовые каналы G->R,B->G,R->B
void processPixels(uint32* raster, uint32* outRaster, int width, int height)
{
  for (int i=0; i<height; ++i)
  {
    for (int j=0; j<width; ++j)
    {
      int pos = index(i,j,width);
      
      uint8 r = TIFFGetG(raster[pos]);
      uint8 g = TIFFGetB(raster[pos]);
      uint8 b = TIFFGetR(raster[pos]);
      
      setColor(outRaster, pos, r,g,b);
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
  uint32* raster = NULL;
  uint32 width, height;
  readImage(argv[1], &raster, &width, &height);
  
  uint32* outRaster = (uint32*)_TIFFmalloc(width*height*sizeof(uint32));
  
  struct timeval start_time, end_time;
  gettimeofday( &start_time, NULL);
  
  processPixels(raster, outRaster, width, height);
  
  gettimeofday( &end_time, NULL);
  double time = end_time.tv_sec - start_time.tv_sec + 0.000001*(end_time.tv_usec - start_time.tv_usec);
  printf("Process time: %f\n", time);
    
  writeImage(argv[2], outRaster, width, height);
  
  _TIFFfree(raster);
  _TIFFfree(outRaster);
}
