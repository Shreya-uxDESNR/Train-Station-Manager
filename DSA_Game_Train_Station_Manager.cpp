#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
#include<limits>
#include<sstream>
using namespace std;

//LINKED LIST NODE
//Stores departure history
struct Node
{
	string trainName;
	Node* next;
};

//TRAIN STATION MANAGER GAME
class TrainStationGame
{
	private:
		//Queue- Incoming trains
		queue<string> incomingTrains;
		
		//Stack- Siding Stack
		stack<string> siding;
		
		//Linked List- Departure history
		Node* departureHistory;
		
		//Target departure order
		vector<string> targetOrder;
		
		int level;
		int score;
		int moves;
		int numberOfTrains;
		
	public:
		//CONSTRUCTOR
		TrainStationGame()
		{
			departureHistory = NULL;
			level = 1;
			score = 0;
			moves = 0;
			numberOfTrains = 3;
		}
		
		//DESTRUCTOR
		~TrainStationGame()
		{
			clearHistory();
		}
		
		//ADD TRAIN TO LINKED LIST
		void addToHistory(string trainName)
		{
			Node* newNode = new Node;
			newNode->trainName = trainName;
			newNode->next = NULL;
			if(departureHistory == NULL)
			{
				departureHistory = newNode;
			}
			else
			{
				Node* temp = departureHistory;
				while(temp->next != NULL)
				{
					temp = temp->next;
				}
				temp->next = newNode;
			}
		}
		
		//Display Departure History
		void displayHistory()
		{
			cout<<"\n----------------------------------------";
			cout<<"DEPARTURE HISTORY\n";
			cout<<"----------------------------------------\n";
			if(departureHistory == NULL)
			{
				cout<<"No trains have departed yet.\n";
				return;
			}
			Node* temp = departureHistory;
			cout<< "HEAD ->";
			while(temp != NULL)
			{
				cout<<"["<<temp -> trainName<<"]";
				if(temp->next != NULL)
				{
					cout<<" -> ";
				}
				temp = temp->next;
			}
			cout<<" -> NULL\n";
		}
		
		//CLEAR LINKED LIST
		void clearHistory()
		{
			Node* temp;
			while(departureHistory != NULL)
			{
				temp = departureHistory;
				departureHistory = departureHistory->next;
				delete temp;
			}
		}
		
		//CLEAR QUEUE
		void clearQueue()
		{
			while(!incomingTrains.empty())
			{
				incomingTrains.pop();
			}
		}
		
		//CLEAR STACK
		void clearStack()
		{
			while(!siding.empty())
			{
				siding.pop();
			}
		}
		
		//GET NUMBER OF TRAINS FOR LEVEL
		int getTrainCount(int currentLevel)
		{
			/*
				Level 1-2 = 3 trains
				Level 3-4 = 4 trains
				Level 5-6 = 5 trains
				Level 7-8 = 6 trains
				an so on
			*/
			return 3 + ((currentLevel-1) / 2);
		}
		
		//GENERATE TARGET ORDER
		//The target order is generated using valid
		//Queue + Stack operations.
		//Therefore, the target is always achievable
		void generateTargetOrder()
		{
			targetOrder.clear();
			queue<string> tempQueue;
			stack<string> tempStack;
			for(int i = 1; i <= numberOfTrains; i++)
			{
				stringstream ss;
				ss<< "T"<<i;
				tempQueue.push(ss.str());
			}
			while(!tempQueue.empty() || !tempStack.empty())
			{
				int operation;
				if(tempQueue.empty())
				{
					operation = 1;
				}
				else if(tempStack.empty())
				{
					operation = 0;
			    }
				else
				{
					operation = rand() % 2;
				}
				
				//OPERATION 0
				//QUEUE -> STACK
				if(operation == 0 && !tempQueue.empty())
				{
					string train = tempQueue.front();
					tempQueue.pop();
					tempStack.push(train);
				}
				else if(operation == 1)
				{
					if(!tempStack.empty())
					{
						string train = tempStack.top();
						tempStack.pop();
						targetOrder.push_back(train);
					}
					else if(!tempQueue.empty())
					{
						string train = tempQueue.front();
						tempQueue.pop();
						targetOrder.push_back(train);
					}
				}
			}
		}
		
		void startLevel(int levelNumber)
		{
			level = levelNumber;
			numberOfTrains = getTrainCount(level);
			clearQueue();
			clearStack();
			clearHistory();
			moves = 0;
			
			for(int i=1; i<=numberOfTrains; i++)
			{
				stringstream ss;
				ss<<"T"<<i;
				incomingTrains.push(ss.str());
			}
			generateTargetOrder();
		}
		
		//DISPLAY QUEUE
		void displayQueue()
		{
			queue<string> temp = incomingTrains;
			cout<<"\nIncoming Queue: ";
			if(temp.empty())
			{
				cout<<"EMPTY";
			}
			else
			{
				while(!temp.empty())
				{
					cout<<temp.front();
					temp.pop();
					
					if(!temp.empty())
					{
						cout<<" -> ";
					}
				}
			}
			cout<<"\n";
		}
		
		void displayStack()
		{
			stack<string> temp = siding;
			cout<<"Siding Stack: ";
			if(temp.empty())
			{
				cout<<"EMPTY";
			}
			else
			{
				cout<<"TOP -> ";
				
				while(!temp.empty())
				{
					cout<<temp.top();
					temp.pop();
					
					if(!temp.empty())
					{
						cout<<" -> ";
					}
				}
			}
			cout<<"\n";
		}
		
		//DISPLAY TARGET
		void  displayTarget()
		{
			cout<<"Target Departure Order: ";
			for(int i=0; i<(int)targetOrder.size(); i++)
			{
				cout<<targetOrder[i];
				
				if(i<(int)targetOrder.size() - 1)
				{
					cout<<" -> ";
				}
			}
			cout<<"\n";
		}
		
		//GET NEXT REQUIRED TRAIN
		string getNextRequiredTrain()
		{
			int departedCount = 0;
			Node* temp = departureHistory;
			
			while(temp!=NULL)
			{
				departedCount++;
				temp = temp -> next;
			}
			
			if(departedCount<(int)targetOrder.size())
			{
				return targetOrder[departedCount];
			}
			return "";
		}
		
		//SEND FRONT TRAIN FROM QUEUE
		void sendTrain()
		{
			if(incomingTrains.empty())
			{
				cout<<"\nThe incoming queue is empty.\n";
				return;
			}
			
			string train = incomingTrains.front();
			string requiredTrain = getNextRequiredTrain();
			cout<<"\nFront train: "<<train<<"\n";
			
			if(train == requiredTrain)
			{
				incomingTrains.pop();
				addToHistory(train);
				moves++;
				score += 10;
				cout<<"\nSUCCESS!\n";
				cout<<train<<" "<<"departed successfully.\n";
				cout<<"+10 points\n";
			}
			
			else
			{
				moves++;
				score -= 5;
				cout<<"\nWRONG MOVE!\n";
				cout<<train<<" "<<"cannot depart now.\n";
				cout<<"You need "<<requiredTrain<<" first.\n";
				cout<<" -5 points\n";
			}
		}
		
		//STORE TRAIN IN STACK
		void storeTrain()
		{
			if(incomingTrains.empty())
			{
				cout<<"\nThe incoming queue is empty.\n";
				return;
			}
			
			string train = incomingTrains.front();
			incomingTrains.pop();
			siding.push(train);
			moves++;
			score -= 1;
			cout<<"\n"<<train<<" "<<"in the siding stack.\n";
			cout<<"-1 point\n";
		}
		
		//RELEASE TRAIN FROM STACK
		void releaseTrain()
		{
			if(siding.empty())
			{
				cout<<"\nThe siding stack is empty.\n";
				return;
			}
			
			string train = siding.top();
			string requiredTrain = getNextRequiredTrain();
			cout<<"\nTop train: "<<train<<"\n";
			cout<<"Required train: "<<requiredTrain<<"\n";
			
			if(train == requiredTrain)
			{
				siding.pop();
				addToHistory(train);
				moves++;
				score += 10;
				cout<<"\nSUCCESS!\n";
				cout<<train<<" "<<"departed from the siding.\n";
				cout<<"+10 points\n";
			}
			
			else
			{
				moves++;
				score -= 5;
				cout<<"\nWRONG MOVE!\n";
				cout<<train<<" "<<"cannot depart now.\n";
				cout<<"You need "<<requiredTrain<<" first.\n";
				cout<<"-5 points\n";
			}
		}
		
		//CHECK LEVEL COMPLETION
		bool isLevelComplete()
		{
			int count = 0;
			Node* temp = departureHistory;
			
			while(temp!=NULL)
			{
				count++;
				temp = temp -> next;
			}
			
			return count == (int)targetOrder.size();
		}
		
		//DISPLAY GAME
		void displayGame()
		{
			cout<<"\n\n============================================\n";
			cout<<"          TRAIN STATION MANAGER\n";
			cout<<"===========================================\n";
			cout<<"Level: "<<level<<"\n";
			cout<<"Trains: "<<numberOfTrains<<"\n";
			cout<<"Score: "<<score<<"\n";
			cout<<"Moves: "<<moves<<"\n";
			cout<<"===========================================\n";
			
			displayQueue();
			displayStack();
			displayHistory();
			
			cout<<"\n";
			
			displayTarget();
			
			cout<<"===========================================\n";
			}
			
			//INSTRUCTIONS
			void showInstructions()
			{
				cout<<"\n=====================================\n";
				cout<<"HOW TO PLAY\n";
				cout<<"=======================================\n";
				
				cout<<"\nQUEUE:\n";
				cout<<"Incoming trains are stored in a Queue.\n";
				cout<<"Queue follows FIFO - First In First Out.\n";
				
				cout<<"\nSTACK:\n";
				cout<<"Trains can be moved into the siding Stack.\n";
				cout<<"Stack follows LIFO - Last In First Out.\n";
				
				cout<<"\nLINKED LIST:\n";
				cout<<"Every successfully departed train is stored\n";
				cout<<"in the Departure History Linked List.\n";
				
				cout<<"\nSCORING:\n";
				cout<<"Correct departure : +10 points\n";
				cout<<"Store train : -1 point\n";
				cout<<"Wrong departure : -5 points\n";
				
				cout<<"\n=====================================\n";
			}
			
			//DISPLAY RESULT
			void displayLevelResult()
			{
				cout<<"\n=====================================\n";
				cout<<"LEVEL COMPLETED!\n";
				cout<<"=====================================\n";
				
				cout<<"Level completed: "<<level<<"\n";
				cout<<"Trains: "<<numberOfTrains<<"\n";
				cout<<"Moves: "<<moves<<"\n";
				cout<<"Current Score: "<<score<<"\n";
				
				int minimumPossible = numberOfTrains;
				
				if(moves <= minimumPossible+2)
				{
					cout<<"Rating : *** 3 STARS\n";
				}
				
				else if(moves <= minimumPossible+5)
				{
					cout<<"Rating : ** 2 STARS\n";
				}
				
				else
				{
					cout<<"Rating : * 1 STAR\n";
				}
				
				cout<<"=====================================\n";
			}
			
			void pauseGame()
			{
				cout<<"\nPress ENTER to continue...";
				
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin.get();
			}
			
			//PLAY CURRENT LEVEL
			bool playLevel()
			{
				int choice;
				
				while(!isLevelComplete())
				{
					displayGame();
					cout<<"\nChoose an action:\n";
					cout<<"1. Send Front Train\n";
					cout<<"2. Store Front Train in Siding\n";
					cout<<"3. Release Train from Siding\n";
					cout<<"4. View Departure History\n";
					cout<<"5. Show Instructions\n";
					cout<<"6. Restart Level\n";
					cout<<"7. Exit Game\n";
					
					cout<<"Enter your choice: ";
					cin>>choice;
					
					//SEND TRAIN
					if(choice == 1)
					{
						sendTrain();
						pauseGame();	
					}
					
					//STORE TRAIN
					else if(choice == 2)
					{
						storeTrain();
						pauseGame();
					}
					
					//RELEASE TRAIN
					else if(choice == 3)
					{
						releaseTrain();
						pauseGame();
					}
					
					//HISTORY
					else if(choice == 4)
					{
						displayHistory();
						pauseGame();
					}
					
					//IMSTRUCTIONS
					else if(choice == 5)
					{
						showInstructions();
						pauseGame();
					}
					
					//RESTART LEVEL
					else if(choice == 6)
					{
						startLevel(level);
						cout<<"\nLevel restarted with a new target order!\n";
						pauseGame();
					}
					
					//EXIT
					else if(choice == 7)
					{
						return false;
					}
					
					//INVALID CHOICE
					else
					{
						cout<<"\nInvalid choice! Please enter 1-7.\n";
						pauseGame();
					}
				}
				return true;
			}
			
			//RUN COMPLETE GAME	
			void runGame()
			{
				int currentLevel = 1;
				char continueGame;
				cout<<"\n============================================\n";
				cout<<"WELCOME TO TRAIN STATION MANAGER\n";
				cout<<"============================================\n";
				
				showInstructions();
				pauseGame();
				
				while(true)
				{
					startLevel(currentLevel);
					cout<<"\nStarting Level "<<currentLevel<<"...\n";
					pauseGame();
					
					bool completed = playLevel();
					
					//Player selected Exit
					if(!completed)
					{
						break;
					}
					
					displayGame();
					displayLevelResult();
					
					cout<<"\nDo you want to continue to the next level? (Y/N): ";
					cin>>continueGame;
					
					if(continueGame == 'Y' || continueGame == 'y')
					{
						currentLevel++;
					}
					
					else
					{
						break;
					}
				}
				
				cout<<"\n============================================\n";
				cout<<"GAME OVER\n";
				cout<<"============================================\n";
				
				cout<<"Final Score: "<<score<<"\n";
				cout<<"Thank you for playing!\n";
			}
};

//MAIN FUNCTION
int main()
{
	//Initialize random number generator
	srand((unsigned int)time(NULL));
	
	//Create game object
	TrainStationGame game;
	
	//Start game
	game.runGame();
	
	return 0;
}
