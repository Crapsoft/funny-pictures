#pragma warning(disable:4996)

#include <iostream>
#include <string>

using namespace std;

int saveBMP(const char *fname, int *v, int mx, int my);
int *loadBMP(const char *fname, int &mx, int &my);

int main()
{
	string path;												// тут зберігаємо назву .bmp файлу
	int px, py;													// ширина і висота .bmp файлу													// висота файлу .bmp файлу
	cout << "Hi! Please enter the path your picture: \n";
	cin >> path;

	int *v = loadBMP("picture 1.bmp", px, py);					// загружаємо .bmp в програму


	delete[] v;													// звільняєм память

	return 0;
}

typedef unsigned __int16 WORD;

typedef struct {
	WORD   bfType;
	int    bfSize;
	int    bfReserved;
	int    bfOffBits;

	int    biSize;

	int    biWidth;
	int    biHeight;
	WORD   biPlanes;
	WORD   biBitCount;
	int    biCompression;

	int    biSizeImage;

	int    biXPelsPerMeter;
	int    biYPelsPerMeter;
	int    biClrUsed;

	int    biClrImportant;
} BMPheader;

int *loadBMP(const char *fname, int &mx, int &my)
{
	mx = my = -1;
	FILE *f = fopen(fname, "rb");
	if (!f) return NULL;
	BMPheader bh;
	size_t res;


	res = fread(&bh, 1, sizeof(BMPheader), f);
	if (res != sizeof(BMPheader)) { fclose(f); return NULL; }

	if (bh.bfType != 0x4d42 && bh.bfType != 0x4349 && bh.bfType != 0x5450) { fclose(f); return NULL; }


	fseek(f, 0, SEEK_END);
	int filesize = ftell(f);

	fseek(f, sizeof(BMPheader), SEEK_SET);

	if (bh.bfSize != filesize ||
		bh.bfReserved != 0 ||
		bh.biPlanes != 1 ||
		(bh.biSize != 40 && bh.biSize != 108 && bh.biSize != 124) ||
		bh.bfOffBits != 14 + bh.biSize ||

		bh.biWidth <1 || bh.biWidth >10000 ||
		bh.biHeight<1 || bh.biHeight>10000 ||
		bh.biBitCount != 24 ||
		bh.biCompression != 0
		)
	{
		fclose(f);
		return NULL;
	}

	mx = bh.biWidth;
	my = bh.biHeight;
	int mx3 = (3 * mx + 3) & (-4);
	unsigned char *tmp_buf = new unsigned  char[mx3*my];
	res = fread(tmp_buf, 1, mx3*my, f);
	if ((int)res != mx3*my) { delete[]tmp_buf; fclose(f); return NULL; }

	fclose(f);


	int* v = new int[mx*my];


	unsigned char *ptr = (unsigned char *)v;
	for (int y = my - 1; y >= 0; y--) {
		unsigned char *pRow = tmp_buf + mx3*y;
		for (int x = 0; x< mx; x++) {
			*ptr++ = *(pRow + 2);
			*ptr++ = *(pRow + 1);
			*ptr++ = *pRow;
			pRow += 3;
			ptr++;
		}
	}
	delete[]tmp_buf;
	return v;
}

int saveBMP(const char *fname, int *v, int mx,int my)	
{
	BMPheader bh;	
	memset(&bh, 0, sizeof(bh));
	bh.bfType = 0x4d42;	
						
	int mx3 = (3 * mx + 3) & (-4);
	int filesize = 54 + my*mx3;
	bh.bfSize = filesize;
	bh.bfReserved = 0;
	bh.biPlanes = 1;
	bh.biSize = 40;
	bh.bfOffBits = 14 + bh.biSize;
	bh.biWidth = mx;
	bh.biHeight = my;
	bh.biBitCount = 24;
	bh.biCompression = 0;

	FILE *f = fopen(fname, "wb");
	if (!f) return -1;
	size_t res;

	
	res = fwrite(&bh, 1, sizeof(BMPheader), f);
	if (res != sizeof(BMPheader)) { fclose(f); return -1; }

	
	unsigned char *tmp_buf = new unsigned char[mx3*my];
	
	unsigned char *ptr = (unsigned char *)v;
	for (int y = my - 1; y >= 0; y--) {
		unsigned char *pRow = tmp_buf + mx3*y;
		for (int x = 0; x< mx; x++) {
			*(pRow + 2) = *ptr++;
			*(pRow + 1) = *ptr++;
			*pRow = *ptr++;
			pRow += 3;
			ptr++;
		}
	}
	
	fwrite(tmp_buf, 1, mx3*my, f);
	fclose(f);
	delete[]tmp_buf;
	return 0;	
}