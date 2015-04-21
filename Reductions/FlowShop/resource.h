#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct _resource {

	unsigned int  numShop;
	unsigned int  numJob;
	unsigned int  value;

} Resource;


void displayResource(Resource* resource);

#endif
