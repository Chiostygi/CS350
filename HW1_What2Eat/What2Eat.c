#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Choice{
	int point;
	char name[200];
};

struct Tracker{
	int is_true;
	int index;
};

const struct Tracker is_choice_in_array(const struct Choice choice,const struct Choice choices[],const int size){
	struct Tracker t;
	for(int i=0; i<size; i++){
		if(strlen(choices[i].name) == 0){
			t.is_true = 0;
			t.index = i;
			return t;
		}
		if(strcmp(choices[i].name, choice.name) == 0){
			t.is_true = 1;
			t.index = i;
			return t;
		}
	}
	t.is_true = 0;
	t.index = size - 1;
	return t;
}

void add_choice_to_array(const struct Choice choice, struct Choice choices[], const int size){
	struct Tracker t = is_choice_in_array(choice,choices,size);
	if(t.is_true){
		choices[t.index].point += choice.point;
	}
	else{
		strcpy(choices[t.index].name, choice.name);
		choices[t.index].point = choice.point;
	}
}

const struct Choice get_max_choice(const struct Choice choices[],const int size){
	int index = 0;
	int max = choices[0].point;
	for(int i=1; i<size;i++){
		if(choices[i].point > max){
			max = choices[i].point;
			index = i;
		}
	}
	return choices[index];
}
const void print_choices(const struct Choice choices[],const int size){
	for(int i=0; i<size; i++){
		printf("%s:%dp\n",choices[i].name,choices[i].point);
	}
}
int main() {
	int number_of_persons = 0;
	const int number_of_choices = 5;
	const int threshold = 10;

	printf("Please enter how many person will eat today \n");
	scanf("%d", &number_of_persons);

	int size = number_of_persons * number_of_choices;
	struct Choice choices[size];
	for(int i=0; i<size; i++){
		choices[i].point = 0;
	}

	printf("First Round!!!\n");
	for(int i=0; i<number_of_persons; i++){
		printf("User %d Please enter your %d choices max = %dp min = %dp (from most preferred to least preferred)\n",i+1,number_of_choices,number_of_choices,1);
		printf("Use underscore for more than one word meals. Don't use capital letters to avoid confusion.\n");
		for(int j = 0; j < number_of_choices; j++){
			struct Choice c;
			printf("%d. Choice:\n",j+1);
			scanf("%s",&c.name);
			c.point = number_of_choices - j;
			add_choice_to_array(c,choices,size);
		}
	}

	int tracker = 0;
	struct Choice filtered_choices[size];
	for(int i=0; i<size; i++){
		filtered_choices[i].point = 0;
	}
	for(int i=0; i<size; i++){
		if(choices[i].point >= threshold){
			filtered_choices[tracker].point = choices[i].point;
			strcpy(filtered_choices[tracker].name,choices[i].name);
			tracker ++;
		}
	}

	printf("Lets see shortlist!\n");
	print_choices(filtered_choices,tracker);
	if(filtered_choices[0].point == 0)
		printf("You are eating at home/dorm today!");
	else if(filtered_choices[1].point == 0)
		printf("There is only one choice which above or equal to threshold and %s will be ordered!",filtered_choices[0].name);
	else{
		printf("Second Round!!!\n");
		printf("Now please order your choices from final list:\n");
		for(int i=0; i<size; i++){
			filtered_choices[i].point = 0;
		}
		for(int i=0; i<number_of_persons; i++){
			printf("User %d Please enter your %d choices from short list max = %dp min = %dp (from most preferred to least preferred)\n",i+1,tracker,tracker,1);
			printf("Use underscore for more than one word meals. Don't use capital letters to avoid confusion.\n");
			for(int j=0; j<tracker; j++){
				struct Choice c;
				while(1){
					printf("%d. Choice:\n",j+1);
					scanf("%s",c.name);
					c.point = tracker - j;
					if(is_choice_in_array(c,filtered_choices,tracker).is_true == 1)
						break;
					printf("Please enter a meal from final list\n");
				}
				add_choice_to_array(c,filtered_choices,tracker);
			}
		}
		printf("Final Results:\n");
		print_choices(filtered_choices,tracker);
		printf("%s will be ordered!!!",get_max_choice(filtered_choices,tracker).name);
	}
	return 0;
}
