#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"

void mergesort(Movie **movies, int array_size, char *column_name) {
	if (movies == NULL) {
		return;
	}
	if (array_size < 2) {
		return;
	}

	Movie **left;
	Movie **right;
	int left_size;
	int right_size;
	int i;
	int k;
	int l;
	int r;

	left_size = array_size / 2;
	right_size = array_size - (array_size / 2);
	left = (Movie **) malloc(sizeof(Movie *) * left_size);
	right = (Movie **) malloc(sizeof(Movie *) * right_size);

	for (i = 0; i < left_size; i++) {
		left[i] = movies[i];
	}
	for (i = 0, k = (array_size / 2); i < right_size; i++, k++) {
		right[i] = movies[k];
	}

	mergesort(left, left_size, column_name);
	mergesort(right, right_size, column_name);

	l = 0;
	r = 0;
	for (i = 0; i < array_size; i++) {
		if (l < left_size && r < right_size) {
			if (strcmp(column_name, "color") == 0) {
				if (strcmp(left[l] -> color, right[r] -> color) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "director_name") == 0) {
				if (strcmp(left[l] -> director_name, right[r] -> director_name) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "num_critic_for_reviews") == 0) {
				if (left[l] -> num_critic_for_reviews <= right[r] -> num_critic_for_reviews) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "duration") == 0) {
				if (left[l] -> duration <= right[r] -> duration) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "director_facebook_likes") == 0) {
				if (left[l] -> director_facebook_likes <= right[r] -> director_facebook_likes) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "actor_3_facebook_likes") == 0) {
				if (left[l] -> actor_3_facebook_likes <= right[r] -> actor_3_facebook_likes) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "actor_2_name") == 0) {
				if (strcmp(left[l] -> actor_2_name, right[r] -> actor_2_name) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "actor_1_facebook_likes") == 0) {
				if (left[l] -> actor_1_facebook_likes <= right[r] -> actor_1_facebook_likes) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "gross") == 0) {
				if (left[l] -> gross <= right[r] -> gross) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "genres") == 0) {
				if (strcmp(left[l] -> genres, right[r] -> genres) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "actor_1_name") == 0) {
				if (strcmp(left[l] -> actor_1_name, right[r] -> actor_1_name) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "movie_title") == 0) {
				if (strcmp(left[l] -> movie_title, right[r] -> movie_title) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "num_voted_users") == 0) {
				if (left[l] -> num_voted_users <= right[r] -> num_voted_users) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "cast_total_facebook_likes") == 0) {
				if (left[l] -> cast_total_facebook_likes <= right[r] -> cast_total_facebook_likes) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "actor_3_name") == 0) {
				if (strcmp(left[l] -> actor_3_name, right[r] -> actor_3_name) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "facenumber_in_poster") == 0) {
				if (left[l] -> facenumber_in_poster <= right[r] -> facenumber_in_poster) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "plot_keywords") == 0) {
				if (strcmp(left[l] -> plot_keywords, right[r] -> plot_keywords) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "movie_imdb_link") == 0) {
				if (strcmp(left[l] -> movie_imdb_link, right[r] -> movie_imdb_link) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "num_user_for_reviews") == 0) {
				if (left[l] -> num_user_for_reviews <= right[r] -> num_user_for_reviews) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "language") == 0) {
				if (strcmp(left[l] -> language, right[r] -> language) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "country") == 0) {
				if (strcmp(left[l] -> country, right[r] -> country) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "content_rating") == 0) {
				if (strcmp(left[l] -> content_rating, right[r] -> content_rating) <= 0) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "budget") == 0) {
				if (left[l] -> budget <= right[r] -> budget) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "title_year") == 0) {
				if (left[l] -> title_year <= right[r] -> title_year) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "actor_2_facebook_likes") == 0) {
				if (left[l] -> actor_2_facebook_likes <= right[r] -> actor_2_facebook_likes) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "imdb_score") == 0) {
				if (left[l] -> imdb_score <= right[r] -> imdb_score) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "aspect_ratio") == 0) {
				if (left[l] -> aspect_ratio <= right[r] -> aspect_ratio) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
			else if (strcmp(column_name, "movie_facebook_likes") == 0) {
				if (left[l] -> movie_facebook_likes <= right[r] -> movie_facebook_likes) {
					movies[i] = left[l];
					l++;
				}
				else {
					movies[i] = right[r];
					r++;
				}
			}
		}
		else {
			if (l < left_size) {
				movies[i] = left[l];
				l++;
			}
			else if (r < right_size) {
				movies[i] = right[r];
				r++;
			}
		}
	}
}