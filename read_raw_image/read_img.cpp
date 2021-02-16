#include <iostream>
#include <fstream>
using namespace std;

void read_img() {
	int width = 256, height = 256;
	unsigned char input_imgBuf[256][256];
	int img_int[256][256];
	int sparse_matrix[256][256];
	unsigned char output_imgBuf[256][256];

	FILE *fp = fopen("./gLenna256_256.raw", "rb");
	if (!fp) {
		cout << "can not open file" << endl;
	}
	fread(input_imgBuf, sizeof(char), width*height, fp);
	

	ofstream fout1;
	ofstream fout2;
	fout1.open("NZVL.txt");
	fout2.open("sparse_matrix.txt");

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			img_int[i][j] = int(input_imgBuf[i][j]);
			if (img_int[i][j] < 50) {
				img_int[i][j] = 0;
			}
			if (img_int[i][j] == 0) {
				sparse_matrix[i][j] = 0;
			}
			else if (img_int[i][j] != 0) {
				sparse_matrix[i][j] = 1;
			}

			output_imgBuf[i][j] = char(img_int[i][j]);
			fout1 << img_int[i][j] << " ";
			fout2 << sparse_matrix[i][j] << " ";
		}
		fout1 << endl;
		fout2 << endl;
	}

	FILE *fp_out = fopen("re_img.raw", "wb");
	fwrite(output_imgBuf, sizeof(char), width*height, fp_out);
}

int main(void) {
	read_img();
	return 0;
}