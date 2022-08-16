#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

struct w_type{ 
	double r; 
	double i;
	w_type(){};
	w_type(int32_t r1, int32_t i1)
	{
		r=r1;
		i=i1;
	}
};

void w_mac(w_type&  cc, w_type a, w_type w, w_type b ){
	cc.r = a.r + w.r * b.r - w.i * b.i;
	cc.i = a.i + w.r * b.i + w.i * b.r;
}

void fft(vector<w_type>& InOut, int n ) 
{ 
	w_type w;
	if (n == 1) 
		return; 
	vector<w_type> A,B;
	for (int i = 0; i < n / 2; i++) {
		A.push_back(InOut[i * 2]);
		B.push_back(InOut[i * 2 + 1]);
	}
	fft( A, n / 2 );
	fft( B, n / 2 );
	for (int i = 0; i < n; i++) {
		w.r = cos(2 * M_PI * i / n);
		w.i = sin(2 * M_PI * i / n);
		w_mac( InOut[i], A[i % (n / 2)], w, B[i % (n / 2)] );
	}
}
int main()
{
	string strInput;
	vector<w_type> InOut;
    ifstream inf("sound_in_the_time_domain.txt");
    while(inf){
		inf >> strInput;
		InOut.push_back(w_type(stoi(strInput),0));
	}
	fft( InOut, InOut.size()-1);
    ofstream outf("sound_in_the_frequency_domain.txt");
	for(int32_t i = 0; i < (InOut.size()-1)/2-1; i++) 
 		outf << InOut[i].r << endl; 	
	outf << InOut[(InOut.size()-1)/2-1].r;
	return 0;
}