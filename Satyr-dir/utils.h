#ifndef UTILS_H
#define UTILS_H
	
	int compare(const void* a, const void* b) {

		int int_a = *( (int*) a);
		int int_b = *( (int*) b);

		if( int_a == int_b ) return 0;

		if(int_a < int_b)    return -1;

		else 				 return 1;

	}

#endif

