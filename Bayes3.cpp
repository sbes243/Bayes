#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;


int randomInt(int ini, int fin) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> randNode(ini, fin);
    return randNode(mt);
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

double
ProbCond(const vector<vector<string>>& datos, int raw, const string& valor, int rawClass, const string& valorClass) {
    double intersect = 0;
    double ClassTotal = 0;
    for (auto i : datos) {
        if (i[rawClass] == valorClass) {
            ClassTotal++;
            if (i[raw] == valor) {
                intersect++;
            }
        }
    }
    return intersect / ClassTotal;
}

string Clasificador(const vector<vector<string>>& datos, const vector<string>& quest, int rawClass,
    const vector<string>& values) {
    vector<double> probs(values.size(), 1);
    for (int i = 0; i < quest.size(); i++) {
        for (int j = 0; j < probs.size(); j++) {
            probs[j] *= ProbCond(datos, i, quest[i], rawClass, values[j]);
        }
    }
    int iter = 0;
    double maxi = probs[0];
    for (int i = 1; i < probs.size(); i++) {
        if (probs[i] > maxi) {
            maxi = probs[i];
            iter = i;
        }
    }
    return values[iter];
}


int main() {
    vector<vector<string>> dataSet1; //good
    vector<vector<string>> dataSet2; //vgood
    vector<vector<string>> dataSet3; //unacc
    vector<vector<string>> dataSet4; //acc

    int nDatos = 65;
    ifstream MyReadFile("car.data");
    string line;
    int good = nDatos, vgood = nDatos, unacc = nDatos, acc = nDatos;
    while (getline(MyReadFile, line)) {
        vector<string> separate = split(line, ',');
        if (separate[6] == "good" and good) {
            dataSet1.push_back(separate);
            good--;
        }
        if (separate[6] == "vgood" and vgood) {
            dataSet2.push_back(separate);
            vgood--;
        }
        if (separate[6] == "unacc" and unacc) {
            dataSet3.push_back(separate);
            unacc--;
        }
        if (separate[6] == "acc" and acc) {
            dataSet4.push_back(separate);
            acc--;
        }
    }

    int classData = 6;
    vector<string> values = { "good", "vgood", "acc", "unacc" };
    double tamTesting = (double)nDatos * 0.2;
    double averageError = 0;
    int n = 10; // Cantidad de pruebas
    int j = 1; // Número de prueba para imprimir

    for (int test = 0; test < n; test++) {
        vector<vector<string>> training;
        vector<vector<string>> testing;

        auto temp1 = dataSet1; //good
        auto temp2 = dataSet2; //vgood
        auto temp3 = dataSet3; //unacc
        auto temp4 = dataSet4; //acc

        // Escojo aleatoriamente el 20% de cada tipo de dato para nuestro testing
        for (int i = 0; i < (int)tamTesting; i++) {
            int iter1 = randomInt(0, (int)temp1.size() - 1);
            testing.push_back(temp1[iter1]);
            temp1.erase(temp1.begin() + iter1);

            int iter2 = randomInt(0, (int)temp2.size() - 1);
            testing.push_back(temp2[iter2]);
            temp2.erase(temp2.begin() + iter2);

            int iter3 = randomInt(0, (int)temp3.size() - 1);
            testing.push_back(temp3[iter3]);
            temp3.erase(temp3.begin() + iter3);

            int iter4 = randomInt(0, (int)temp4.size() - 1);
            testing.push_back(temp4[iter4]);
            temp4.erase(temp4.begin() + iter4);
        }

        // El resto lo uso como entrenamiento del algoritmo
        training.insert(training.end(), temp1.begin(), temp1.end());
        training.insert(training.end(), temp2.begin(), temp2.end());
        training.insert(training.end(), temp3.begin(), temp3.end());
        training.insert(training.end(), temp4.begin(), temp4.end());


        int acierto = 0;
        int error = 0;
        for (auto& i : testing) {
            auto quest = i;
            quest.pop_back();
            auto res = Clasificador(training, quest, classData, values);
            (i[classData] == res) ? acierto++ : error++;
        }
        cout << "Prueba " << j++ << endl;
        cout << "Cantidad de errores:" << error << endl;
        cout << "Porcentaje de error:" << (double)error * 100.0f / (double)testing.size() << endl << endl;
        averageError += (double)error * 100.0f / (double)testing.size();
    }
    cout << "Porcentaje de error promedio: " << averageError / n;

}
