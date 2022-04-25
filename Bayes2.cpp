#include <vector>
#include <math.h>
#include <numeric>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>

using namespace std;

class ClasificadorBayes
{
private:
	unordered_map<int, double> clases;

	unordered_map<int, unordered_map<int, double>> atributos;
public:

	ClasificadorBayes(vector<vector<int>>& data, int tam)
	{

		for (auto entry : data)
		{
			if (clases.find(entry[0]) == clases.end())
			{
				clases[entry[0]] = 1;
				unordered_map<int, double> pxc;
				atributos[entry[0]] = pxc;
			}
			else
			{
				clases[entry[0]] += 1;
			}
			for (int k = 1; k <= tam; k++)
			{
				if (atributos[entry[0]].find(entry[k]) == atributos[entry[0]].end())
				{
					atributos[entry[0]][entry[k]] = 1;
				}
				else
				{
					atributos[entry[0]][entry[k]] += 1;
				}
			}
		}

		for (auto seg : atributos)
		{
			cout << "Class " << seg.first << " " << endl;
			for (auto entry : seg.second)
			{
				entry.second /= clases[seg.first];
				cout << "Atributo P(x = " << entry.first << " | C = " << seg.first << ") = " << entry.second << endl;
			}
			clases[seg.first] /= data.size();
			cout << "Clase P(C = " << seg.first << ") = " << clases[seg.first] << endl;
		}
	}

	int predict(vector<int> atributos2)
	{
		int maxcid = -1;
		double maxp = 0;
		for (auto cls : clases)
		{
			// p(C|x) = p(C)*p(x1|C)*p(x2|C)*…
			double pCx = cls.second;
			for (int i = 0; i < atributos2.size(); i++)
			{
				double f= atributos[cls.first][atributos2[i]];
				pCx *= f;
			}
			if (pCx > maxp)
			{
				maxp = pCx;
				maxcid = cls.first;
			}
		}
		cout << "Posible clase : " << maxcid << " P(C | x) = " << maxp << endl;
		return maxcid;
	}
};
void IngresarData(vector<vector<int>>& data, unordered_map<string, int>& mapclases, unordered_map<string, int>& mapatributos,
	unordered_map<string, int>& mapatributos2, unordered_map<string, int>& mapatributos3, unordered_map<string, int>& mapatributos4,
	string c, string a1, string a2,string a3,string a4,string a5,string a6, int K)
{
	vector<int> apair = { mapclases[c],mapatributos[a1], mapatributos2[a2],mapatributos3[a3],mapatributos[a4],mapatributos[a5],mapatributos4[a6]};
	vector<vector<int>> newarr(K, apair);
	data.insert(data.end(), newarr.begin(), newarr.end());
}
vector<string> split(const string& sentence, char param) {
	string parte;
	vector<string> separate;
	stringstream x(sentence);
	while (getline(x, parte, param)) {
		separate.push_back(parte);
	}
	return separate;
}
int main() {

	int numv = 0;
	int numg = 0;
	int numu = 0;
	int numa = 0;
	unordered_map<string, int> mapclases = { {"good", 0}, {"vgood", 1},{"unacc",2},{"acc",3}};
	unordered_map<string, int> mapatributos =
		// buying
	{ {"v-high", 0}, {"high", 1}, {"med", 2},{"low",3},
		//persons
		{"2",20},{"4",21},{"more",22},
		//lug_boot
		{"small",30},{"med",31},{"big",32}
	};
	unordered_map<string, int> mapatributos2 =// maint
	{
	{ "v-high", 5 }, { "high", 6 }, { "med", 7 }, { "low",8 } };
	unordered_map<string, int> mapatributos3 =
	{
		// doors
		{"2",10},{"3",11},{"4",12},{"5more",13}
	};
	unordered_map<string, int> mapatributos4 =// maint
	{
		//safety
		{"low",40},{"med",41},{"high",42}
	};

	vector<vector<int>> data;

	ifstream archivo;
	archivo.open("prueba.txt",ios::in);
	string texto;
	vector<string> a;
	while (!archivo.eof()) {
		getline(archivo,texto);
		a=split(texto,',');
		if (a[6] == "vgood") {
			if (numv<65) {
				numv++;
				IngresarData(data, mapclases, mapatributos, mapatributos2, mapatributos3, mapatributos4, a[6], a[0], a[1], a[2], a[3], a[4], a[5], 1);
			}
		}
		else if (a[6] == "good") {
			if (numg<65) {
				numg;
				IngresarData(data, mapclases, mapatributos, mapatributos2, mapatributos3, mapatributos4, a[6], a[0], a[1], a[2], a[3], a[4], a[5], 1);
			}
		}
		else if (a[6] == "unacc") {
			if (numu<65) {
				numu++;
				IngresarData(data, mapclases, mapatributos, mapatributos2, mapatributos3, mapatributos4, a[6], a[0], a[1], a[2], a[3], a[4], a[5], 1);
			}
		}
		else if (a[6]=="acc") {
			if (numa < 65) {
				numa++;
				IngresarData(data, mapclases, mapatributos, mapatributos2, mapatributos3, mapatributos4, a[6], a[0], a[1], a[2], a[3], a[4], a[5], 1);
			}
		}
	}
	random_shuffle(data.begin(), data.end());
	ClasificadorBayes modelo(data, 6);
	int cls = modelo.predict({ mapatributos["low"],mapatributos2["low"],mapatributos3["5more"],mapatributos["more"],mapatributos["big"],mapatributos4["high"] });
	cout << "Predicted class " << cls << endl; 
	return 0;
}