#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct letter_node {
	struct letter_node * parent; 
	struct letter_node ** letter_node_array;
	bool is_end_of_word;
} letter_node;


void error(char * message)
{
	printf("\nError: %s\n", message);
	exit(1);
}

bool is_word(string line, int left_marker, int right_marker, struct letter_node * root)
{
	string subline = line.substr(left_marker, right_marker - left_marker);
	struct letter_node * current_node = root;
	char c;
	for(int i = 0; i < (int) subline.length(); i++)
	{
		c = tolower(subline[i]);
		if(*(current_node->letter_node_array + c - 'a') == NULL)
			return false;
		current_node = *(current_node->letter_node_array + c - 'a');
	}
	if(current_node->is_end_of_word)
		return true;
	return false;

}

vector<int> * construct_space_array(string line, struct letter_node * root)
{
	vector<int> * space_array = new vector<int>();
	bool loop_cont;
	for (int j = 1; j <= (int)line.length(); j++)
	{
		loop_cont = true;
		for(int i = j - 1; i > 0  && loop_cont == true; i --)
		{
			if(space_array->at(i-1) != -1 && is_word(line, i, j, root))
			{
				space_array->push_back(i);
				loop_cont = false;
			}
		}
		if(loop_cont)	//didn't find a match
		{
			if(is_word(line, 0, j, root))
				space_array->push_back(-2); //we need a special value to take care of the case where our 
			//first letter is actually a whole word... and -1 was already in use XD
			else
				space_array->push_back(-1);
		}
}

return space_array;
}
struct array_entry {
int min_value;
int left_word_index;
} array_entry;
int cube (int number)
{
return number * number * number;
}
struct array_entry * construct_line_vector(vector<int> word_length, int line_length)
//construct_line_vector takes a vector of ints indicating the sizes of each word, and a max line length,
//and it returns an array of array_entries.  Each "array_entry" is a descriptor of an output line, 
//telling us the leftmost word index, the rightmost word index, and the total cost of the line
{
	struct array_entry * return_vector = (struct array_entry *) malloc(sizeof(struct array_entry) * (word_length.size() + 1));
	//maximum number of lines is one per word 

	for(int i = 0; i <= word_length.size() + 1; i++)
	{
		(return_vector + i)->left_word_index = -1;
		(return_vector + i)->min_value = 400000000; // very large int value;
	}//initiallize array to NULL
	int current_line_size;
	return_vector->min_value = 0; //we can output a blank line for free... kind of.  
	//Not really, but it makes our algorithm work XD(line_length - word_length.at(0))^3;
	
	return_vector->left_word_index = 0;

	for(int i = 1; i <= word_length.size(); i++) //construct array
	{
		current_line_size = word_length.at(i - 1);
		for(int j = 0; current_line_size <= line_length; j++)
		{	
			if((return_vector + i - j -1)->min_value + cube(line_length - current_line_size) < (return_vector + i)->min_value)
				{
					(return_vector + i)->min_value = (return_vector + i - j - 1)->min_value + cube(line_length - current_line_size);
					(return_vector + i)->left_word_index = i - j - 1;
				}
			
			if(i - j - 1 == 0)
				break;
		current_line_size += (1 + word_length.at(i - j - 2));
		}
	}
	

	return return_vector;
}
void output_space_array(vector<int> * space_array, int line_length, string line, bool debug)
{
	if(debug)
		cout << endl << endl << "---------------------------" << endl << "printing line: " << endl << endl;
	if(space_array->at(space_array->size() - 1) == -1)
		error("unparseable line for the given dictionary");
	vector<int> temp;
	for(int j = space_array->size() - 1; j >= 0 && j != -2; ) //-2 is special value for line beginning with 1 character word
	{
		if(j == -2)
		{
			temp.push_back(0);
			j = -1;
			break;
		}
		temp.push_back(j);
		j = space_array->at(j) - 1;
	}
	vector<int> word_lengths;
	word_lengths.push_back(temp.at(temp.size() - 1) + 1);
	for(int j = temp.size() - 2; j >= 0; j--)
		word_lengths.push_back(temp.at(j) - temp.at(j+1));
	
	if(debug)
	{		cout << "word length array:" << endl;
		for(int j = 0; j < word_lengths.size(); j++)
			cout << word_lengths.at(j) << " ";
		cout << endl;
	}

	string sentence;
	struct array_entry * spacing_by_line = construct_line_vector(word_lengths, line_length);
	int k = 0;
	if(debug)
	{	
		cout << endl << "printing sentence:" << endl;
		for(int i = temp.size() - 1; i >= 0; i--)
		{	sentence += line.substr(k, temp.at(i) + 1 - k);
			sentence += " ";
			k = temp.at(i) + 1;
		}
		cout << sentence << endl << endl << "printing spacing by line array:" << endl;
	}
	vector<int> paragraph;
	int counter;
	if(debug)
		for(counter = 1; counter <= word_lengths.size(); counter++)
		{
			cout << "min value: " << (spacing_by_line + counter)->min_value << "\tleft word index: " << (spacing_by_line + counter)->left_word_index << endl;
		}

	counter = word_lengths.size();
	int last = counter;//(spacing_by_line + i)->left_word_index;
	int starting_point = last;
	int starting_point_offset = 0;
	for(; starting_point_offset <= line_length; starting_point --)	//so we don't count the last line
		{
			if((spacing_by_line + starting_point)->min_value < (spacing_by_line + last)->min_value)
				last = starting_point;
			if(starting_point == 0)
				break;
			starting_point_offset += (1 + word_lengths.at(starting_point -1));
		}
	if(debug)
		cout << endl << "last index: " << last << endl;
	paragraph.push_back(word_lengths.size());
	if(last == 0) //special case where line fits onto one output line
		paragraph.push_back(0);
	for(; last != 0; last = (spacing_by_line + last)->left_word_index)
		paragraph.push_back(last);
	if(debug)
		cout << endl << "printing adjusted paragraph: " << endl;
	int i = 1;
	string final_output;
	k = 0;
	for(counter = paragraph.size() -1; counter >= 0; counter --)
	{
		for(; i <= paragraph.at(counter); i++)
			{
				final_output += line.substr(k, word_lengths.at(i - 1));
				final_output += " ";
				k += (word_lengths.at(i -1));
			}
		final_output = final_output.substr(0, final_output.size() - 1);
		if(i != 1)	//if i == 1, special case where input line fits onto one output line
			final_output += "\n";
	}

	cout << final_output << (spacing_by_line + paragraph.at(1))->min_value << endl;
	return;
}
struct letter_node * create_new_node(struct letter_node * parent)
{
	struct letter_node * return_node = (struct letter_node *) malloc(sizeof(struct letter_node));
	return_node->parent = parent;
	return_node->is_end_of_word = false;
	return_node->letter_node_array = (struct letter_node **) malloc(26 * sizeof(struct letter_node *));
	for(int k = 0; k < 26; k++)
		*(return_node->letter_node_array + k) = NULL;
	return return_node;
}
void print_node_tree(struct letter_node * root, string current)
{
	if(root->is_end_of_word)
		{
			cout << current << endl;
		}
	for(int i = 0; i< 26; i++)
	{	
		if(*(root->letter_node_array + i) == NULL)
			;
		else
		{
			current += (char) i + 'a';		
			print_node_tree(*(root->letter_node_array + i), current);
			current.erase(current.length() - 1, 1);
		}
	}
}
struct letter_node * construct_dictionary(char * file, bool debug)
{
	
	ifstream dictionary;
	string line;
	struct letter_node * root = create_new_node(NULL);
	//to conserve memory, letter_node_array is an array of pointers to letter nodes.
	struct letter_node * current_node = root;
	dictionary.open(file, ios_base::in);
	if(!dictionary.is_open())
		error("Dictionary file doesn't exist");
	getline(dictionary, line);
	while(!dictionary.eof())
	{
	if(debug)
		cout << "got word " << line << endl;
	for(int i = 0; i < (int)line.length(); i++)
		{
			char c = tolower(line[i]);
			if(*(current_node->letter_node_array + c - 'a') == NULL)	
			{
				*(current_node->letter_node_array + c - 'a') = create_new_node(current_node);
				current_node = *(current_node->letter_node_array + c - 'a');
			}
			else
			{
				current_node = *(current_node->letter_node_array + c - 'a');
			}
		}
	current_node->is_end_of_word = true;
	getline(dictionary, line);
	current_node = root;
	}
	dictionary.close();
	return root;
}
int main(int argc, char *argv[])
{
	bool debug = false;
	if (argc != 4)
		{
		printf("\nWrong number of arguments- expected \"./Printing dictionary inputfile linenumber\"");
		exit(0);
		}
	ifstream infile;
	infile.open(argv[2], ios_base::in);
	if(!infile.is_open())
		printf("Cannot open input file");
	int MAX_LINE_SIZE = atoi(argv[3]);

	if(debug)
		cout<< "constructing dictionary node tree:" << endl << endl;
	struct letter_node * root_node = construct_dictionary(argv[1], debug);
	
	if(debug)
		cout<< "printing dictionary node tree:" << endl << endl;
	
	if(debug)
		print_node_tree(root_node, "");
	
	if(debug)
		cout << endl;

	string line;
	getline (infile, line);
	do
	{	
	vector<int> * space_array = construct_space_array(line, root_node);
	output_space_array(space_array, MAX_LINE_SIZE, line, debug);
	delete space_array;
	getline (infile, line);
	}while (! infile.eof() );
	delete root_node;
	infile.close();

}
		