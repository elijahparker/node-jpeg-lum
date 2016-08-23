#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <math.h>

unsigned int size;
unsigned int histogram[256];
unsigned int width;
unsigned int height;
double pixel;
double luminance;
double rgb;


int read_jpeg_file(char *filename)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  FILE *infile = fopen(filename, "rb");
  unsigned int i = 0, component = 0;
  if (!infile) {
      printf("Error opening jpeg file %s\n!", filename);
      return -1;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  width = cinfo.output_width;
  height = cinfo.output_height;
  luminance = 0.0;
  size = cinfo.output_width*cinfo.output_height*cinfo.num_components*sizeof(unsigned int);
  memset(histogram, 0, sizeof(int)*256);
  row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);
  unsigned long count = 0;
  while (cinfo.output_scanline < cinfo.image_height) {
      jpeg_read_scanlines( &cinfo, row_pointer, 1 );
      for (i=0; i<cinfo.image_width;i++) {
          pixel = 0.0;
          for(component=0;component<cinfo.num_components;component++) {
              if(component < 2) {
                  rgb = (double) row_pointer[0][i + component];
                  rgb = pow(rgb / 255.0, 1.0 / 2.2) * 255.0; // convert to linear (remove gamma correction)
                  pixel += rgb;
              }
          }
          pixel /= 3.0;
          if(pixel > 255.0) pixel = 255.0;
          histogram[(int)pixel]++;
          luminance += (pixel - 127.0) / (256.0 / 8.0);
          count++;
      }
  }
  luminance /= (double)count;
  luminance /= 2.0;
  luminance += 2.0;
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  free(row_pointer[0]);
  fclose(infile);
  return 1;
}

#ifndef BUILDING_NODE_EXTENSION
  #define BUILDING_NODE_EXTENSION
#endif

#include <node.h>

using namespace v8;

Handle<Value> CreateObject(const Arguments& args) {
  HandleScope scope;

  Local<Object> obj = Object::New();
  Local<Array> histArray = Array::New(256);
  for (unsigned int i = 0; i < 256; i++) {
    histArray->Set(i, Number::New(histogram[i]));
  }
  obj->Set(String::NewSymbol("histogram"), histArray);
  obj->Set(String::NewSymbol("luminance"), Number::New(luminance));
  obj->Set(String::NewSymbol("width"), Number::New(width));
  obj->Set(String::NewSymbol("height"), Number::New(height));

  return obj;
}

Handle<Value> Read(const Arguments& args) {
  HandleScope scope;

  Local<Function> callback = Local<Function>::Cast(args[1]);

  if (args.Length() < 2) {
    Local<Value> err = Exception::Error(String::New("Specify an image filename to read"));    
    Local<Value> argv[] = { err };

    callback->Call(Context::GetCurrent()->Global(), 1, argv);

    return scope.Close(Undefined());
  }

  String::AsciiValue string(args[0]);
  char *filename = (char *) malloc(string.length() + 1);
  strcpy(filename, *string);

  if (read_jpeg_file(filename)) {
    Handle<Value> value = CreateObject(args);
    Local<Value> argv[] = {
            Local<Value>::New(Null()),
            Local<Value>::New(value),
    };
    callback->Call(Context::GetCurrent()->Global(), 2, argv);
    return scope.Close(value);
  }
  else {
    Local<Value> err = Exception::Error(String::New("Error reading image file"));    
    Local<Value> argv[] = { err };

    callback->Call(Context::GetCurrent()->Global(), 1, argv);
    
    return scope.Close(Undefined());
  }

  return scope.Close(CreateObject(args));
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("read"),
      FunctionTemplate::New(Read)->GetFunction());
}

NODE_MODULE(jpeglum, init)
