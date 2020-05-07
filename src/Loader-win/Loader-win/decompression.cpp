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

//returns size of unzipped file if succeeds
int uziphelp(char* ibuf, unsigned char* obuf, unsigned int inbuf_size)
{
	printf("Decompressing using miniz.c version: %s\n", MZ_VERSION);

	//variable declarations
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
		int status;
		if (!stream.avail_in)
		{
			// Input buffer is empty, so read more bytes from input file.
			uint n = my_min(BUF_SIZE, inbuf_remaining);

			memcpy((char*)s_inbuf, ibuf, n); // Read input buffer into the stream buffer

			// Increment input buffer and zstream
			ibuf += n;
			stream.next_in = s_inbuf;
			stream.avail_in = n;
			inbuf_remaining -= n;
		}

		// Does the hard work
		status = inflate(&stream, Z_SYNC_FLUSH);

		// Output buffer is full, or decompression is done, so write buffer to output buffer.
		if ((status == Z_STREAM_END) || (!stream.avail_out))
		{
			uint n = BUF_SIZE - stream.avail_out;
			memcpy(obuf, s_outbuf, n);

			// Increment output buffer and zstream
			obuf += n;
			stream.next_out = s_outbuf;
			stream.avail_out = BUF_SIZE;
		}
		// error handling
		if (status == Z_STREAM_END)
			break;
		else if (status != Z_OK)
		{
			printf("inflate() failed with status %i!\n", status);
			return EXIT_FAILURE;
		}
	}
	// more error handling
	if (inflateEnd(&stream) != Z_OK)
	{
		printf("inflateEnd() failed!\n");
		return EXIT_FAILURE;
	}

	//success
	printf("Decompression successful.\n");
	//printf("Total input bytes: %u\n", (mz_uint32)stream.total_in);
	//printf("Total output bytes: %u\n", (mz_uint32)stream.total_out);
	return (mz_uint32)stream.total_out;
}
