/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include <zlib.h>

#include "libGINI.h"
#include "error.h"

#define PREFIX_LEN 4096
#define NWS_LEN 21
#define HEADER_LEN 512
#define BUFSIZE 8192

typedef struct {
	unsigned char *buf;
	size_t alloced;
	size_t used;
	size_t parsed;
} buffer;

FILE *GINI_open(char *filename) {
	FILE *result = NULL;
	
	if (!(result = fopen(filename, "r")))
		ERROR("NIDS_open");
	
	return result;
}

void GINI_close(FILE *fp) {
	
	fclose(fp);
	
}

unsigned char *parse_pdb (unsigned char *buf, GINI_pdb *p);
unsigned char *parse_ids(unsigned char *buf, GINI_ids *ids, size_t bytes);

char *zerr(int err)
{
	char *result = NULL;
	
	switch (err) {
		case Z_ERRNO:
			result = "Z_ERRNO;";
			break;
		
		case Z_STREAM_ERROR:
			result = "invalid compression level";
			break;
		
		case Z_DATA_ERROR:
			result = "invalid or incomplete deflate data";
			break;
		
		case Z_MEM_ERROR:
			result = "out of memory";
			break;
		
		case Z_VERSION_ERROR:
			result = "zlib version mismatch!";
			break;
		
		default:
			result = "default";
			break;
		
		
	}
	
	return result;
}

int is_zlib(unsigned char *buf) {
	int result = 0;
	
	if ( (*buf & 0xf) == Z_DEFLATED )
		if ( (*buf >> 4) + 8 <= 15 )
			if ( !(((*buf << 8) + *(buf+1)) % 31) )
				result = 1;
	
	return result;
}

void GINI_read (FILE *fp, GINI *data) {
  buffer buf = {};
	buffer img = {};
	unsigned char *temp = NULL;
	unsigned char *p = NULL;
	unsigned char *header = NULL;

	int err;
	z_stream strm = {};
	
	
	/***** read the tacked on nws header *****/
	
	if (!fread(data->nws, NWS_LEN, 1, fp))
		ERROR("GINI_read");
	
	printf("%s\n", data->nws);
	
	if (!(buf.buf = malloc(BUFSIZE)))
		ERROR("GINI_read");
	
	buf.alloced = BUFSIZE;
	
	do {
		size_t read;
		
		while (buf.used + BUFSIZE > buf.alloced) {
			buf.alloced *= 2;
			
			if (!(temp = realloc(buf.buf, buf.alloced)))
				ERROR("NIDS_read");
			
			buf.buf = temp;
		}
		
		if (!(read = fread(buf.buf + buf.used, 1, BUFSIZE, fp))) {
			if (ferror(fp))
				ERROR("NIDS_read");
		}
		
		buf.used += read;
		
	} while (!feof(fp) && read > 0);
	
	 p = buf.buf;
	
	/***** is the header compressed *****/
	
	if (is_zlib(buf.buf)) {
		
		if (!(p = header = malloc(NWS_LEN + HEADER_LEN)))
			ERROR("NIDS_read");
		
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
		strm.next_in = buf.buf;
    strm.avail_in = buf.used;
    strm.next_out = header;
    strm.avail_out = NWS_LEN + HEADER_LEN;
		
		/***** init *****/
		
		err = inflateInit(&strm);
		
		if (Z_OK != err)
  		ERROR(zerr(err));
		
		/***** inflate *****/
		
		err = inflate(&strm, Z_NO_FLUSH);

		if (Z_OK >= err)
  		ERROR(zerr(err));
		
		buf.parsed = strm.total_in;
		
		inflateEnd(&strm);

		/****** the second nws header ******/
		p += 21;
		
	}
	
	/***** parse the product description block *****/
	
	p = parse_pdb(p, &(data->pdb));
	
	/***** if it was compressed free the buffer *****/
	
	if (header)
		free(header);
	else
		buf.parsed = HEADER_LEN;
	
	data->pdb.compression = 0;
	
	size_t bytes = data->pdb.num_lines * data->pdb.pixels_per_line;
	
	/***** is the img compressed *****/
	
	if (is_zlib(buf.buf + buf.parsed)) {
		
		if (!(p = img.buf = malloc(bytes)))
			ERROR("NIDS_read");
		
		do {
			
		  strm.zalloc = Z_NULL;
		  strm.zfree = Z_NULL;
		  strm.opaque = Z_NULL;
			strm.next_in = buf.buf + buf.parsed;
		  strm.avail_in = buf.used - buf.parsed;
		  strm.next_out = img.buf + img.used;
		  strm.avail_out = bytes - img.used;
		
			/***** init *****/
		
			err = inflateInit(&strm);
		
			if (Z_OK != err)
				ERROR(zerr(err));
		
			/***** inflate *****/
		
			err = inflate(&strm, Z_NO_FLUSH);
			//printf ("err = %i strm.total_in = %lu\n", err, strm.total_in);

		
			buf.parsed += strm.total_in;
			img.used += strm.total_out;
			
		} while (err == Z_OK || err == Z_STREAM_END);
		
		inflateEnd(&strm);
	}
	
	p = parse_ids (p, &(data->ids), bytes);
	
	if (img.buf)
		free(img.buf);
	
	free(buf.buf);
	
	return;
}

void GINI_free(GINI *data) {
	free (data->ids);
}

void print_pdb(GINI_pdb *p, char *prefix);
void print_ids(GINI_ids *ids, size_t bytes);

void GINI_print(GINI *data, char *prefix) {
	char myprefix[PREFIX_LEN];
	
	snprintf(myprefix, PREFIX_LEN, "%s.GINI", prefix);
	
	print_pdb (&(data->pdb), myprefix);
	//print_ids(&(data->ids), data->pdb.num_lines * data->pdb.pixels_per_line);
	
	return;
};
