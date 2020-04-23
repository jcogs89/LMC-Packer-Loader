#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <limits.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <cstring>


#include "miniz.h"

using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

#define my_max(a,b) (((a) > (b)) ? (a) : (b))
#define my_min(a,b) (((a) < (b)) ? (a) : (b))

#define BUF_SIZE (1024 * 1024)
static uint8 s_inbuf[BUF_SIZE];
static uint8 s_outbuf[BUF_SIZE];

#pragma warning(disable:4996) // ToDo disabled insecure fopen warning

int uziphelp(char* ibuf, char* obuf)
{

	//const char *pMode;
	//FILE* pInfile, * pOutfile;
	uint inbuf_size = strlen(ibuf); //previously infile_size
	int level = Z_BEST_COMPRESSION;
	z_stream stream;
	//int p = 1;
	//const char* pSrc_filename = in.c_str();
	//const char* pDst_filename = out.c_str();
	//long file_loc;

	printf("miniz.c version: %s\n", MZ_VERSION);

	// Init the z_stream
	memset(&stream, 0, sizeof(stream));
	stream.next_in = s_inbuf;
	stream.avail_in = 0;
	stream.next_out = s_outbuf;
	stream.avail_out = BUF_SIZE;

	// Decompression.
	uint inbuf_remaining = inbuf_size; //previously infile_remaining 

	if (inflateInit(&stream))
	{
		printf("inflateInit() failed!\n");
		return EXIT_FAILURE;
	}
	//ToDo integrate decompression
	for (; ; )
	{
		int status;
		if (!stream.avail_in)
		{
			// Input buffer is empty, so read more bytes from input file.
			uint n = my_min(BUF_SIZE, inbuf_remaining);
			printf("N: %d\nBUF_SIZE: %d\n, inbuf_rem: %d\n", n, BUF_SIZE, inbuf_remaining);

			//printf("ibuf: %s, uintibuf: %s", ibuf, (uint8*)ibuf);
			//std::copy(&s_inbuf, ibuf, inbuf_remaining); //todo just reads entire buffer lol
			sprintf((char*)s_inbuf, ibuf, 1024 * 1024);
			printf("sbuf %s", s_inbuf);

			stream.next_in = s_inbuf;
			stream.avail_in = n;

			inbuf_remaining -= n;
		}

		status = inflate(&stream, Z_SYNC_FLUSH);

		if ((status == Z_STREAM_END) || (!stream.avail_out))
		{
			// Output buffer is full, or decompression is done, so write buffer to output file.
			uint n = BUF_SIZE - stream.avail_out;
			obuf = (char*)s_outbuf; //todo just reads entire buffer lol
			printf("obuf: %s", obuf);

			stream.next_out = s_outbuf;
			stream.avail_out = BUF_SIZE;
		}

		if (status == Z_STREAM_END)
			break;
		else if (status != Z_OK)
		{
			printf("inflate() failed with status %i!\n", status);
			return EXIT_FAILURE;
		}
	}

	if (inflateEnd(&stream) != Z_OK)
	{
		printf("inflateEnd() failed!\n");
		return EXIT_FAILURE;
	}

	printf("Total input bytes: %u\n", (mz_uint32)stream.total_in);
	printf("Total output bytes: %u\n", (mz_uint32)stream.total_out);
	printf("Success.\n");
	return EXIT_SUCCESS;

	return 0; //ToDo temp
}
