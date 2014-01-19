#include "MapToWavefront.hpp"
#include "Main.hpp"

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;
using namespace glPortal::map;

int main(int argc, char **argv) {
  char *argument = NULL;
  int index;
  int option;
     
  opterr = 0;
     
  while ((option = getopt (argc, argv, "abcfh:")) != -1)
    switch (option){
    case 'f':
      {
	argument = optarg;
	//	MapToWavefront *mapToObj = new MapToWavefront("data/map/1.map");
	MapToWavefront *mapToObj = new MapToWavefront(argument);
        cout << "Argument f.";
	break;
      }

    case 'b':
      {
        cout << "Argument b.";
	break;
      }
      
    case 'c':
      {
	argument = optarg;
	break;
      }

    case 'h':
      {
	if (optopt == 'c')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (optopt == 'f')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (isprint (optopt))

	  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	return 1;
      }
      default:
	{
	abort ();
	}
      }
        
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;

  //  return EXIT_SUCCESS;
  return 1;
}
