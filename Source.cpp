/**************************************************************/
/*  Algorithms and Data Structures 2                          */
/*  Assignment 1                                              */
/**************************************************************/

//Definitions
#define _CRT_SECURE_NO_WARNINGS 1
#define bool int
#define false 0
#define true (!false)

//Libraries
#include <stdio.h>
#include <stdlib.h>

//Preprocessor Variable
#define SIZE 5

//Stucture template for cars
struct cars {
	char carReg[8];
	char make[20];
	char model[20];
	char colour[20];
	int prevOwners;
	bool reserved;
	double reserveAmount;
};

//Stucture template for one node
struct LinearNode {
	struct cars* element;
	struct LinearNode* next;
};

//Function prototypes
void menu();                    //display menu
void addCar();                  //add a new car to the showroom
void sellACar(aReg);            //sell a car from the showroom
void reserveACar(aReg);         //reserve a car in the showroom
void unreserveACar(aReg);       //unreserve a car in the showroom
void viewCars();                //view all cars in the showroom
void viewSpecificCar(aReg);     //view a specific car in the showroom
void viewSales();               //view the cars sold and the balance of the business
bool isEmpty();                 //method checking if the list is empty
bool isUnique(carReg);          //boolean method checking if the car registration is unique
void saveToFile(FILE* fp);      //save to file
void getFromFile(FILE* fp);     //retrieve from file 

//Global Variables
struct LinearNode* list = NULL; //front of list
struct LinearNode* last = NULL; //pointer to last node in list
int count;                      //count of cars
int carsSold;                   //number of cars sold
double balance;                 //total balance of the business

/******************** Main Method ********************/
int main() {
	FILE* fp;
	int i;

	if ((fp = fopen("car.dat", "rb")) == NULL) {
		//file DOES NOT exist so get user to input students
		printf("\nFile does not exist\n");
		menu();
	}//end if
	else {//file DOES exist
		getFromFile(fp);
		menu();
	}
	saveToFile(fp);
	getchar();
	getchar();

} //end main

void menu(){
	//local variables
	int menuOption;
	char aReg[8];

	//do the following while the user doesnt choose menu option 8
	do {
		//display the menu
		printf("\n~~~~~~~~~~~~~~~~~~~ Menu ~~~~~~~~~~~~~~~~~~~\n");
		printf("1.    Add a new car to the showroom\n");
		printf("2.    Sell a car from the showroom\n");
		printf("3.    Reserve a car in the showroom\n");
		printf("4.    Unreserve a car in the showroom\n");
		printf("5.    View all cars in the showroom\n");
		printf("6.    View a specific car in the showroom\n");
		printf("7.    View sales\n");
		printf("8.    Exit the system\n");
		printf("Menu option : ");
		scanf("%d", &menuOption);

		//switch statement for the menu options
		switch (menuOption) {
		case 1:
			addCar();
			break;
		case 2:
			printf("\nEnter the registration number of the car you want to sell : ");
			scanf("%s", aReg);
			sellACar(aReg);
			break;
		case 3:
			printf("\nEnter the registration number of the car you want to reserve : ");
			scanf("%s", aReg);
			reserveACar(aReg);
			break;
		case 4:
			printf("\nEnter the registration number of the car you want to unreserve : ");
			scanf("%s", aReg);
			unreserveACar(aReg);
			break;
		case 5:
			viewCars();
			break;
		case 6:
			printf("\nEnter the registration number of the car you want to view : ");
			scanf("%s", aReg);
			viewSpecificCar(aReg);
			break;
		case 7:
			viewSales();
			break;
		case 8:
			printf("\nGood bye!\n");
			break;
		default:
			printf("\nError - not a choice!\n");
		} //end switch
	} while (menuOption != 8); //end while
}//end menu

/******************** Add a car method ********************/
void addCar() {
	//if the count of cars is less than 5 continue taking in information about cars
	if (count < 5) {
		//local variables
		count++;
		char carReg[8];
		char make[20];
		char model[20];
		char colour[20];
		int prevOwners;
		struct LinearNode* aNode;
		struct cars* anElement;

		//get user to input details of a car
		printf("\nEnter the car registration number in the form yyDnnnn : ");
		scanf("%s", carReg);

		//check if the car registration entered is unique
		if (isUnique(carReg) == false) {
			printf("\nError - this car registration already exists!\n");
			do {
				printf("\nTry again! ");
				scanf("%s", carReg);
			} while (isUnique(carReg) == false); //end do while
		} //end if

		printf("Enter car make : ");
		scanf("%s", make);
		printf("Enter car model : ");
		scanf("%s", model);
		printf("Enter colour : ");
		scanf("%s", colour);
		printf("How many previous owners are there : ");
		scanf("%d", &prevOwners);

		//if previous owners are more than 3 display error message
		if (prevOwners > 3) {
			printf("\nError - a car cannot have more than 3 owners!\n");
			do {
				printf("\nTry again! ");
				scanf("%d", &prevOwners);
			} while (prevOwners > 3); //end do while
		} //end if

		//create space for new car 
		anElement = (struct cars*)malloc(sizeof(struct cars));

		//create space for new node
		aNode = (struct LinearNode*)malloc(sizeof(struct LinearNode));

		//add the information of the local variables of a car to the node variables
		strcpy(anElement->carReg, carReg);
		strcpy(anElement->make, make);
		strcpy(anElement->model, model);
		strcpy(anElement->colour, colour);
		anElement->prevOwners = prevOwners;
		anElement->reserved = false;
		anElement->reserveAmount = 0;

		//if the node is null display error meesage else add to the node
		if (aNode == NULL)
			printf("\nError - no space for the new node!\n");
		else {
			aNode->next = NULL;
			aNode->element = anElement;

			//add node to end of the list
			if (isEmpty()) {
				list = aNode;
				last = aNode;
			} //end if
			else {
				last->next = aNode;
				last = aNode;
			} //end else
		} //end else
	} //end if
	else {
		printf("\nError - the list has reached its maximum capacity!\n");
	} //end else
} //end addCar

/******************** Method checking if the car registration is unique ********************/
bool isUnique(carReg) {
	//local variables
	struct LinearNode* current, * previous;
	bool notFound = true;
	bool unique = true;
	current = previous = list;

	//while its not found and the current node is not null check if the car registration inputed matches any in the linked list
	while (notFound && current != NULL) {
		if (strcmp(carReg, current->element->carReg) == 0)
			notFound = false;
		else {
			previous = current;
			current = current->next;
		} //end else
	} //end while

	//if its not found then its unique else its not unique
	if (notFound)
		unique = true;
	else
		unique = false;

	return unique;
} //end isUnique

/******************** Sell a car method ********************/
void sellACar(aReg) {
	//local variables
	struct LinearNode* current, * previous;
	bool notFound = true;
	double amount;


	//if the list is empty print message else find the car registration and check if its reserved
	if (isEmpty()) 
		printf("\nError - there are no nodes in the list!\n");
	else {
		current = previous = list;
		while (notFound && current != NULL) {
			if (strcmp(aReg, current->element->carReg) == 0 && current->element->reserved == true) {
				notFound = false;
				balance = balance + current->element->reserveAmount;
			} //end if
			else {
				previous = current;
				current = current->next;
			} //end else
		} //end while

		//if it was not found display message else complete the sale and delete the node
		if (notFound)
			printf("\nError - there is not such node with value %s OR the car you are trying to sell has not been reserved!\n", aReg);
		else {
			if (current == list) {
				list = list->next;
				free(current);
			} //end else
			else {
				previous->next = current->next;
				free(current);
			} //end else
			printf("\nAmount received on the car besides deposit : ");
			scanf("%lf", &amount);
			balance = balance + amount;
			printf("\nNode with value %s has been sold\n", aReg);
			count--;
			carsSold++;
		}//end else
	}//end else
}// end sellACar

/******************** View cars method ********************/
void viewCars() {
	//local variables
	struct LinearNode* current, * previous;
	bool notFound = true;
	int viewOption;
	char viewMake[20];
	char viewModel[20];
	char viewColour[20];

	//display a menu with options of the way cars would like to be viewed
	printf("\nChoose one of the following options\n");
	printf("1. View all cars\n");
	printf("2. View all cars of a specific make\n");
	printf("3. View all cars of a specific model\n");
	printf("4. View all cars of a specific colour\n");
	printf("5. View all cars that have been reserved\n");
	printf("6. View all cars that have not been reserved\n");
	printf("Menu option : ");
	scanf("%d", &viewOption);

	//if the options require specification ask the user to input what they would like to view
	if (viewOption == 2) {
		printf("\nEnter the make you want to view : ");
		scanf("%s", viewMake);

	}
	else if (viewOption == 3) {
		printf("\nEnter the model you want to view : ");
		scanf("%s", viewModel);
	}
	else if (viewOption == 4) {
		printf("\nEnter the colour you want to view : ");
		scanf("%s", viewColour);
	} //end if

	//switch statement for the viewing options
	switch (viewOption) {
	case 1:
		//if the list is empty display message else display the information
		if (isEmpty())
			printf("\nError - there are no cars in this list!\n");
		else {
			current = list;
			while (current != NULL) {
				printf("\n\nRegistration : %s", current->element->carReg);
				printf("\nMake : %s", current->element->make);
				printf("\nModel : %s", current->element->model);
				printf("\nColour : %s", current->element->colour);
				printf("\nNumber of previous owners : %d", current->element->prevOwners);
				if (current->element->reserved == true) {
					printf("\nReserved : Yes");
				}
				else if (current->element->reserved == false) {
					printf("\nReserved : No");
				}
				printf("\nReserve amount : %.2f\n", current->element->reserveAmount);
				current = current->next;
			} //end while
		}//end else
		break;
	case 2: case 3: case 4: case 5: case 6:
		current = previous = list;

		//for every node in the linked list check if it matches the users option
		for (int i = 0; i < count; i++) {
			notFound = true;
			if (viewOption == 2 && strcmp(viewMake, current->element->make) == 0)
				notFound = false;
			else if (viewOption == 3 && strcmp(viewModel, current->element->model) == 0)
				notFound = false;
			else if (viewOption == 4 && strcmp(viewColour, current->element->colour) == 0)
				notFound = false;
			else if (viewOption == 5 && current->element->reserved == true)
				notFound = false;
			else if (viewOption == 6 && current->element->reserved == false)
				notFound = false;
			else {
				previous = current;
			} //end else

			//if its not found then display message else display the information
			if (notFound)
				printf("\nError - there is not such node!\n");
			else {
				printf("\n\nRegistration : %s", current->element->carReg);
				printf("\nMake : %s", current->element->make);
				printf("\nModel : %s", current->element->model);
				printf("\nColour : %s", current->element->colour);
				printf("\nNumber of previous owners : %d", current->element->prevOwners);

				if (current->element->reserved == true) {
					printf("\nReserved : Yes");
				}
				else if (current->element->reserved == false) {
					printf("\nReserved : No");
				}
				printf("\nReserve amount : %.2f\n", current->element->reserveAmount);

			} //end else
			current = current->next;
		} //end for
		break;
	default:
		printf("\nError - not a choice!\n");
	}//end switch	
} //end viewCars

/******************** Reserve a car method ********************/
void reserveACar(aReg){
	//local variables
	double reserveAmount;
	struct LinearNode* current, * previous;
	bool notFound = true;

	//if the list is empty display message else check if it matches the registration the user inputed
	if (isEmpty())
		printf("\nError - there are no nodes in the list!\n");
	else {
		current = previous = list;

		while (notFound && current != NULL) {
			if (strcmp(aReg, current->element->carReg) == 0 && current->element->reserved == false)
				notFound = false;
			else {
				previous = current;
				current = current->next;
			} //end else
		} //end while

		//if its not found display message else ask user for the reserve amount and set reserved to true
		if (notFound)
			printf("\nError - there is not such node with value %s!\n", aReg);
		else {
			current->element->reserved = true;
			printf("\nPlease input the deposit amount : ");
			scanf("%lf", &reserveAmount);

			//chek its between 500 and 1500
			if (reserveAmount < 500 || reserveAmount >1500) {
				printf("\nError - must be between 500 and 1500!\n");
				do {
					printf("\nTry again! ");
					scanf("%lf", &reserveAmount);
				} while (reserveAmount < 500 || reserveAmount >1500); //end do while
			} //end if

			current->element->reserveAmount = reserveAmount;
		}//end else
	}//end else
} //end reserveACar

/******************** Unreserve a car method ********************/
void unreserveACar(aReg){
	//local variables
	double reserveAmount;
	struct LinearNode* current, * previous;
	bool notFound = true;

	//if the list is empty display message else check that the inputed value matches
	if (isEmpty())
		printf("\nError - there are no nodes in the list!\n");
	else {
		current = previous = list;

		while (notFound && current != NULL) {
			if (strcmp(aReg, current->element->carReg) == 0 && current->element->reserved == true)
				notFound = false;
			else {
				previous = current;
				current = current->next;
			} //end else
		} //end while

		//if its not found display message else set reserved to false and amount to 0
		if (notFound)
			printf("\nError - there is not such node with value %s!\n", aReg);
		else {
			current->element->reserved = false;
			current->element->reserveAmount = 0;
		} //end else
	} //end else
}//end unreserveACar

/******************** View a specific car method ********************/
void viewSpecificCar(aReg){
	//local variables
	struct LinearNode* current, * previous;
	bool notFound = true;

	if (isEmpty())
		printf("\nError - there are no nodes in the list!\n");
	else {
		current = previous = list;

		while (notFound && current != NULL) {
			if (strcmp(aReg, current->element->carReg) == 0)
				notFound = false;
			else {
				previous = current;
				current = current->next;
			}//end else
		} //end while

		if (notFound)
			printf("\nError - there is not such node with value %s!\n", aReg);
		else {
			printf("\nRegistration : %s", current->element->carReg);
			printf("\nMake : %s", current->element->make);
			printf("\nModel : %s", current->element->model);
			printf("\nColour : %s", current->element->colour);
			printf("\nNumber of previous owners : %d", current->element->prevOwners);
			if (current->element->reserved == true)
			{
				printf("\nReserved : Yes");
			}
			else if (current->element->reserved == false)
			{
				printf("\nReserved : No");
			}
			printf("\nReserve amount : %.2f\n", current->element->reserveAmount);
		}//end else
	}//end else
}//end viewSpecificCar

/******************** View sales method********************/
void viewSales(){
	printf("\nNumber of cars sold : %d", carsSold);
	printf("\nTotal balance of the business : %lf\n", balance);
}

/******************** Is empty function ********************/
bool isEmpty() {
	if (list == NULL)
		return true;
	else
		return false;
}

/******************** Save to file method ********************/
void saveToFile(FILE* fp) {
	struct LinearNode* current = list;

	//open the file
	fp = fopen("car.dat", "wb");
	printf("\nSaving cars to the file ....\n");

	//while the current node is not null write the cars to the file
	while (current != NULL)
	{
		fwrite(current->element->carReg, 1, strlen(current->element->carReg) + 1, fp);
		fwrite(current->element->make, 1, strlen(current->element->make) + 1, fp);
		fwrite(current->element->model, 1, strlen(current->element->model) + 1, fp);
		fwrite(current->element->colour, 1, strlen(current->element->colour) + 1, fp);
		fwrite(&current->element->prevOwners, 1, sizeof(current->element->prevOwners), fp);
		fwrite(&current->element->reserved, 1, sizeof(current->element->reserved), fp);
		fwrite(&current->element->reserveAmount, 1, sizeof(current->element->reserveAmount), fp);

		current = current->next;
	}
	//close the file
	fclose(fp);
}//end saveToFile

/******************** Retrieving from file method ********************/
void getFromFile(FILE* fp) {
	struct cars* anElement;
	struct LinearNode* aNode;

	printf("Retriving cars from file...\n");

	//call malloc to create anElement
	anElement = (struct cars*)malloc(sizeof(struct cars));

	//call malloc to create a linear node
	aNode = (struct LinearNode*)malloc(sizeof(struct LinearNode));

	//use the rest of the add code to add this new element into the linked list
	fread(anElement->carReg, strlen(anElement->carReg) - 1, 1, fp);
	fread(anElement->make, strlen(anElement->make) - 1, 1, fp);
	fread(anElement->model, strlen(anElement->model) - 1, 1, fp);
	fread(anElement->colour, strlen(anElement->colour) - 1, 1, fp);
	fread(&anElement->prevOwners, sizeof(anElement->prevOwners), 1, fp);
	fread(&anElement->reserved, sizeof(anElement->reserved), 1, fp);
	fread(&anElement->reserveAmount, sizeof(anElement->reserveAmount), 1, fp);

	if (aNode == NULL)
		printf("\nError - no space for the new node!\n");
	else {
		aNode->next = NULL;
		aNode->element = anElement;

		//add node to end of the list
		if (isEmpty()) {
			list = aNode;
			last = aNode;
		} //end if
		else {
			last->next = aNode;
			last = aNode;
		} //end else
	} //end else
}//end getFromFile