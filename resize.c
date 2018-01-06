/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, but also resizes it, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames and resize n
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    // check n
    if (n < 1 || n > 100)
    {
        printf("n must be in the range [1-100]\n");
        return 1;
    }

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    BITMAPFILEHEADER new_bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    new_bf = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    BITMAPINFOHEADER new_bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    new_bi = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // set new width and height dimensions
    new_bi.biWidth = bi.biWidth * n;
    new_bi.biHeight = bi.biHeight * n;

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding =  (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set new file size shout-out to https://stackoverflow.com/questions/25713117/what-is-the-difference-between-bisizeimage-bisize-and-bfsize
    new_bi.biSizeImage = (new_bi.biWidth * sizeof(RGBTRIPLE) + new_padding) * abs(new_bi.biHeight);
    new_bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + new_bi.biSizeImage;




    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // each row will be printed out n times


        for (int row = 0; row < n; row++)
        {

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;


                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int p = 0; p < n; p++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding in infile
			fseek(inptr, padding, SEEK_CUR);

			// then add it to outfile
			for (int k = 0; k < new_padding; k++)
			    fputc(0x00, outptr);

			fseek(inptr, -(bi.biWidth * 3 + padding ), SEEK_CUR);

        }
			fseek(inptr, bi.biWidth*3+padding, SEEK_CUR);
    }

		// close infile
		fclose(inptr);

		// close outfile
		fclose(outptr);

		// that's all folks
		return 0;
}