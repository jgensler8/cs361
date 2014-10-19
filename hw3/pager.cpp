#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <sys/mman.h>

#define FRAMETABLESIZE 256
#define PAGETABLESIZE 256
#define NPROCESSES 256

// can be run with both
#define RUN_UNLIMITED 1
#define RUN_FIFO 1

//use to print human readable output
//#define OUTPUT_HUMAN
//use to generate .csv data
//#define OUTPUT_CALC
//user to generate data for graphs
#define OUTPUT_GRAPHS

using namespace std;

typedef struct {
	uint8_t pid;
	uint8_t page;
} MemoryAccess;

typedef struct {
	int frame;
	int pageHits;
	int pageMisses;
	unsigned long int timesSwapped;
	//int timePeriodSwapped? //relative to other pages
} PageTableEntry;

typedef struct {
	uint8_t pid;
	uint8_t page;
	bool vacant;
} FrameTableEntry;

PageTableEntry pageTables[PAGETABLESIZE][NPROCESSES];



void initPageTable(PageTableEntry table[PAGETABLESIZE][NPROCESSES])
{
  for(int i = 0; i < PAGETABLESIZE; ++i)
  {
    for(int j = 0; j < NPROCESSES; ++j)
    {
      table[i][j].frame = -1;
      table[i][j].pageHits = 0;
      table[i][j].pageMisses = 0;
      table[i][j].timesSwapped = 0;
    }
  }
}

void initFrameTable(FrameTableEntry* frameTable, int tableSize)
{
  for(int i = 0; i < tableSize; ++i)
  {
    frameTable[i].pid = -1;
    frameTable[i].page = -1;
    frameTable[i].vacant = true;
  }
}

bool parseArgs(int argc, char** argv, char** filename, unsigned long int *numberReferences, unsigned long int *frameTableSize)
{
	//we need 1 args + the file name
	if(argc < 2) return false;

	//get the file name argv[1]
	*filename = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
	strcpy(*filename, argv[1]);
	FILE *file;
	if( NULL == ( file = fopen(*filename, "r")))
	{
		cout << "File doesn't exist" << endl << flush;
		free(*filename);
		return false;
	}
	else
	{
		fclose(file);
	}
	
	//get the number of references, if applicable argv[2]
	if(argc >= 3)
	{
	  *numberReferences = atoi(argv[2]);
	  #ifdef OUTPUT_HUMAN
	  cout << "ARG: NumberReferences: " << *numberReferences << endl;
	  #endif
	}
	//get the frame table size, if applicable argv[3]
	if(argc >= 4)
	{
	  *frameTableSize = atoi(argv[3]);
	  #ifdef OUTPUT_HUMAN
	  cout << "ARG: FrameTableSize:   " << *frameTableSize << endl << flush;
	  #endif
	}

	return true;
}

void printStats(double pageHits, double pageMisses, char* filename, unsigned long int frameTableSize)
{
  double total = pageHits + pageMisses;
  #ifdef OUTPUT_HUMAN
  cout  << fixed
        << "PAGE HITS:   " << pageHits << "\t\t" << pageHits / total << endl
        << "PAGE MISSES: " << pageMisses << "\t\t" << pageMisses / total << endl << flush;
  #endif
  
  #ifdef OUTPUT_GRAPHS
  cout  << fixed
        << filename << ","
        << frameTableSize << ","
        << pageHits / total << ","
        << pageMisses / total <<  ",";
  #endif
}

void printPageTableAdvanced()
{
  unsigned long int highestTimesSwapped, lowestTimesSwapped;
  
  double totalTimesSwapped = 0, nzrTimesSwapped = 0, nzrCount = 0;
  
  for(int pid = 0; pid < PAGETABLESIZE; ++pid)
  {
    for(int page = 0; page < NPROCESSES; ++page)
    {
      if(pageTables[pid][page].timesSwapped < lowestTimesSwapped) lowestTimesSwapped = pageTables[pid][page].timesSwapped;
      
      if(pageTables[pid][page].timesSwapped > highestTimesSwapped) highestTimesSwapped = pageTables[pid][page].timesSwapped;
      
      totalTimesSwapped += pageTables[pid][page].timesSwapped;
      if(pageTables[pid][page].timesSwapped > 256)
      {
        nzrTimesSwapped += pageTables[pid][page].timesSwapped;
        nzrCount += 1;
      }
    }
  }
  
  #ifdef OUTPUT_HUMAN
  cout  << fixed
        << "HIGHEST TIMES SWAPPED: " << highestTimesSwapped << endl
        << "LOWEST  TIMES SWAPPED: " << lowestTimesSwapped << endl
        << "AVG ALL TIMES SWAPPED: " << totalTimesSwapped / (PAGETABLESIZE * NPROCESSES) << endl
        << "AVG >256 TIMES SWAPPED: " << nzrTimesSwapped / nzrCount << endl << flush;
  #endif
  #ifdef OUTPUT_GRAPHS
  cout << totalTimesSwapped / (PAGETABLESIZE * NPROCESSES) << ",";
  #endif
}

void loadPage(FrameTableEntry* frameTable, int tableSize, int *top, int pid, int page)
{
  if(!frameTable[*top].vacant)
  {
   //go out and adjust the thing we are about to overwrite
   pageTables[ frameTable[*top].pid ][ frameTable[*top].page ].frame = -1;
   pageTables[ frameTable[*top].pid ][ frameTable[*top].page ].timesSwapped += 1;
  }
  
  //load the new page in
  frameTable[*top].pid = pid;
  frameTable[*top].page = page;
  frameTable[*top].vacant = false;
  //update pageTable
  pageTables[pid][page].frame = *top;
  //increase top, check if too high
  ++(*top);
  if(*top == tableSize) *top = 0;
}

void simulate_unlimited(MemoryAccess* memblock, int numBlocks, FrameTableEntry* frameTable, unsigned long int frameTableSize, unsigned long int numberReferences, char* filename)
{
 double pageHits = 0, pageMisses = 0;
 for( int i = 0; i < numberReferences; ++i)
 {
   //see if the struct is in the page table
   if(pageTables[ memblock[i].pid ][ memblock[i].page ].frame < 0 )
   {
     //miss
     ++pageMisses;
     //load the page, assign the page to the frame table if need be
     pageTables[ memblock[i].pid ][ memblock[i].page ].frame = 1;
     pageTables[ memblock[i].pid ][ memblock[i].page ].pageMisses++;
   }
   else
   {
     //hit
     ++pageHits;
     //page is in the frame table, nothing should be loaded
     pageTables[ memblock[i].pid ][ memblock[i].page ].pageHits++;
   }
  }
  printStats(pageHits, pageMisses, filename, frameTableSize);
}

void simulate_FIFO(MemoryAccess* memblock, int numBlocks, FrameTableEntry* frameTable, unsigned long int frameTableSize, unsigned long int numberReferences, char* filename)
{
  double pageHits = 0, pageMisses = 0;
  int topStack = 0;
  for( int i = 0; i < numberReferences; ++i)
  {
    //see if the struct is in the page table
    if(pageTables[ memblock[i].pid ][ memblock[i].page ].frame < 0 )
    {
      //miss
      ++pageMisses;
      //load the page, assign the page to the frame table if need be
      loadPage(frameTable, frameTableSize, &topStack, memblock[i].pid, memblock[i].page);
    }
    else
    {
      //hit
      ++pageHits;
      //page is in the frame table, nothing should be loaded
      pageTables[ memblock[i].pid ][ memblock[i].page ].pageHits++;
    }
   }
   printStats(pageHits, pageMisses, filename, frameTableSize);
}

#ifdef OUTPUT_CALC
void generateCSV(MemoryAccess* memblock, int numBlocks, FrameTableEntry* frameTable, unsigned long int frameTableSize, unsigned long int numberReferences, char* filename)
{
  cout << "PID,PAGE,HITS,MISSES,TIMESSWAPPED" << endl;
  for(int pid = 0; pid < PAGETABLESIZE; ++pid)
  {
    for(int page = 0; page < NPROCESSES; ++page)
    {
      cout << pid << "," << page << "," << pageTables[pid][page].pageHits << "," << pageTables[pid][page].pageMisses << "," << pageTables[pid][page].timesSwapped << endl;
    }
    cout << flush;
  }
}
#endif

int main(int argc, char** argv)
{
	char* filename = NULL;
	unsigned long int numberReferences = -1, frameTableSize = FRAMETABLESIZE;

  #ifdef OUTPUT_HUMAN
	cout << "Jeff Gensler, jgensl2@uic.edu, jgensler -- CS account name" << endl << flush;
  #endif
  
	//store informations from the command line
	if(!parseArgs(argc, argv, &filename, &numberReferences, &frameTableSize))
	{
	  #ifdef OUTPUT_HUMAN
		cout << "Invalid command line args" << endl << flush;
		#endif
		return -1;
	}
	
	//open the file
	FILE* file = fopen(filename, "r");
	//use fstat
	struct stat statistics;
	fstat(fileno(file), &statistics);
	unsigned long int filesize = statistics.st_size;
	//mmap the file to memory
	MemoryAccess* memblock = (MemoryAccess*)mmap(0, filesize, PROT_READ, MAP_PRIVATE, fileno(file), 0);
	int numBlocks = filesize / sizeof(MemoryAccess);
	if(numberReferences < 0 || numberReferences > numBlocks) numberReferences = numBlocks;
	//create the frame table
	FrameTableEntry* frametable = (FrameTableEntry*)malloc(sizeof(FrameTableEntry)*frameTableSize);
	fclose(file);
	
	//now to record the page hits and misses
	#ifdef OUTPUT_HUMAN
  cout << "===== " << filename << " =====" << endl << flush;
	#endif
	
	#ifdef RUN_FIFO
	#ifdef OUTPUT_HUMAN
	cout << "== SIMULATING FIFO ==" << endl << flush;
	#endif
  initPageTable(pageTables);
  initFrameTable(frametable, frameTableSize);
	simulate_FIFO(memblock, numBlocks, frametable, frameTableSize, numberReferences, filename);
	printPageTableAdvanced();
	#ifdef OUTPUT_GRAPHS
	cout << "FIFO" << endl << flush;
	#endif
	#ifdef OUTPUT_CALC
	generateCSV(memblock, numBlocks, frametable, frameTableSize, numberReferences, filename);
	#endif
	#endif
	#ifdef RUN_UNLIMITED
	#ifdef OUTPUT_HUMAN
	cout << "== SIMULATING UNLIMITED ==" << endl << flush;
	#endif
  initPageTable(pageTables);
  initFrameTable(frametable, frameTableSize);
	simulate_unlimited(memblock, numBlocks, frametable, frameTableSize, numberReferences, filename);
	#ifdef OUTPUT_GRAPHS
	cout << "0,UNLIMITED" << endl << flush;
	#endif
	#ifdef OUTPUT_CALC
	generateCSV(memblock, numBlocks, frametable, frameTableSize, numberReferences, filename);
	#endif
	#endif
	
	//free the memory allocated by mmap
	munmap(memblock, filesize);
	free(filename);
	return 0;
}
