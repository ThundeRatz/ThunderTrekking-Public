/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/*
 * Algoritmos genéticos são bem fáceis de escrever, mas usar a GAlib tem a
 * vantagem de ter infinitos métodos diferentes para fitness, seleção,
 * cruzamento, mutação... Se quisermos fazer benchmarks com estruturas diferentes
 * é bem menos trabalho.
 * Esse é só um exemplo de como usar, é basicamente uma cópia do exemplo 9 do
 * código fonte com algumas simplificações pequenas.
 */


#include <iostream>
#include <ga/ga.h>
#include <ga/std_stream.h>

using namespace std;

float objective(GAGenome &);

int main(int argc, char **argv) {
    int popsize  = 30;
    int ngen     = 100;
    float pmut   = 0.01;
    float pcross = 0.6;

    // Create a phenotype for two variables.  The number of bits you can use to
    // represent any number is limited by the type of computer you are using.  In
    // this case, we use 16 bits to represent a floating point number whose value
    // can range from -5 to 5, inclusive.  The bounds on x1 and x2 can be applied
    // here and/or in the objective function.

    GABin2DecPhenotype map;
    map.add(16, -5, 5);
    map.add(16, -5, 5);

    // Create the template genome using the phenotype map we just made.

    GABin2DecGenome genome(map, objective);

    // Now create the GA using the genome and run it.  We'll use sigma truncation
    // scaling so that we can handle negative objective scores.

    GASimpleGA ga(genome);
    GASigmaTruncationScaling scaling;
    ga.populationSize(popsize);
    ga.nGenerations(ngen);
    ga.pMutation(pmut);
    ga.pCrossover(pcross);
    ga.scaling(scaling);
    ga.scoreFilename("genetic.dat");
    ga.scoreFrequency(10);
    ga.flushFrequency(50);
    ga.evolve(14); // seed

    genome = ga.statistics().bestIndividual();
    cout << "the ga found an optimum at the point (";
    cout << genome.phenotype(0) << ", " << genome.phenotype(1) << ")\n\n";
    cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";
    return 0;
}

// y = -(x1*x1 + x2*x2)
float objective(GAGenome & c) {
    GABin2DecGenome & genome = (GABin2DecGenome &)c;
    return -(genome.phenotype(0) * genome.phenotype(0) + genome.phenotype(1) * genome.phenotype(1));
}
