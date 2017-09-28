#include "Application.h"
#include "MessageType.h"


#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


int main()
{
	Application app(6666, 10);
	app.run();

	return 0;
}
