/*$$FILE$$*/
/*$$VERSION$$*/
/*$$DATE$$*/
/*$$LICENSE$$*/
/*
 *        Process command line arguments for the example programs ... 
 *        This module is shared by all of the demo programs.
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "htmlUtil.h"

#include "DicFile.h"

#include "CifFileUtil.h"

using std::cout;
using std::endl;

void usage(FILE * fp, char * name);


void usage(FILE * fp, char * name)
{
  fprintf(fp,"This example program reads dictionary:\n\n");
  fprintf(fp,"Usage: %s \n",name);
  fprintf(fp,"          -i <filename> (input mmCIF data file)\n");
  fprintf(fp,"          -dict <dictionary path> (input mmCIF dictionary file)\n");

}

int demoGetArgs(int argc, char *argv[], DicFile ** dic, string & htmlPath)
{
  int i, ninfiles = 0;
  char **infiles = NULL;
  char *pname;
  string ddl;
  string dictionary;
  string hPath;
  
#if 0

  _main();

#endif

  pname = argv[0];

  if (argc < 3) {
    usage(stderr,pname);
    exit(1);
  }

  for (i=1; i < argc; i++) {
    if ( argv[i][0] == '-' ) {

      if (strcmp(argv[i],"-i") == 0 ) {
	i++;
	ninfiles ++;
	infiles = (char **) realloc(infiles, ninfiles* sizeof(char *));
	infiles[ninfiles-1] = (char *) calloc(strlen(argv[i])+1, sizeof(char));
	strcpy(infiles[ninfiles-1],argv[i]);
      }

      if (strcmp(argv[i],"-dictSdbFile") == 0 ) {
	i++;
	dictionary = argv[i];  
      }

      if (strcmp(argv[i],"-htmlpath") == 0 ) {
	i++;
	hPath = argv[i];  
      }

    } else {
      usage(stderr,pname);
      exit(1);
    }
  }

/*
 *            Handle a few error conditions ... 
 */ 

  if (dictionary.empty())
  {
    fprintf(stderr,"No dictionary file specified\n");
    usage(stderr,pname);
    exit(1);
  }

  cout<<dictionary<<endl;

  DicFile* dic2 = GetDictFile(NULL, string(), dictionary);

  *dic = dic2;

  htmlPath=hPath;

  return(0);
}


