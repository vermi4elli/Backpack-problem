#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

const int p = 150;
const int amount = 100;

void FillCostWeightOfThings(vector<int>& cost, vector<int>& weight) {
	for (int i = 0; i < amount; i++) {
		//setting the cost from 2 to 20
		cost[i] = rand() % 19 + 2;
		//setting the weight from 1 to 10
		weight[i] = rand() % 10 + 1;
	}
}
vector<pair<vector<bool>, long>> CreateFirstPopulation(const vector<int>& cost, const vector<int>& weight) {
	vector<bool> temp_v(amount, 0);
	long temp_l = 0;
	pair<vector<bool>, long> temp = make_pair(temp_v, temp_l);
	vector<pair<vector<bool>, long>> result(amount, temp);
	for (int i = 0; i < amount; i++) {
		for (int j = 0; j < amount; j++) {
			if (j != i) result[i].first[j] = 0;
			result[i].first[i] = 1;
			result[i].second = cost[i];
		}
	}
	sort(result.begin(), result.end(), [](pair<vector<bool>, long> l, pair<vector<bool>, long> r) {
		return l.second > r.second;
		});
	return result;
}

pair<vector<bool>, long> Crossover(const pair<vector<bool>, long>& l, const pair<vector<bool>, long>& r, const vector<int>& cost, const vector<int>& weight) {
	vector<bool> temp_v(amount);
	long temp_l = 0;
	long temp_w = 0;
	for (int i = 0; i < amount; i++) {
		if (i < amount / 2) temp_v[i] = l.first[i];
		else temp_v[i] = r.first[i];
		if (temp_v[i]) {
			temp_l += cost[i];
			temp_w += weight[i];
		}
	}
	if (temp_w > p) {
		int i = amount - 1;
		for (int i = temp_v[amount - 1]; i >= 0; i--) {
			if (temp_v[i]) {
				temp_v[i] = 0;
				temp_l -= cost[i];
				temp_w -= weight[i];
			}
			if (temp_w <= p) break;
		}
	}
	return make_pair(temp_v, temp_l);
}
pair<vector<bool>, long> Mutate(pair<vector<bool>, long>& descendant, const vector<int>& cost, const vector<int>& weight) {
	//cout << "MUTATED!!!" << endl;
	
	vector<bool> temp_v(amount);
	long temp_l = 0;
	long temp_w = 0;

	temp_v = descendant.first;
	temp_v[rand() % amount] = 1;
	for (int i = 0; i < amount; i++) {
		if (temp_v[i]) {
			temp_l += cost[i];
			temp_w += weight[i];
		}
	}

	if (temp_w > p) {
		return descendant;
	}
	return make_pair(temp_v, temp_l);
}
void Evolve(vector<pair<vector<bool>, long>>& population, const vector<int>& cost, const vector<int>& weight) {
	
	pair<vector<bool>, long> descendant = Crossover(population[0], population[amount - 1], cost, weight);
	int probability = rand() % 20 + 1;
	if (probability == 1) {
		descendant = Mutate(descendant, cost, weight);
	}
	population.push_back(descendant);
	sort(population.begin(), population.end(), [](pair<vector<bool>, long> l, pair<vector<bool>, long> r) {
		return l.second > r.second;
	});

	population.pop_back();
}


int main() {
	vector<int> cost(amount);
	vector<int> weight(amount);
	FillCostWeightOfThings(cost, weight);
	vector<pair<vector<bool>, long>> population = CreateFirstPopulation(cost, weight);
	cout << "Enter the amount of repetitions: ";
	int repetitions = 0;
	cin >> repetitions;
	for (int i = 1; i <= repetitions; i++) {
		Evolve(population, cost, weight);
		if (i % 20 == 0) cout << "Rep #" << i << ": The best is: " << population[0].second << endl;
	}
	
	return 0;
}
