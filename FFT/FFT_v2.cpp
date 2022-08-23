#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

struct W_type
{
    double r; 
    double i;
    W_type() {};
    W_type(int32_t r1, int32_t i1)
    {
        r = r1;
        i = i1;
    }
};

void w_mac(W_type &cc, W_type a, W_type w, W_type b) 
{
    cc.r = a.r + w.r * b.r - w.i * b.i;
    cc.i = a.i + w.r * b.i + w.i * b.r;
}

void fft(vector<W_type> &InOut, int n ) 
{ 
    W_type w;
	
    if (n == 1) 
        return; 
	
    vector<W_type> A,B;
	
    for (int i = 0; i < n / 2; i++) {
        A.push_back(InOut[i * 2]);
        B.push_back(InOut[i * 2 + 1]);
    }
	
    fft(A, n / 2);
    fft(B, n / 2);
	
    for (int i = 0; i < n; i++) {
        w.r = cos(2 * M_PI * i / n);
        w.i = sin(2 * M_PI * i / n);
        w_mac(InOut[i], A[i % (n / 2)], w, B[i % (n / 2)]);
    }
}

void reverse_fft(vector<W_type> &InOut, int n)
{	
    for (int i = 0; i < n; i++)
        InOut[i].i =- InOut[i].i;
       
    fft(InOut, n);

    for (int i = 0; i < n; i++) { //Деление на N во временной области со сменой арифметического знака мнимой части
        InOut[i].r = InOut[i].r / n;
        InOut[i].i = -InOut[i].i / n;
    }

    ofstream outf("the_result_of_the_reverse_FFT.txt");
    for (int32_t i = 0; i < n - 1; i++)
        outf << InOut[i].r << endl;
    outf << InOut[n-1].r;
    outf.close();
}

void low_pass_parabolic_filter(vector<W_type> &InOut, int32_t pass_frequency, int32_t stop_frequency)
{	
    ofstream outf("window_for_low_pass_parabolic_filter.txt");

    for (int32_t i = pass_frequency + 1; i < stop_frequency; i++)
        InOut[i].r = InOut[i].r * (1 - (pow(i - pass_frequency, 2) / pow(stop_frequency - pass_frequency, 2)));
    for (int32_t i = stop_frequency; i <= InOut.size() / 2; i++)
        InOut[i].r = 0;	
    for (int32_t i = InOut.size() - pass_frequency - 1; i > InOut.size() / 2; i--) //отражаем результат на отрицательную часть спектра
        InOut[i].r = InOut[InOut.size() - i].r;

    for (int32_t i = pass_frequency + 1; i < stop_frequency - 1; i++)
        outf << InOut[i].r << endl;
    outf << InOut[stop_frequency - 1].r;
}

int main()
{
    string strInput;
    vector<W_type> InOut;

    ifstream inf("sound_in_the_time_domain.txt");
    while (getline(inf, strInput))
        InOut.push_back(W_type(stoi(strInput), 0));
    inf.close();

    fft(InOut, InOut.size());

    low_pass_parabolic_filter(InOut, 10, 15);
    
    ofstream outf("sound_in_the_frequency_domain.txt");
    for (int32_t i = 0; i < InOut.size() - 1; i++) 
        outf << InOut[i].r << endl; 	
    outf << InOut[InOut.size() - 1].r;
    outf.close();

    reverse_fft(InOut, InOut.size());

    return 0;
}