#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXC 12

typedef struct line {
	char *blocks[MAXC];
	int length[MAXC];
	struct line *next;
} line;

char *strstrip(char *string, char c){
	char *stripped = malloc(strlen(string)+1);
	int j = 0;
	for (int i = 0; i < strlen(string)+1; i++){
		if (string[i] != c){
			stripped[j] = string[i];
			j++;
		}
	}
	return stripped;
}

line *putinlines(FILE *f){
	line *newline = malloc(sizeof(line));
	char *buffer = malloc(256);
	char *strippedString;
	char *tok;
	size_t i = 0;
	
	if (fgets(buffer, 256, f) == NULL || buffer[0] == '\n'){
		free(newline);
		free(buffer);
		return NULL;
	}
		

	strippedString = strstrip(buffer, '\n');
	tok = strtok(strippedString, "|");
	do {
		newline->length[i] = strlen(tok);
		newline->blocks[i] = malloc(strlen(tok)+1);
		memcpy(newline->blocks[i], tok, strlen(tok)+1);
		i++;
		tok = strtok(NULL, "|");
	} while (tok != NULL);
	free(buffer);
	free(strippedString);

	newline->next = putinlines(f);
	return newline;
}

char *padstring(char *string, int length){
	char *paddedstring = calloc(0, length+1);
	int i;

	for (i = 0; i < strlen(string); i++){
		paddedstring[i] = string[i];
	}
	for (i = i; i < length; i++){
		paddedstring[i] = ' ';
	}

	paddedstring[i+1] = '\0';
	
	return paddedstring;
}

void freelines(line *head){
	for (int i = 0; i < MAXC; i++){
		if (head->blocks[i] != NULL) free(head->blocks[i]);
	}

	if (head->next != NULL) freelines(head->next);

	free(head);
}

int main(int argc, char **argv){
	FILE *f;
	if (argc < 2) f = stdin; 
	else if (argv[1][0] == '-') f = stdin;
	else f = fopen(argv[1], "r");

	line *head = putinlines(f);
	line *node = head;

	int longest[MAXC];

	do{ 
		for (int i = 0; i < MAXC; i++){
			if (node->length[i] > longest[i]) 
				longest[i] = node->length[i];
		}
		node = node->next;
	}while (node != NULL);

	for (int i = 0; i < MAXC; i++) longest[i]++;

	node = head;
	do{ 
		for (int i = 0; i < MAXC; i++){
			if (node->blocks[i] == NULL) break;
			char *padded = padstring(node->blocks[i], longest[i]);
			printf(" | %s", padded); 
			free(padded);
		}
		printf("|\n ");
		for (int i = 0; i < MAXC; i++){
			if (node->blocks[i] != NULL){
				printf("+");
				for (int j = 0; j < longest[i]+2; j++){ 
					if (node->blocks[i+1] == NULL && j == longest[i] + 1)
						printf("+");
					else 
						printf("-");
				}
			}
		}
		printf("\n");
		node = node->next;
	}while (node != NULL);


	

	freelines(head);
	fclose(f); 
	return 0;
}
