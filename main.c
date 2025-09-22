#include <stdio.h>
#include <stdlib.h>

//magic numbers
#define code_end 9
#define debug_mode 1

typedef enum {
	MOVE_RIGHT,
	MOVE_LEFT,
	INCREMENT,
	DECREMENT,
	PRINT,
	LOOP_BEGIN,
	LOOP_END,
	INPUT,
} TokenType;

long int fsize(FILE *f) {
	fseek(f, 0l, SEEK_END);
	long int size = ftell(f);
	rewind(f);

	return size;
}

TokenType* lexer(FILE *source_file) {
	size_t i = 0;
	long int file_length = fsize(source_file);
	char source_chars[file_length];
	
	char c;

	while( (c = fgetc(source_file)) != EOF) {
		if(c != '\n') {
			source_chars[i] = (char)c;
			i++;
		}

	}
	
	source_chars[i] = '\0';

	if(debug_mode) {
		printf("source code: %s", source_chars);
	}

	int code_length = i;

	TokenType* tokens = malloc(sizeof(TokenType) * code_length);
	for(int i = 0; i < code_length; i++) {
		if(source_chars[i] == '+') tokens[i] = INCREMENT;
		if(source_chars[i] == '-') tokens[i] = DECREMENT;

		if(source_chars[i] == '>') tokens[i] = MOVE_RIGHT;
		if(source_chars[i] == '<') tokens[i] = MOVE_LEFT;

		if(source_chars[i] == '.') tokens[i] = PRINT;
		if(source_chars[i] == ',') tokens[i] = INPUT;

		if(source_chars[i] == '[') tokens[i] = LOOP_BEGIN;
		if(source_chars[i] == ']') tokens[i] = LOOP_END;
	}

	if(debug_mode) {
		printf("\nenum tokens: ");
		for(int i = 0; i < code_length; i++) {
			printf("%d", tokens[i]);
		}
		printf("\n");
	}
	
	tokens[i] = code_end;

	return tokens;
}

void execute(TokenType *tokens) {
	char code[3000] = {0};
	size_t ptr = 0;

	int loop_map[1000] = {0};
	int loop_p = 0;
	
	for (int i = 0; tokens[i] != code_end; i++) {
		if (tokens[i] == LOOP_BEGIN) {
			loop_map[loop_p++] = i;
		} else if (tokens[i] == LOOP_END) {
			if (loop_p == 0) {
				printf("Unmatched ']' at %d\n", i);
				exit(-1);
			}
			int start = loop_map[--loop_p];
			loop_map[start] = i;
			loop_map[i] = start;
		}
	}

	if (loop_p != 0) {
		printf("Unmatched '[' at %d\n", loop_map[loop_p - 1]);
		exit(-1);
	}

	for (int i = 0; tokens[i] != code_end; i++) {
		switch (tokens[i]) {
			case MOVE_RIGHT:
				ptr++;
				break;
			case MOVE_LEFT:
				ptr--;
				break;
			case INCREMENT:
				code[ptr]++;
				break;
			case DECREMENT:
				code[ptr]--;
				break;
			case PRINT:
				if (code[ptr] >= 32 && code[ptr] <= 126) {
        				putchar(code[ptr]);
				} else {
        				printf("[%d]", code[ptr]);
    				}
				break;
			case INPUT:
				break;
			case LOOP_BEGIN:
				if (code[ptr] == 0) {
					i = loop_map[i];
				}
				break;
			case LOOP_END:
				if (code[ptr] != 0) {
					i = loop_map[i];
				}
				break;
		}
	}}

int main() {
	FILE *f_ptr = fopen("code.bf", "r");

	if(f_ptr == NULL) {
		printf("file not found");
		exit(-1);
	}

	TokenType *tokens = lexer(f_ptr);
	
	execute(tokens);

	free(tokens);
	fclose(f_ptr);
	return 0;
}
