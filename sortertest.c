
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include "sorter.h"
#include "mergesort.c"

//-----------------------------

int array_size = 0;

FILE *file; 				// maybe?
Movie **movies;				// maybe?
char *str_buffer;			//idk
char *file_path;			//idk
char *csv_ext_ptr;			//idk
char *directory_name;		//def
int directory_name_len;		//def
char* filename;				//maybe
int filename_len;
DIR *d;						//prob
struct dirent *dir;			//prob

pthread_t tid;				//def
pthread_t current_tid;		//def
pthread_t *tids[255];		//def 		child_pids
int *num_children;			// maybe 

//int i;						// no
//size_t line_length;			//prob no
//char *arg_token;				//prob no
char *output_directory_path;	// not sure
char *field_names;				// not sure

//------------------------



Buffer *init_buffer(char *str_buffer) {
	Buffer *buffer;

	buffer = (Buffer *) malloc(sizeof(Buffer));
	buffer -> str_buffer = str_buffer;
	buffer -> current_token = buffer -> str_buffer;
	buffer -> pointer = buffer -> str_buffer;
	buffer -> hasQuote = 0;
	buffer -> isEnd = 0;

	return buffer;
}

char *get_token(Buffer *buffer) {
	if (buffer == NULL) {
		return NULL;
	}

	// Returns NULL if a newline '\n' is detected
	if (buffer -> isEnd) {
		return NULL;
	}
	int c;

	c = 0;
	if (buffer -> pointer[c] == '\0') {
		buffer -> current_token = &(buffer -> pointer[1]);
	}
	else {
		buffer -> current_token = buffer -> pointer;
	}
	while (buffer -> current_token[0] == ' ') {
		buffer -> current_token = &(buffer -> current_token[1]);
	}
	if (!buffer -> hasQuote) {
		while (buffer -> pointer[c] != ',' && buffer -> pointer[c] != '\n') {
			c++;
		}
	}
	else {
		while (buffer -> pointer[c] != '"') {
			c++;
		}
		buffer -> pointer[c] = '\0';
		c++;
	}
	if (buffer -> pointer[c] == '\n') {
		buffer -> isEnd = 1;
		buffer -> pointer[c] = '\0';
		buffer -> pointer = &(buffer -> pointer[c]);
		return buffer -> current_token;
	}
	if (buffer -> pointer[c + 1] == '"') {
		buffer -> hasQuote = 1;
	}
	else {
		buffer -> hasQuote = 0;
	}
	buffer -> pointer[c] = '\0';
	if (!buffer -> hasQuote) {
		buffer -> pointer = &(buffer -> pointer[c]);
	}
	else {
		buffer -> pointer = &(buffer -> pointer[c + 1]);
		buffer -> pointer[0] = '\0';
	}

	return buffer -> current_token;
}

void free_buffer(Buffer *buffer) {
	free(buffer);
}

/*---------- END BUFFER ----------*/

/*---------- MOVIE ----------*/

Movie **init_array() {
	Movie **movies;

	movies = (Movie **) malloc(sizeof(Movie *));

	return movies;
}

Movie **add_movie(Movie **movies, char *movie_row) {
	Buffer *buffer;
	char *str_token;
	int int_token;
	float float_token;
	int movie_pos;

	// Add one to global variable array length
	// Reallocate movies struct to fit new movie struct
	// Dynamically allocate memory for a new movie struct
	array_size++;
	movie_pos = array_size - 1;
	if (array_size > 1) {
		movies = (Movie **) realloc(movies, sizeof(Movie *) * array_size);
	}
	movies[movie_pos] = (Movie *) malloc(sizeof(Movie));

	// Initialize Buffer struct and tokenize each item in row
	// Use each token and assign it to the proper variable in the movie struct
	buffer = init_buffer(movie_row);
	
	str_token = get_token(buffer);
	movies[movie_pos] -> color = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> color, str_token);

	str_token = get_token(buffer);
	movies[movie_pos] -> director_name = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> director_name, str_token);

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> num_critic_for_reviews = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> duration = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> director_facebook_likes = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> actor_3_facebook_likes = int_token;

	str_token = get_token(buffer);
	movies[movie_pos] -> actor_2_name = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> actor_2_name, str_token);

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> actor_1_facebook_likes = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> gross = int_token;

	str_token = get_token(buffer);
	movies[movie_pos] -> genres = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> genres, str_token);

	str_token = get_token(buffer);
	movies[movie_pos] -> actor_1_name = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> actor_1_name, str_token);

	str_token = get_token(buffer);
	movies[movie_pos] -> movie_title = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> movie_title, str_token);

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> num_voted_users = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> cast_total_facebook_likes = int_token;

	str_token = get_token(buffer);
	movies[movie_pos] -> actor_3_name = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> actor_3_name, str_token);

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> facenumber_in_poster = int_token;

	str_token = get_token(buffer);
	movies[movie_pos] -> plot_keywords = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> plot_keywords, str_token);

	str_token = get_token(buffer);
	movies[movie_pos] -> movie_imdb_link = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> movie_imdb_link, str_token);

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> num_user_for_reviews = int_token;

	str_token = get_token(buffer);
	movies[movie_pos] -> language = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> language, str_token);

	str_token = get_token(buffer);
	movies[movie_pos] -> country = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> country, str_token);

	str_token = get_token(buffer);
	movies[movie_pos] -> content_rating = (char *) malloc(sizeof(char) * strlen(str_token) + 1);
	strcpy(movies[movie_pos] -> content_rating, str_token);

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> budget = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> title_year = int_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> actor_2_facebook_likes = int_token;

	str_token = get_token(buffer);
	float_token = atof(str_token);
	movies[movie_pos] -> imdb_score = float_token;

	str_token = get_token(buffer);
	float_token = atof(str_token);
	movies[movie_pos] -> aspect_ratio = float_token;

	str_token = get_token(buffer);
	int_token = atoi(str_token);
	movies[movie_pos] -> movie_facebook_likes = int_token;

	// Frees the dynamically allocated buffer
	free_buffer(buffer);

	return movies;
}

void print_movies_csv(Movie **movies) {
	char *csv_header;
	int has_comma;
	int m;
	int i;

	csv_header = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";

	printf("%s\n", csv_header);
	
	has_comma = 0;
	for (m = 0; m < array_size; m++) {
		for (i = 0; i < strlen(movies[m] -> color); i++) {
			if (movies[m] -> color[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> color);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> color);
		}

		for (i = 0; i < strlen(movies[m] -> director_name); i++) {
			if (movies[m] -> director_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> director_name);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> director_name);
		}

		printf("%d,", movies[m] -> num_critic_for_reviews);
		printf("%d,", movies[m] -> duration);
		printf("%d,", movies[m] -> director_facebook_likes);
		printf("%d,", movies[m] -> actor_3_facebook_likes);

		for (i = 0; i < strlen(movies[m] -> actor_2_name); i++) {
			if (movies[m] -> actor_2_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> actor_2_name);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> actor_2_name);
		}

		printf("%d,", movies[m] -> actor_1_facebook_likes);
		printf("%d,", movies[m] -> gross);

		for (i = 0; i < strlen(movies[m] -> genres); i++) {
			if (movies[m] -> genres[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> genres);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> genres);
		}

		for (i = 0; i < strlen(movies[m] -> actor_1_name); i++) {
			if (movies[m] -> actor_1_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> actor_1_name);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> actor_1_name);
		}

		for (i = 0; i < strlen(movies[m] -> movie_title); i++) {
			if (movies[m] -> movie_title[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> movie_title);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> movie_title);
		}

		printf("%d,", movies[m] -> num_voted_users);
		printf("%d,", movies[m] -> cast_total_facebook_likes);

		for (i = 0; i < strlen(movies[m] -> actor_3_name); i++) {
			if (movies[m] -> actor_3_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> actor_3_name);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> actor_3_name);
		}

		printf("%d,", movies[m] -> facenumber_in_poster);

		for (i = 0; i < strlen(movies[m] -> plot_keywords); i++) {
			if (movies[m] -> plot_keywords[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> plot_keywords);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> plot_keywords);
		}

		for (i = 0; i < strlen(movies[m] -> movie_imdb_link); i++) {
			if (movies[m] -> movie_imdb_link[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> movie_imdb_link);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> movie_imdb_link);
		}

		printf("%d,", movies[m] -> num_user_for_reviews);

		for (i = 0; i < strlen(movies[m] -> language); i++) {
			if (movies[m] -> language[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> language);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> language);
		}

		for (i = 0; i < strlen(movies[m] -> country); i++) {
			if (movies[m] -> country[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> country);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> country);
		}

		for (i = 0; i < strlen(movies[m] -> content_rating); i++) {
			if (movies[m] -> content_rating[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			printf("\"");
			printf("%s", movies[m] -> content_rating);
			printf("\",");
			has_comma = 0;
		}
		else {
			printf("%s,", movies[m] -> content_rating);
		}

		printf("%d,", movies[m] -> budget);
		printf("%d,", movies[m] -> title_year);
		printf("%d,", movies[m] -> actor_2_facebook_likes);
		printf("%.2f,", movies[m] -> imdb_score);
		printf("%.2f,", movies[m] -> aspect_ratio);
		printf("%d\n", movies[m] -> movie_facebook_likes);
	}
}

/*
 * Creates output csv file in same directory for testing purposes.
 */
void output(Movie **movies) {
	char *csv_header;
	int m;
	FILE *file;
	int has_comma;
	int i;

	file = fopen("output.csv", "w");

	csv_header = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";

	fprintf(file, "%s\n", csv_header);
	
	has_comma = 0;
	for (m = 0; m < array_size; m++) {
		for (i = 0; i < strlen(movies[m] -> color); i++) {
			if (movies[m] -> color[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> color);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> color);
		}

		for (i = 0; i < strlen(movies[m] -> director_name); i++) {
			if (movies[m] -> director_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> director_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> director_name);
		}

		fprintf(file, "%d,", movies[m] -> num_critic_for_reviews);
		fprintf(file, "%d,", movies[m] -> duration);
		fprintf(file, "%d,", movies[m] -> director_facebook_likes);
		fprintf(file, "%d,", movies[m] -> actor_3_facebook_likes);

		for (i = 0; i < strlen(movies[m] -> actor_2_name); i++) {
			if (movies[m] -> actor_2_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> actor_2_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> actor_2_name);
		}

		fprintf(file, "%d,", movies[m] -> actor_1_facebook_likes);
		fprintf(file, "%d,", movies[m] -> gross);

		for (i = 0; i < strlen(movies[m] -> genres); i++) {
			if (movies[m] -> genres[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> genres);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> genres);
		}

		for (i = 0; i < strlen(movies[m] -> actor_1_name); i++) {
			if (movies[m] -> actor_1_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> actor_1_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> actor_1_name);
		}

		for (i = 0; i < strlen(movies[m] -> movie_title); i++) {
			if (movies[m] -> movie_title[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> movie_title);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> movie_title);
		}

		fprintf(file, "%d,", movies[m] -> num_voted_users);
		fprintf(file, "%d,", movies[m] -> cast_total_facebook_likes);

		for (i = 0; i < strlen(movies[m] -> actor_3_name); i++) {
			if (movies[m] -> actor_3_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> actor_3_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> actor_3_name);
		}

		fprintf(file, "%d,", movies[m] -> facenumber_in_poster);

		for (i = 0; i < strlen(movies[m] -> plot_keywords); i++) {
			if (movies[m] -> plot_keywords[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> plot_keywords);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> plot_keywords);
		}

		for (i = 0; i < strlen(movies[m] -> movie_imdb_link); i++) {
			if (movies[m] -> movie_imdb_link[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> movie_imdb_link);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> movie_imdb_link);
		}

		fprintf(file, "%d,", movies[m] -> num_user_for_reviews);

		for (i = 0; i < strlen(movies[m] -> language); i++) {
			if (movies[m] -> language[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> language);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> language);
		}

		for (i = 0; i < strlen(movies[m] -> country); i++) {
			if (movies[m] -> country[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> country);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> country);
		}

		for (i = 0; i < strlen(movies[m] -> content_rating); i++) {
			if (movies[m] -> content_rating[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> content_rating);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> content_rating);
		}

		fprintf(file, "%d,", movies[m] -> budget);
		fprintf(file, "%d,", movies[m] -> title_year);
		fprintf(file, "%d,", movies[m] -> actor_2_facebook_likes);
		fprintf(file, "%.2f,", movies[m] -> imdb_score);
		fprintf(file, "%.2f,", movies[m] -> aspect_ratio);
		fprintf(file, "%d\n", movies[m] -> movie_facebook_likes);
	}

	fclose(file);
}
/*---------- END MOVIE ----------*/

int check_options(int argc, char **argv) {
	// Check if enough arguments
	if (argc < 3) {
		return 0;
	}
	
	// Check if second argument is correct
	if (strcmp(argv[1], "-c") != 0) {
		return 0;
	}

	char *token;
	char *arg_copy;
	int arg_copy_len;

	arg_copy_len = strlen(argv[2]);
	arg_copy = (char *) malloc((sizeof(char) * arg_copy_len) + 1);
	arg_copy = strcpy(arg_copy, argv[2]);

	token = strtok(arg_copy, ",");

	while (token) {
		// Check if third argument is correct; the argument must be a column
		// name from the given csv file
		if (
			strcmp(token, "color") != 0 &&
			strcmp(token, "director_name") != 0 &&
			strcmp(token, "num_critic_for_reviews") != 0 &&
			strcmp(token, "duration") != 0 &&
			strcmp(token, "director_facebook_likes") != 0 &&
			strcmp(token, "actor_3_facebook_likes") != 0 &&
			strcmp(token, "actor_2_name") != 0 &&
			strcmp(token, "actor_1_facebook_likes") != 0 &&
			strcmp(token, "gross") != 0 &&
			strcmp(token, "genres") != 0 &&
			strcmp(token, "actor_1_name") != 0 &&
			strcmp(token, "movie_title") != 0 &&
			strcmp(token, "num_voted_users") != 0 &&
			strcmp(token, "cast_total_facebook_likes") != 0 &&
			strcmp(token, "actor_3_name") != 0 &&
			strcmp(token, "facenumber_in_poster") != 0 &&
			strcmp(token, "plot_keywords") != 0 &&
			strcmp(token, "movie_imdb_link") != 0 &&
			strcmp(token, "num_user_for_reviews") != 0 &&
			strcmp(token, "language") != 0 &&
			strcmp(token, "country") != 0 &&
			strcmp(token, "content_rating") != 0 &&
			strcmp(token, "budget") != 0 &&
			strcmp(token, "title_year") != 0 &&
			strcmp(token, "actor_2_facebook_likes") != 0 &&
			strcmp(token, "imdb_score") != 0 &&
			strcmp(token, "aspect_ratio") != 0 &&
			strcmp(token, "movie_facebook_likes") != 0
		) {
			return 0;
		}

		token = strtok(NULL, ",");
	}

	if (argc > 3) {
		if (strcmp(argv[3], "-d") != 0 && strcmp(argv[3], "-o") != 0) {
			return 0;
		}
		if (argc < 5) {
			return 0;
		}
	}
	if (argc > 5) {
		if (strcmp(argv[5], "-o") != 0) {
			return 0;
		}
		if (argc < 7) {
			return 0;
		}
	}

	return 1;
}

void create_output(Movie **movies, char *output_directory_path, char *filename, char *field_name) {
	char *final_path;
	struct stat st = {0};
	int c;
	FILE *file;
	char *csv_header;
	int m;
	int i;
	int has_comma;

	// Replace '.' with '\0' to remove '.csv' suffix from filename
	c = 0;
	while (filename[c] != '.') {
		c++;
	}
	filename[c] = '\0';

	// Create final path string
	// Create directory if does not exist
	if (strcmp(output_directory_path, ".") == 0) {
		final_path = (char *) malloc((sizeof(char) * strlen(filename)) + (sizeof(char) * 8) + (sizeof(char) * strlen(field_name) + (sizeof(char) * 4) + (sizeof(char) * 1)));
		final_path = strcpy(final_path, filename);
		final_path = strcat(final_path, "-sorted-");
		final_path = strcat(final_path, field_name);
		final_path = strcat(final_path, ".csv");
	}
	else {
		final_path = (char *) malloc((sizeof(char) * strlen(output_directory_path)) + (sizeof(char) * 1) + (sizeof(char) * strlen(filename)) + (sizeof(char) * 8) + (sizeof(char) * strlen(field_name)) + (sizeof(char) * 4) + (sizeof(char) * 1));
		final_path = strcpy(final_path, output_directory_path);
		final_path = strcat(final_path, "/");
		final_path = strcat(final_path, filename);
		final_path = strcat(final_path, "-sorted-");
		final_path = strcat(final_path, field_name);
		final_path = strcat(final_path, ".csv");

		if (stat(output_directory_path, &st) == -1) {
			mkdir(output_directory_path, 0700);
		}
	}

	// Checks if duplicate file exists. If it does delete it.
	file = fopen(final_path, "r");
	if (file) {
		fclose(file);
		remove(final_path);
	}

	// Create new csv file and print output
	file = fopen(final_path, "w");

	csv_header = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";

	fprintf(file, "%s\n", csv_header);
	
	has_comma = 0;
	for (m = 0; m < array_size; m++) {
		for (i = 0; i < strlen(movies[m] -> color); i++) {
			if (movies[m] -> color[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> color);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> color);
		}

		for (i = 0; i < strlen(movies[m] -> director_name); i++) {
			if (movies[m] -> director_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> director_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> director_name);
		}

		fprintf(file, "%d,", movies[m] -> num_critic_for_reviews);
		fprintf(file, "%d,", movies[m] -> duration);
		fprintf(file, "%d,", movies[m] -> director_facebook_likes);
		fprintf(file, "%d,", movies[m] -> actor_3_facebook_likes);

		for (i = 0; i < strlen(movies[m] -> actor_2_name); i++) {
			if (movies[m] -> actor_2_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> actor_2_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> actor_2_name);
		}

		fprintf(file, "%d,", movies[m] -> actor_1_facebook_likes);
		fprintf(file, "%d,", movies[m] -> gross);

		for (i = 0; i < strlen(movies[m] -> genres); i++) {
			if (movies[m] -> genres[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> genres);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> genres);
		}

		for (i = 0; i < strlen(movies[m] -> actor_1_name); i++) {
			if (movies[m] -> actor_1_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> actor_1_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> actor_1_name);
		}

		for (i = 0; i < strlen(movies[m] -> movie_title); i++) {
			if (movies[m] -> movie_title[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> movie_title);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> movie_title);
		}

		fprintf(file, "%d,", movies[m] -> num_voted_users);
		fprintf(file, "%d,", movies[m] -> cast_total_facebook_likes);

		for (i = 0; i < strlen(movies[m] -> actor_3_name); i++) {
			if (movies[m] -> actor_3_name[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> actor_3_name);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> actor_3_name);
		}

		fprintf(file, "%d,", movies[m] -> facenumber_in_poster);

		for (i = 0; i < strlen(movies[m] -> plot_keywords); i++) {
			if (movies[m] -> plot_keywords[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> plot_keywords);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> plot_keywords);
		}

		for (i = 0; i < strlen(movies[m] -> movie_imdb_link); i++) {
			if (movies[m] -> movie_imdb_link[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> movie_imdb_link);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> movie_imdb_link);
		}

		fprintf(file, "%d,", movies[m] -> num_user_for_reviews);

		for (i = 0; i < strlen(movies[m] -> language); i++) {
			if (movies[m] -> language[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> language);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> language);
		}

		for (i = 0; i < strlen(movies[m] -> country); i++) {
			if (movies[m] -> country[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> country);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> country);
		}

		for (i = 0; i < strlen(movies[m] -> content_rating); i++) {
			if (movies[m] -> content_rating[i] == ',') {
				has_comma = 1;
			}
		}
		if (has_comma == 1) {
			fprintf(file, "\"");
			fprintf(file, "%s", movies[m] -> content_rating);
			fprintf(file, "\",");
			has_comma = 0;
		}
		else {
			fprintf(file, "%s,", movies[m] -> content_rating);
		}

		fprintf(file, "%d,", movies[m] -> budget);
		fprintf(file, "%d,", movies[m] -> title_year);
		fprintf(file, "%d,", movies[m] -> actor_2_facebook_likes);
		fprintf(file, "%.2f,", movies[m] -> imdb_score);
		fprintf(file, "%.2f,", movies[m] -> aspect_ratio);
		fprintf(file, "%d\n", movies[m] -> movie_facebook_likes);
	}

	fclose(file);
}


int main(int argc, char **argv) {

	int *num_children;
	int i;						
	size_t line_length;			
	char *arg_token;	

	for(i=0; i < 255; i++){

		pthread_attr_t attr;
		pthread_attr_init(&attr); // dont know if need this 
		pthread_create(&tids[i], attr /*or NULL*/, ___, &argv); 
	}
	
	//num_children = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	*num_children = 0; 

	tid = gettid();
	printf("Initial TID: %d\n", tid);

	// Checks whether a directory path is given
	if (argc > 3 && strcmp(argv[3], "-d") == 0) {
		directory_name = argv[4];
		directory_name_len = strlen(directory_name);
	}
	else {
		directory_name = ".";
		directory_name_len = 0;
	}

	d = opendir(directory_name);
	if (d) {
		// Takes all filenames in directory and searches for those that end with ".csv"
		while ((dir = readdir(d)) != NULL) {
			filename = dir -> d_name;
			filename_len = strlen(filename);
			csv_ext_ptr = &filename[filename_len - 4];
			if (strcmp(csv_ext_ptr, ".csv") == 0) {
				// Creates child thread that will perform the movie array creation
				// and mergesort functions
				//current_pid = fork()
				pthread_create(&current_tid, NULL, function, *argv /*or NULL*/);	///FIGURE THIS SHIT OUT
				*tid = gettid();

				if (current_tid == 0) {
					// Keeps track of the number of child threads
					// Inputs child pthread ids to tids array
					(*num_children)++;
					*(tids[*num_children - 1]) = tid;

					// Creates file path based on given input
					if (argc > 3 && strcmp(argv[3], "-d") == 0) {
						file_path = (char *) malloc((sizeof(char) * directory_name_len) + (sizeof(char) * filename_len) + 2);
						file_path = strcpy(file_path, directory_name);

						file_path = strcat(file_path, "/");
						file_path = strcat(file_path, filename);
					}
					else {
						file_path = (char *) malloc((sizeof(char) * filename_len) + 1);
						file_path = strcpy(file_path, filename);
					}

					// Opens files with ".csv" suffix, creates movie array and sorts it
					file = fopen(file_path, "r");
					movies = init_array();

					str_buffer = (char *) malloc(sizeof(char) * 1000);
					line_length = getline(&str_buffer, &line_length, file);

					while (line_length != -1) {
						line_length = getline(&str_buffer, &line_length, file);
						if (line_length != -1) {
							movies = add_movie(movies, str_buffer);
						}
					}

					// Takes all column name arguments and sorts it. The last one given
					// will be the last sorted column
					field_names = (char *) malloc((sizeof(char) * strlen(argv[2])) + 1);
					field_names = strcpy(field_names, argv[2]);

					arg_token = strtok(field_names, ",");
					while (arg_token) {
						mergesort(movies, array_size, arg_token);

						arg_token = strtok(NULL, ",");
					}

					// Creates file with output in same directory for testing purposes
					// output(movies);

					// Gets output directory path and prints out csv file in given or 
					// default directory
					if (argc > 3 && strcmp("-o", argv[3]) == 0) {
						output_directory_path = argv[4];
					}
					else if (argc > 5 && strcmp("-o", argv[5]) == 0) {
						output_directory_path = argv[6];
					}
					else {
						output_directory_path = ".";
					}

					create_output(movies, output_directory_path, filename, argv[2]);

					// Frees memory that is no longer being used
					free(str_buffer);
					fclose(file);

					exit(EXIT_SUCCESS);
					
				}
			}
		}
	}

	// Waits for all children to exit
	//while(wait(NULL) != -1) {}
	for(i = 0; i<225; i++){
		pthread_join(*tids[i], NULL);
	}

	// Parent process will print out process information
	if (current_tid > 0) {
		printf("TIDS of all child threads: ");
		for (i = 0; i < *num_children; i++) {
			printf("%lu ", *(tids[i]));
		}
		printf("\n");
		printf("Total number of threads: %d\n", *num_children + 1);
	}

	return 0;
}










