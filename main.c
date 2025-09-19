#include <stdio.h>

typedef enum {
	move_right,
	move_left,
	increment,
	decrement,
	print,
	loop_begin,
	loop_end,
	input,
} TokenType;

long int fsize(FILE *f) {
	fseek(f, 0l, SEEK_END);
	long int size = ftell(f);
	rewind(f);

	return size;
}

TokenType* lexer(FILE *source_file) {
	size_t i = 0;
	long int f_size = fsize(source_file);
	char cmds[f_size];
	
	char c;

	while( (c = fgetc(source_file)) != EOF) {
		if(c != '\n') {
			cmds[i] = (char)c;
			i++;
		}

	}
	
	cmds[i] = '\0';
	int code_length = i;

	TokenType tokens[code_length];
	for(int i = 0; i < code_length; i++) {
		if(cmds[i] == '+') tokens[i] = increment;
		if(cmds[i] == '-') tokens[i] = decrement;

		if(cmds[i] == '>') tokens[i] = move_right;
		if(cmds[i] == '<') tokens[i] = move_left;

		if(cmds[i] == '.') tokens[i] = print;
		if(cmds[i] == ',') tokens[i] = input;

		if(cmds[i] == '[') tokens[i] = loop_begin;
		if(cmds[i] == ']') tokens[i] = loop_end;

		printf("%c", cmds[i]);
	}

	printf("\n");
	
	for(int i = 0; i < code_length; i++) {
		printf("%d", tokens[i]);
	}

	return tokens;

}

int main() {
	FILE *f_ptr = fopen("code.bf", "r");
	long int f_size = fsize(f_ptr);
	char code_tape[f_size];

	lexer(f_ptr);

	fclose(f_ptr);
	return 0;
}
