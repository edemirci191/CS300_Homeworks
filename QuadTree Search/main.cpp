// Emre Demirci, 26531
// Start Date: 11.05 PM 4 days Before.Deadline
// Finish Date: 12/3/2020

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

string found = ""; // store found cities within the radius
string listing = ""; // store all the visited nodes

class  QuadtreeNode
{
public:
	QuadtreeNode* NW;
	QuadtreeNode* NE;
	QuadtreeNode* SE;
	QuadtreeNode* SW;

	string name_of_city;
	int cor_x;
	int cor_y;

	QuadtreeNode()
	{
		cor_x = 0;
		cor_y = 0;
		name_of_city = "<None>";
		NW = NULL;
		NE = NULL;
		SW = NULL;
		SE = NULL;
	}
	QuadtreeNode(int corx, int cory, string name)
	{
		cor_x = corx;
		cor_y = cory;
		name_of_city = name;
		NW = NULL;
		NE = NULL;
		SW = NULL;
		SE = NULL;
	}
};


class Quadtree
{
public:
	QuadtreeNode* root;
	Quadtree()
	{
		root = NULL;
	}
	bool QuadtreeEmpty()
	{
		if (root == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void pretty_print(QuadtreeNode* root)
	{  // pretty print the quadtree rooted at root: 
		if (root != NULL)
		{   // if the tree is not empty 
			cout << root->name_of_city << endl;
			pretty_print(root->SE); // recursively print the south east subtree 
			pretty_print(root->SW);// recursively print the south west subtree  
			pretty_print(root->NE);// recursively print the north east subtree  
			pretty_print(root->NW);// recursively print the north west subtree  
		}
	}
	void insertNode(QuadtreeNode* new_node)
	{
		if (root == NULL)
		{
			root = new_node;
		}
		else
		{
			QuadtreeNode* temp = root;
			while (temp != NULL)
			{
				if (((new_node->cor_x == temp->cor_x) && (new_node->cor_y == temp->cor_y)) || (new_node->name_of_city == temp->name_of_city))
				{
					if (new_node->name_of_city == temp->name_of_city)
					{
						cout << "This city is already inserted into the map" << endl;
						return;
					}
					else
					{
						cout << "This location is already occupied" << endl;
						return;
					}
				}
				else if (((new_node->cor_x >= temp->cor_x) && (new_node->cor_y < temp->cor_y)) && (temp->SE == NULL))
				{
					temp->SE = new_node;
					break;
				}
				else if ((new_node->cor_x >= temp->cor_x) && (new_node->cor_y < temp->cor_y))
				{
					temp = temp->SE;
				}
				else if (((new_node->cor_x < temp->cor_x) && (new_node->cor_y < temp->cor_y)) && (temp->SW == NULL))
				{
					temp->SW = new_node;
					break;
				}
				else if ((new_node->cor_x < temp->cor_x) && (new_node->cor_y < temp->cor_y))
				{
					temp = temp->SW;
				}
				else if (((new_node->cor_x >= temp->cor_x) && (new_node->cor_y >= temp->cor_y)) && (temp->NE == NULL))
				{
					temp->NE = new_node;
					break;
				}
				else if (((new_node->cor_x >= temp->cor_x) && (new_node->cor_y >= temp->cor_y)))
				{
					temp = temp->NE;
				}
				else if (((new_node->cor_x < temp->cor_x) && (new_node->cor_y >= temp->cor_y)) && (temp->NW == NULL)) // NW
				{
					temp->NW = new_node;
					break;
				}
				else
				{
					temp = temp->NW;
				}
			}
		}
	}


	double find_distance_between_two(int x1, int x2, int y1, int y2) 
	{
		return sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2));
	}

	
	void find_quadrant_between_two_coordinates(int x1, int y1, int x2, int y2, int rad, QuadtreeNode* NN) //finds the direction of the quadrant 2 belongs to from 1
	{ 
		bool quadrant_se_search = false;
		bool quadrant_sw_search = false;
		bool quadrant_ne_search = false;
		bool quadrant_nw_search = false;

		double distance = find_distance_between_two(x1, x2, y1, y2);


		bool NORTH = false;
		bool SOUTH = false;
		bool EAST = false;
		bool WEST = false;

		//checking which quadrants we are touching, in order, SE, SW, NE, NW
		//to go south east use, y - distance

		if ((y2 - rad) <= y1) 
		{ // touching S
			SOUTH = true;
		}
		if ((x2 - rad) <= x1)
		{ // touching W
			WEST = true;
		}
		//cout << "y2 " << y2 << " y1" << y1;
		if ((y2 + rad) >= y1) 
		{ // touching N
			NORTH = true;
		}
		if ((x2 + rad) >= x1) 
		{ // touching E
			EAST = true;
		}

		//now will check if we touch the exact reverse quadrant by the distance measure
		//where are we ?
		bool us_ne = false;
		bool us_nw = false;
		bool us_se = false;
		bool us_sw = false;

		if ((x2 > x1) && (y2 > y1)) 
		{ //ne
			us_ne = true;
			if (find_distance_between_two(x2 - 1, x1, y2 - 1, y1) <= rad)
			{
				quadrant_sw_search = true;
			}
		}
		if ((x2 < x1) && (y2 > y1)) 
		{ //nw
			us_nw = true;
			if (find_distance_between_two(x2 + 1, x1, y2 - 1, y1) <= rad) 
			{
				quadrant_se_search = true;
			}
			
		}
		if ((x2 < x1) && (y2 < y1)) 
		{ //sw
			us_sw = true;
			if (find_distance_between_two(x2 + 1, x1, y2 + 1, y1) <= rad)
			{
				quadrant_ne_search = true;//get N
			}
			
		}
		if ((x2 > x1) && (y2 < y1))
		{ //se
			us_se = true;
			if (find_distance_between_two(x2 - 1, x1, y2 + 1, y1) <= rad)
			{
				quadrant_nw_search = true; //get NW
			}
			
		}

		if ((SOUTH == true && EAST == true) || quadrant_se_search == true)
		{
			if (us_nw && quadrant_se_search == false) 
			{

			}
			else 
			{
				if (NN->SE != NULL) 
				{
					listing += ", ";
					listing += NN->SE->name_of_city; //add to visited list
					if (find_distance_between_two(x2, NN->SE->cor_x, y2, NN->SE->cor_y) <= rad) 
					{
						found += NN->SE->name_of_city;
						found += ", ";
					}
					find_quadrant_between_two_coordinates(NN->SE->cor_x, NN->SE->cor_y, x2, y2, rad, NN->SE);
				}
			}
		}
		if ((SOUTH == true && WEST == true) || quadrant_sw_search == true) 
		{
			if (us_ne && quadrant_sw_search == false)
			{
			}
			else
			{
				if (NN->SW != NULL) 
				{
					listing += ", ";
					listing += NN->SW->name_of_city; //add to visited list
					if (find_distance_between_two(x2, NN->SW->cor_x, y2, NN->SW->cor_y) <= rad) 
					{
						//add to found list
						found += NN->SW->name_of_city;
						found += ", ";
					}
					find_quadrant_between_two_coordinates(NN->SW->cor_x, NN->SW->cor_y, x2, y2, rad, NN->SW);
				}
			}
		}
		if ((NORTH == true && EAST == true) || quadrant_ne_search == true)
		{
			//we are in sw and quadrant_ne_search is false, dont proceed.
			if (us_sw && quadrant_ne_search == false) 
			{				
			}else 
			{
				if (NN->NE != NULL) 
				{
					listing += ", ";
					listing += NN->NE->name_of_city; //add to visited list
					if (find_distance_between_two(x2, NN->NE->cor_x, y2, NN->NE->cor_y) <= rad) 
					{
						//add to found list
						found += NN->NE->name_of_city;
						found += ", ";
					}
					find_quadrant_between_two_coordinates(NN->NE->cor_x, NN->NE->cor_y, x2, y2, rad, NN->NE);
				}
			}
		}
		if ((NORTH == true && WEST == true) || quadrant_nw_search == true) 
		{
			if (us_se && quadrant_nw_search == false) 
			{
				
			}
			else 
			{
				if (NN->NW != NULL) 
				{
					listing += ", ";
					listing += NN->NW->name_of_city; //add to visited list
					if (find_distance_between_two(x2, NN->NW->cor_x, y2, NN->NW->cor_y) <= rad) 
					{
						//add to found list
						found += NN->NW->name_of_city;
						found += ", ";
					}
					find_quadrant_between_two_coordinates(NN->NW->cor_x, NN->NW->cor_y, x2, y2, rad, NN->NW);
				}
			}		
		}
	}


};


bool is_number(const std::string& s) // this function is discovered on stackoverflow.com
{
	return !s.empty() && find_if(s.begin(),
		s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

int find_int_in_str(string str) // this function is found in this website "geeksforgeeks.org/extract-integers_string-c/" and modified
{
	stringstream ss;
	ss << str;
	string temp;
	int found;
	while (!ss.eof()) {
		ss >> temp;
		if (stringstream(temp) >> found)
		{
			return found;
		}
		temp = "";
	}
}

int main()
{

	Quadtree ent;  // Ents are a race of beings in J. R. R. Tolkien's fantasy world Middle-earth who closely resemble trees
	const string file_name1 = "cities.txt";
	const string file_name2 = "queries.txt";
	ifstream file(file_name1);
	if (file)
	{
		string coor_size1;
		string coor_size2;
		string first_line;
		getline(file, first_line);
		stringstream ss(first_line);
		ss >> coor_size1 >> coor_size2;
		//cout << "coor_size1 = " << coor_size1<< endl;
		//cout << "coor_size2 =" << coor_size2 << endl;
		if (is_number(coor_size1) && is_number(coor_size2))
		{
			const int int_coor_size1 = stoi(coor_size1);
			const int int_coor_size2 = stoi(coor_size2);
			string line;
			while (getline(file, line))
			{
				string name;
				string coor_x;
				string coor_y;
				stringstream name_x_y(line);
				name_x_y >> name >> coor_x >> coor_y;
				//cout << "name = " << name << endl; // for debug purposes
				//cout << "coor_x = " << coor_x << endl;
				//cout << "coor_y = " << coor_y << endl;
				//check if coordinates are int and not negative
				if (is_number(coor_x) && is_number(coor_y))
				{
					int int_coor_x = stoi(coor_x);
					int int_coor_y = stoi(coor_y);
					if (((int_coor_x >= 0) && (int_coor_y >= 0)) && ((int_coor_x <= int_coor_size1) && (int_coor_y <= int_coor_size2)))// y ro size2
					{ // if it is executing here format is correct till now
						QuadtreeNode* new_node = new QuadtreeNode();
						new_node->cor_x = int_coor_x;
						new_node->cor_y = int_coor_y;
						new_node->name_of_city = name;
						ent.insertNode(new_node);
					}
				}
				else
				{
					cout << "Text file contains non-integers as coordinates" << endl;
					cout << "which are" << coor_x << "or/and" << coor_y << endl;
				}
				//finish checking validity for coordinates
			}
			file.close();
			// if it is here it must finish creating the tree
			// so try to print the tree with pretty print
			ent.pretty_print(ent.root);
			cout << endl;
			// then given the queries txt file first check it again to see if its in correct shape
			ifstream file2(file_name2);
			if (file2)
			{
				int int_x_coor = 0;
				int int_y_coor = 0;
				int int_rad = 0;
				string line;
				//int sum = 0;
				while (getline(file2, line))
				{

					string x_coor;
					string y_coor;
					string rad;
					stringstream x_y_rad(line);
					x_y_rad >> x_coor >> y_coor >> rad;
					int_x_coor = find_int_in_str(x_coor);
					int_y_coor = find_int_in_str(y_coor);

					if (is_number(rad))
					{
						int_rad = stoi(rad);
						listing += ent.root->name_of_city;

						if(int_rad >= ent.find_distance_between_two(ent.root->cor_x,int_x_coor, ent.root->cor_y, int_y_coor))
						{
							found += ent.root->name_of_city;
							found += ", ";
						}

						//cout << "x " << int_x_coor << " y " << int_y_coor << "rad " << int_rad << endl;
						ent.find_quadrant_between_two_coordinates(ent.root->cor_x, ent.root->cor_y, int_x_coor, int_y_coor, int_rad, ent.root);
						if (found == "") 
						{
							cout << "<None>" << endl;
						}
						else 
						{
							found.pop_back();
							found.pop_back(); //deleting extra comma

							cout << found << endl;
						}
						cout << "" << listing << endl << endl;
						found.clear();
						listing.clear();
						
					}
					else
					{
						cout << "Rad is not a number " << endl;
					}
				}
				//cout << sum;
			}
			else
			{
				cout << "Can not open queries.txt file or maybe the file is empty " << endl;
			}
		}
		else
		{
			cout << "Size values should be integers only";
		}
	}
	else
	{
		cout << "Can not open the cities.txt file or maybe the file is empty" << endl;
	}
	getchar();
	//cout << "NW of Ent " << ent->NW << endl;
	
	return 0;
}