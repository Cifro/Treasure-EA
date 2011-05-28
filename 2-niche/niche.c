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
#define maxGenerations 300

// size of world
#define xAxis 30
#define yAxis 25

#define genotypeSize yAxis + xAxis - 2

// number of good solutions
#define solutions 6

#define maxFitness 540.0

typedef struct{
	double fitness;
	int genotype[genotypeSize];	// binary representation of path, 0 = move right, 1 = move up
}Individual;

Individual population[populationSize];
Individual parents[populationSize];
Individual theBestOf[solutions]; // array of best (and unique) individuals (paths)

// world is indexed from 1
double world[yAxis + 1][xAxis + 1];

int uniques = 0;


void worldInit();
void populationInit();
void fitness();
void sort(Individual *individuals);


void worldInit()
{
	int x, y;

	for(y = 1; y <= yAxis; y++){
		for(x = 1; x <= xAxis; x++){
			if((y == yAxis || y == 10 || y == 1) || (x == 1 || x == 10 || x == xAxis)){
				world[y][x] = 10.0;
			}else{
				// +-----+-----+
				// |  3  |  4  |
				// +-----+-----+
				// |  1  |  2  |
				// +-----+-----+

				// Damn ASCII art... use recursion

				// 1st sector                                4th sector
				if(((x > 1 && x < 10)&& (y > 1 && y < 10))|| ((x > 10 && x < xAxis)     && (y > 10 && y < yAxis)))     world[y][x] = 9.0;
				if(((x > 2 && x < 9) && (y > 2 && y < 9)) || ((x > 11 && x < xAxis - 1) && (y > 11 && y < yAxis - 1))) world[y][x] = 8.0;
				if(((x > 3 && x < 8) && (y > 3 && y < 8)) || ((x > 12 && x < xAxis - 2) && (y > 12 && y < yAxis - 2))) world[y][x] = 7.0;
				if(((x > 4 && x < 7) && (y > 4 && y < 7)) || ((x > 13 && x < xAxis - 3) && (y > 13 && y < yAxis - 3))) world[y][x] = 6.0;
														   if((x > 14 && x < xAxis - 4) && (y > 14 && y < yAxis - 4))  world[y][x] = 5.0;
														   if((x > 15 && x < xAxis - 5) && (y > 15 && y < yAxis - 5))  world[y][x] = 4.0;
														   if((x > 16 && x < xAxis - 6) && (y > 16 && y < yAxis - 6))  world[y][x] = 3.0;

				// 3rd sector                                         2nd sector
				if(((x > 1 && x < 10)&& (y > 10 && y < yAxis))     || ((x > 10 && x < xAxis)     && (y > 1 && y < 10)))world[y][x] = 9.9;
				if(((x > 2 && x < 9) && (y > 11 && y < yAxis - 1)) || ((x > 11 && x < xAxis - 1) && (y > 2 && y < 9))) world[y][x] = 9.8;
				if(((x > 3 && x < 8) && (y > 12 && y < yAxis - 2)) || ((x > 12 && x < xAxis - 2) && (y > 3 && y < 8))) world[y][x] = 9.7;
				if(((x > 4 && x < 7) && (y > 13 && y < yAxis - 3)) || ((x > 13 && x < xAxis - 3) && (y > 4 && y < 7))) world[y][x] = 9.6;
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
	int i, j, x, y;

	for(i = 0; i < populationSize; i++){
		x = 1;
		y = 1;

		population[i].fitness = 0.0;
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



// checking if we have different individual
int unique(Individual currentBest)
{
	int i, j, u = 0;
	int equality[solutions];

	if(theBestOf[0].fitness <= 0.0){ // empty array, first best is the best
		return 1; // unique
	}else{
		for(i = 0; i < uniques; i++){
			equality[i] = 0;
			for(j = 0; j < genotypeSize; j++){
				if(theBestOf[i].genotype[j] == currentBest.genotype[j]){
					equality[i]++;
				}
			}
		}

		for(i = 0; i < uniques; i++){
			if(equality[i] != genotypeSize){
				u++;
			}
		}

		if(u == uniques)
			 return 1; // unique
		else
			return 0; // duplicate


	}
}



void printAxisOfBest()
{
	int i, j, x = 1, y = 1;

	for(i = 0; i < uniques; i++){
		printf("# individual: %d\n", i);
		printf("%2d %2d\n", 1, 1);
		for(j = 0; j < genotypeSize; j++){
			(theBestOf[i].genotype[j] == 0) ? x++ : y++;
			printf("%2d %2d\n", x, y);
		}
		x = 1;
		y = 1;
		printf("\n\n");
	}

	// just for better UX
	// last individual once more
	x = 1;
	y = 1;
	printf("# ##### Don't close gnuplot! #####\n");
	printf("%2d %2d\n", 1, 1);
	for(j = 0; j < genotypeSize; j++){
		(theBestOf[i-1].genotype[j] == 0) ? x++ : y++;
		printf("%2d %2d\n", x, y);
	}
	printf("\n\n");

}



void main()
{
	int i;
	int isUnique;
	Individual best;

	randomize();

	worldInit();

	// max 4 unique individuals, 5 and more last long to find
	while(uniques < 4){
		populationInit();
		fitness();

		for(i = 0; i < maxGenerations; i++){

			selection();
			crossover();
			mutation();
			fitness();

			best = getBestIndividual();

			if(best.fitness == maxFitness){
				isUnique = unique(best);
				if(isUnique){
					theBestOf[uniques] = best;
					uniques++;
				}
				break;
			}
		}
	}

	printAxisOfBest();
}