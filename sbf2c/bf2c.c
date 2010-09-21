/* 
 * Copyright (C) 2010 Vladimir Badaev <skif_ne@mail.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

enum Errors {ERROR_OK, ERROR_ARGS, ERROR_OPEN_FILE, ERROR_NOT_RELEASED, ERROR_COMPILE};
enum Compile {COMP_TO_C, COMP_TO_ASM, COMP_TO_ZIP};
#define COMP_TO_ASM_NOT_RELEASED 1
#define COMP_TO_ZIP_NOT_RELEASED 1

#define CHANGABLE_DATA 1
#if CHANGABLE_DATA == 0
# define BF_BYTE "unsigned char"
# define BF_SIZE "30000"
#else /* CHANGABLE_DATA != 0 */
char *default_bf_byte = "unsigned char";
int default_bf_size = 30000;

char *bf_byte; // = default_bf_byte;
int bf_size; // = default_bf_size;
#endif /* CHANGABLE_DATA != 0 */

int	compile = COMP_TO_C;
int	dump = 0;
int	dump_byte = -1;
int	dump_length = 1;

typedef	int (*compiler_t)(FILE *src, FILE *dst);

int	compiler_to_c(FILE *, FILE *);
int	compiler_to_asm(FILE *, FILE *);
int	compiler_to_zip(FILE *, FILE *);
int	usage(int);

compiler_t	compilers[] =
{
	&compiler_to_c,
	&compiler_to_asm,
	&compiler_to_zip
};


int	main(int argc, char *argv[])
{
	int	ch;
	char	*src_name = NULL;
	FILE	*src_file = NULL;
	char	*dst_name = NULL;
	FILE	*dst_file = NULL;
#if CHANGABLE_DATA == 0
# define GETOPT_STR "adD:hL:o:z"
#else
	bf_byte = default_bf_byte;
	bf_size = default_bf_size;
# define GETOPT_STR "adD:hL:o:s:t:z"
#endif
	while((ch = getopt(argc, argv, GETOPT_STR)) != EOF)
	{
		switch(ch)
		{
			case	'd':
				dump = 1;
				break;
			case	'D':
				dump_byte = atoi(optarg);
				break;
			case	'L':
				dump_length = atoi(optarg);
				break;
			case	'a':
				compile = COMP_TO_ASM;
				break;
			case	'z':
				compile = COMP_TO_ZIP;
				break;
			case	'h':
				usage(ERROR_OK);
				break;
			case	'o':
				dst_name = optarg;
				break;
#if CHANGABLE_DATA != 0
			case	's':
				bf_size = atoi(optarg);
				break;
			case	't':
				bf_byte = optarg;
				break;
#endif
			case	'?':
			default:
				usage(ERROR_ARGS);
				break;
		}
	}

	argc -= optind;
	argv += optind;

	if(argc >= 1)
	{
		src_name = argv[0];
		if((src_file = fopen(src_name, "r")) == NULL)
		{
			perror("Can't open source file");
			exit(ERROR_OPEN_FILE);
		};
	} else {
		src_name = "stdin";
		src_file = stdin;
	};

	if(dst_name == NULL)
	{
		dst_name = "stdout";
		dst_file = stdout;
	} else {
		if((dst_file = fopen(dst_name, "w")) == NULL)
		{
			perror("Can't open distantion file");
			exit(ERROR_OPEN_FILE);
		};
	};
	if(compilers[compile](src_file, dst_file))
		exit(ERROR_COMPILE);

	return ERROR_OK;
}







int	usage(int er)
{
	FILE *o = er ? stderr : stdout;
#if CHANGABLE_DATA == 0
	fprintf(o, "bf2c [-adhz] [-D offset] [-L length] [-o dst_file] [src_file]\n");
#else 
	fprintf(o, "bf2c [-adhz] [-D offset] [-L length] [-s data size] [-t data type] [-o dst_file] [src_file]\n");
#endif
	fprintf(o, "\t-a\t\tCompile to asm. (Not released)\n"
			"\t-d\t\tDump ???\n"
			"\t-D <offset>\tDump bytes from <offset>.\n"
			"\t-L <length>\tDump <length> bytes.\n"
			"\t-h\t\tPrint this message.\n"
			"\t-z\t\tCompile to zip. (Not relesed)\n"
			"\t-o <dst>\tOutput to <dst> file.\n"
#if CHANGABLE_DATA != 0
			"\t-s <size>\tData size.\n"
			"\t-t <type>\tData type.\n"
#endif
			);
	exit(er);
}
int	compiler_to_c(FILE *src, FILE *dst)
{
	int ch;
	fprintf(dst,
			"/***********************************************************\n"
			" * Created with sbf2c -- simple brainfuck to c translator. *\n"
			" ***********************************************************/\n\n"
			"#include <stdio.h>\n");
#if CHANGABLE_DATA == 0
	fprintf(dst,
			BF_BYTE "\tmemory[" BF_SIZE "];\n"
			BF_BYTE "\t*ptr = memory;\n"
			"int\tch;\n"
			"\n");
#else
	fprintf(dst, 
			"%s\tmemory[%i];\n"
			"%s\t*ptr = memory;\n"
			"\n", bf_byte, bf_size, bf_byte);
#endif
	if(dump_byte != -1) {
		fprintf(dst,
			"void dump(char c) {\n"
			"\tint i;\n"
			"\tfprintf(stderr, \"DUMP: code=%%c, memory[%%d..%%d]=\", c, %d, %d);\n"
			"\tfor(i = 0; i < %d; i++){\n"
			"\t\tfprintf(stderr, ((ptr - memory) == i) ? \" (%%d)\" : \" %%d\", memory[%d+i]);\n"
			"\t}\n"
			"\tfprintf(stderr, \"\\n\");\n"
			"}\n",
			dump_byte, (dump_byte+dump_length), dump_length, dump_byte);
	}
	fprintf(dst,
			"int\tmain()\n"
			"{\n");
	while((ch = fgetc(src)) != EOF)
	{
		switch(ch)
		{
			case	'>':
				fprintf(dst, "\t++ptr;\n");
				break;
			case	'<':
				fprintf(dst, "\t--ptr;\n");
				break;
			case	'+':
				fprintf(dst, "\t++*ptr;\n");
				break;
			case	'-':
				fprintf(dst, "\t--*ptr;\n");
				break;
			case	'[':
				fprintf(dst, "\twhile(*ptr) {\n");
				break;
			case	']':
				fprintf(dst, "\t};\n");
				break;
			case	'.':
				fprintf(dst, "\tputchar(*ptr);\n");
				break;
			case	',':
#if CHANGABLE_DATA == 0
				fprintf(dst, "\tch = getchar();\n"
						"\t*ptr = (ch == EOF) ? 0 : (" BF_BYTE ")ch;\n");
#else
				fprintf(dst, "\tch = getchar();\n"
						"\t*ptr = (ch == EOF) ? 0 : (%s)ch;\n", bf_byte);
#endif
				break;
			case	'd':
				if(dump)
					fprintf(dst, "fprintf(stderr, \"DUMP: *ptr(%%x)\\nDUMP:\", ptr-memory);\n"
					"\t{\n\t\tint i;\n"
					"\t\tfor(i = 0; i < 10; i++){\n"
					"\t\t\tfprintf(stderr, \"%%3d \", memory[i]);\n"
					"\t\t};\n"
				       	"\t\tfprintf(stderr, \"\\n\");\n\t};");
				break;
			default:
				continue;	
		};
		//		if(dump)
		//			fprintf(dst, "\tfprintf(stderr, \"DUMP: cmd = `%c', *ptr(%%x) = `%%c'(%%d)\\n\", ptr-memory, *ptr, *ptr);\n", ch);
#if 0
		if(dump)
			fprintf(dst, "fprintf(stderr, \"DUMP: *ptr(%%x)\\nDUMP:\", ptr-memory);\n"
				"\t{\n\t\tint i;\n"
				"\t\tfor(i = 0; i < 10; i++){\n"
				"\t\t\tfprintf(stderr, \"%%3d \", memory[i]);\n"
				"\t\t};\n"
			       	"\t\tfprintf(stderr, \"\\n\");\n\t};");
#endif
		if((dump_byte != -1) && (dump_length != -1)){
			fprintf(dst, "\tdump(%d);\n", ch);
		}
	}
	if((dump_byte != -1) && (dump_length == -1)) {
		fprintf(dst, "\tfprintf(stderr, \"%%d\", memory[%d]);\n", dump_byte);
	}
	fprintf(dst, "\treturn 0;\n"
			"}\n");
	return ERROR_OK;
}

int	compiler_to_asm(FILE *src, FILE *dst)
{
#if defined(COMP_TO_ASM_NOT_RELEASED) && (COMP_TO_ASM_NOT_RELEASED != 0)
	(void) src;
	(void) dst;
#endif /* COMP_TO_ASM_NOT_RELEASED && COMP_TO_ASM_NOT_RELEASED != 0 */
	return ERROR_NOT_RELEASED;
}
int	compiler_to_zip(FILE *src, FILE *dst)
{
#if defined(COMP_TO_ZIP_NOT_RELEASED) && (COMP_TO_ZIP_NOT_RELEASED != 0)
	(void) src;
	(void) dst;
#endif /* defined(COMP_TO_ZIP_NOT_RELEASED) && (COMP_TO_ZIP_NOT_RELEASED != 0) */
	return ERROR_NOT_RELEASED;
}
