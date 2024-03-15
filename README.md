Project Overview:
The project operates within an infinite while loop, awaiting user commands from the keyboard. These commands, along with their functionalities, are detailed below:

register: Registers a new account by collecting user input, writing it into a JSON file, composing a POST message, and sending it to the server. Displays the server's response.

login: Handles user authentication by collecting input, saving the cookie, sending the message to the server, and displaying the response.

enter_library: Requests access to the library by sending a GET message with the saved cookie, receiving a response, and displaying it.

get_books: Retrieves and displays summary information about books by sending a GET message with the saved cookie to demonstrate access.

get_book: Displays detailed information about a specific book by reading its ID from the keyboard, sending a GET message with the ID to the server, and displaying the response.

add_book: Adds a new book to the library by reading input data, writing it into a JSON file, composing and sending the message to the server, and displaying the response.

delete_book: Deletes a book from the library by reading its ID from the keyboard, composing a delete message with the ID, sending it to the server, and displaying the response.

logout: Logs out by closing the connection.

exit: Exits the application by terminating program execution.

Implementation Details:
Input handling: Utilizes fgets to read input from the keyboard, addressing the newline character issue by copying the variable and decrementing it by 1.
Server Communication: Communicates with the server via HTTP requests and processes responses.
Response Parsing: Utilizes the cJSON library (cJSON.c and cJSON.h files) to parse server responses.
