//Order Management System

#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
const int infinity = INT_MAX;
using namespace std;

// Defining Data Types
struct customer
{
    int age;
    string name;
    string itemName;
    int quantity;
    double bill;

    customer() {}

    customer(int age, string name, int quantity, string itemName, double bill)
    {
        this->age = age;
        this->name = name;
        this->itemName = itemName;
        this->quantity = quantity;
        this->bill = bill;
    }
};

struct takeAwayCustomer
{
    customer cusotomer;
    takeAwayCustomer *next = NULL;

    takeAwayCustomer(int age, string name, int quantity, string itemName, double bill)
    {
        this->cusotomer = customer(age, name, quantity, itemName, bill);
    }
};

struct dineInCustomer
{
    customer cusotomer;
    dineInCustomer *next = NULL;

    dineInCustomer(int age, string name, int quantity, string itemName, double bill)
    {
        this->cusotomer = customer(age, name, quantity, itemName, bill);
    }
};

struct homeDeliveryCustomer
{

    customer cusotomer;
    string address;
    double deliveryCharges;
    int distanceDelivery;
    struct homeDeliveryCustomer *next = NULL;

    homeDeliveryCustomer(int age, string name, int quantity, string itemName, double bill, string address, double deliveryCharges, int distanceDelivery)
    {
        this->cusotomer = customer(age, name, quantity, itemName, bill);
        this->address = address;
        this->deliveryCharges = deliveryCharges;
        this->distanceDelivery = distanceDelivery;
    }
};

struct FoodStore
{

    string shopName;
    string *menu;
    int *price;
    string address;
    takeAwayCustomer *nextTakeAwayCustomer = NULL;
    dineInCustomer *nextDineInCustomer = NULL;
    homeDeliveryCustomer *nextHomeDeliveryCustomer = NULL;
};

// Globally declaring the Shop's pointer
FoodStore *myFoodStore = NULL;

// Globally Declaring the Current Customer's Pointers for all three Types
takeAwayCustomer *currentTakeAwayCustomer = NULL;
dineInCustomer *currentDineInCustomer = NULL;
homeDeliveryCustomer *currentHomeDeliveryCustomer = NULL;

// Globally declaring the variables for the total of all the orders in queue!
double total, takeAway, dineIn, homeDelivery;
// Globally declaring the variables for the total of all the orders served!
double servedTotal;

// In case of Serving , to keep the record of Customers Served, implementing AVL Tree for efficient Search
// to search the record of Customers by Name
// It can also Display all the customers Served

struct servedCustomer
{

    int age;
    string name;
    int quantity;
    string itemName;
    double bill;
    string customerType;
    servedCustomer *left;
    servedCustomer *right;

    // Constructor
    servedCustomer(int age, string name, int quantity, string itemName, double bill, string customerType)
    {
        // setting customers details

        this->age = age;
        this->name = name;
        this->quantity = quantity;
        this->itemName = itemName;
        this->bill = bill;
        this->customerType = customerType;

        // child to NULL
        this->left = NULL;
        this->right = NULL;
    }
};

servedCustomer *root = NULL; // Global pointer for the root of AVLTree

// isEmpty or not
int isEmpty(servedCustomer *root)
{
    return root == NULL;
}

// display Customers Details

void display(servedCustomer *root)
{
    cout << "Name :" << root->name << endl;
    cout << "Age  :" << root->age << endl;
    cout << "Item :" << root->itemName << endl;
    cout << "Quantity : " << root->quantity << endl;
    cout << "Bill : " << root->bill << endl;
    cout << "Customer Type: " << root->customerType << endl;
}

// Traversal for the served Customers

servedCustomer *displayAllServedOrders(servedCustomer *root)
{

    if (root)
    {
        displayAllServedOrders(root->left);
        display(root); // will display all the served Customers
        displayAllServedOrders(root->right);
    }

    return root;
}

// Height of servedCustomer tree

int height(servedCustomer *root)
{
    if (!root)
        return 0;

    return max(height(root->left), height(root->right)) + 1;
}

// Balance Factor for each ServedCustomer node

int balanceFactor(servedCustomer *root)
{
    if (!root)
        return 0;

    return height(root->left) - height(root->right);
}

// Maximum of two integers as a helper function for height
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Searching in servedCustomer tree

servedCustomer *search(servedCustomer *root, string keyName)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->name == keyName)
    {
        return root;
    }
    else if (root->name < keyName)
    {
        return search(root->right, keyName);
    }
    else if (root->name > keyName)
    {
        return search(root->left, keyName);
    }

    return root;
}

// Finding Maximum Node of servedCustomer tree

servedCustomer *maxservedCustomer(servedCustomer *root)
{
    // Maximum Node is Present in the most Right Node  of the served Customer Tree

    servedCustomer *p = root;
    servedCustomer *temp = NULL;

    while (p != NULL)
    {
        temp = p;
        p = p->right;
    }

    return temp;
}

// Balancing the ServedCustomer's Tree thorugh AVL Rotations

// LL Rotation
servedCustomer *LLRotation(servedCustomer *root)
// rotate wese right per krna hai!
{
    // saving the new root and the lost element in case of rotation
    servedCustomer *x = root->left;
    servedCustomer *temp = x->right;

    // Performing rotation
    x->right = root;
    root->left = temp;

    // updating the root
    root = x;
    // returning the root
    return x;
}

// RR Rotation
servedCustomer *RRRotation(servedCustomer *root)
{
    // rotate wese left per krna hai!

    // saving the new root and the lost element in case of rotation

    servedCustomer *x = root->right;
    servedCustomer *temp = x->left;

    // Performing rotation
    x->left = root;
    root->right = temp;

    // updating the root
    root = x;

    // returning the root
    return x;
}

// LR Rotation
servedCustomer *LRRotation(servedCustomer *root)
{
    root->left = RRRotation(root->left);
    return LLRotation(root);
}

// RL Rotation
servedCustomer *RLRotation(servedCustomer *root)
{
    root->right = LLRotation(root->right);
    return RRRotation(root);
}

//  INSERTION in servedCustomer Tree

servedCustomer *insertion(int age, string name, int quantity, string itemName, double bill, string customerType, servedCustomer *root)
{
    servedCustomer *newNode = new servedCustomer(age, name, quantity, itemName, bill, customerType);

    if (root == NULL)
    {
        root = newNode;
    }

    else if (root->name > newNode->name)
    {
        root->left = insertion(age, name, quantity, itemName, bill, customerType, root->left);
    }
    else if (root->name < newNode->name)
    {
        root->right = insertion(age, name, quantity, itemName, bill, customerType, root->right);
    }

    else
    {
        cout << "No duplicates values are allowed " << endl;
        return root;
    }

    int bf = balanceFactor(root);

    if (bf == 2)
    {
        // LL
        if (root->left->name > newNode->name)
        {
            return LLRotation(root);
        }

        // LR
        if (root->left->name < newNode->name)
        {
            return LRRotation(root);
        }
    }
    else if (bf == -2)
    {
        // RR
        if (root->right->name < newNode->name)
        {
            return RRRotation(root);
        }

        // RL
        if (root->right->name > newNode->name)
        {
            return RLRotation(root);
        }
    }

    return root; // in case there is no need of rotation
}

servedCustomer *deleteNode(servedCustomer *root, string key)
{
    if (root == NULL)
        return root;
    else if (key < root->name)
        root->left = deleteNode(root->left, key);
    else if (key > root->name)
        root->right = deleteNode(root->right, key);
    else
    {
        // if deleteroot has one child or zero child
        if ((root->left == NULL) || (root->right == NULL))
        {
            servedCustomer *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            delete (temp);
        }
        else
        {
            // if deleteroot has two or more childs
            servedCustomer *temp = maxservedCustomer(root->right);
            root->name = temp->name;
            root->right = deleteNode(root->right, temp->name);
        }
    }

    if (root == NULL)
        return root;

    // getting the balance factor
    int balance = balanceFactor(root);

    // Rotation Cases
    // LEFT LEFT CASE
    if (balance > 1 && key < root->left->name)
        return LLRotation(root);

    // LEFT RIGHT CASE
    if (balance > 1 && key > root->left->name)
    {
        root->left = LLRotation(root->left);
        return LRRotation(root);
    }

    // RIHGT RIGHT CASE
    if (balance < -1 && key > root->right->name)
        return RRRotation(root);

    // RIGHT LEFT CASE
    if (balance < -1 && key < root->right->name)
    {
        return RLRotation(root);
    }

    return root;
}

void deleteAllServedCustomers(servedCustomer *root)
{

    while (root)
    {
        root = deleteNode(root, root->name);
    }

    cout << "The Served Customer's List is Cleared " << endl;
}

// Now defining Order Placing and Serving of Take Away Customer
// Based on : Older person will be served first (PRIORITY QUEUE)

void placeOrderTakeAwayCustomer(int age, string name, string itemName, int quantity, double bill)
{
    // making new Customer
    currentTakeAwayCustomer = new takeAwayCustomer(age, name, quantity, itemName, bill);

    if (myFoodStore->nextTakeAwayCustomer == NULL)
    {
        // if first then insert in front
        myFoodStore->nextTakeAwayCustomer = currentTakeAwayCustomer;
    }
    else
    {
        // finding the last Node
        takeAwayCustomer *temp = myFoodStore->nextTakeAwayCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        if (temp->cusotomer.age < currentTakeAwayCustomer->cusotomer.age)
        {
            // insert at front
            takeAwayCustomer *firstCustomer = myFoodStore->nextTakeAwayCustomer;
            myFoodStore->nextTakeAwayCustomer = currentTakeAwayCustomer;
            currentTakeAwayCustomer->next = firstCustomer;
        }
        else
        {
            // insert at end
            temp->next = currentTakeAwayCustomer;
            currentTakeAwayCustomer->next = NULL;
        }
    }
    cout << "Your Order has been Placed Mr/Ms " << name << " and your order is " << itemName << " with " << quantity << " quantity and total bill is ₹" << bill << endl;
}
void serveOrderTakeAwayCustomer()
{
    if (myFoodStore->nextTakeAwayCustomer == NULL)
    {
        cout << "No Take Away Customer to Serve" << endl;
    }
    else
    {
        // serving the first customer
        takeAwayCustomer *temp = myFoodStore->nextTakeAwayCustomer;
        // if it has some next element
        if(temp->next != NULL){
            myFoodStore->nextTakeAwayCustomer = temp->next;
        }
        else{
            myFoodStore->nextTakeAwayCustomer = NULL;
        }
        
        cout << "Take Away Customer Served : " << temp->cusotomer.name << endl;

        string customerType = "Take-Away";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cusotomer.age, temp->cusotomer.name, temp->cusotomer.quantity, temp->cusotomer.itemName, temp->cusotomer.bill, customerType, root);

        delete temp; // deleting the customer
    }
}

// Now defining Order Placing and Serving of Dine-In Customer
// Based on : First Come First Served (FIFO) (QUEUE)

void placeOrderDineInCustomer(int age, string name, string itemName, int quantity, double bill)
{
    // making new Customer
    currentDineInCustomer = new dineInCustomer(age, name, quantity, itemName, bill);

    if (myFoodStore->nextDineInCustomer == NULL)
    {
        // if first insert in front
        myFoodStore->nextDineInCustomer = currentDineInCustomer;
    }
    else
    {
        // finding the last Node
        dineInCustomer *temp = myFoodStore->nextDineInCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentDineInCustomer;
        currentDineInCustomer->next = NULL;
    }
    cout << "Your Order has been Placed Mr/Ms " << name << " and your order is " << itemName << " with " << quantity << " quantity and total bill is ₹" << bill << endl;
}
void serveOrderDineInCustomer()
{
    if (myFoodStore->nextDineInCustomer == NULL)
    {
        cout << "No Dine-In Customer to Serve" << endl;
    }
    else
    {
        // serving the first customer
        dineInCustomer *temp = myFoodStore->nextDineInCustomer;
        if(temp->next != NULL){
            myFoodStore->nextDineInCustomer = temp->next;
        }
        else{
            myFoodStore->nextDineInCustomer = NULL;
        }

        cout << "Dine-In Customer Served : " << temp->cusotomer.name << endl;

        string customerType = "Dine-In";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cusotomer.age, temp->cusotomer.name, temp->cusotomer.quantity, temp->cusotomer.itemName, temp->cusotomer.bill, customerType, root);

        delete temp; // deleting the customer
    }
}

// Now defining Order Placing and Serving of Home Delivery Customer
// Based on : (when all orders are ready)(LIFO)(Stack)

void placeOrderHomeDeliveryCustomer(int age, string name, string itemName, int quantity, double bill, string address, int deliveryCharges, int distanceDelivery)
{
    // making new Customer
    currentHomeDeliveryCustomer = new homeDeliveryCustomer(age, name, quantity, itemName, bill, address, deliveryCharges, distanceDelivery);

    if (myFoodStore->nextHomeDeliveryCustomer == NULL)
    {
        // if first insert in front
        myFoodStore->nextHomeDeliveryCustomer = currentHomeDeliveryCustomer;
    }
    else
    {
        // finding the last Node
        homeDeliveryCustomer *temp = myFoodStore->nextHomeDeliveryCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentHomeDeliveryCustomer;
        currentHomeDeliveryCustomer->next = NULL;
    }
    cout << "Your Order has been Placed Mr/Ms " << name << " and your order is " << itemName << " with " << quantity << " quantity and total bill is ₹"  << bill << endl;
}

void serveOrderHomeDeliveryCustomer()
{
    if (myFoodStore->nextHomeDeliveryCustomer == NULL)
    {
        cout << "No Home Delivery Customer to Serve" << endl;
    }
    else
    {

        // serving the last customer first
        homeDeliveryCustomer *first = myFoodStore->nextHomeDeliveryCustomer;

        if (first->next == NULL)
        {
            // if it is the only customer

            myFoodStore->nextHomeDeliveryCustomer = NULL;

            cout << "Home Delivery Customer Served : " << first->cusotomer.name << endl;
            string customerType = "Home-Delivery Customer";
            root = insertion(first->cusotomer.age, first->cusotomer.name, first->cusotomer.quantity, first->cusotomer.itemName, first->cusotomer.bill, customerType, root);

            // now deleting the node
            delete (first);
        }
        else {
            homeDeliveryCustomer *s = first->next;
            while(s->next !=NULL){
                first = first->next;
                s = s->next;
            }

            first->next = NULL;

            cout << "Home Delivery Customer Served : " << s->cusotomer.name << endl;
            string customerType = "Home-Delivery Customer";
            root = insertion(s->cusotomer.age, s->cusotomer.name, s->cusotomer.quantity, s->cusotomer.itemName, s->cusotomer.bill, customerType, root);

            // deleting the node

            delete (s);
        }

        
        // deleting the customer
    }
}

// It will serve all the waiting orders

void serveAllOrders()
{

    while (myFoodStore->nextTakeAwayCustomer != NULL)
    {
        serveOrderTakeAwayCustomer();
    }
    while (myFoodStore->nextDineInCustomer != NULL)
    {
        serveOrderDineInCustomer();
    }
    while (myFoodStore->nextHomeDeliveryCustomer != NULL)
    {
        serveOrderHomeDeliveryCustomer();
    }
}

void displayAllOrdersTakeAwayCustomers()
{
    if (myFoodStore->nextTakeAwayCustomer == NULL)
    {
        cout << "There is no Order for Walk-in Customer yet" << endl;
    }
    else
    {
        takeAwayCustomer *traversal = myFoodStore->nextTakeAwayCustomer;
        while (traversal != NULL)
        {

            cout << "-----------------------------------------------------" << endl;
            cout << "Take-Away Customer : " << traversal->cusotomer.name << endl;
            cout << "Age : " << traversal->cusotomer.age << endl;
            cout << "Item Name : " << traversal->cusotomer.itemName << endl;
            cout << "Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "Bill : " << "₹" << traversal->cusotomer.bill << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrdersHomeDeliveryCustomers()
{
    if (myFoodStore->nextHomeDeliveryCustomer == NULL)
    {
        cout << "There is no Order for Home Delivery Customer yet" << endl;
    }
    else
    {
        homeDeliveryCustomer *traversal = myFoodStore->nextHomeDeliveryCustomer;
        while (traversal != NULL)
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "Home Delivery Customer : " << traversal->cusotomer.name << endl;
            cout << "Age : " << traversal->cusotomer.age << endl;
            cout << "Item Name : " << traversal->cusotomer.itemName << endl;
            cout << "Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "Delivery Distance : " << traversal->deliveryCharges << "KM"<<endl;
            cout << "Delivery Charges : " << traversal->distanceDelivery << endl;
            cout << "Bill : " <<  "₹" << traversal->cusotomer.bill << endl;
            cout << "Delivery Address : " << traversal->address << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrdersDineInCustomers()
{
    if (myFoodStore->nextDineInCustomer == NULL)
    {
        cout << "There is no Order for Dine-In Customer yet" << endl;
    }
    else
    {
        dineInCustomer *traversal = myFoodStore->nextDineInCustomer;
        while (traversal != NULL)
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "Walking Customer : " << traversal->cusotomer.name << endl;
            cout << "Age : " << traversal->cusotomer.age << endl;
            cout << "Item Name : " << traversal->cusotomer.itemName << endl;
            cout << "Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrders()
{

    cout << "The Take-Away Customers Are :" << endl;
    displayAllOrdersTakeAwayCustomers();

    cout << "The Dine-In Customers Are :" << endl;
    displayAllOrdersDineInCustomers();

    cout << "The Home Delivery Customers Are :" << endl;
    displayAllOrdersHomeDeliveryCustomers();
}

void totalbillofPendingOrders()
{
    takeAwayCustomer *p = myFoodStore->nextTakeAwayCustomer;
    while (p != NULL)
    {
        takeAway += p->cusotomer.bill;
        p = p->next;
    }
    dineInCustomer *q = myFoodStore->nextDineInCustomer;
    while (q != NULL)
    {
        dineIn += q->cusotomer.bill;
        q = q->next;
    }
    homeDeliveryCustomer *r = myFoodStore->nextHomeDeliveryCustomer;
    while (r != NULL)
    {
        homeDelivery += r->cusotomer.bill;
        r = r->next;
    }
    total = takeAway + dineIn + homeDelivery;

    cout << "The total bill of pending orders for Take-Away customers are : " <<  "₹" << takeAway << endl;
    cout << "The total bill of pending orders for Dine-In customers are : "  <<  "₹" << dineIn << endl;
    cout << "The total bill of pending orders for Delivery customers are : " <<  "₹" << homeDelivery << endl;
    cout << "The Total orders pending are : " <<  "₹" << total << endl;
}

double totalEarnings(servedCustomer *root){

    if(root){
        totalEarnings(root->left);
        servedTotal += root->bill;
        totalEarnings(root->right);
    }

    return servedTotal;
}

// making a graph for the available delivery options

//                           0              1             2        3          4           5
string deliveryPoints[] = {"FoodStore", "Gurgaon", "Krishna Nagar", "Pitampura", "Dwarka", "CP"};

// first value in the pair is vertex and second is the distance (weight) in KM

vector<vector<pair<int, int>>> deliveryMap = {

    // first value in the pair is vertex and second is the distance (weight) in KM
    {{1, 2}, {2, 3}, {3, 5}, {5, 4}}, //  0  (Food Store)
    {{0, 2}, {5, 1}},                 //  1  (Gurgaon)
    {{0, 3}, {3, 1}},                 //  2  (Krishna Nagar)
    {{0, 5}, {4, 2}, {5, 2}, {2, 1}}, //  3  (Pitampura)
    {{3, 2}, {5, 2}},                 //  4  (Dwarka)
    {{0, 4}, {1, 1},{3,2} ,{4, 2}}    //  5  (CP)

};

vector<int> dijkstra(int sourceNode)
{
    vector<int> distance(6, infinity);
    set<pair<int, int>> s;
    distance[sourceNode] = 0; // Food Store
    s.insert(make_pair(0, sourceNode));

    while (!s.empty())
    {
        auto top = *(s.begin());
        int u = top.first;  //   current weight
        int v = top.second; //  current vertex

        s.erase(s.begin());

        // traversing the adjacency list of v
        for (auto child : deliveryMap[v])
        {
            int childVertex = child.first;
            int childWeight = child.second;

            if (u + childWeight < distance[childVertex])
            {
                distance[childVertex] = u + childWeight;
                s.insert(make_pair(distance[childVertex], childVertex));
            }
        }
    }

    return distance;
}

int main()
{
    // making Food Store
    myFoodStore = new FoodStore;

    // setting the name
    myFoodStore->shopName = "The Foodies Palace";

    // setting the address
    myFoodStore->address = "Rajiv Chowk, Delhi";

    // Setting the menu
    myFoodStore->menu = new string[11]{"",
                                       "Veg Burger", "Crispy Corn",
                                       "Veg Pizza", "French Fries",
                                       "Paneer Sandwich", "Mojito",
                                       "Veg Subway", "Cold Coffee",
                                       "Butter Popcorn", "Chocolate Shake"};

    // setting the price

    myFoodStore->price = new int[11]{0, 100, 150, 240, 90, 60, 100, 150, 150, 90, 150};

    int option = -99;

    // now starting the main program
    do
    {

        cout << "-------------------------------------------------------------------------" << endl;
        cout << "---------------------------------" << myFoodStore->shopName << "-----------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;

        cout << "Located at " << myFoodStore->address << endl;
        cout << "Our Menu is as follows: " << endl;
        for (int i = 1; i <= 10; i++)
        {
            cout << i << ". " << myFoodStore->menu[i] << " - " << myFoodStore->price[i] << endl;
        }

        cout << "-------------------------------------------------------------------------" << endl;
        cout << "---------------------------------Operations------------------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;

        // Order placing
        cout << "1. Place order for Take-Away Customer" << endl;
        cout << "2. Place order for Home Delivery Customer" << endl;
        cout << "3. Place order for Dine-In Customer" << endl;

        // order serving
        cout << "4. Serve order for Take-Away Customer" << endl;
        cout << "5. Serve order for Home Delivery Customer" << endl;
        cout << "6. Serve order for Dine-In Customer" << endl;
        cout << "7. Serve All Orders " << endl;

        // Displaying orders
        cout << "8. Display all orders of Take-Away Customer" << endl;
        cout << "9. Display all orders of Home Delivery Customers" << endl;
        cout << "10. Display all orders of Dine-In Customers" << endl;
        cout << "11. Display all orders of all Customers" << endl;

        // Served orders
        cout << "12. Display all served Orders" << endl;
        cout << "13. Search Served Orders " << endl;
        cout << "14. Clear the Served Orders List " << endl;
        cout << "15. Display total bill of Pending Orders " << endl;
        cout << "16. Display the total Earnings of Served Orders " << endl;
        cout << "0.  EXIT " << endl;

        cout << "Enter your choice: ";

        cin >> option;

        // for taking input of Customer Details
        int age, quantity, itemIndex;
        double bill;
        string address;
        string name;

        switch (option)
        {
        case 1:
        { // placing order for take away customer
            cout << "Enter the name of the customer: ";
            cin >> name;
            cout << "Enter the age of the customer: ";
            cin >> age;
            cout << "Enter the option for the Item: ";
            cin >> itemIndex;
            cout << "Enter the quantity of selected Item: ";
            cin >> quantity;

            bill = quantity * myFoodStore->price[itemIndex];
            placeOrderTakeAwayCustomer(age, name, myFoodStore->menu[itemIndex], quantity, bill);
        }
        break;

        case 2:

        {                                               // placing order for Home Delivery customer
            vector<int> distanceFromShop = dijkstra(0); // As Zero is our Source Node

            int optionDelivery = -999;

            do
            {
                cout << "The delivery is available for following Areas : " << endl;
                for (int i = 1; i <= 5; i++)
                {
                    cout << i << ". " << deliveryPoints[i] << endl;
                }

                cout << "Enter your option :" << endl;
                cin >> optionDelivery;

            } while (!(optionDelivery >= 0 && optionDelivery <= 5));

            // setting the delivery address of the Customer
            address = deliveryPoints[optionDelivery];

            cout << "Enter the name of the customer: ";
            cin >> name;
            cout << "Enter the age of the customer: ";
            cin >> age;
            cout << "Enter the option for the Item: ";
            cin >> itemIndex;
            cout << "Enter the quantity of selected Item: ";
            cin >> quantity;

            int deliveryChargesPerKM = 9;
            int deliveryCharges = deliveryChargesPerKM * distanceFromShop[optionDelivery];
            bill = quantity * myFoodStore->price[itemIndex] + deliveryCharges;

            // distance from the shop
            int distanceFromTheShop = distanceFromShop[optionDelivery];
            placeOrderHomeDeliveryCustomer(age, name, myFoodStore->menu[itemIndex], quantity, bill, address, distanceFromTheShop, deliveryCharges);
        }
        break;

        case 3:
        { // placing order for Dine-in customer

            cout << "Enter the name of the customer: ";
            cin >> name;
            cout << "Enter the age of the customer: ";
            cin >> age;
            cout << "Enter the option for the Item: ";
            cin >> itemIndex;
            cout << "Enter the quantity of selected Item: ";
            cin >> quantity;

            bill = quantity * myFoodStore->price[itemIndex];
            placeOrderDineInCustomer(age, name, myFoodStore->menu[itemIndex], quantity, bill);
        }
        break;

        case 4:
            // serving order for Take Away customer
            serveOrderTakeAwayCustomer();
            break;

        case 5:
            // serving order for Home Delivery customer
            serveOrderHomeDeliveryCustomer();
            break;

        case 6:
            // serving order for Dine-in customer
            serveOrderDineInCustomer();
            break;

        case 7:
            // serving all orders
            serveAllOrders();
            break;

        case 8:
            // displaying all orders of Take-away customers
            displayAllOrdersTakeAwayCustomers();
            break;

        case 9:
            // displaying all orders of Home Delivery customers
            displayAllOrdersHomeDeliveryCustomers();
            break;

        case 10:
            // displaying all orders of Dine-in customers
            displayAllOrdersDineInCustomers();
            break;

        case 11:
            // displaying all orders of all customers
            displayAllOrders();
            break;

        case 12:
        { // displaying all served orders
            servedCustomer *r = displayAllServedOrders(root);
            if (!r)
                cout << "No Served Customer yet " << endl;
        }
        break;

        case 13:
        { // searching served orders
            cout << "Enter the name of the customer you want to search: " << endl;
            cin >> name;
            servedCustomer *searchedCustomer = search(root, name);
            if (searchedCustomer == NULL)
                cout << "No such Customer was Served " << endl;
            else
                display(searchedCustomer);
        }
        break;

        case 14:
        { // clearing the served Orders

            deleteAllServedCustomers(root);
            root = NULL;
        }
        break;

        case 15:
        { // pending orders bill in queue for all!

            // resetting the state of total orders
            takeAway = 0, dineIn = 0, homeDelivery = 0, total = 0; 
            totalbillofPendingOrders();
        }
        break;

        case 16:
        { // total earnings from served orders!

        // resetting thr state of total served orders
        servedTotal = 0;
        double totalx = totalEarnings(root);
        cout << "The Total Earnings are : " <<  "₹" << totalx << endl;
        }
        break;
        }

    } while (option != 0);

    cout << "Order Management System -- Terminated" << endl;
    cout << "Thank you for using our Services " << endl;

    return 0;
}