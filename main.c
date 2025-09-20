#include <stdio.h>
#include <stdlib.h>

//magic numbers
#define code_end 9

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
	int code_length = i;

	TokenType* tokens = malloc(sizeof(source_chars) * code_length);
	for(int i = 0; i < code_length; i++) {
		if(source_chars[i] == '+') tokens[i] = INCREMENT;
		if(source_chars[i] == '-') tokens[i] = DECREMENT;

		if(source_chars[i] == '>') tokens[i] = MOVE_RIGHT;
		if(source_chars[i] == '<') tokens[i] = MOVE_LEFT;

		if(source_chars[i] == '.') tokens[i] = PRINT;
		if(source_chars[i] == ',') tokens[i] = INPUT;

		if(source_chars[i] == '[') tokens[i] = LOOP_BEGIN;
		if(source_chars[i] == ']') tokens[i] = LOOP_END;

		printf("%c", source_chars[i]);
	}

	printf("\n");
	
	for(int i = 0; i < code_length; i++) {
		//printf("%d", tokens[i]);
	}
	
	tokens[i] = code_end;
	return tokens;

}

void execute(TokenType *tokens) {
	char code[1000] = {0};
	size_t ptr = 0;

	int loops = 0;
	size_t loop_numeraters[100] = {0};
	size_t loop_starts[100] = {0};
	int numeraters = 0;
	int current_loop = -1;

	int i = 0;
	while(tokens[i] != code_end) {
		switch(tokens[i]) {
			case MOVE_RIGHT:
				ptr += 1;
				break;
			case MOVE_LEFT:
				if(ptr != 0) {
					ptr -= 1;
				}
				break;
			case INCREMENT:
				code[ptr] += 1;
				break;
			case DECREMENT:
				code[ptr] -= 1;
				break;
			case LOOP_BEGIN:
				loop_starts[current_loop] += i;
				loop_numeraters[numeraters] = i - 1;
				current_loop += 1;
				numeraters += 1;
				break;

			case LOOP_END:
				if(loops > 0 && numeraters > 0 && loop_numeraters[current_loop] >= 0) {
						loop_numeraters[current_loop] -= 1;
						ptr = loop_starts[current_loop] + 1;
				}
				break;
			case PRINT:
				printf("%d", code[ptr]);
				break;
			case INPUT:
				//printf("taking input isnt implemented yet");
		}

		i++;
	}
}

int main() {
	FILE *f_ptr = fopen("code.bf", "r");

	if(f_ptr == NULL) {
		printf("file not found");
		exit(-1);
	}

	TokenType *tokens = lexer(f_ptr);
	
	execute(tokens);

	fclose(f_ptr);
	return 0;
}
