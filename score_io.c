
int get_score(char * file_line){
	int line_length=(int) strlen(file_line);
	int score_delimiter=line_length;
	//find the position in the line where the score is
	while(file_line[score_delimiter] !=':')
	{
		score_delimiter--;
	}

	//store score character into a buffer
	char buffer[line_length-score_delimiter];
	for(int i =score_delimiter+1; file_line[i]!='\n';i++)
	{

		buffer[i-(score_delimiter+1)]=file_line[i];
	
	}
	//null terminate buffer so it's a proper string then convert from a string into an int
	buffer[line_length-score_delimiter]='\0';
	return atoi(buffer);
}



//file_line is a string of the form "X level:Y score:Z\n"
//get name places "X ", space included, in return_buffer
void get_name(char * return_buffer,char * file_line){
	int line_length=(int) strlen(file_line);
	int colon_counter=0;
	//count from the end of the line to find username
	int separating_point=line_length;
	//find where the 2nd from the right colon is
	while(colon_counter!=2){
		separating_point--;
		if(file_line[colon_counter]==":"){colon_counter++;}
	}
	//shift left by 5 to skip the word level
	separating_point-=5;

	for(int i=0;i<separating_point;i++){
		return_buffer[i]=file_line[i];
	}
	return_buffer[separating_point]='\0';
}

	


int number_of_lines(FILE * file){
	int lines=0;
	char useless_buffer[400];
	while(fgets(useless_buffer,400,file)!= NULL)
	{
		lines++;
	}
	fseek(file,0,SEEK_SET);
	return lines;
}

int integer_comparison(const void * entry_1, const void * entry_2){
	int a = ((int *) entry_1)[0];
	int b = ((int *) entry_2)[0];
	if(a == b){
		return 0;
	}
	if(a>b){
		return 1;
	}
	if(a<b){
		return -1;
	}
}