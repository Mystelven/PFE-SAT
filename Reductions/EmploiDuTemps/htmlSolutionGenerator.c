#include "htmlSolutionGenerator.h"

#include <string.h>
#include <stdlib.h>

const char* headerHTML = "<style type=\"text/css\">body { margin:5px; }.tg  {border-collapse:collapse;border-spacing:0;border-color:#ccc; margin:auto;} .tg td{ width: 140px; font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#ccc;color:#333;background-color:#fff;} .tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#ccc;color:#333;background-color:#f0f0f0;} .tg .tg-59zk{font-weight:bold;font-family:\"Lucida Console\", Monaco, monospace !important;;background-color:#efefef;text-align:center} .tg .tg-xahr{font-family:\"Lucida Console\", Monaco, monospace !important;;background-color:#efefef;text-align:center} .tg .tg-slkj{font-family:\"Lucida Console\", Monaco, monospace !important;;text-align:center} .tg .tg-kkwu{font-weight:bold;font-family:\"Lucida Console\", Monaco, monospace !important;;text-align:center} </style> <table class=\"tg\"><tr> <th class=\"tg-kkwu\">-----------</th> <th class=\"tg-kkwu\">Monday</th> <th class=\"tg-kkwu\">Tuesday</th> <th class=\"tg-kkwu\">Wednesday</th> <th class=\"tg-kkwu\">Thursday</th> <th class=\"tg-kkwu\">Friday</th> </tr>";

const char* footerHTML = "</table>";

	int	correspondances[5][6] = {
								{8,10,12,14,16,18} , 
								{32,34,36,38,40,42}, 
								{56,58,60,62,64,66}, 
								{80,82,84,86,88,90}, 
								{104,106,108,110,112,114} 
								};

char* getDisplaySubjectAndRoom(Planning* planning, unsigned int* solution, int ligne, int colonne) {

	int 		   i = 0;
	unsigned long  j = 0;
	unsigned long  k = 0;

	char  tmp[512];
	char* result = (char*)malloc(sizeof(char)*2048);

	while(solution[i] != 0) {

		for(j = 0; j < planning->nbSubjects ; j++) {

			Subject* subject = planning->array_subjects[j];				

			for(k = 0; k < subject->nbSlots; ++k) {
				
				if(subject->slots[k]->id == solution[i]) {	
					
					Interval* interval = subject->slots[k];

					double start = interval->start;
					int intpart = (int)start;
					start  = (double)(start - intpart);
					start *= 1000;
					
					double salle = start;
					int intpart2 = (int)salle;
					salle = salle - intpart2;
					salle *= 100;
					

					start  = (int)start;
			
					if( ((int)(start - correspondances[colonne][ligne])) != 0) {
						
						continue;

					} else {
						if(strstr(subject->subjectName,"_CM") == NULL) {
							char src[2000];
							char grp[256];
							strcpy(src,subject->subjectName);
							grp[0] = 'g';
							grp[1] = 'r';
							grp[2] = 'p';
							grp[3] = ' ';
							sprintf(&grp[4],"%d",intpart);
							grp[5] = '\0';
							strcat(src,grp);
							sprintf(tmp,"Teacher : %5d <br /><br /><b>%10s</b> <br /><br />Room &nbsp;: %5d",intpart,src,intpart);						

						} else {

							sprintf(tmp,"Teacher : %5d <br /><br /><b>%10s</b> <br /><br />Room &nbsp;: %5d",intpart,subject->subjectName,intpart);						
						}

						result = strcat(result,tmp);
					}

					result = strcat(result,"<br /><br />------------<br /><br />");
				}
			}
		}	

		i++;
	}

	return result;
}

char* createHTMLplan(Planning* planning, unsigned int* solution,double generationTime, char* input) {

	char* filename = (char*)strdup("output.html");
	FILE* file 	   = fopen(filename,"w+");

	int result = isSolutionExisting(planning,solution);

	fprintf(file,"%s",headerHTML);

	for(int i = 0; i < 5 ; i++) {

		if(i == 2) {
			fprintf(file,"%s","<tr><td class=\"tg-xahr\" colspan=\"6\">");
			fprintf(file,"%s","------------------------ MIAM ------------------------");
			fprintf(file,"%s"," </td></tr>");
			continue;
		}

		fprintf(file,"%s","<tr>");
    	fprintf(file,"%s","<td class=\"tg-59zk\">");

    	switch(i) {

    		case 0:
    			fprintf(file,"%s","08:15 - 10:15");
    			break;
    		case 1:
    			fprintf(file,"%s","10:15 - 12:30");
    			break;

    		case 3:
    			fprintf(file,"%s","14:00 - 16:00");
    			break;
    		case 4:
    			fprintf(file,"%s","16:15 - 18:15");
    			break;
    	}
    	fprintf(file,"%s","</td>");

    	// ----------------------------------------

    	fprintf(file,"%s","<td class=\"tg-slkj\">");

    		// 1re colonne
    		fprintf(file,"%s",getDisplaySubjectAndRoom(planning,solution,i,0));

    	fprintf(file,"%s","</td>");

    	// ----------------------------------------

    	fprintf(file,"%s","<td class=\"tg-slkj\">");
    		
    		// 2e colonne
    		fprintf(file,"%s",getDisplaySubjectAndRoom(planning,solution,i,1));

    	fprintf(file,"%s","</td>");

    	// ----------------------------------------

    	fprintf(file,"%s","<td class=\"tg-slkj\">");

    		// 3e colonne
    		fprintf(file,"%s",getDisplaySubjectAndRoom(planning,solution,i,2));

    	fprintf(file,"%s","</td>");

    	// ----------------------------------------

    	fprintf(file,"%s","<td class=\"tg-slkj\">");
    		// 4e colonne
    		fprintf(file,"%s",getDisplaySubjectAndRoom(planning,solution,i,3));

    	fprintf(file,"%s","</td>");

    	// ----------------------------------------

    	fprintf(file,"%s","<td class=\"tg-slkj\">");

    		// 5e colonne
    		fprintf(file,"%s",getDisplaySubjectAndRoom(planning,solution,i,4));

    	fprintf(file,"%s","</td>");

		// ----------------------------------------

    	fprintf(file,"%s","</tr>");
  	}

  	fprintf(file,"%s","<div style=\"float:left;\" ><ul>");

  	char buf[1024];
  	FILE* fileInput = fopen(input,"r+");
  	while(fgets(buf,sizeof(buf),fileInput)) {
  		if(buf[0] == 'c')
  			fprintf(file,"<li>%s</li>",buf+1);
  	}
  	fclose(fileInput);

  	fprintf(file,"%s","</ul></div>");


  	fprintf(file,"<h1 style=\"text-align: center;\" > Generation of the Class Scheduling by <a href=\"mailto:valentin.montmirail@gmail.com\">Valentin Montmirail</a></h1>");
  	fprintf(file,"<h2 style=\"text-align: center;\" > Creating file + Solving + Generation HTML time : %8.0f ms</h2>",generationTime);

	if(result == 0) fprintf(file,"<h3 style=\"text-align: center; color:red\"> This problem has been proved without any solution.</h3>");
	
	fprintf(file,"%s",footerHTML);

	fclose(file);

	return filename;
}

