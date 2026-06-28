#include <iostream>	
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

//Stores the information for a single course
struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;

};

class HashTable {
private:

	//The size of the hash table
	static const int tableSize = 179;

	//The hash table being implemented by chaining
	vector<Course> table[tableSize];

	//Generates a hash value for a given course number
	int hashFunction(string courseNumber) {
		int sum = 0;

		for (char ch : courseNumber) {
			sum += ch;
		}
		return sum % tableSize;
	}

public:

	//Inserting a course into the hash table
	void insertCourse(Course course) {
		int index = hashFunction(course.courseNumber);
		table[index].push_back(course);

	}

	//Search a course by course number 
	Course* searchCourse(string courseNumber) {
		int index = hashFunction(courseNumber);

		for (int i = 0; i < table[index].size(); i++) {
			if (table[index][i].courseNumber == courseNumber) {
				return &table[index][i];
			}
		}

		return nullptr;

	}

	//Returns all courses
	vector<Course> getAllCourses() {
		vector<Course> allCourses;

		for (int i = 0; i < tableSize; i++) {
			for (Course course : table[i]) {
				allCourses.push_back(course);

			}
		}

		return allCourses;

	}

	//Removes data from the hash table 
	void clearTable() {
		for (int i = 0; i < tableSize; i++) {
			table[i].clear();

		}
	}

};

//Reads course data from the file and loads it in the table
bool loadCourses(string fileName, HashTable& courses) {
	ifstream file(fileName);

	//verify file opened correctly
	if (!file.is_open()) {
		cout << "Error: file could not open." << endl;
		return false;
	}

	courses.clearTable();
	string line;

	//Processing the lines in the input file 
	while (getline(file, line)) {
		stringstream ss(line);
		string item;
		vector<string> tokens;

		//split the lines with commas
		while (getline(ss, item, ',')) {
			tokens.push_back(item);

		}

		//Validating course record format
		if (tokens.size() < 2) {
			cout << "Error detected: course line skipped." << endl;
			continue;

		}

		Course course;

		//Storing the course number and title 
		course.courseNumber = tokens[0];
		course.courseTitle = tokens[1];

		//storing the prerequistes if they are needed
		for (int i = 2; i < tokens.size(); i++) {
			course.prerequisites.push_back(tokens[i]);

		}
		courses.insertCourse(course);

	}
	file.close();
	cout << " Course data has been loaded successfully." << endl;
	return true;

}

//displaying all the coutses in alphanumeric order
void printCourseList(HashTable& courses) {
	vector<Course> allCourses = courses.getAllCourses();

	//sorts courses by their course number 
	sort(allCourses.begin(), allCourses.end(), [](Course a, Course b) {
		return a.courseNumber < b.courseNumber;

		});
	cout << endl;
	cout << " Here is the course schedule" << endl;
	cout << endl;

	// pritning each course
	for (Course course : allCourses) {
		cout << course.courseNumber << ","
			<< course.courseTitle << endl;
	}

}

//showing info for a course
void printCourseInformation(HashTable& courses) {
	string courseNumber;
	cout << "What course are you intrested in?";
	cin >> courseNumber;

	transform(courseNumber.begin(),
		courseNumber.end(),
		courseNumber.begin(),
		::toupper);

	Course* course = courses.searchCourse(courseNumber);

	//confirming that the course is exiting
	if (course == nullptr) {
		cout << "No course found." << endl;
		return;

	}

	cout << endl;
	cout << course->courseNumber
		<< ","
		<< course->courseTitle << endl;

	cout << "Prerequisites:";
	if (course->prerequisites.empty()) {
		cout << " no prerequisites";

	}

	else {
		//printing the prereq's course number and title
		for (int i = 0; i < course->prerequisites.size(); i++) {
			Course* prereq =
				courses.searchCourse(course->prerequisites[i]);
			if (prereq != nullptr) {
				cout << prereq->courseNumber
					<< ","
					<< prereq->courseTitle;
			}

			else {
				cout << course->prerequisites[i];

			}

			if (i < course->prerequisites.size() - 1) {
				cout << ",";

			}
		}
	}

	cout << endl;

}

int main() {
	//creating the hash table in order to store the course data
	HashTable courses;

	string fileName;
	int choice = 0;
	bool dataLoaded = false;

	cout << "Welcome to Course viewer." << endl;

	//The menu until the user decides to exit the app
	while (choice != 9) {
		cout << endl;
		cout << "1: Load the file data" << endl;
		cout << "2: Print the list of courses" << endl;
		cout << "3: Print individual course" << endl;
		cout << "9: Exit" << endl;
		cout << endl;
		cout << " Which option would you like to execute?";

		cin >> choice;

		switch (choice) {

		case 1:

			//Loads the course file into the hash table

			dataLoaded = loadCourses("CS 300 ABCU_Advising_Program_Input.CSV", courses);
			
			break;

		case 2:

			//Displaying all of the courses in alphanumeric order
			if (!dataLoaded) {
				cout << "Error detected must load course data first." << endl;

			}
			else {
				printCourseList(courses);

			}
			break;

		case 3:
			//Searching for a speific course to display
			if (!dataLoaded) {
				cout << "Error detected must load course data first." << endl;

			}
			else {
				printCourseInformation(courses);

			}
			break;

		case 9:
			//Exiting the program
			cout << "Thank you for using the course viewer." << endl;
			break;

		default:

			//If an invalid option is selected
			cout << choice
				<< " Selected option is not valid, please choose a valid option."
				<< endl;
			break;
		}
	}

	return 0; 
}

