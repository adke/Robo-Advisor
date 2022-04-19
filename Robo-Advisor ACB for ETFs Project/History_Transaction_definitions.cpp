#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction (std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned int year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount) {

        symbol = ticker_symbol;
        day = day_date;
        month = month_date;
        year = year_date;
        shares = number_shares;
        amount = trans_amount;

        if(buy_sell_trans == true){
          trans_type = "Buy";
        }
        else{
          trans_type = "Sell";
        }

        trans_id = assigned_trans_id;
        assigned_trans_id ++;
        p_next = nullptr;

     acb = 0;
     acb_per_share = 0;
     share_balance= 0;
     cgl = 0;

}


// Destructor
// TASK 1
//

Transaction::~Transaction(){

}


// Overloaded < operator.
// TASK 2
//

bool Transaction::operator<( Transaction const &other ){
//can access all private variables and methods from 'other' parameter because we are in the transaction class and other is also taking a data type of 'transaction'
// this a pointer keyword (the '->' notation must be used)
// -> notation means:
//Transaction *test_transaction {new Transaction{...}};
//the following two statements are the same:
//*test_transaction.get_trans_type(); //this test_transaction
//test_transaction -> get_trans_type(); //this test_transaction
//need two operands to compare the two transactions
//right operand is 'other' parameter
//left operand is 'this' current transaction (or use member variables directly) - implicit
//if implicit is sooner, return true

if(this -> year < other.year){
  return true;
}
else if((this-> year == other.year) && (this-> month < other.month)){
  return true;
}
else if((this-> year == other.year) && (this-> month == other.month) && (this-> day < other.day)){
  return true;
}

else{
 return false;
}

}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History(){

p_head = nullptr;

}

// Destructor
// TASK 3
//

History::~History(){

while(p_head != nullptr){
  Transaction *p_old_head{p_head};
  p_head = p_head->get_next();

  delete p_old_head;
  p_old_head = nullptr;
}

}



// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
   
   ece150::open_file();

   while(ece150::next_trans_entry()==true){

    insert(new Transaction{ece150::get_trans_symbol(),ece150::get_trans_day(),ece150::get_trans_month(),
     ece150::get_trans_year(),ece150::get_trans_type(), ece150::get_trans_shares(),ece150::get_trans_amount()});
    
   }

   ece150::close_file();
}



// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){


if(p_head == nullptr){

this ->p_head = p_new_trans;
}
else{

Transaction * p_traverse{p_head};

while (p_traverse ->get_next() != nullptr)
{
  p_traverse = p_traverse->get_next();
}

p_traverse->set_next(p_new_trans);

}


}


// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

void History::sort_by_date(){

//followed david lau's session
  Transaction *p_front{nullptr};
  Transaction *p_head_new{nullptr};

  while(p_head != nullptr){

    //isolating the front node
    p_front = p_head;
    p_head = p_head->get_next();
    p_front -> set_next(nullptr);

    //inserting the node into the sorted list
    if(p_head_new == nullptr){
      p_head_new = p_front;
      p_front = nullptr;
    }
    else{

       if(*p_front < *p_head_new){ //front case
        p_front ->set_next(p_head_new);
        p_head_new = p_front;
      }
      else{

      Transaction *p_traverse{p_head_new}; 
      while ((p_traverse->get_next()!= nullptr) && *(p_traverse->get_next()) < *p_front)
      {
        p_traverse = p_traverse->get_next();
      }

      //do the insert
      p_front ->set_next(p_traverse ->get_next());
      p_traverse ->set_next(p_front);
      p_front = nullptr;
      p_traverse = nullptr;

      }
      
    } 

  }

  p_head = p_head_new;

  
}


// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl(){

Transaction *p_old_head{p_head};
double shares{0};
double amount{0.0};
double acb_share{0.0};
double acb{0};
double share_balance{0};
double previous_acb_share{0};
double previous_acb{0};
double acb_sell{0};
double cgl{0};
double current_acb{0};




while(p_old_head != nullptr)
{
    
 if(p_old_head->get_trans_type()==true){ //buy

  amount += p_old_head->get_amount();
  p_old_head->set_acb(amount);

  shares += p_old_head->get_shares();
  p_old_head->set_share_balance(shares);

  acb = p_old_head-> get_acb();

  previous_acb = acb;
  share_balance = p_old_head->get_share_balance();

  
  acb_share = (acb)/(share_balance);
  p_old_head -> set_acb_per_share(acb_share);

  previous_acb_share = p_old_head->get_acb_per_share();
  

  p_old_head = p_old_head->get_next(); 
  

  }
  else{ //sell

      shares -= p_old_head->get_shares();
      p_old_head->set_share_balance(shares);
      
      share_balance = p_old_head->get_share_balance();
      
      acb_sell = (previous_acb)-((p_old_head->get_shares())*previous_acb_share);
      p_old_head -> set_acb(acb_sell);

      cgl = (p_old_head->get_amount())-((p_old_head->get_shares())*previous_acb_share);
      p_old_head -> set_cgl(cgl);

      current_acb = p_old_head->get_acb();

      amount = acb_sell;

      previous_acb = acb_sell;

      
      p_old_head -> set_acb_per_share(acb_share);

      previous_acb_share = acb_share;
      

      p_old_head = p_old_head->get_next(); 

  } 
  
  
} 

}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl(unsigned int year){
  Transaction *p_old_head{p_head};
  double sum{0};

  while (p_old_head != nullptr)
  {
   if (p_old_head->get_year()== year){
     sum += p_old_head->get_cgl();
   }


   p_old_head = p_old_head ->get_next();

  }

  return sum;
  
}


// print() Print the transaction history.
//TASK 9
//

void History::print(){

std::cout << "========== BEGIN TRANSACTION HISTORY ============" <<std::endl;

 Transaction *p_old_head{p_head};
 
 while(p_old_head != nullptr){

  p_old_head -> print();

  p_old_head = p_old_head->get_next();

 }

 std::cout <<"========== END TRANSACTION HISTORY ============" <<std::endl;




 

  
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
