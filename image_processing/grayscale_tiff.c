#include <stdio.h>
#include <tiffio.h>

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

void writeImage(char* newFile, uint8* raster, uint32 width, uint32 height)
{
  TIFF* out = TIFFOpen(newFile, "w");
  
  // Выставляем теги, чтобы изображение правильно записалось
  TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
  
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  
  TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  
  TIFFSetField(out, TIFFTAG_SOFTWARE, TIFFGetVersion());
  TIFFSetField(out, TIFFTAG_DOCUMENTNAME, newFile);
  
  TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  
  // Запись растра в файл
  TIFFWriteEncodedStrip(out, 0, raster, width*height);
  
  TIFFClose(out);
}

void processPixels(uint32* raster, uint8* outRaster, int width, int height)
{
  for (int i=0; i<width*height; ++i)
  {
    outRaster[i] = TIFFGetR(raster[i])*3/10+TIFFGetG(raster[i])*6/10+TIFFGetB(raster[i])/10;
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
  
  uint8* outRaster = (uint8*)_TIFFmalloc(width*height*sizeof(uint8));
  
  processPixels(raster, outRaster, width, height);
  
  writeImage(argv[2], outRaster, width, height);
  
  _TIFFfree(raster);
  _TIFFfree(outRaster);
}
