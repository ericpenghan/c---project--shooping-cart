#include <iostream>
#include<string>
#include<iomanip>
#include<fstream>
/*
 Author:Eric Han
 Date:Sepetember 19 2018        \t|" << endl;
 Program name: Online shopping 
 Program description: funtion as an online shopping site
 */



using namespace std;


void printlogo() {
  cout << "+-----------------------------------------------+" << endl;
  cout << "|              CRAZTSHOP.COM                    |" << endl;
  cout << "|              by Eric Han                      |" << endl;
  cout << "+-----------------------------------------------+" << endl;
}//pringt starting logo

void printoption(){
  cout << "I - List our Inventory\n";
  cout << "O - Make an Order\n";
  cout << "L - List all Orders made\n";
  cout << "X - Exit\n";
}//print option

bool isValidOption(char letter){
  letter = toupper(letter); //replace option with capital letter
  if (letter == 'I' || letter == 'O' || letter == 'L' || letter == 'X' )
    return true;
  else
    return false;
}


char getMainOption(){
  string option;
  char firstLetter;
  printlogo();
  printoption();
  cout << "Enter an option: ";
  cin >> option;
  firstLetter = option[0];
  
  while (isValidOption(firstLetter) == false ){
    cout << "Invalid option, enter I, O, L or X!\n";
    cout << "Enter an option: ";
    cin >> option;
    firstLetter = option[0];
  }
  firstLetter = toupper(firstLetter);
  return firstLetter;
}//option menu, return option entered

const int READ_ERROR    = -1;
const int MAX_INV_ITEMS = 10;

// describes a single item in the inventory, and on an order
struct inventoryItem {
  string prodCode;    // product code: length 12, no spaces
  string description;    // product description: max length 20, has spaces
  double price;      // price of the product, max 999.99
};

const int MAX_BASKETS = 10;
// describes a customer order or "basket"
const int MAX_ORDER_ITEMS = 5;

struct basket {
  long   orderNumber;            // unique order number for this order
  string custName;            // customer name
  double totalPrice;            // price of all items purchased
  inventoryItem item[MAX_ORDER_ITEMS];  // list of items purchased
  int numItems ;              // number of items purchased
};



//----------------------------------------------------------------------------
//                                  readInventory
//----------------------------------------------------------------------------
// Modifies: inventory list (sets numberOfItems to -1 on READ ERROR)
//           lastOrderNum
// Reads inventory data from a file into an array of inventory items
//----------------------------------------------------------------------------
void readInventory(inventoryItem inv[], int & numberOfInvItems, int & lastOrderNum) {
  ifstream f;
  
  // open the inventory file
  f.open("inventory.txt");
  if (f.fail()) {
    cout << "readFile:: error opening inventory.txt\n";
    numberOfInvItems = READ_ERROR;
    return;
  }
  
  // read number of items from first line
  f >> numberOfInvItems >> lastOrderNum;
  
  // for each item, read the data for the item
  for (int i = 0; i < numberOfInvItems; i++) {
    f >> inv[i].prodCode >> inv[i].price;
    f.ignore(); // finished reading integer, getline() on string is next
    getline(f, inv[i].description);
  }
  f.close();
} // readInventory()

void displayList(inventoryItem inv[], int numberOfInvItems ){
  for(int i = 0; i < numberOfInvItems; i++){
    cout << right << setw(3) << i << "  " << left << setw(12) << inv[i].prodCode << "  $" << right << setw(6) << inv[i].price << "  " << inv[i].description << endl;
  }
}//inventory list

void displayInventory(inventoryItem inv[], int numberOfInvItems){
  cout << "+-----------------------------------------------------------+\n";
  cout << "|                          Products                         |\n";
  cout << "+-----------------------------------------------------------+\n";
  cout << " #   PRODUCT CODE   PRICE   PRODUCT DESCRIPTION\n";
  cout << "---  ------------  -------  ---------------------------------------\n";
  displayList(inv, numberOfInvItems);
  cout << "Number of items in inventory:  " << numberOfInvItems << endl;
}// option I list our inventory

void startOrder(int lastOrdNUm,basket cart[], int &numOfOrders){
  
  
  
  lastOrdNUm++;
  cart[numOfOrders].orderNumber = lastOrdNUm;
  cart[numOfOrders].totalPrice = 0;
  cart[numOfOrders].numItems = 0;// maybe this is the mistake
  
  cout << left << setw(21) << "Order Number:" << lastOrdNUm << endl;
  cout << left << setw(15) << "Enter customer name: ";
  cin.ignore(256,'\n');
  getline (cin, cart[numOfOrders].custName);
  numOfOrders++;
  
}//start order fuction

bool orderItem(inventoryItem inv[], int numberOfInvItems, basket &v){
  cout << endl;
  int ItemOption;
  cout << "Enter item number (-1 to end):  ";
  cin >> ItemOption;
  while(ItemOption < -1 || ItemOption >= numberOfInvItems){
    
    cout << "Invalid entry. Enter number -1 to " << numberOfInvItems-1 << endl;
    cout << "Enter item number (-1 to end):  ";
    cin >> ItemOption;
  }
  if (ItemOption == -1){
    return true;
  }
  
  v.item[v.numItems].price= inv[ItemOption].price;
  v.item[v.numItems].prodCode= inv[ItemOption].prodCode;
  v.item[v.numItems].description= inv[ItemOption].description;
  v.totalPrice +=  v.item[v.numItems].price;
  cout << v.item[v.numItems].description << " added to your basket. Current total is $ " << v.totalPrice ;
  v.numItems ++;
  return false ;
}

void displayOrder(basket &v){
  cout << "Order: " << v.orderNumber << "      " << v.custName << endl;
  displayList(v.item, v.numItems);
  cout << left << setw(19) << "Total"<< "$ " << v.totalPrice << endl;
}

void makeOrder(inventoryItem inventory[], int numberOfInvItems, int lastOrdNUm,basket cart[], int &numOfOrders){
  if(numOfOrders== MAX_ORDER_ITEMS){
    cout << "Sorry, we can not take more orders today.\n";
  }
  startOrder(lastOrdNUm, cart, numOfOrders);
  displayInventory(inventory, numberOfInvItems);
  while( orderItem(inventory, numberOfInvItems, cart[numOfOrders-1]) == false){}
  cout << "Thank you for your order!\n";
  displayOrder(cart[numOfOrders-1]);
  system ("pause");
}

void listOrders(basket cart[], int numOfOrders){//delete global variable, use num of order instead pass around
  cout << "+-----------------------------------------------------------+\n";
  cout << "|                          Orders                           |\n";
  cout << "+-----------------------------------------------------------+\n";
  for(int i = 0; i < numOfOrders; i++){
    displayOrder(cart[i]);
  }
  cout << endl;
  cout << "Total Number of Orders = " << numOfOrders << endl;
}

void writeOrders(basket cart[], int &numOfOrders){
  ofstream f;
  f.open("orders.txt");
  
  if (!f.fail()){
    f << numOfOrders << '\n';
    for(int i = 0; i < numOfOrders; i++){
  
      f << cart[i].orderNumber << " " << cart[i].numItems << " " << cart[i].totalPrice <<" " << cart[i].custName << "\n";
      for (int w = 0; w < cart[i].numItems; w++){
        f << cart[i].item[w].prodCode << " " << cart[i].item[w].price << " " << cart[i].item[w].description << "\n";
        
        
      }
    }
    f.close();
  }
  else
    cout << "Open Error\n";
  
  
}

int main() {
  inventoryItem inventory[MAX_INV_ITEMS];//declare
  int numInvItems = 0;//declare
  int lastOrdNUm;//declare
  basket cart[MAX_BASKETS];//declare
  char option;
  basket v ;
  int numOfOrders = 0;
  readInventory(inventory, numInvItems, lastOrdNUm);//declear
  
  
  option = getMainOption();
  
  while(option != 'X'){
  
  if (option == 'I'){
    displayInventory(inventory, numInvItems);
    
  }
  else if(option == 'O'){
    makeOrder(inventory, numInvItems, lastOrdNUm, cart, numOfOrders);
  }
  else if(option == 'L'){
    listOrders(cart, numOfOrders);
    
    
  }
   option = getMainOption();
  }
  writeOrders(cart, numOfOrders);

  
  cout << endl;
  system("pause");
  return 0;
}
