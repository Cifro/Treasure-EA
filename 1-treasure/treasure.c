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
#include <string.h>

#define random(num) (rand()%(num))
#define randomize() srand((unsigned)time(NULL))

// max individuals
#define populationSize 100
// max generations
#define maxGenerations 100

// size of world
#define xAxis 30
#define yAxis 25

#define genotypeSize yAxis + xAxis - 2

// y = 1050
// x+y = 1539
// max(y,25-x) = 1326
#define targetFitness 1326


typedef struct{
	int fitness;
	int genotype[genotypeSize];	// binary representation of path, 0 = move right, 1 = move up
}Individual;


Individual population[populationSize];
Individual parents[populationSize];

// world is indexed from 1
int world[yAxis + 1][xAxis + 1];


void worldInit();
void populationInit();
void fitness();
void sort(Individual *individuals);


int maximum(int y, int x)
{
	return (y > 25 - x) ? y : 25 - x;
}



void worldInit(int type)
{
	int y, x;

	for(y = 1; y < yAxis + 1; y++){
		for(x = 1; x < xAxis + 1; x++){
			if(type == 1)
				world[y][x] = maximum(y, x); // function = max(y,25-x), x+y, y

			if(type == 2)
				world[y][x] = x + y;

			if(type == 3)
				world[y][x] = y;
		}
	}
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



void populationInit()
{
	int i, j, direction, x, y;

	for(i = 0; i < populationSize; i++){
		y = 1;
		x = 1;

		for(j = 0; j < genotypeSize; j++){
			direction = random(2);

			// 0 = move right, 1 = move up
			(direction == 0) ? x++ : y++;

			population[i].genotype[j]= direction;
		}
	}
	fitness();
}



void fitness()
{
	int i, j, x, y;

	for(i = 0; i < populationSize; i++){
		x = 1;
		y = 1;

		population[i].fitness = 0;
		population[i].fitness += world[1][1];

		for(j = 0; j < genotypeSize; j++){

			// fix genotype
			(population[i].genotype[j] == 0) ? x++ : y++;

			if(x > xAxis){
				x--; y++;
				population[i].genotype[j] = 1;
			}

			if(y > yAxis){
				y--; x++;
				population[i].genotype[j] = 0;
			}

			population[i].fitness += world[y][x];
		}
	}
}



// Crop, sort, random selection from 0.2 of the best individuals
void selection()
{
	int i, r, threshold;

	threshold =	(int) (0.2 * populationSize);
	sort(population);

	for(i = 0; i < populationSize; i++){
		r = random(threshold);
		parents[i] = population[r];
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



// get individual from population in generation
Individual getBestIndividual()
{
	int i = 0;
	Individual best;

	best = population[0];

	for(i = 0; i < populationSize; i++){
		if(best.fitness < population[i].fitness){
			best = population[i];
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



void main(int argc, char *argv[], char **envp)
{
	int i, test = 0;
	int functionType = 0;
	char* option;
	Individual best;

	if(argc > 1){
		option = "-t";
		if(strcmp(argv[1], option) == 0){
			functionType = atoi(argv[2]);
			if(functionType <= 0 || functionType > 3) {
				printf("Option -t must have value: 1, 2 or 3");
				exit(0);
			}
		}
	}else{
		functionType = 1;
	}


	randomize();

	worldInit(functionType);

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
		fitness();

		best = getBestIndividual();
	}


	printf("#generation: %4d, fitness: %4d\n", i, best.fitness);
	if(!test)
		printAxisOfBest(best);
}
