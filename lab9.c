#include <stdio.h>
#include <stdlib.h>

#define tableSize 11

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType *data;
	struct HashType *next;
};

struct HashType *chain[tableSize];

// Compute the hash function
int hash(int id, char name, int order)
{
	struct HashType *newHashType = malloc(sizeof(struct HashType));
	newHashType->data = malloc(sizeof(struct RecordType));
	
	newHashType->data->id = id;
	newHashType->data->name = name;
	newHashType->data->order = order;
	newHashType->next = NULL;

	int key = id % tableSize;

	if (chain[key] == NULL)
		chain[key] = newHashType;
	else
	{
		struct HashType *temp = chain[key];
		while (temp->next)
			temp = temp->next;

		temp->next = newHashType;
	}
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash()
{
	int i;

	for (i=0;i<tableSize;++i)
	{
		// if index is occupied with any records, print all
		struct HashType *temp = chain[i];
		if (temp != NULL)
		{
			printf("Index[%d] --> ", i);
			while (temp)
			{
				printf("%d, %c, %d -> ", temp->data->id, temp->data->name, temp->data->order);
				temp = temp->next;
			}
			printf("NULL\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	int i;
	for (i = 0; i < tableSize; i++)
		chain[i] = NULL;

	for (i = 0; i < recordSz; i++)
		hash(pRecords[i].id, pRecords[i].name, pRecords[i].order);

	displayRecordsInHash();
}