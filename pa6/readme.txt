Hua Yang
Alexio Mota

Readme:

The book orders are all stored in a "Queue" struct, which keeps track of the book title, price, customer's id, category, and a pointer to the next element struct. For each elements in the queue, we create a producer thread for it.

Each customer thread calls a "Customers" struct, which keeps track of the customer's name, id, balance, address, and two "Trans" struct. The "Trans" struct keeps track of the orders, one for successful orders and another for failure.

For the threads, we incorporate mutexes to lock and unlock resources.

The outputed result is stored in finalreport.txt
