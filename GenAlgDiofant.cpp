﻿#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int max_pop = 200;

struct gene {
    int chromosomes[7], fitness;
    float likelihood;

	bool checkEquality(gene gn) {
		for (int i = 0; i < 7; i++) {
			if (gn.chromosomes[i] != chromosomes[i]) 
				return false;
		}

		return true;
	}
};

class Diophantine {
public:
	Diophantine(int a, int b, int c, int d, int e, int f, int g, int res) : ca(a), cb(b), cc(c), cd(d), ce(e), cf(f), cg(g), result(res) {};

	int Solve() {
		int fitness = -1;
		srand((unsigned)time(NULL));

		//генерация первого поколения
		for (int i = 0; i < max_pop; i++) {
			for (int j = 0; j < 7; j++) {
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
	int ca, cb, cc, cd, ce, cf, cg;// Коэффиценты
	int result;
	gene population[max_pop]; // Пупуляция

	int Fitness(gene& gn) {
		int total = total = ca * gn.chromosomes[0] + cb * gn.chromosomes[1] + cc * gn.chromosomes[2] + cd * gn.chromosomes[3] +
			ce * gn.chromosomes[4] + cf * gn.chromosomes[5] + cg * gn.chromosomes[6];

		return gn.fitness = abs(total - result);
	}// Функция вычисления fitness

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

			if (rand() % 100 < 8) {
				temp_pop[i] = mutation(population[GetIndex((float)i)]); //мутация данного гена с вероятность 0.08
			}
			else {
				while (parent1 == parent2 || population[parent1].checkEquality(population[parent2])) {
					parent1 = GetIndex((float)(rand() % 101));
					parent2 = GetIndex((float)(rand() % 101));

					if (++iterations > 25)
						break;
				}

				temp_pop[i] = Breed(parent1, parent2);//Создание потомка(размножение)
			}
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

		return 7;
	}; // Поиск гена в популяции по его значению

	gene mutation(gene& gn) {
		gn.chromosomes[gn.chromosomes[rand() % 7]] = rand() % 100 + 1;
		return gn;
	} // Функция мутации гена

	gene Breed(int parent1, int parent2) {
		int crossover = rand() % 6 + 1;// Случайный выбор точки-разделителя
		int first = rand() % 100;// Выбор первого родителя

		gene child = population[parent1]; // Взятие генов от первого родителя

		int initial = 0, final = 6;// Границы кроссовера
		if (first < 50)
			initial = crossover;	// Определение принадлежности первой и второй частей потомка первому или второму родителю
		else final = crossover + 1;

		for (int i = initial; i < final; i++) {// Скрещивание
			child.chromosomes[i] = population[parent2].chromosomes[i];
			if (rand() % 101 < 8)
				child.chromosomes[i] = rand() % (result + 1);
		}

		return child;
	}; // Функция размножения двух особей
};

int main()
{
	Diophantine diop(1, 2, 3, 4, 5, 6, 7, 60);
	int ans;

	for (int i = 0; i < 100; i++)
	{
		ans = diop.Solve();

		if (i == 99) {
			cout << "There is no solution founded. " << endl;
			break;
		}
		else if (ans != -1) {
			gene gn = diop.GetGene(ans);
			cout << "The solution for a + 2b + 3c + 4d + 5e + 6f + 7g = 60 is:\n " << "a= " << gn.chromosomes[0] << ".\n" <<
				"b= " << gn.chromosomes[1] << ".\n" << "c= " << gn.chromosomes[2] << ".\n" <<
				"d= " << gn.chromosomes[3] << ".\n" << "e= " << gn.chromosomes[4]
				<< ".\n" << "f= " << gn.chromosomes[5] << ".\n" << "g= " << gn.chromosomes[6] << "." << endl;
			break;
		}
	}
}
