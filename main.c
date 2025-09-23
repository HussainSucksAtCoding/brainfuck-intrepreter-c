#include <stdio.h>
#include <stdlib.h>

//magic numbers
#define debug_mode 0

typedef enum {
	NO_INSTRUCTIONS,
	MOVE_RIGHT,
	MOVE_LEFT,
	INCREMENT,
	DECREMENT,
	PRINT,
	LOOP_BEGIN,
	LOOP_END,
	INPUT,
	CODE_END,
} TokenType;

int code_length(FILE *source_file) {
	int code_len = 0;
	
	char c;
	while( ( c = fgetc(source_file) ) != EOF) {
		code_len++;
	}
	rewind(source_file);
	return code_len;
}

TokenType* lexer(FILE *source_file) {
	int code_len = code_length(source_file);

	char c;
	char char_array[code_len];
	for(int i = 0; i < code_len; i++) {
		char_array[i] = (c = fgetc(source_file));	
	}
	char_array[code_len] = '\0';

	TokenType* tokens = malloc(sizeof(TokenType) * (code_len + 1));
	for(int i = 0; i < code_len; i++) {
		if(char_array[i] == '+') tokens[i] = INCREMENT;
		if(char_array[i] == '-') tokens[i] = DECREMENT;

		if(char_array[i] == '>') tokens[i] = MOVE_RIGHT;
		if(char_array[i] == '<') tokens[i] = MOVE_LEFT;

		if(char_array[i] == '.') tokens[i] = PRINT;
		if(char_array[i] == ',') tokens[i] = INPUT;

		if(char_array[i] == '[') tokens[i] = LOOP_BEGIN;
		if(char_array[i] == ']') tokens[i] = LOOP_END;
	}

	if(debug_mode) {
		printf("source code: %s", char_array);
	
		printf("enum tokens: ");
		for(int i = 0; i < code_len; i++) {
			printf("%d", tokens[i]);
		}
		printf("\n");
		
		printf("code tape length: %d\n", code_len);
	}
	
	tokens[code_len] = CODE_END;

	return tokens;
}

void execute(TokenType *tokens) {
	char code[3000] = {0};
	size_t ptr = 0;

	int loop_map[1000] = {0};
	int loop_p = 0;
	
	for (int i = 0; tokens[i] != CODE_END; i++) {
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

	for (int i = 0; tokens[i] != CODE_END; i++) {
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
				printf("too lazy to implement inputs for now");
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
			default:
				break;
		}
	}
}

int main(int argc, char *argv[]) {
	FILE *f_ptr = fopen(argv[1], "r");

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
