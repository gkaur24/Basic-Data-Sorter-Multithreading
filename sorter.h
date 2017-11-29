
/**
 * STRUCT: Movie
 * -------------
 * A struct containing information about a single movie
 */

#ifndef SORTER_H_
#define SORTER_H_

typedef struct movie {
	char *color;
	char *director_name;
	int num_critic_for_reviews;
	int duration;
	int director_facebook_likes;
	int actor_3_facebook_likes;
	char *actor_2_name;
	int actor_1_facebook_likes;
	int gross;
	char *genres;
	char *actor_1_name;
	char *movie_title;
	int num_voted_users;
	int cast_total_facebook_likes;
	char *actor_3_name;
	int facenumber_in_poster;
	char *plot_keywords;
	char *movie_imdb_link;
	int num_user_for_reviews;
	char *language;
	char *country;
	char *content_rating;
	int budget;
	int title_year;
	int actor_2_facebook_likes;
	float imdb_score;
	float aspect_ratio;
	int movie_facebook_likes;
} Movie;

/**
 * FUNCTION: init_array
 * --------------------
 * Initializes the movie array by dynamically allocating a pointer
 * to pointers to the Movie struct
 *
 * returns: A dynamically allocated Movie array containing pointers
 * to Movie structs
 */
Movie **init_array();

/**
 * FUNCTION: add_movie
 * -------------------
 * Adds a movie struct into the given array using the information
 * given from a line (row) of a csv file. This function uses the
 * Buffer struct to tokenize each line it is given and assigns the
 * tokenized values to its proper variable in the Movie struct.
 *
 * movies: the array containing movies
 * movie_row: a line of string containing information of the movie,
 * retrieved from a row on a csv file
 */
Movie **add_movie(Movie **movies, char *movie_row);

/**
 * FUNCTION: print_movies_csv
 * --------------------------
 * Prints the given movies as a csv file to STDOUT
 *
 * movies: the movie struct to print
 */
void print_movies_csv(Movie **movies);

/*---------- END MOVIE STRUCT ----------*/

/**
 * STRUCT: Buffer
 * --------------
 * A buffer struct that holds a line of strings and allows
 * the tokenization of the line using a comma or a double quote
 * as delimiters. Values separated by a double quote is not 
 * tokenized with a comma.
 */
typedef struct buffer {
	char *str_buffer;
	char *current_token;
	char *pointer;
	int hasQuote;
	int isEnd;
} Buffer;

/**
 * FUNCTION: init_buffer
 * ---------------------
 * Creates and returns an initialized Buffer struct
 * that contains the given line of strings
 *
 * str_buffer: the line of string to tokenize
 *
 * returns: the initialized buffer struct
 */
Buffer *init_buffer(char *str_buffer);

/**
 * FUNCTION: get_token
 * -------------------
 * Retrieves and returns the next token in the buffer. For each
 * token, a pointer points to the null terminating byte '\0'
 * located before the new token. This pointer is then used to find
 * the next comma or double quote located after this new token and
 * is changed to a null terminating byte. If a newline is found,
 * it is changed to a null terminating byte and a variable isEnd
 * is changed to true to indicate the end of the string buffer.
 *
 * buffer: the buffer to retrieve the token from
 *
 * returns: the next token
 */
char *get_token(Buffer *buffer);

/**
 * FUNCTION: free_buffer
 * ---------------------
 * Frees the dynamically allocated memory for the buffer
 *
 * buffer: the pointer to the buffer that is to be freed
 */
void free_buffer(Buffer *buffer);

/*---------- END BUFFER STRUCT ---------- */

/**
 * FUNCTION: mergesort
 * -------------------
 * Takes a movie array as a parameter and sorts it alphabetically
 * using the mergesort algorithm.
 *
 * movies: the movie array to sort
 * array_size: the size of the movie array
 * column_name: the name of the column to sort it by
 */
void mergesort(Movie **movies, int array_size, char *column_name);

/**
 * FUNCTION: check_options
 * -----------------------
 * Checks the arguments that the user gives and checks if it 
 * is correct. Correct arguments must have "-c" as the second argument
 * and a column name from the csv file as the third argument.
 *
 * argc: the number of arguments given
 * argv: the pointer to the argument strings
 *
 * returns: 1 if it is correct, 0 if not
 */
int check_options(int argc, char **argv);

/**
 * FUNCTION: create_output
 * -----------------------
 * Creates the output csv to given or default directory
 *
 * movies: the movie array to print
 * output_directory_path: the given output directory
 * filename: the name of the input file that was sorted
 * field_name: the column name of the csv file that is sorted
 */
void create_output(Movie **movies, char *output_directory_path, char *filename, char *field_name);

#endif