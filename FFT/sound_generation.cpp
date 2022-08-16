#include<iostream>
#include<cmath>
#include<fstream>
#include <ctime>

using namespace std;

const int32_t samplelenght=4096; //длина сэмпла
//samplerate - частота сэмпла
//wavefrequency - частота волны
//wavevolume - громкость волны

//Синусоида
void generate_sin(int32_t *buffer, const float& samplerate, const float& wavefrequency, const float& wavevolume){
	float period=samplerate/wavefrequency/2; //вычисляем период волны
	float n;
	for(int32_t a=0;a<samplelenght;a++) //устанавливаем цикл на длину сэмпла
	{
		n=wavevolume*sin(a*M_PI/period); //вычисление sine-волны
		buffer[a]=n; //заносим вычисленное значение в буфер
	}
}

//Пилообразная волна
void generate_saw(int32_t *buffer, const float& samplerate, const float& wavefrequency, const float& wavevolume){
	float period=samplerate/wavefrequency/2; //вычисляем период волны
	float sr_2m=samplerate/wavefrequency; //Вычисляем период
	int32_t c=0; //Специальная переменная для проверки на окончание периода
	for(int32_t a=0;a<samplelenght;a++){ //цикл на длину сэмпла
		if(c>=sr_2m) c=0; //если период закончился, то начать следующий
		buffer[a]=wavevolume*(c/period)-wavevolume; //вычисление волны
		c++;
	}
}

//Треугольная волна
void generate_triangle(int32_t *buffer, const float& samplerate, const float& wavefrequency, const float& wavevolume){
	float period=samplerate/wavefrequency/2; //вычисляем период волны
	int32_t c=period*2;
	int32_t c2=-1;
	float sr_2m=period;
	float sr=samplerate/wavefrequency/4;

	for(int32_t a=0;a<samplelenght;a++)
	{
		if(c>sr_2m) c=sr_2m,c2=-1;
		if(c<0) c=0,c2=1;
		buffer[a]=wavevolume*(c/sr)-wavevolume;
		c+=c2;
	}
}

void generate_noise(int32_t *buffer, const float& wavevolume){
	srand(time(0));
	for (int i = 0; i < samplelenght; i++) {
        const int noise = (rand() % int(2*wavevolume+1) + int(-wavevolume)); //[a,b]:rand() % (b - a + 1) + a
        buffer[i]=noise;
    }
}

//Cложение волн
void addition_of_waves(int32_t *buffer1, int32_t *buffer2, int32_t *result){
	int32_t res;
	for(int i = 0; i < samplelenght; i++){
		res = buffer1[i]+buffer2[i];
		if (res > INT32_MAX)
			res = INT32_MAX;
		else if (res < INT32_MIN)
			res = INT32_MIN;
		result[i] = res;
	}
}

int main()
{
	int32_t *sin1 = new int32_t[samplelenght];
	generate_sin(sin1, 4097, 500, 6536);

	int32_t *sin2 = new int32_t[samplelenght];
	generate_sin(sin2, 4097, 200, 6536);

	int32_t *sin3 = new int32_t[samplelenght];
	generate_sin(sin3, 4097, 1024, 6536);
	
	//int32_t *saw = new int32_t[samplelenght];
	//generate_saw(saw, 44100, 100, 65536);

	//int32_t *triangle = new int32_t[samplelenght];
	//generate_triangle(triangle, 44100, 100, 65536);

	//int32_t *result = new int32_t[samplelenght];
	//addition_of_waves(sin, triangle, result);

	//int32_t *noise = new int32_t[samplelenght];
	//generate_noise(noise, 65536);

	int32_t *result = new int32_t[samplelenght];
	addition_of_waves(sin1, sin2, result);

	addition_of_waves(result, sin3, result);

	ofstream outf("sound_in_the_time_domain.txt");
	for (int32_t i = 0; i < samplelenght-1; i++)
		outf << result[i] << endl;
	outf << result[samplelenght-1];
	return 0;
}