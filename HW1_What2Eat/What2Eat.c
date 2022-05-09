#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Choice{
	int point;
	char name[200];
}Choice;

typedef struct Vector{
	size_t size;
	void ** items
}Vector;

Vector* vector_init(size_t item_size){
	Vector* vector;
	vector = malloc(sizeof(Vector));
	vector -> size = 0;
	vector -> items = calloc(0, item_size);
	return vector;
}

void vector_append(Vector* vector, void* item){
	vector -> size ++;
	vector -> items = realloc(vector -> items, vector -> size * sizeof(item));
	vector -> items[vector -> size - 1] = item;
}

void vector_free(Vector* vector){
	for(int i = 0 ; i < vector -> size ; i++)
		free(vector -> items[i]);
	
	free(vector -> items);
	free(vector);
}

void vector_remove_all(Vector* vector){
	vector_free(vector);
	vector -> size = 0;
	vector -> items = NULL;
}

typedef struct Function_Object{
	int value;
	int(*func)(int,int);
}Function_Object;

const int is_bigger_or_equal(int threshold, int number){
	return threshold <= number;
}

Function_Object* Is_Bigger_Or_Equal(int threshold){
	Function_Object* func_object = malloc(sizeof(Function_Object));
	func_object -> value = threshold;
	func_object -> func = &is_bigger_or_equal;
	return func_object;
}

void add_choice_to_choices(Choice* choice, Vector* choices){
	int is_in_vector = 0;
	size_t size = choices -> size;
	for(int i = 0 ; i < size ; i++){
		Choice* index_choice = choices -> items[i];
		if(strcmp(index_choice -> name, choice -> name) == 0){
			index_choice -> point += choice -> point;
			is_in_vector = 1;
			break;
		}
	}
	if(!is_in_vector)
		vector_append(choices,choice);
}

int is_choice_in_choices(Choice* choice, Vector* choices){
	size_t size = choices -> size;
	for(int i = 0 ; i < size ; i++){
		Choice* index_choice = choices -> items[i];
		if(strcmp(index_choice -> name, choice -> name) == 0)
			return i + 1; // ith number (index has issue because of 0 == false)
	}
	return -1;
}
Choice* get_max_choice(const Vector* choices){
	int index = 0;
	int max = ((struct Choice*)(choices -> items[0])) -> point;
	size_t size = choices -> size;
	for(int i = 1; i < size ; i++){
		Choice* index_choice = choices -> items[i];
		if(index_choice -> point > max){
			max = index_choice -> point;
			index = i;
		}
	}
	return choices -> items[index];
}
Vector* filter_choices(Function_Object* func_object, const Vector* vector){
	size_t size = vector -> size;
	int value = func_object -> value;
	Vector* filtered_vector = vector_init(sizeof(vector -> items[0]));
	for(int i = 0 ; i < size ; i++){
		Choice* index_choice = vector -> items[i];
		if(func_object -> func(value, index_choice -> point)){
			Choice* cpy_choice = malloc(sizeof(Choice));
			strcpy(cpy_choice -> name, index_choice -> name);
			cpy_choice -> point = index_choice -> point;
			vector_append(filtered_vector, cpy_choice);
		}
	}
	return filtered_vector;
}

const void print_choices(const Vector* choices){
	size_t size = choices -> size;
	for(int i = 0; i < size; i++){
		Choice* index_choice = choices -> items[i];
		printf("%s : %dp\n", index_choice -> name, index_choice -> point);
	}
}

int main() {
	int number_of_persons = 0;
	const int number_of_choices = 5;
	Vector* choices = vector_init(sizeof(Choice));

	printf("Please enter how many person will eat today \n");
	scanf("%d", &number_of_persons);
	const int threshold = (number_of_choices + 1) / 2 * number_of_persons;

	printf("First Round!!!\n");
	for(int i = 0 ; i < number_of_persons ; i++){
		printf("User %d Please enter your %d choices max = %dp min = %dp (from most preferred to least preferred)\n", i+1, number_of_choices, number_of_choices, 1);
		printf("Use underscore for more than one word meals. Don't use capital letters to avoid confusion.\n");
		for(int j = 0; j < number_of_choices; j++){
			Choice* choice = malloc(sizeof(Choice));
			printf("%d. Choice:\n", j+1);
			scanf("%s", choice -> name);
			choice -> point = number_of_choices - j;
			add_choice_to_choices(choice, choices);
		}
	}

	Vector* filtered_choices;
	Function_Object* func_object = Is_Bigger_Or_Equal(threshold);
	filtered_choices = filter_choices(func_object,choices);

	printf("Lets see shortlist!\n");
	print_choices(filtered_choices);
	if(filtered_choices -> size == 0)
		printf("You are eating at home/dorm today!");
	else if(filtered_choices -> size == 1)
		printf("There is only one choice which above or equal to threshold and %s will be ordered!", ((struct Choice*)filtered_choices -> items[0]) -> name);
	else{
		printf("Second Round!!!\n");
		printf("Now please order your choices from final list:\n");
		size_t size = filtered_choices -> size;
		for(int i = 0; i < size; i++)
			((struct Choice*)filtered_choices -> items[i]) -> point = 0;
		for(int i = 0; i < number_of_persons; i++){
			printf("User %d Please enter your %d choices from short list max = %dp min = %dp (from most preferred to least preferred)\n", i+1, size, size, 1);
			printf("Use underscore for more than one word meals. Don't use capital letters to avoid confusion.\n");
			for(int j = 0 ; j < size ; j++){
				Choice* choice = malloc(sizeof(Choice));
				while(1){
					printf("%d. Choice:\n", j+1);
					scanf("%s", choice -> name);
					choice -> point = size - j;
					if(is_choice_in_choices(choice,filtered_choices) != -1)
						break;
					printf("Please enter a meal from final list\n");
				}
				add_choice_to_choices(choice,filtered_choices);
			}
		}
		printf("Final Results:\n");
		print_choices(filtered_choices);
		Choice* max_choice = get_max_choice(filtered_choices);
		printf("%s will be ordered!!!", max_choice -> name);
	}
	vector_free(choices);
	vector_free(filtered_choices);
	return 0;
}
