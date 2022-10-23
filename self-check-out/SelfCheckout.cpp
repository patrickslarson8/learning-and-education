#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

class Product
    // Product object that contain a description,
    // ID, and price. These properties
    // must be set when initialized using the constructor
    // with arguments (string ID, string description,
    // float price).
{
private:
    const std::string product_desc{};
    const std::string product_id{};
    const float product_price{};

    // assemble_price takes two input integers, one
    // representing dollars and the other cents.
    // This is to ensure fractional cents are not
    // store as a product price.
    float assemble_price(int dollars, int cents)
    {
        if ((cents > 99) || (cents < 0) || (dollars < 0))
        {
            std::cout << "Provided object price is outside of valid range. Price set to 0." << std::endl;
            return 0;
        }
        else
        {
            // cents must be cast to float before the division works
            float price = dollars + (static_cast<float>(cents) / 100);
            return price;
        }
    }

    // Truncates the description if it is longer
    // than 40 characters.
    std::string check_desc(std::string description)
    {
        if (description.length() > 40)
        {
            std::cout << "Product description shortened to first 40 characters." << std::endl;
            description = description.substr(0, 40);
        }
        return description;
    }

    // Truncates the ID if it is longer
    // than 20 characters.
    std::string check_ID(std::string ID)
    {
        if (ID.length() > 20)
        {
            std::cout << "Product ID shortened to first 20 characters." << std::endl;
            ID = ID.substr(0, 20);
        }
        return ID;
    }

public:
    // The ctor initializer list is the only way to
    // set the Product member values. To change member values
    // you will need to create a new Product object.
    // It is assumed that a knowledgable admin is creating the
    // products so minimal checking is performed.
    Product(std::string id, std::string description, int dollars, int cents)
        : product_id{ check_ID(id) }
        , product_desc{ check_desc(description) }
        , product_price{ assemble_price(dollars, cents) }
    {}

    // Prevent using the default ctor which would be wrong
    Product() = delete;

    // Overload the == operator so that you can
    // compare products. All description and name
    // must be equal to return true. Price is not
    // compared to allow for the price to change in the database.
    friend bool operator==(const Product& aRef, const Product& bRef);

    // Getters
    const std::string get_id() { return product_id; }
    const std::string get_desc() { return product_desc; }
    const float get_price() { return product_price; }
};

// Friend function declared outside of class scope
bool operator==(const Product& aRef, const Product& bRef)
{
    if ((aRef.product_desc == bRef.product_desc) &&
        (aRef.product_id == bRef.product_id))
    {
        return true;
    }
    else { return false; }
}

class ReceiptPrinter
// Class for outputting self-checkout receipt
// information to the console. This emulates
// printing to a paper receipt so the class
// also tracks how many lines of receipt paper
// are left. All public methods return a int status
// where 0: greater than 200 lines remaining,
// 1: less than 200 lines remaining,
// 2: out of receipt paper.
{
private:
    int lines_of_paper{ 500 };
    // Shows current stat of printer
    // 0: normal operation
    // 1: less than 200 lines of paper left
    // 2: no lines of paper left
    int status{ 2 };

    // updates the status member value according
    // to the number of lines of receipt paper
    // that can be printed.
    void update_status()
    {
        if (lines_of_paper <= 0) { status = 2; return; }
        else if (lines_of_paper <= 200) { status = 1; return; }
        else if (lines_of_paper > 200) { status = 0; return; }
    }

public:

    // Prints the total on the console.
    int print_subtotal(float subtotal)
    {
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout << "Subtotal: $" << subtotal << std::endl;
        --lines_of_paper;
        update_status();
        return status;
    }

    // Prints the subtotal on the console.
    int print_total(float total)
    {
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout << "Total: $" << total << std::endl;
        --lines_of_paper;
        update_status();
        return status;
    }

    int print_approval_code(int approval_code)
    {
        std::cout
            << "Card Approval Code: "
            << approval_code
            << std::endl;
        update_status();
        return status;
    }

    // Resets the internal counter to 500 lines of
    // receipt paper left to print on
    int reload_paper()
    {
        lines_of_paper = 500;
        update_status();
        return status;
    }

    // Prints the top header information to the console
    // and decrements the amount of paper left for
    // a receipt.
    int print_header()
    {
        std::cout
            << std::endl
            << "___________________________________________"
            << std::endl
            << "Here's your receipt!" << std::endl
            << " Item Number  |"
            << " Product ID          |"
            << " Product Description                     |"
            << " Unit Price      "
            << std::endl;

        --lines_of_paper;
        update_status();
        return status;
    };

    // Receives a Product object and prints its relevant
    // information to the console while tracking the
    // usage of receipt paper left to print on.
    int print_products(std::vector<Product> input_products)
    {
        // Loop through all the products in the vector argument
        for (int i = 0; i < input_products.size(); i++)
        {
            std::string input_product_desc = input_products[i].get_desc();
            std::string input_product_ID = input_products[i].get_id();
            float input_product_price = input_products[i].get_price();

            // Pad the input strings to their max
            // so that the printing looks uniform
            if (input_product_desc.length() < 40)
            {
                for (int i = input_product_desc.length(); i < 40; i++)
                {
                    input_product_desc.append(" ");
                }
            }
            if (input_product_ID.length() < 20)
            {
                for (int i = input_product_ID.length(); i < 20; i++)
                {
                    input_product_ID.append(" ");
                }
            }

            // Now print to the console
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            if (i < 9) { std::cout << " "; }
            std::cout
                << " " << i+1 << "           | "
                << input_product_ID << "| "
                << input_product_desc << "| "
                << input_product_price << std::endl;

            // And update the current status
            --lines_of_paper;
        }
        update_status();
        return status;
    }

    int print_thankyou()
    {
        std::cout
            << "Thank you for shopping with Pat's Mart!"
            << std::endl
            << "___________________________________________"
            << std::endl;
        --lines_of_paper;
        update_status();
        return status;
    }

};

class CashHandler
// Class to handle all cash transactions
// All currency is assumed to be USD.
// General usage: call accept_cash passing in 
// the purchase cost of the transaction. This will
// wait until all cash is fed into the machine
// before returning the current status of the
// cash reserve.
{
private:
    // The monetary value of all cash
    // to be used as change. All monetary values
    // are calculated as integers with but the last
    // two digits are assumed to be after the decimal
    // point. Example $4.99 is int 499.
    int total_reserve{};

    // Current change in the system is 
    // represented with a vector of format:
    // [quarters, dimes, nickels, pennies]
    std::vector<int> coin_reserve{ 0,0,0,0 };

    // Current cash in the system is 
    // represented with a vector of format:
    // [$20, $10, $5, $1]
    std::vector<int> cash_reserve{ 0,0,0,0 };

    // The monetary value of received customer
    // cash currently in the machine
    int total_repository{};

    // Store the values for the transaction
    int change_due{};
    int total_cost{};
    
    // Current state of cash handler
    // 0: normal operation
    // 1: less than $50 in reserve
    // 2: currently being serviced
    int status{1};

    const std::string accept_cash_menu[9] =
    {
        "Please select a number to simulate putting cash into the machine",
        "1: $20.00",
        "2: $10.00",
        "3: $5.00",
        "4: $1.00",
        "5: $0.25",
        "6: $0.10",
        "7: $0.05",
        "8: $0.01"
    };

    // Calculates the total reserve from the
    // change and cash reserve vectors
    // and updates the status
    void update_reserve()
    {
        // Cash
        total_reserve = 0;
        total_reserve += (cash_reserve[0] * 2000);
        total_reserve += (cash_reserve[1] * 1000);
        total_reserve += (cash_reserve[2] * 500);
        total_reserve += (cash_reserve[3] * 100);
        // Coins
        total_reserve += (coin_reserve[0] * 25);
        total_reserve += (coin_reserve[1] * 10);
        total_reserve += (coin_reserve[2] * 5);
        total_reserve += (coin_reserve[3] * 1);
        // return status based on total
        if (total_reserve < 5000) { status = 1; }
        else { status = 0; }
    }

public:
    // Getters convert currency into floats for use by other programs
    float get_total_reserve() { return ((float)total_reserve/100); }
    float get_total_repository() { return ((float)total_repository/100); }

    void empty_repository() { total_repository = 0; }

    // Used to accept cash from the customer
    // Takes a float that represents the amount
    // of cash to take from the customer.
    // Returns the status of the cash reserve.
    int accept_cash(const float total_cost)
    {
        // convert to cash handler's method of tracking money
        int total_remaining = (int)(total_cost *100);
        float difference = (total_cost * 100)- total_remaining;
        // Round out any error from float conversion
        if (difference >= 0.5)
        {
            total_remaining++;
        }
        int cash_input{};
        while (total_remaining > 0)
        {
            // Emulates feeding in case by getting an
            // input from the user and adjusting the
            // purchase cost appropriately.
            // Display the menu
            for (int i = 0; i < 9; i++)
            {
                std::cout << accept_cash_menu[i] << std::endl;
            }
            // Display current total
            std::cout 
                << "Total Cost: $" << total_cost << "   "
                << "Total Remaining: $"
                << (total_remaining/100) << "."
                << (total_remaining%100)
                << std::endl;
            std::cin >> cash_input;
            switch (cash_input)
            {
            case 1: total_remaining -= 2000; break;
            case 2: total_remaining -= 1000; break;
            case 3: total_remaining -= 500; break;
            case 4: total_remaining -= 100; break;
            case 5: total_remaining -= 25; break;
            case 6: total_remaining -= 10; break;
            case 7: total_remaining -= 5; break;
            case 8: total_remaining -= 1; break;
            }
        }

        // Once no more money is owed then run the
        // give_change routine and return the status
        change_due = -total_remaining;
        return give_change();
    }

    // Returns change to the customer
    // based on the change_due member variable
    // Will use the largest bills/coins available unless
    // there are none left. Alao cleans up variables
    // after cash transaction so must be called every time.
    int give_change()
    {
        // make empty vectors to compare to later
        const std::vector<int> empty_coin{ 0,0,0,0 };
        const std::vector<int> empty_cash{ 0,0,0,0 };

        //Print to the screen to emulate handing change back
        std::cout
            << "The following bills and coins were returned: "
            << std::endl;
            

        // Loop through payout routine while change due
        while (change_due != 0)
        {
            // Payout largest bills if they are available
            if (cash_reserve[0] != 0)
            {
                if ((change_due-2000) >= 0) 
                { 
                    change_due -= 2000;
                    --cash_reserve[0];
                    std::cout << "$20.00 returned" << std::endl;
                    continue;
                }
            }
            if (cash_reserve[1] != 0)
            {
                if ((change_due - 1000) >= 0)
                { 
                    change_due -= 1000; 
                    --cash_reserve[1];
                    std::cout << "$10.00 returned" << std::endl;
                    continue;
                }
            }
            if (cash_reserve[2] != 0)
            {
                if ((change_due - 500) >= 0)
                {
                    change_due -= 500; 
                    --cash_reserve[2]; 
                    std::cout << "$5.00 returned" << std::endl;
                    continue;
                }
            }
            if (cash_reserve[3] != 0)
            {
                if ((change_due - 100) >= 0)
                { 
                    change_due -= 100; 
                    --cash_reserve[3];
                    std::cout << "$1.00 returned" << std::endl;
                    continue;
                }
            }
            // Payout coins in same manner
            if (coin_reserve[0] != 0)
            {
                if ((change_due - 25) >= 0)
                {
                    change_due -= 25;
                    --coin_reserve[0];
                    std::cout << "$0.25 returned" << std::endl;
                    continue;
                }
            }
            if (coin_reserve[1] != 0)
            {
                if ((change_due - 10) >= 0)
                {
                    change_due -= 10; 
                    --coin_reserve[1];
                    std::cout << "$0.10 returned" << std::endl;
                    continue;
                }
            }
            if (coin_reserve[2] != 0)
            {
                if ((change_due - 5) >= 0)
                {
                    change_due -= 5; 
                    --coin_reserve[2];
                    std::cout << "$0.05 returned" << std::endl;
                    continue;
                }
            }
            if (coin_reserve[3] != 0)
            {
                if ((change_due - 1) >= 0)
                {
                    change_due -= 1;
                    --coin_reserve[3];
                    std::cout << "$0.01 returned" << std::endl;
                    continue;
                }
            }

            // if made it to this point then the machine is out
            // of change required to finish transaction
            std::cout << "Error: Not enough change! Please alert attendant." << std::endl;
            status = 1;
            return status;
        }

        // Once paid out, check amount remaining
        // and continue operations.
update_reserve();
        return status;
    }

    // Refills the cash reserve with the following
    // amounts: $20 - 4, $10 - 5, $5 - 6, $1 - 20,
    // $ 0.25 - 54, $0.10 - 50, $0.05 - 20, $0.01 - 50
    int fill_reserve()
    {
        cash_reserve = { 4, 5, 6, 20 };
        coin_reserve = { 54, 50, 20, 50 };
        update_reserve();
        return status;
    }
 
};

class CardHandler
{
private:
    unsigned long long int card_number{};
    unsigned int card_exp_month{};
    unsigned int card_exp_year{};

    // Code returned when card is used
    // 0: charged successfully
    // 1: charge declined
    int approval_code{};

    // Current status of the machine
    // 0: Completed successfully
    // 1: no connection
    // 2: charge declined
    int status{ 1 };

public:
    int get_approval_code() { return approval_code; }

    // Method to emulate connecting to
    // payment service
    void connect() { status = 0; }

    // Method to emulate getting card information
    // from a magnetic card reader. This sets
    // the member variables directly as checking
    // is performed in the get_authorization()
    // method.
    int get_card_information()
    {
        std::cout <<
            "Please enter your credit card number:"
            << std::endl;
        std::cin >> card_number;
        std::cout <<
            "Please enter your credit card expiration month:"
            << std::endl;
        std::cin >> card_exp_month;
        std::cout <<
            "Please enter your credit card expiration year:"
            << std::endl;
        std::cin >> card_exp_year;
        return 0;
    }

    // Checks the card information in the
    // member variables. Returns True if valid
    // or False if invalid.
    bool get_authorization()
    {
        if ((card_number > 9999999999999999)
            || (card_exp_month > 12)
            || (card_exp_year > 2100)
            || (card_exp_year < 2022))
        {
            std::cout << "Card Authorization Failed" << std::endl;
            return false;
        }
        else { return true; }
    }

    // Emulates charging the passed in value
    // to the customer's card.
    // Returns the status of the card machine
    // which is assumed to be operational (int 0)
    // since no change is required.
    int charge_card(float card_total)
    {
        std::cout << "$ " << card_total << " charged to card account."
            << std::endl;
        return 0;
    }
};

class MoneyHandler
// Class for tracking the current
// subtotal, applying the local tax,
// and handling the transaction.
// Uses two subclasses CashHandler and
// CardHandler. Assumes USD currency.
{
private:
    int payment_type{};
    float tax_rate{ 0.05 };
    float subtotal{};
    float total{};

    // Status of the money handler
    // 0: normal operation
    // 1: attention required, see error log
    // 2: currently being serviced
    int status{};
    CashHandler cash_handler{};
    CardHandler card_handler{};

    // Takes the current subtotal,
    // calculates the tax, and updates
    // the total. Rounds to two decimals
    // of precision
    void update_total(void)
    {
        // Multiply by 100 to get two decimals places
        // to the left of the decimal before rounding
        float var = (subtotal * tax_rate) * 100;
        var = round(var);
        total = (var / 100) + subtotal;
    }
public:
    // Ctor initializes cash and card handlers.
    // The cash handler is assumed to be full on
    // initialization.
    MoneyHandler()
    {
        cash_handler.fill_reserve();
        card_handler.connect();
    }

    // Getters
    int get_status() { return status; }
    int get_payment_type() { return payment_type; }
    int get_approval_code() { return card_handler.get_approval_code(); }
    float get_subtotal() { return subtotal; }
    float get_total() { return total; }

    // Checks if input is negative and if
    // if not adds it to the subtotal member
    void add_to_subtotal(float product_price)
    {
        if (product_price < 0)
        {
            std::cout <<
                "Cannot add negative value to subtotal. No value changed."
                << std::endl;
        }
        else
        {
            subtotal += product_price;
            update_total();
        }
    }

    // Emulates a screen where the customer can
    // select their payment method and sets that
    // in the payment_type member variable
    void set_payment_type()
    {
        int chosen_payment_method{};
        // Display a menu and get payment method from
        // customer
        std::cout <<
            "Please select your preferred payment type"
            << std::endl << "1: Cash"
            << std::endl << "2: Credit Card"
            << std::endl << "3: Debit Card"
            << std::endl;

        // Wait for a response
        std::cin >> chosen_payment_method;
        if ((chosen_payment_method < 1) || (chosen_payment_method > 3))
        {
            std::cout <<
                "Invalid payment method chosen. Valid payment methods are: 1. Cash  2. Credit Card  3. Debit Card"
                << std::endl;
        }
        else { payment_type = chosen_payment_method; }
    }

    // Function to get payment from customer
    // using the currently set payment method.
    // If none is set then it will get it.
    int execute_transaction()
    {
        int sub_handler_status{};
        // Based on the currently set payment type
        // run the transaction using the appropriate
        // method
        switch (payment_type)
        {
        case 0: 
            set_payment_type();
            break;
        case 1: 
            sub_handler_status = cash_handler.accept_cash(total);
            // set appropriate status based on subhandler
            // In this case, they match
            status = sub_handler_status;
            // If it is anything but 0, the machine needs to be serviced
            //if (status != 0) { cash_handler.fill_reserve(); }
            break;
        case 2: 
            // Get card information and authorize charge
            card_handler.get_card_information();
            if (card_handler.get_authorization())
            {
                sub_handler_status = card_handler.charge_card(total);
            };
            status = sub_handler_status;
            break;
        case 3:
            card_handler.get_card_information();
            if (card_handler.get_authorization())
            {
                sub_handler_status = card_handler.charge_card(total);
            };
            sub_handler_status = card_handler.charge_card(total);
            status = sub_handler_status;
            break;
        }

        // clean up variables
        subtotal = 0;
        total = 0;
        return status;
    }

    void refill()
    {
        std::string null_str{};
        std::cout
            << "Machine is currently out of service." << std::endl
            << "Please press any key followed by enter to refill machine."
            << std::endl;
        std::cin >> null_str;
        cash_handler.fill_reserve();
    }
};

class SelfCheckOut
// Application object to run the self check-out and
// manage the individual objects. Must be passed
// a vector on initialization that will populate
// the product database.
{
private:
    // Vector of products so customers can
    // look up objects.
    const std::vector<Product> product_lookup{};
    std::vector<Product> cust_cart{};
    // instantiate required objects
    MoneyHandler money_handler{};
    ReceiptPrinter receipt_printer{};
    // Current system status
    // 0: normal operation
    // 1: service required
    // 2: currently in service
    int status{};

    // Utility function used to add a
    // product to the customer cart.
    // Returns true if successful or false
    // if product could not be found
    bool is_in_database(Product item)
    {
        for (int i = 0; i < product_lookup.size(); i++)
        {
            if (product_lookup[i] == item)
            {
                return true;
            }
        }
        return false;
    }

    // Takes the customer cart, subtotal, and total
    // as arguments and simulates printing them to 
    // a paper receipt. Returns the status of the receipt
    // printer.
    int print_receipt(std::vector<Product> cart, float subtotal, float total)
    {
        int payment_type = money_handler.get_payment_type();

        //Build receipt
        receipt_printer.print_header();
        receipt_printer.print_products(cart);
        receipt_printer.print_subtotal(subtotal);
        receipt_printer.print_total(total);

        // Return approval code if card used
        if ((payment_type == 2) || (payment_type == 3))
        {
            receipt_printer.print_approval_code(money_handler.get_approval_code());
        }
        return receipt_printer.print_thankyou();
    }

    void alert_control_center()
    {
        std::cout
            << "EMPLOYEE ALERT: SELF CHECK OUT CHANGE RESERVES ARE BELOW $50.00 OR RECEIPT PRINTER IS OUT OF PAPER."
            << std::endl
            << "PLEASE SERVICE IMMEDIATELY"
            << std::endl;
    }

public:
    // instantiate the product lookup table
    // This is hardcoded into the constructor but
    // could be changed to pull from a database
    SelfCheckOut(std::vector<Product> initializing_database)
        : product_lookup{ initializing_database }
    { }

    // Prevent instantiation without a product database
    SelfCheckOut() = delete;

    //Getters
    std::vector<Product> get_products() { return product_lookup; }
    std::vector<Product> get_cart() { return cust_cart; }
    int get_status() { return status; }
    float get_subtotal() { return money_handler.get_subtotal(); }
    float get_total() { return money_handler.get_total(); }

    // Takes an item as an argument and checks that
    // it is a local store item before adding to the cart
    // returns the current customer cart, subtotal, and total
    void add_to_cart(Product item)
    {
        if (is_in_database(item))
        {
            cust_cart.push_back(item);
            money_handler.add_to_subtotal(item.get_price());
            std::cout << "Item successfully added to cart!" << std::endl;
        }
        else
        {
            std::cout << "Item not found." << std::endl;
        }
    }
    
    // Stores the subtotal and total from the money
    // handler, lets money handler finish transaction
    // and then prints a receipt to the screen. Returns an int of
    // the most constrictive status (e.g. low paper, low change).
    // Alerts control center if necessary.
    int check_out()
    {
        // store totals before they are wiped
        float sub = money_handler.get_subtotal();
        float tot = money_handler.get_total();

        // begin the money transaction process
        money_handler.set_payment_type();
        int money_stat = money_handler.execute_transaction();
        int receipt_stat = print_receipt(cust_cart, sub, tot);

        // Empty cart
        cust_cart.clear();

        // Alert control if needed
        if ((money_stat > 0) || (receipt_stat > 0))
        {
            alert_control_center();
            refill();
        }

        //return most constrictive status
        if (money_stat > receipt_stat) { return money_stat; }
        else { return receipt_stat; }
    }

    // Refills the receipt printer and cash reserve
    void refill()
    {
        money_handler.refill();
        receipt_printer.reload_paper();
        status = 0;
    }
};

class CustInterface
// Customer interface for self check out
// takes a vector representing the local 
// store database as an argument
// and prints choices to the console.
{
private:
    SelfCheckOut sco;

    // Displays a list of options on the screen and returns
    // the customer's selection.
    // 1: scan product
    // 2: search product
    // 3: check out
    int display_input_options()
    {
        int selection{};
        
        // Print options to screen
        // Note that the admin menu is accessed by entering 4
        std::cout
            << std::endl
            << "*******************************************"
            << std::endl
            << "Welcome to Pat's Mart!" << std::endl;
        // Show cart and current total if anything present
        if (sco.get_cart().size() > 0)
        {
            std::cout << "Your current cart contains:" << std::endl;
            display_cart(sco.get_cart());
            std::cout << "Subtotal: $" << sco.get_subtotal() << std::endl;
            std::cout << "Total: $" << sco.get_total() << std::endl;
        }
        // Show menu
        std::cout
            << "Please select an option to get started."
            << std::endl
            << "1: Scan product" << std::endl
            << "2: Search for product" << std::endl
            << "3: Check out" << std::endl
            << "******************************************"
            << std::endl;

        // Return the input
        std::cin >> selection;
        return selection;
    }

    // Prints out the current customer cart to the screen
    void display_cart(std::vector<Product> cart)
    {
        // Loop through all the products in the vector argument
        for (int i = 0; i < cart.size(); i++)
        {
            std::string input_product_desc = cart[i].get_desc();
            std::string input_product_ID = cart[i].get_id();
            float input_product_price = cart[i].get_price();

            // Pad the input strings to their max
            // so that the printing looks uniform
            if (input_product_desc.length() < 40)
            {
                for (int i = input_product_desc.length(); i < 40; i++)
                {
                    input_product_desc.append(" ");
                }
            }
            if (input_product_ID.length() < 20)
            {
                for (int i = input_product_ID.length(); i < 20; i++)
                {
                    input_product_ID.append(" ");
                }
            }

            // Now print to the console
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            // an extra character to offset the id being only one digit
            if (i < 9) { std::cout << " "; }
            std::cout
                << " " << i+1 << "           | "
                << input_product_ID << "| "
                << input_product_desc << "| "
                << input_product_price << std::endl;
        }
    }

    // Displays all products in database for customer to select
    // to emulate scanning an object then adds the selction
    // to the customer's cart. No arguments, returns void.
    void scan_product()
    {
        int selection = 0;

        // Displays all items in database
        std::cout <<
            "Please select the option corresponding to the item you have scanned"
            << std::endl;
        display_cart(sco.get_products());

        // Gets customer input, which emulates them scanning
        // the item and adds that to the cart
        std::cin >> selection;
        sco.add_to_cart(sco.get_products()[selection-1]);
    }

    // Help message for when machine needs service
    // It is assumed when the machine is serviced that the
    // printer and cash reserve are both refilled.
    void display_help_needed()
    {
        std::cout <<
            "Machine requires attention. Please wait for help."
            << std::endl
            << "Press any key to continue."
            << std::endl;
        // Pause here for attendent to fix the problem
        std::cin;
        // and reset the status.
        refill_machine();
    }

    // refills printer and cash reserve
    void refill_machine()
    {
        sco.refill();
    }

    // Allows a customer to search for a product
    // by its description and then choose if they
    // want to add that to their cart. No arguments
    // return void.
    void search_product()
    {
        // Get search term from customer
        std::string search_input{};
        std::cout << "Search product description: " << std::endl;
        std::cin >> search_input;

        // Header for search window
        std::cout
            << "All products matching your search term are below."
            << std::endl
            << "Enter the number to the left of the product to add it to your cart."
            << std::endl
            << "Enter 0 to return without adding any items to your cart."
            << std::endl;

        // Loop through all available products looking
        // for a match
        std::string product_description{};
        for (int i = 0; i < sco.get_products().size(); i++)
        {
            product_description = sco.get_products()[i].get_desc();
            if (product_description.find(search_input) != std::string::npos)
            {
                // if a match is found display them
                std::string input_product_desc = sco.get_products()[i].get_desc();
                std::string input_product_ID = sco.get_products()[i].get_id();
                float input_product_price = sco.get_products()[i].get_price();

                // Pad the input strings to their max
                // so that the printing looks uniform
                if (input_product_desc.length() < 40)
                {
                    for (int i = input_product_desc.length(); i < 40; i++)
                    {
                        input_product_desc.append(" ");
                    }
                }
                if (input_product_ID.length() < 20)
                {
                    for (int i = input_product_ID.length(); i < 20; i++)
                    {
                        input_product_ID.append(" ");
                    }
                }

                // Now print to the console
                std::cout << std::fixed;
                std::cout << std::setprecision(2);
                std::cout
                    << " " << i+1 << "           | "
                    << input_product_ID << "| "
                    << input_product_desc << "| "
                    << input_product_price << std::endl;
            }
        }

        // Wait for customer to input the product they want to add
        int selection{};
        std::cin >> selection;
        if (selection != 0)
        {
            sco.add_to_cart(sco.get_products()[selection-1]);
        }
    }

public:
    CustInterface(std::vector<Product> local_database)
        :sco{ local_database }
    {}

    // Prevent creation of empty object
    CustInterface() = delete;

    void run()
    {
        while (true)
        {
            while (sco.get_status() == 0)
            {
                switch (display_input_options())
                {
                case 1: scan_product(); break;
                case 2: search_product(); break;
                case 3: sco.check_out(); break;
                case 4: sco.refill(); break;
                }
            }
            display_help_needed();
        }
    }

};

int main()
{
    // update UML diagram

    // Create the products for the local store
    Product meat01{ "Meat01", "T-Bone Steak", 7, 99 };
    Product meat02{ "Meat02", "Tyson Fresh Chicken Wings", 10, 00 };
    Product icecream01{ "Icecream01", "Chocolate Ice Cream", 2, 50 };
    Product icecream02{ "Icecream02", "Vanilla Ice Cream", 2,50 };
    Product corn01{ "Corn01", "Fresh Sweet Corn", 2, 00 };
    Product casewater01{ "Casewater01", "24 Bottles 16-Oz of Deer Park Water", 4, 99 };
    Product potatochips01{ "Potatochips01","Plain Potate Chips", 2, 00 };
    Product potatochips02{ "Potatochips02","Green Onion Potato Chips", 2, 00 };
    Product donuts01{ "Donuts01","Glazed Donuts One-Dozen", 4, 99 };
    Product sausage01{ "Sausage01","8-Sausage Pack", 4, 99 };
    Product eggs01{ "Eggs01","Dozen Eggs", 3, 00 };
    Product milk01{ "Milk01", "Gallon Milk", 4, 00 };

    // Load products into a vector so they can
    // be used to instantiate a SelfCheckOut object.
    std::vector<Product> local_store_database{ meat01, meat02,icecream01,
        icecream02, corn01, casewater01, potatochips01, potatochips02,
        donuts01, sausage01, eggs01, milk01 };

    // Create the interface which will spawn
    // the necessary objects
    CustInterface customer_interface(local_store_database);
    customer_interface.run();
    std::cout << "Test";
};