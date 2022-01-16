#include <iostream>

using std::cout;
using std::cin;
using std::endl;
#include <iomanip>

using std::setprecision;
using std::fixed;
#include <sstream>

using std::istringstream;
using std::ostringstream;
#include <string>

using std::string;
#include <vector>

using std::vector;
#include <algorithm>

using std::find;
using std::min_element;
#include <set>

using std::set;
#include <cctype>

//Make struct for store type
struct Store {
  string name = "none";
  string location = "none";
  vector < string > items;
  vector < string > item_names;
  vector < int > item_quantities;
  vector < double > item_prices;
  void separate();
};

//Make struct for shopping list type
struct Shopping {
  int number_of_items;
  vector < string > items;
  vector < string > item_names;
  vector < int > item_quantities;
  void separate();
};

//Method to split a store into its parts
void store_split(const string & s, vector < string > & v, char chr) {
  istringstream iss(s);
  string item;
  while (getline(iss, item, chr)) {
    v.push_back(item);
  }
}

//Method to split a shopping list into its parts
void shopping_split(const string & s, vector < string > & v, char chr) {
  istringstream iss(s);
  string item;
  getline(iss, item, chr);
  v.push_back(item);
  getline(iss, item);
  v.push_back(item);
}

//Shopping list member function to separate shopping list into quantity
//and item name using shopping list split method
void Shopping::separate() {
  for (string s: items) {
    vector < string > fields;
    shopping_split(s, fields, ' ');
    istringstream quantity(fields.at(0));
    int quantity_int;
    quantity >> quantity_int;
    this -> item_names.push_back(fields.at(1));
    this -> item_quantities.push_back(quantity_int);
  }
}

//Store member function to separate store into vector of item names,
//vector of quantitites, and vector of prices using store split method
void Store::separate() {
  for (string s: items) {
    vector < string > fields;
    store_split(s, fields, ',');
    istringstream quantity(fields.at(1));
    fields.at(2).erase(0, 1);
    istringstream price(fields.at(2));
    int quantity_int;
    double price_double;
    quantity >> quantity_int;
    price >> price_double;
    price_double *= 100;
    int price_int = price_double;
    this -> item_names.push_back(fields.at(0));
    this -> item_quantities.push_back(quantity_int);
    this -> item_prices.push_back(price_int);
  }
}

//Method to print all stores' information. This includes:
//Number of stores
//How many distinct items each store has
void print_store_info(const vector < Store > & stores) {
  cout << "Store Related Information (ordered by in-file order):" << endl;
  cout << "There are " << static_cast < int > (stores.size()) 
  << " store(s)." << endl;
  for (auto store: stores) {
    cout << store.name << " has " << static_cast < int > (store.items.size())
    << " distinct items." << endl;
  }
  cout << endl;
}

//Method to print items' information. This includes:
//Number of distinct items there are across all stores
//Number of each distinct item in total across all stores
void print_item_info(const vector < Store > & stores) {
  set < string > all_items;
  for (auto store: stores) {
    for (auto item: store.item_names) {
      all_items.insert(item);
    }
  }
  cout << "Item Related Information (ordered alphabetically):" << endl;
  cout << "There are " << static_cast < int > (all_items.size())
  << " distinct item(s) available for purchase." << endl;
  for (auto item: all_items) {
    int quantity = 0;
    for (auto store: stores) {
      auto mark = find(store.item_names.begin(), store.item_names.end(), item);
      if (mark != store.item_names.end()) {
        quantity += 
        store.item_quantities.at(static_cast < int > 
        (mark - store.item_names.begin()));
      }
    }
    cout << "There are " << quantity << " " << item << "(s)." << endl;
  }
  cout << endl;
}

//Method to print shopping list
void print_shopping(const vector < Store > & stores,
  const Shopping & shopping) {
  cout << "Shopping:" << endl;
  double result = 0; //Final total of purchase
  for (auto itr = shopping.item_names.begin(); 
  itr != shopping.item_names.end(); ++itr) {
    int total = 0; //Total of each item purchase
    ostringstream purchases;
    int amount = shopping.item_quantities.at(static_cast < int > 
    (itr - shopping.item_names.begin()));
    cout << "Trying to order " << amount << " " << * itr << "(s)." << endl;
    vector < string > store_names;
    vector < string > locations;
    vector < int > prices;
    vector < int > quantities;
    for (auto store: stores) { //Loop over every store
      auto mark = find(store.item_names.begin(), 
      store.item_names.end(), * itr);
      if (mark != store.item_names.end()) { 
        //If the store has the item, execute statement below
        store_names.push_back(store.name);
        locations.push_back(store.location);
        prices.push_back(store.item_prices.at(static_cast < int >
        (mark - store.item_names.begin())));
        quantities.push_back(store.item_quantities.at(static_cast < int > 
        (mark - store.item_names.begin())));
        //Above, we record the store name, the location, 
        //the price, and the quantity in separate vectors
      }
    }
    cout << static_cast < int > (store_names.size()) << " store(s) sell " 
    << * itr << "." << endl;
    while (amount > 0) { 
    //Condition is amount > 0 because we will keep subtracting until
    //we have obtained the necessary amount of the item
      if (quantities.at(static_cast < int > 
      (min_element(prices.begin(), prices.end()) - prices.begin())) > amount) {
        //Execute the statements below if the quantity offered at a store is 
        //greater than the amount of the item we still need
        total += amount * * min_element(prices.begin(), prices.end()); 
        //Add to the total of the item purchase
        purchases << "Order " << amount << " from " 
        << store_names.at(static_cast < int > 
        (min_element(prices.begin(), prices.end()) - prices.begin())) 
        << " in " <<
        locations.at(static_cast < int > 
        (min_element(prices.begin(), prices.end()) - prices.begin())) << endl;
      } else {
        total += quantities.at(static_cast < int > 
        (min_element(prices.begin(), prices.end()) - prices.begin())) * 
        * min_element(prices.begin(), prices.end());
        purchases << "Order " << quantities.at(static_cast < int > 
        (min_element(prices.begin(), prices.end()) - prices.begin())) 
        << " from " <<
        store_names.at(static_cast < int > 
        (min_element(prices.begin(), prices.end()) - prices.begin())) 
        << " in " <<
        locations.at(static_cast < int > 
        (min_element(prices.begin(), prices.end()) - prices.begin())) << endl;
      }
      string erase_store = store_names.at(static_cast < int > 
      (min_element(prices.begin(), prices.end()) - prices.begin()));
      string erase_location = locations.at(static_cast < int > 
      (min_element(prices.begin(), prices.end()) - prices.begin()));
      int erase_quantity = quantities.at(static_cast < int > 
      (min_element(prices.begin(), prices.end()) - prices.begin()));
      amount -= quantities.at(static_cast < int > 
      (min_element(prices.begin(), prices.end()) - prices.begin())); //Subtract how many of the item we obtained
      store_names.erase(find(store_names.begin(), store_names.end(), erase_store));
      locations.erase(find(locations.begin(), locations.end(), erase_location));
      quantities.erase(find(quantities.begin(), quantities.end(), erase_quantity));
      prices.erase(min_element(prices.begin(), prices.end()));
      //Above, we erase the necessary values from the four vectors (store name,
      //location, quantity, and price) since we are done using the store
      if (static_cast < int > (store_names.size()) == 0) {
        break; 
        //Break out of the while loop if we still need 
        //more of the item, but no more stores have it available
      }
    }
    cout << "Total price: $" << fixed << setprecision(2) 
    << static_cast < double > (total) / static_cast < double > (100) << endl;
    cout << purchases.str();
    cout << endl;
    result += total; //Add the item total to the purchase total
  }
  result /= 100; //Since the purchase total is in cents, we divide by 100
  cout << "Be sure to bring $" << fixed << setprecision(2) << result 
  << " when you leave for the stores." << endl;
}

int main() {
  ostringstream store_inventory;
  ostringstream shopping_list;
  string input;
  getline(cin, input);
  store_inventory << input << endl;
  while (getline(cin, input)) {
    if (input == "") {
      store_inventory << endl; 
      //We add a new line to the store inventory 
      //stream when there is a empty string
    } else if (isdigit(input.at(0))) {
      shopping_list << input << endl;
      break; 
      //Once we get to the shopping list we break 
      //and stop adding to the store inventory stream
    } else {
      store_inventory << input << endl; 
      //For all other cases, we just add the 
      //line to the store inventory stream as is
    }
  }
  while (getline(cin, input)) {
    shopping_list << input << endl; 
    //The rest of the input is the shopping list
  }
  istringstream inventory_iss(store_inventory.str());
  istringstream list_iss(shopping_list.str()); 
  //Create istringstreams for the shopping list and store inventory for input
  getline(inventory_iss, input); 
  //Ignore the first line of inventory as it just says the number of stores
  vector < Store > stores;
  for (int i = 0; i < static_cast < int > 
  (store_inventory.str().at(0) - '0'); i++) {
    Store my_store;
    getline(inventory_iss, my_store.name); //The first line is the name
    getline(inventory_iss, my_store.location); //Second line is location
    while (getline(inventory_iss, input)) {
      if (input == "") {
        break; 
        //We break out of the while loop when there is an 
        //empty string because that indicates the next store
      } else {
        my_store.items.push_back(input); 
        //Add each item (string of name,quantity,$price) 
        //to the store's item vector
      }
    }
    my_store.separate();
    stores.push_back(my_store); 
    //Add the store to the vector of all of the stores
  }
  print_store_info(stores);
  print_item_info(stores);
  Shopping shopping; //Create an instance of the shopping type
  shopping.number_of_items = static_cast < int > 
  (shopping_list.str().at(0) - '0'); 
  //First char of shopping list is the # of items
  getline(list_iss, input); //Ignore first line that states number of items
  while (getline(list_iss, input)) {
    if (input != "") {
      shopping.items.push_back(input); 
      //Add each item (quantity item_name) 
      //to a the shopping list's items vector
    }
  }
  shopping.separate();
  print_shopping(stores, shopping);
}