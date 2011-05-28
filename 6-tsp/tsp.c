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

#define populationSize 100 // number of individuals in on population
#define maxGenerations 2000


#define maxX 30
#define maxY 25
#define maxCities 30

#define target 118.25

typedef struct{
	int x;
	int y;
}Point;


typedef struct{
	int path[maxCities];
	double fitness;
}Individual;


Point cities[maxCities];

Individual population[populationSize];
Individual parents[populationSize];
Individual newPopulation[populationSize];


void citiesInit();
void populationInit();
void fitness();
Individual getBestIndividual();
void selection();
void sort(Individual *individual);
void fixInvalidIndividuals();
void newGeneration();
void mutation();



void citiesInit()
{
     cities[0].x = 1;
     cities[0].y = 13;

     cities[1].x = 4;
     cities[1].y = 10;

	 cities[2].x = 1;
     cities[2].y = 7;

	 cities[3].x = 4;
     cities[3].y = 4;

	 cities[4].x = 7;
     cities[4].y = 1;

	 cities[5].x = 10;
     cities[5].y = 4;

	 cities[6].x = 13;
     cities[6].y = 1;

	 cities[7].x = 15;
     cities[7].y = 4;

	 cities[8].x = 16;
     cities[8].y = 4;

	 cities[9].x = 18;
     cities[9].y = 1;

     cities[10].x = 21;
     cities[10].y = 4;

	 cities[11].x = 24;
     cities[11].y = 1;

	 cities[12].x = 27;
     cities[12].y = 4;

	 cities[13].x = 30;
     cities[13].y = 7;

	 cities[14].x = 27;
     cities[14].y = 10;

	 cities[15].x = 30;
     cities[15].y = 13;

	 cities[16].x = 27;
     cities[16].y = 16;

	 cities[17].x = 30;
     cities[17].y = 19;

	 cities[18].x = 27;
     cities[18].y = 22;

	 cities[19].x = 24;
     cities[19].y = 25;

	 cities[20].x = 21;
     cities[20].y = 22;

	 cities[21].x = 18;
     cities[21].y = 25;

	 cities[22].x = 16;
     cities[22].y = 22;

	 cities[23].x = 15;
     cities[23].y = 22;

	 cities[24].x = 13;
     cities[24].y = 25;

	 cities[25].x = 10;
     cities[25].y = 22;

	 cities[26].x = 7;
     cities[26].y = 25;

	 cities[27].x = 4;
     cities[27].y = 22;

	 cities[28].x = 1;
     cities[28].y = 19;

	 cities[29].x = 4;
     cities[29].y = 16;
}



void sort(Individual *individuals)
{
	int i, j;
	Individual temp;

	// sort it please
	for(i = 0; i < populationSize - 1; i++){
		for(j = i + 1; j < populationSize; j++){
			if(individuals[i].fitness > individuals[j].fitness){
				temp = individuals[j];
				individuals[j] = individuals[i];
				individuals[i] = temp;
			}
		}
	}
}



void populationInit()
{
	int i, j, a = 0, b = 0, temp = 0;
	int maxRandom, max;

	citiesInit();

	maxRandom = maxCities - 2;
	max = random(maxCities - 15) + 10;

	//generate random paths for individuals
	for(i = 0; i < populationSize; i++){

		for(j = 0; j < maxCities + 1; j++){
			population[i].fitness = 0.0;
			population[i].path[j] = j;
		}

		for(j = 1; j < max; j++){

			a = random(maxRandom) + 1; // from 1 to 28, don't change 1st & last city
			b = random(maxRandom) + 1;

			temp = population[i].path[a];
			population[i].path[a] = population[i].path[b];
			population[i].path[b] = temp;
		}
	}

	fitness();
}



void fitness()
{
	int  i, j, x, y;

	// fitness for paths for individuals
	x = y = 0;

	for(i = 0; i < populationSize; i++){
		population[i].fitness = 0.0;

		for(j = 0; j < maxCities; j++){

			// last one is first one
			if(j + 1 == maxCities){
				x = cities[population[i].path[0]].x - cities[population[i].path[j]].x;
				y = cities[population[i].path[0]].y - cities[population[i].path[j]].y;
			}else{
				x = cities[population[i].path[j + 1]].x - cities[population[i].path[j]].x;
				y = cities[population[i].path[j + 1]].y - cities[population[i].path[j]].y;
			}

			population[i].fitness += sqrt(x*x + y*y); // Pythagoras is alive!
		}
	}
}



void selection()
{
	int i = 0;
	int r;
	int threshold = (int) 0.3 * populationSize;

	sort(population); // sort population
	while(i < populationSize){
		r = random(threshold);
		parents[i] = population[r];
		i++;
	}
}



void selectionTournament()
{
	int i = 0;
	int player1, player2;

	while(i < populationSize){
		player1 = random(populationSize);
		player2 = random(populationSize);

		if(population[player1].fitness < population[player2].fitness){
			parents[i] = population[player1];
		}else{
			parents[i] = population[player2];
		}

		i++;
	}
}



void crossover()
{
	int i, j;
	int a, b;
	int cut, genes, interval;

	for(i = 0; i < populationSize; i+=2){

		do{
			cut = random(maxCities - 2) + 1;
			genes = random(8) + 1;
		}while((cut + genes) > maxCities);

		a = random(populationSize);
		b = random(populationSize);

		newPopulation[i] = parents[a];
		newPopulation[i + 1] = parents[b];

		interval = cut + genes;

		for(j = cut; j < interval; j++){
			newPopulation[i].path[j] = parents[b].path[j];
			newPopulation[i + 1].path[j] = parents[a].path[j];
		}
	}

	fixInvalidIndividuals();
}



void fixInvalidIndividuals()
{
	int i, j, k;
	int count[maxCities];
	int duplicates[maxCities];
	int none[maxCities];

	for(i = 0; i < populationSize; i++){

		for (j = 0; j < maxCities; j++){
			count[j] = 0;
			duplicates[j] = 0;
			none[j] = 0;
		}

		for(j = 0; j < maxCities; j++){
			count[newPopulation[i].path[j]]++;
		}

		k = 0;
		for(j = 0; j < maxCities; j++){
			if(count[j] > 1){
				duplicates[k] = j;
				k++;
			}
		}

		k = 0;
		for(j = 0; j < maxCities; j++){
			if(count[j] == 0){
				none[k] = j;
				k++;
			}
		}

		k = 0;
		for(j = 1; j < maxCities; j++){
			if(newPopulation[i].path[j] == duplicates[k]){
				newPopulation[i].path[j] = none[k];
				k++;
			}
		}
	}
}



void mutation()
{
	// Teenage Mutant Ninja Turtles

	int i, j, a = 0, b = 0, temp = 0;
	int mutate = populationSize - (populationSize / 3); // last 3rd
	int maxMutations = random(maxCities - 2) + 1; // 1 - 28

	for(i = mutate; i < populationSize; i++){
		for(j = 1; j < maxMutations; j++){
			a = random(maxCities - 2) + 1; // from 1 to 28, don't change 1st & last city
			b = random(maxCities - 2) + 1;
			temp = population[i].path[a];
			population[i].path[a] = population[i].path[b];
			population[i].path[b] = temp;
		}
	}

	fitness();
}



void newGeneration()
{
	int i;

	sort(newPopulation);

	for(i = 0; i < populationSize; i++){
		population[i] = newPopulation[i];
	}
}



Individual getBestIndividual()
{
	int i;
	Individual best = population[0];

	for(i = 0; i < populationSize; i++){
		// smaller is better
		if(best.fitness > population[i].fitness){
			best = population[i];
		}
	}

	return best;
}



void main()
{
	int i = 0, j, test;
	Individual best;

	randomize();

	populationInit();

	test = 0;
	best = getBestIndividual();

	while(best.fitness > target){
		i++;

		if(test){
			printf("# %d. generacia: %3.2f\n", i, best.fitness);
		}else{
			printf("#generacia: %d, fitness: %f\n", i, best.fitness);
			for(j = 0; j < maxCities; j++){
				printf("%2d %2d\n", cities[best.path[j]].x, cities[best.path[j]].y);
			}
			printf("\n\n");
		}

		if(i >= maxGenerations) break;

		selectionTournament();
		crossover();
		newGeneration();
		mutation();
		best = getBestIndividual();
	}


	printf("# %d. generation, best fitness %3.2f\n#", i + 1, best.fitness);

	for(i = 0; i < maxCities; i++){
		printf("%2d -> ", best.path[i]);
	}
	printf("%d\n", best.path[0]);

	for(j = 0; j < maxCities; j++){
		printf("%2d %2d\n", cities[best.path[j]].x, cities[best.path[j]].y);
	}
}