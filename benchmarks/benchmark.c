#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gnuplot_i.h"

int main(int argc, char * argv[])
{
	char* output = "default.png";
	char cmd[255];

	if(argc > 1) {

		output = strcat(argv[1],".png");

	} else {

		fprintf(stderr,"Vous n'avez rentrez aucun nom, le nom default.png va être utilisé\n");
		fprintf(stderr,"Si vous voulez changer le nom, executez comme ./benchmark [nomOutput]\n");
	}

	gnuplot_ctrl * g = gnuplot_init();

	strcpy(cmd,"set output \"");
	strcat(cmd,output);
	strcat(cmd,"\"");

	gnuplot_cmd(g, "set terminal png");
	gnuplot_cmd(g, cmd);
	gnuplot_plot_equation(g, "exp(x)", "Benchmark");
	gnuplot_close(g);

	return 0 ;
}

