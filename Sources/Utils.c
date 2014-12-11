void exchange(Genetic_Solver* solver,Problem * problem, unsigned int a, unsigned int b) {

	int temp;

	for(unsigned int i = 0; i < problem->nbVariables; ++i) {
		
		temp 									   = solver->population[a]->arrayOfSolutions[i];
		solver->population[a]->arrayOfSolutions[i] = solver->population[b]->arrayOfSolutions[i];
		solver->population[b]->arrayOfSolutions[i] = temp;
	}

}

void sort(Genetic_Solver* solver,Problem* problem) {

	for(unsigned int i =0 ; i < problem->nbVariables -1; i++) {
		for(unsigned int j = 0 ; j < problem->nbVariables - i -1; j++) {
			if(solver->population[j]->fitness <= solver->population[j+1]->fitness) {
				exchange(solver,problem,j,j+1);
			}
		}
	}
}

/*
void quickSort(Genetic_Solver* solver,Problem* problem,unsigned int debut,unsigned int fin) {

	int gauche = debut-1;
	int droite = fin+1;

	const float pivot = solver->population[(debut+fin)/2]->fitness;

	if(debut >= fin)
		return ;

	while(1) {

		do droite--; while(solver->population[droite]->fitness > pivot);
		do gauche++; while(solver->population[gauche]->fitness < pivot);

		if(gauche < droite)
			exchange(solver,problem,gauche,droite);

		else break;
	}

	quickSort(solver,problem,debut,droite);

	quickSort(solver,problem,droite+1,fin);

}
*/
