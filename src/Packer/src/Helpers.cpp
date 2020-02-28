/*
 * Helpers.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <assert.h>
#include <limits.h>
#include <iostream>
#include "miniz/miniz.h"
using namespace std;


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

#define my_max(a,b) (((a) > (b)) ? (a) : (b))
#define my_min(a,b) (((a) < (b)) ? (a) : (b))

#define BUF_SIZE (1024 * 1024)
static uint8 s_inbuf[BUF_SIZE];
static uint8 s_outbuf[BUF_SIZE];



int intinput()
{
	string c;
	while (1)
	{
		cin >>c;
		try
		{
			return stoi(c);
		}
		catch (...)
		{
			printf("non numeric input\n>> ");
		}
	}
	return -1;
}

int ziphelp(string in, string out)
{
	const char *pMode;
	  FILE *pInfile, *pOutfile;
	  uint infile_size;
	  int level = Z_BEST_COMPRESSION;
	  z_stream stream;
	  int p = 1;
	  const char *pSrc_filename = in.c_str();
	  const char *pDst_filename = out.c_str();
	  long file_loc;
	  printf("setsetstsetsetestestsetset\n");

	  printf("miniz.c version: %s\n", MZ_VERSION);
	  pInfile = fopen(pSrc_filename, "rb");
	  if (!pInfile)
	  {
	    printf("Failed opening input file!\n");
	    return EXIT_FAILURE;
	  }

	  // Determine input file's size.
	    fseek(pInfile, 0, SEEK_END);
	    file_loc = ftell(pInfile);
	    fseek(pInfile, 0, SEEK_SET);

	    if ((file_loc < 0) || (file_loc > INT_MAX))
	    {
	       // This is not a limitation of miniz or tinfl, but this example.
	       printf("File is too large to be processed by this example.\n");
	       return EXIT_FAILURE;
	    }

	    infile_size = (uint)file_loc;

	    // Open output file.
	    pOutfile = fopen(pDst_filename, "wb");
	    if (!pOutfile)
	    {
	      printf("Failed opening output file!\n");
	      return EXIT_FAILURE;
	    }

	    printf("Input file size: %u\n", infile_size);

	    // Init the z_stream
	    memset(&stream, 0, sizeof(stream));
	    stream.next_in = s_inbuf;
	    stream.avail_in = 0;
	    stream.next_out = s_outbuf;
	    stream.avail_out = BUF_SIZE;





	    // Compression.
	    uint infile_remaining = infile_size;
		  printf("setsetstsetsetestestsetset\n");

	    if (deflateInit(&stream, level) != Z_OK)
	    {
	      printf("deflateInit() failed!\n");
	      return EXIT_FAILURE;
	    }

	    for ( ; ; )
	    {
	  	  printf("setsetstsetsetestestsetset\n");

	      int status;
	      if (!stream.avail_in)
	      {
	        // Input buffer is empty, so read more bytes from input file.
	        uint n = my_min(BUF_SIZE, infile_remaining);

	        if (fread(s_inbuf, 1, n, pInfile) != n)
	        {
	          printf("Failed reading from input file!\n");
	          return EXIT_FAILURE;
	        }

	        stream.next_in = s_inbuf;
	        stream.avail_in = n;

	        infile_remaining -= n;
	        //printf("Input bytes remaining: %u\n", infile_remaining);
	      }
		  printf("setsetstsetsetestestsetset3456\n");

	      status = deflate(&stream, infile_remaining ? Z_NO_FLUSH : Z_FINISH);

	      if ((status == Z_STREAM_END) || (!stream.avail_out))
	      {
	        // Output buffer is full, or compression is done, so write buffer to output file.
	        uint n = BUF_SIZE - stream.avail_out;
	        if (fwrite(s_outbuf, 1, n, pOutfile) != n)
	        {
	          printf("Failed writing to output file!\n");
	          return EXIT_FAILURE;
	        }
	        stream.next_out = s_outbuf;
	        stream.avail_out = BUF_SIZE;
	      }

	      if (status == Z_STREAM_END)
	        break;
	      else if (status != Z_OK)
	      {
	        printf("deflate() failed with status %i!\n", status);
	        return EXIT_FAILURE;
	      }
	    }

	    if (deflateEnd(&stream) != Z_OK)
	    {
	      printf("deflateEnd() failed!\n");
	      return EXIT_FAILURE;
	    }

	    //
	    //
		  printf("setsetstsetsetestestsetset9872345978523987523\n");

	    return -69;
}
