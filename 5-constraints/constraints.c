/**
 * Author: Cifro Nix
 * Version: 1.0
 *
 * Copyright (c) 2011 Cifro Nix (http://about.me/Cifro)
 *
 * For the full copyright and license information, please view
 * the file license.txt that is distributed with this source code.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define random(num) (rand()%(num))
#define randomize() srand((unsigned)time(NULL))

// max individuals
#define populationSize 500
// max generations
#define maxGenerations 1000

// size of world
#define xAxis 30
#define yAxis 25


#define genotypeSize yAxis + xAxis - 2

#define targetFitness 1099


typedef struct{
	int fitness;
	int genotype[genotypeSize];	// binary representation of path, 0 = move right, 1 = move up
	int penalized;
}Individual;


// struct for not penalized individuals
typedef struct{
	int number;
	Individual notPenalized[populationSize];
}NotPenalized;


Individual population[populationSize];
Individual parents[populationSize];

// world is indexed from 1
int world[yAxis + 1][xAxis + 1];


void worldInit();
void populationInit();
void setFitnessAndPenalize();
void sort(Individual *individuals);



void worldInit()
{
	int y, x;

	for(y = 1; y < yAxis + 1; y++){
		for(x = 1; x < xAxis + 1; x++){
			if((x >= 6 && x <= 10 && y >= 9 && y <= 15) || (x >= 21 && x <= 25 && y >= 11 && y <= 17)){
				world[y][x] = 0;
			}else{
				world[y][x] = 25 - abs(y - 13);
			}
		}
	}
}



void populationInit()
{
	int i, j, direction, x, y;

	for(i = 0; i < populationSize; i++){
		y = 1;
		x = 1;

		population[i].penalized = 0; // no one is penalized

		for(j = 0; j < genotypeSize; j++){
			direction = random(2);

			// 0 = move right, 1 = move up
			(direction == 0) ? x++ : y++;

			population[i].genotype[j]= direction;
		}
	}

	setFitnessAndPenalize();
}



void sort(Individual *individuals)
{
	int i, j;
	Individual temp;

	// sort it please
	for(i = 0; i < populationSize - 1; i++){
		for(j = i + 1; j < populationSize; j++){
			if(individuals[i].fitness < individuals[j].fitness){
				temp = individuals[j];
				individuals[j] = individuals[i];
				individuals[i] = temp;
			}
		}
	}
}



void setFitnessAndPenalize()
{
	int i, j, maxFitness = 0, x, y;

	for(i = 0; i < populationSize; i++){
		x = 1;
		y = 1;

		population[i].penalized = 0;
		population[i].fitness = 0;
		population[i].fitness += world[1][1];

		for(j = 0; j < genotypeSize; j++){

			// fix
			(population[i].genotype[j] == 0) ? x++ : y++;

			if(x > xAxis){
				x--; y++;
				population[i].genotype[j] = 1;
			}

			if(y > yAxis){
				y--; x++;
				population[i].genotype[j] = 0;
			}

			if((x >= 6 && x <= 10 && y >= 9 && y <= 15) || (x >= 21 && x <= 25 && y >= 11 && y <= 17) || (x >= 11 && x <= 20 && y == 14)){
				// if individual goes through forbidden area penalize it
				population[i].penalized = 1;
			}else{
				// else set fitness
				population[i].fitness += world[y][x];
			}
		}
	}
}



// get not penalized individuals from population and
// store and return its in NotPenalized struct
NotPenalized getNotPenalized(){

	int i = 0, k = 0;
	NotPenalized individuals;

	individuals.number = 0;

	for(i = 0; i < populationSize; i++){
		if(population[i].penalized == 0){
			individuals.notPenalized[individuals.number] = population[i];
			individuals.number++;
		}
	}

	return individuals;
}



// Crop, sort, random selection
void selection()
{
	NotPenalized goodOnes;
	int i = 0, r;
	float c = 0.4;
	int threshold;

	goodOnes = getNotPenalized();

	// we haven't enaugh of good individuals => reinitialize population
	if(goodOnes.number <= 2)
		populationInit();

	// there is few individuals, so threshold may be full
	if(goodOnes.number <= 10) c = 1.0;

	threshold =	(int) (c * goodOnes.number);
	sort(goodOnes.notPenalized);

	for(i = 0; i < populationSize; i++){
		r = random(threshold);
		parents[i] = goodOnes.notPenalized[r];
	}
}



// one point crossover
void crossover()
{
	int i, j, half = genotypeSize / 2;

	for(i = 0; i < populationSize; i += 2){

		for(j = 0; j < half; j++){
			population[i].genotype[j] = parents[i].genotype[j];
			population[i + 1].genotype[j] = parents[i + 1].genotype[j];
		}

		for(j = half; j < genotypeSize; j++){
			population[i].genotype[j] = parents[i + 1].genotype[j];
			population[i + 1].genotype[j] = parents[i].genotype[j];
		}
	}
}



// mutation of two random bits in each individual
void mutation()
{
	int i;

	for(i = 0; i < populationSize; i++){
		population[i].genotype[random(genotypeSize)] = 1;
		population[i].genotype[random(genotypeSize)] = 0;
	}
}



// get individual from not penalized ones in generation
Individual getBestIndividual()
{
	int i = 0;
	NotPenalized goodOnes;
	Individual best;

	goodOnes = getNotPenalized();
	best = goodOnes.notPenalized[0];

	for(i = 0; i < goodOnes.number; i++){
		if(best.fitness < goodOnes.notPenalized[i].fitness){
			best = goodOnes.notPenalized[i];
		}
	}

	return best;
}



void printAxisOfBest(Individual daBest)
{
	int i, x = 1, y = 1;

	printf(" 1  1\n");
	for(i = 0; i < genotypeSize; i++){
		(daBest.genotype[i] == 0) ? x++ : y++;
		printf("%2d %2d\n", x, y);
	}
	printf("\n\n");
}



void main()
{
	int i = 0, test = 0;
	Individual best;

	randomize();
	worldInit();
	populationInit();

	best = getBestIndividual();

	for(i = 0; i < maxGenerations; i++){

		if(test){
			printf("#generation: %4d, fitness: %4d\n", i, best.fitness);
		}else{
			printf("#generation: %4d, fitness: %4d\n", i, best.fitness);
			printAxisOfBest(best);
		}


		selection();

		crossover();
		mutation();
		setFitnessAndPenalize();

		best = getBestIndividual();
	}


	printf("#generation: %4d, fitness: %4d\n", i, best.fitness);
	if(!test)
		printAxisOfBest(best);
}
