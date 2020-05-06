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

int uziphelp(char* ibuf, char* obuf, unsigned int size)
{
	printf("Decompressing using miniz.c version: %s\n", MZ_VERSION);
	
	uint inbuf_size = size; //todo strlen terminates on null byte
	uint inbuf_remaining = inbuf_size;
	int level = Z_BEST_COMPRESSION;
	z_stream stream;

	// Init the z_stream
	memset(&stream, 0, sizeof(stream));
	stream.next_in = s_inbuf;
	stream.avail_in = 0;
	stream.next_out = s_outbuf;
	stream.avail_out = BUF_SIZE;
	if (inflateInit(&stream))
	{
		printf("inflateInit() failed!\n");
		return EXIT_FAILURE;
	}
	
	// Decompression
	for (; ; )
	{
		//printf("\n\nstartloop...\n");
		//printf("sinbuf: %s\n", s_inbuf);
		//printf("soutbuf: %s\n", s_outbuf);

		int status;
		if (!stream.avail_in)
		{
			// Input buffer is empty, so read more bytes from input file.
			uint n = my_min(BUF_SIZE, inbuf_remaining);
			//printf("N: %d\nBUF_SIZE: %d\n, inbuf_rem: %d\n", n, BUF_SIZE, inbuf_remaining);

			memcpy((char*)s_inbuf, ibuf, n); //ToDo - works! NEED to test with file bigger than buffer.

			ibuf += n;
			stream.next_in = s_inbuf;
			stream.avail_in = n;
			inbuf_remaining -= n;
		}

		status = inflate(&stream, Z_SYNC_FLUSH);
		//printf("stream in: %s\n", stream.next_in);
		//printf("stream out: %s\n", stream.next_out);

		if ((status == Z_STREAM_END) || (!stream.avail_out))
		{
			// Output buffer is full, or decompression is done, so write buffer to output buffer.
			uint n = BUF_SIZE - stream.avail_out;
			snprintf(obuf, n, (char*)s_outbuf);
			//memcpy(obuf, (char*)s_outbuf); // appears to work just the same
			
			//printf("WORKED YES --- last s_outbuf: %s\nobuf: %s\n", obuf);

			obuf += n;
			stream.next_out = s_outbuf;
			stream.avail_out = BUF_SIZE;
		}
		// error handling
		if (status == Z_STREAM_END)
			break;
		else if (status != Z_OK)
		{
			//printf("s_outbuf: %s\n", s_outbuf);
			printf("inflate() failed with status %i!\n", status);
			return EXIT_FAILURE; // return EXIT_FAILURE;
		}
	}
	// error handling
	if (inflateEnd(&stream) != Z_OK)
	{
		printf("inflateEnd() failed!\n");
		return EXIT_FAILURE;
	}

	printf("Total input bytes: %u\n", (mz_uint32)stream.total_in);
	printf("Total output bytes: %u\n", (mz_uint32)stream.total_out);
	printf("Success.\n");
	return EXIT_SUCCESS;
}
