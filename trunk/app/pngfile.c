/*
上一篇文章对C语言中的goto语句进行了较深入的阐述，实际上goto语句是面向过程与面向结构化程序语言中，
进行异常处理编程的最原始的支持形 式。
后来为了更好地、更方便地支持异常处理编程机制，
使得程序员在C语言开发的程序中，能写出更高效、更友善的带有异常处理机制的
代码模块来。于是，C语 言中出现了一种更优雅的异常处理机制，
那就是setjmp()函数与longjmp()函数。
实际上，这种异常处理的机制不是C语言中自身的一部分，
而是在C标准库中实现的两个非常有技巧的库函数，
也许大多数C程序员朋友们对它都很熟悉，而 且，
通过使用setjmp()函数与longjmp()函数组合后，而提供的对程序的异常处理机制，
以被广泛运用到许多C语言开发的库系统中，如jpg解 析库，加密解密库等等。
*/
/*
int setjmp( jmp_buf env );
void longjmp( jmp_buf env, int value );

这是MSDN中对它的评论，如下：
setjmp函数用于保存程序的运行时的堆栈环境，接下来的其它地方，
你可以通过调用longjmp函数来恢复先前被保存的程序堆栈环境。
当setjmp和longjmp组合一起使用时，它们能提供一种在程序中实现
“非本地局部跳转”（"non-local goto"）的机制。
并且这种机制常常被用于来实现，把程序的控制流传递到错误处理模块之中；
或者程序中不采用正常的返回（return）语句，或函数的正常调用等方法，
而使程序能被恢复到先前的一个调用例程（也即函数）中。
对setjmp函数的调用时，会保存程序当前的堆栈环境到env参数中；
接下来调用longjmp时，会根据这个曾经保存的变量来恢复先前的环境，
并且 当前的程序控制流，会因此而返回到先前调用setjmp时的程序执行点。
此时，在接下来的控制流的例程中，所能访问的所有的变量（除寄存器类型的变量以 外），
包含了longjmp函数调用时，所拥有的变量。
对setjmp函数的调用时，会保存程序当前的堆栈环境到env参数中；
接下来调用longjmp时，会根据这个曾经保存的变量来恢复先前的环境，
并且 因此当前的程序控制流，会返回到先前调用setjmp时的执行点。

try/catch is used only in C++, can not be applied in ANSI C.
*/
/*
PNG是20世纪90年代中期开始开发的图像文件存储格式，其目的是企图替代GIF和TIFF
文件格式，同时增加一些GIF文件格式所不具备的特性。流式网络图形格式(Portable 
Network Graphic Format，PNG)名称来源于非官方的“PNG's Not GIF
”，是一种位图文件(bitmap file)存储格式，读成“ping”。PNG
用来存储灰度图像时，灰度图像的深度可多到16
位，存储彩色图像时，彩色图像的深度可多到48位，并且还可存储多到16
位的α通道数据。PNG使用从LZ77派生的无损数据压缩算法。

2、流式读/写性能(streamability)
：图像文件格式允许连续读出和写入图像数据，这个特性很适合于在通信过程中生成
　　和显示图像。
　　3、逐次逼近显示(progressive display)
：这种特性可使在通信链路上传输图像文件的同时就在终端上显示图像，
　　把整个轮廓显示出来之后逐步显示图像的细节，也就是先用低分辨率显示图像，然后?
鸩教岣咚姆直媛省?
　　4、透明性(transparency)
：这个性能可使图像中某些部分不显示出来，用来创建一些有特色的图像。
　　5、辅助信息(ancillary information)
：这个特性可用来在图像文件中存储一些文本注释信息。 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pngfile.h"

#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif


/* Check to see if a file is a PNG file using png_sig_cmp().  png_sig_cmp()
 * 0:success or equal to, -1:fail.
 */
#define PNG_BYTES_TO_CHECK 4
int png_check(char *file_name)
{
	char buf[PNG_BYTES_TO_CHECK];
	FILE *fp;
	int ret=-1;

	if ((fp = fopen(file_name, "rb")) == NULL)
		return -1;

	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
	{
		ret=-1;
	}
	else
	{
		ret = png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
	}

	fclose(fp);
	return ret;
}

#if 0
static void user_read_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
//	if (fread(data,1,length) != length) 
//		png_error(png_ptr, "Read Error");
}


static void user_write_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
//	if (fwrite(data,1,length) != length) 
//		png_error(png_ptr, "Write Error");
}


static void user_error_fn(png_structp png_ptr,png_const_charp error_msg)
{
	strncpy((char*)png_ptr->error_ptr,error_msg,255);
	longjmp(png_ptr->jmpbuf, 1);
}

#endif

/*
func desp: this func supports alpha channel. buffer is BGRA format

*/
unsigned char *png_load(char *filename,unsigned int *width,unsigned int *height)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	int ret=-1;
	png_bytep row_buf;
	png_uint_32 y;
	int num_pass, pass;

	//check to see if png type file
	ret = png_check(filename);
	if (ret)
	{
		printf( "%s is not exist or NOT png format. pls check \n", filename);
		return NULL;		//it is NOT a png file.
	}
   	//printf( "libpng version is:%s\n", PNG_LIBPNG_VER_STRING);

	//prepare to read png file
	fp = fopen(filename, "rb");		//has pass test. fp will not equal to NULL.
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (void *)NULL,(png_error_ptr)NULL,  (png_error_ptr)NULL);
	if (png_ptr == NULL)
	{
		printf("preparing png_ptr... fail\n");
		fclose(fp);
		return NULL;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		printf("preparing info_ptr... fail\n");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return NULL;
	}

	//png_read_png(png_ptr, info_ptr,png_transforms_flag,NULL);		//high level  func
	//init stream I/O func
	png_init_io(png_ptr, fp);	//using standard C API
	png_read_info(png_ptr, info_ptr);		//read the file information

	//tell libpng to strip 16 bit depth files down to 8 bits.  
	if (info_ptr->bit_depth == 16)	
	{
		printf("NOT be supported 16 bit-depth png file operation!\n");
		png_set_strip_16(png_ptr);
	}

	if (info_ptr->pixel_depth != 32){
		printf("NOT be supported non-4-bpp png file operation!\n");
		return NULL;
	}
	
	/*
	If there is no interlacing (check interlace_type == PNG_INTERLACE_NONE), 
	this is simple:
	png_read_rows(png_ptr, row_pointers, NULL, number_of_rows);
	*/
	if (info_ptr->interlace_type)
	{
		num_pass = png_set_interlace_handling(png_ptr);
	}
	else
	{
		num_pass = 1;
	}

	// Allocate the memory to hold the image using the fields of info_ptr.
	row_buf = (png_bytep)malloc(info_ptr->rowbytes * info_ptr->height *num_pass);

	for (pass = 0; pass < num_pass; pass++)
	{
		for (y = 0; y < info_ptr->height; y++)
		{
			//png_read_row(png_structp png_ptr, png_bytep row, png_bytep dsp_row)
			png_read_row(png_ptr,row_buf+(info_ptr->rowbytes*y) + (info_ptr->rowbytes*info_ptr->height*pass), NULL);
		}
	}
	

	if (width)
		*width = info_ptr->width;
	if (height)
		*height = info_ptr->height;
	
	png_read_end(png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
	return ((unsigned char *)row_buf);
}

/*
This func only support 4 bpp BGRA format.
*/
int png_save(char * filename,unsigned char *img_buf,int width,int height)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	int number_passes = 1,pass,y;
	int bytes_per_pixel=4;

	fp = fopen(filename, "wb+");
	if (fp == NULL)	return (-1);

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if (png_ptr == NULL)
	{
		fclose(fp);
		return (-2);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, NULL);
		return (-3);
	}

	png_init_io(png_ptr, fp);

	info_ptr->width = width;
	info_ptr->height = height;
	info_ptr->pixel_depth = 32;
	info_ptr->channels = 4;
	info_ptr->bit_depth = 8;
	info_ptr->color_type = PNG_COLOR_TYPE_RGB_ALPHA;		//  4 means COLORTYPE_ALPHA
	info_ptr->compression_type = info_ptr->filter_type = info_ptr->interlace_type=0;
	info_ptr->valid = 0;
	info_ptr->interlace_type=PNG_INTERLACE_NONE;
	info_ptr->rowbytes = width*4;	

   /* Set the image information here.  Width and height are up to 2^31,
    * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
    * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
    * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
    */
   png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   // Write the file header information.  REQUIRED 
   png_write_info(png_ptr, info_ptr);

   for (pass = 0; pass < number_passes; pass++)
   {
	  //png_write_row(png_structp png_ptr, png_bytep row)
        for (y = 0; y < height; y++)
           png_write_row(png_ptr, img_buf + (info_ptr->rowbytes * y));
   }

   // It is REQUIRED to call this to finish writing the rest of the file 
   png_write_end(png_ptr, info_ptr);

   png_destroy_write_struct(&png_ptr, &info_ptr);
   fclose(fp);
   return 0;
}


/*
For testing purpose, do NOT support alpha channel. 
*/
int png2png(char *inname, char *outname)
{
	FILE *fpin, *fpout;
	png_structp read_ptr;
	png_structp write_ptr;
	png_infop info_ptr;
	png_infop end_info;
	png_bytep row_buf;
	png_uint_32 rowbytes;
	png_uint_32 y;
	int num_pass, pass;

	fpin = fopen(inname, "rb");
	if (!fpin)
	{
		printf( "Could not find input file %s\n", inname);
		return -1;
	}

	fpout = fopen(outname, "wb");
	if (!fpout)
	{
		printf( "Could not open output file %s\n", outname);
		fclose(fpin);
		return -1;
	}

	read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (void *)NULL,(png_error_ptr)NULL,  (png_error_ptr)NULL);
	write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (void *)NULL,(png_error_ptr)NULL, (png_error_ptr)NULL);
	info_ptr = png_create_info_struct(read_ptr);
	end_info = png_create_info_struct(read_ptr);

	if (setjmp(read_ptr->jmpbuf))
	{
		printf( "libpng read error\n");
		png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
		png_destroy_write_struct(&write_ptr, (png_infopp)NULL);
		fclose(fpin);
		fclose(fpout);
		return -2;
	}

	if (setjmp(write_ptr->jmpbuf))
	{
		printf( "libpng write error\n");
		png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
		png_destroy_write_struct(&write_ptr, (png_infopp)NULL);
		fclose(fpin);
		fclose(fpout);
		return -2;
	}

	png_init_io(read_ptr, fpin);
	png_init_io(write_ptr, fpout);

	png_read_info(read_ptr, info_ptr);
	png_write_info(write_ptr, info_ptr);

/*
	if ((info_ptr->color_type & PNG_COLOR_TYPE_PALETTE)==PNG_COLOR_TYPE_PALETTE)
		channels = 1;
	else
		channels = 3;
	
	if (info_ptr->color_type & PNG_COLOR_MASK_ALPHA)
		channels++;
*/
	//rowbytes = info_ptr->width * info_ptr->pixel_depth;
	
	rowbytes = info_ptr->rowbytes;
	row_buf = (png_bytep)malloc((size_t)rowbytes);
	if (!row_buf)
	{
		printf( "No memory to allocate row buffer\n");
		png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
		png_destroy_write_struct(&write_ptr, (png_infopp)NULL);
		fclose(fpin);
		fclose(fpout);
		return -3;
	}

/*
 If there is no interlacing (check interlace_type == PNG_INTERLACE_NONE), 
this is simple:
    png_read_rows(png_ptr, row_pointers, NULL, number_of_rows);
*/
   if (info_ptr->interlace_type)
   {
      num_pass = png_set_interlace_handling(read_ptr);
      num_pass = png_set_interlace_handling(write_ptr);
   }
   else
   {
      num_pass = 1;
   }

   for (pass = 0; pass < num_pass; pass++)
   {
      for (y = 0; y < info_ptr->height; y++)
      {
         png_read_rows(read_ptr, (png_bytepp)&row_buf, (png_bytepp)0, 1);
         png_write_rows(write_ptr, (png_bytepp)&row_buf, 1);	//reuse the same row pointer
      }
   }

   png_read_end(read_ptr, end_info);
   png_write_end(write_ptr, end_info);

   png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
   png_destroy_write_struct(&write_ptr, (png_infopp)NULL);

   fclose(fpin);
   fclose(fpout);

   free((void *)row_buf);

   return 0;
}


