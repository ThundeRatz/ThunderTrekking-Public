#include <ga/GASimpleGA.h>
#include <ga/GABin2DecGenome.h>
#include <iostream>
#include "TrekkingKF.hh"

#define len(array)	(sizeof(array) / sizeof(array[0]))

using namespace std;
using namespace Trekking;

float genetic_objective(GAGenome &);

int main() {
	GABin2DecPhenotype map;
	float min[] = {0, 1,   1,   1,   100,  0.5, 0.5};
    float max[] = {1, 100, 100, 100, 1000, 100, 7};
	for (unsigned int i = 0; i < len(min); i++)
		map.add(32, min[i], max[i]);

	float target[len(min)];
	for (unsigned int i = 0; i < len(min); i++)
		target[i] = GARandomFloat(min[i], max[i]);

	GABin2DecGenome genome(map, genetic_objective, (void *) target);
	GASimpleGA genetic(genome);
	genetic.populationSize(100);
    genetic.nConvergence(50);
    genetic.pConvergence(.999);
    genetic.pMutation(0.01);
    genetic.pCrossover(0.6);
    genetic.scoreFilename("kalman_genetic.dat");
    genetic.flushFrequency(50);
    genetic.evolve();

	return 0;
}

float genetic_objective(GAGenome& raw_genome) {
	TrekkingKF filtro;
	GABin2DecGenome& genome = (GABin2DecGenome&) raw_genome;

	// float value = genome.nPhenotypes();
	// for(int i=0; i<genome.nPhenotypes(); i++)
	// 	value += 1.0 / (1.0 + fabs(genome.phenotype(i) - sequence[i]));
	return value;
}
