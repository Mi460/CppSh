/*
This is a linked list struct
Used for lists of integers
*/
struct Chain {
	Chain* last;
	int value;
	Chain* next;
};

/*
These are user flags
(~= groups)
*/
#include <map>
#include <iostream>

std::map<std::string, int> GROUPS = {
	{"U_WHEEL", 1 << 0},
	{"U_USER", 1 << 1},
	{"U_SERIAL", 1 << 2},
	{"U_READ", 1 << 3},
	{"U_WRITE", 1 << 4},
};

/*
This is the console class
Used for I/O handling over the console
*/
class Console
{
	private:
		std::string DEFAULT = "\e[1;0m";
		std::string RED = "\e[1;31m";
		std::string YELLOW = "\e[1;33m";
	public:
		std::string END = "\n";

		// Constructor
		Console();

		// Functions
		void print(std::string, std::string);
		void warn(std::string);
		void tip(std::string);

		std::string input(std::string, std::string, std::string);
};

Console::Console()
{
	this->tip("Initialized Console class successfully.");
}

void Console::print(std::string text, std::string color = "")
{
	std::cout << color << text << this->DEFAULT << this->END;
}

void Console::warn(std::string text)
{
	this->print(text, this->RED);
}

void Console::tip(std::string text)
{
	this->print(text, this->YELLOW);
}

std::string Console::input(std::string text, std::string end = "", std::string color = "")
{
	std::string END = this->END;
	this->END = end;
	this->print(text, color);
	this->END = END;
	std::string temporary;
	std::cin >> temporary;
	return temporary;
}

/*
This is the file class
Used for I/O handling over files
(NOT BINARY)
*/
#include <fstream>

class File
{
	private:
		// (OTHER CLASSES)
		Console* c;

		std::string PATH;
		std::fstream STREAM;

		// Functions
		void load(std::string);
		void open(std::ios_base::openmode);
		void close();
	public:

		// Constructor
		File(Console*, std::string);

		// Functions
		void touch();
		std::string read();
		void write(std::string);
		void append(std::string = "");
};

File::File(Console* c, std::string path)
{
	this->c = c;
	c->tip("Initialized File class successfully.");
	this->load(path);
}

void File::load(std::string path)
{
	this->PATH = path;
	this->touch();
}

void File::open(std::ios_base::openmode flags)
{
	this->STREAM.open(this->PATH, flags);
}

void File::close()
{
	this->STREAM.close();
}

void File::touch()
{
	this->append();
}

std::string File::read()
{
	this->open(std::fstream::in);
	std::string temporary;
	this->STREAM >> temporary;
	this->close();
	return temporary;
}

void File::write(std::string text)
{
	this->open(std::fstream::out);
	this->STREAM << text;
	this->close();
}

void File::append(std::string text /*= ""*/)
{
	this->open(std::fstream::app);
	this->STREAM << text;
	this->close();
}

/*
This is a session class
Used for keeping track of users in the command line
*/
#include <bitset>

class Session
{
	private:
		std::string HASH;
		int FLAGS;

		// Functions
		void setPassword(std::string);
	public:
		std::string USER;

		// Constructor
		Session();

		// Functions
		void createPassword();
		void addFlag(int);
		void removeFlag(int);

		std::string interpretFlags();
};

Session::Session()
{
	this->USER = "";
	this->FLAGS = 0;
	this->HASH = "";
}

void Session::setPassword(std::string password)
{
	std::hash<std::string> object;
	this->HASH = object(password);
}

void Session::addFlag(int flag)
{
	this->FLAGS = this->FLAGS | flag;
}

void Session::removeFlag(int flag)
{
	this->addFlag(flag);
	this->FLAGS = this->FLAGS ^ flag;
}

std::string Session::interpretFlags()
{
	std::string temporary = "";
	for (auto& it : GROUPS)
	{
		if (this->FLAGS & it.second == 1)
		{
			temporary += (it.first + "\n");
		}
	}
	return temporary;
}

int main(int argc, char *argv[])
{
	Console *c = new Console();
	File *userStream = new File(c, "./.private/users");
	Session s = Session();

	std::string user_input;
	while (user_input != "exit")
	{
		user_input = c->input(">>> ");
	}

	std::cout << "Session ended for user: " << std::endl;

	return 0;
}
