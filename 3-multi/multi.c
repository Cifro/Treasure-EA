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
#define populationSize 100
// max generations
#define maxGenerations 100

// size of world
#define xAxis 30
#define yAxis 25


#define DP 1;
#define DE 1;

#define genotypeSize yAxis + xAxis - 2

typedef struct{
	int fitness;
	int genotype[genotypeSize];	// binary representation of path, 0 = move right, 1 = move up
}Individual;

typedef struct{
	int treasure;
	int energy;
}Cell;

Individual population[populationSize];
Individual parents[populationSize];

// world is indexed from 1
Cell world[yAxis + 1][xAxis + 1];


void worldInit();
void populationInit();
void fitness();
void sort(Individual *individuals);



void worldInit()
{
	int y, x;

	for(y = 1; y < yAxis + 1; y++){
		for(x = 1; x < xAxis + 1; x++){
			if(x == 12) world[y][x].treasure = xAxis; else world[y][x].treasure = xAxis - abs(x - 12) * DP;
			if(x == 19) world[y][x].energy = 0;       else world[y][x].energy = 0 + abs(x - 19) * DE;
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



void fitness()
{
	int i, j, maxFitness = 0, x, y;

	for(i = 0; i < populationSize; i++){
		x = 1;
		y = 1;

		population[i].fitness = 0;
		population[i].fitness += (world[1][1].treasure - world[1][1].energy);

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

			population[i].fitness += (world[y][x].treasure - world[y][x].energy);
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



void main()
{
	int i, test = 0;
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
		fitness();

		best = getBestIndividual();
	}


	printf("#generation: %4d, fitness: %4d\n", i, best.fitness);
	if(!test)
		printAxisOfBest(best);
}
