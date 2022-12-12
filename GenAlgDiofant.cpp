#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int max_pop = 25;

struct gene {
    int chromosomes[4], fitness;
    float likelihood;

	bool checkEquality(gene gn) {
		for (int i = 0; i < 4; i++) {
			if (gn.chromosomes[i] != chromosomes[i]) 
				return false;
		}

		return true;
	}
};

class Diophantine {
public:
	Diophantine(int a, int b, int c, int d, int res) : ca(a), cb(b), cc(c), cd(d), result(res){}; //коструктор со значениями a, b, c, d и result

	int Solve() {
		int fitness = -1;
		srand((unsigned)time(NULL));

		//генерация первого поколения
		for (int i = 0; i < max_pop; i++) {
			for (int j = 0; j < 4; j++) {
				population[i].chromosomes[j] = rand() % (result + 1);
			}
		}

		//нахождение fitness(близости к искомому значению) для всех особей(генов в популяции)
		if (fitness = CreateFitnesses()) {
			return fitness;
		}

		int iterations = 0;// Запись итераций алгоритма
		while (fitness != 0 || iterations < 50) {
			GenerateLikelihoods();// Вычисление вероятности выживаемости генов

			CreateNewPopulation();// Создание новой популяции(поколения)
			if (fitness = CreateFitnesses()) {
				return fitness;
			}

			iterations++;
		}

		return -1;
	};
	
	gene GetGene(int i) {
		return population[i];
	}

protected:
	int ca, cb, cc, cd;// Коэффиценты
	int result;
	gene population[max_pop]; // Пупуляция

	int Fitness(gene& gn) {
		int total = ca * gn.chromosomes[0] + cb * gn.chromosomes[1] + cc * gn.chromosomes[2] + cd * gn.chromosomes[3];
		return gn.fitness = abs(total - result);
	};// Функция вычисления fitness

	int CreateFitnesses() {
		float averagefit = 0;
		int fitness = 0;
		for (int i = 0; i < max_pop; i++) {
			fitness = Fitness(population[i]);
			averagefit += fitness;

			if (fitness == 0)
				return i;
		}

		return 0;
	};

	void GenerateLikelihoods() {
		float multinv = MultInv();

		float last = 0;
		for (int i = 0; i < max_pop; i++) {
			population[i].likelihood = last = last + ((1 / ((float)population[i].fitness) / multinv) * 100);
		}
	};	// Генерация вероятностей выживаемости

	float MultInv() {
		float sum = 0;

		for (int i = 0; i < max_pop; i++) {
			sum += 1 / ((float)population[i].fitness);
		}

		return sum;
	};// мультипликативная инверсия

	void CreateNewPopulation() {
		gene temp_pop[max_pop];

		for (int i = 0; i < max_pop; i++) {
			int parent1 = 0, parent2 = 0, iterations = 0;

			while (parent1 == parent2 || population[parent1].checkEquality(population[parent2])) {
				parent1 = GetIndex((float)(rand() % 101));
				parent2 = GetIndex((float)(rand() % 101));

				if (++iterations > 25)
					break;
			}

			temp_pop[i] = Breed(parent1, parent2);//Создание потомка(размножение)
		}

		for (int i = 0; i < max_pop; i++)
			population[i] = temp_pop[i];
	};

	int GetIndex(float value) {
		float last = 0;
		for (int i = 0; i < max_pop; i++) {
			if (last <= value && value <= population[i].likelihood)
				return i;
			else
				last = population[i].likelihood;
		}

		return 4;
	}; //Поиск гена в популяции по его значению


	gene Breed(int parent1, int parent2) {
		int crossover = rand() % 3 + 1;// Случайный выбор точки-разделителя
		int first = rand() % 100;// Выбор первого родителя

		gene child = population[parent1]; // Взятие генов от первого родителя

		int initial = 0, final = 3;// Границы кроссовера
		if (first < 50)
			initial = crossover;	// Определение принадлежности первой и второй частей потомка первому или второму родителю
		else final = crossover + 1;

		for (int i = initial; i < final; i++) {// Скрещивание
			child.chromosomes[i] = population[parent2].chromosomes[i];
			if (rand() % 101 < 5)
				child.chromosomes[i] = rand() % (result + 1);
		}

		return child;
	}; // Функция размножения двух особей
};

int main()
{
	Diophantine diop(1, 2, 3, 4, 30);
	int ans;

	for (int i = 0; i < 10; i++)
	{
		ans = diop.Solve();

		if (ans == -1 && i == 9) {
			cout << "There is no solution founded. " << endl;
		}
		else {
			gene gn = diop.GetGene(ans);
			cout << "The soultion for a + 2b + 3c + 4d = 30 is:\n " << "a= " << gn.chromosomes[0] << ".\n" <<
				"b= " << gn.chromosomes[1] << ".\n" << "c= " << gn.chromosomes[2] << ".\n" <<
				"d= " << gn.chromosomes[3] << "." << endl;
			break;
		}
	}
}
