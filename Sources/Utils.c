void sort(Genetic_Solver* solver) {
	
	Solver* temp;

	for(unsigned int i =0 ; i < solver->nbIndividual -1; ++i) {

		for(unsigned int j = 0 ; j < solver->nbIndividual - i -1; ++j) {

			if(solver->population[j]->fitness >= solver->population[j+1]->fitness) {
					temp 				  = solver->population[j];
					solver->population[j] = solver->population[j+1];
					solver->population[j+1] = temp;
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
