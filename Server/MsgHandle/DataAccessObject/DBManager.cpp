#include "DBManager.h"


DBManager* DBManager::pInstance = nullptr;

DBManager* DBManager::getInstance()
{
	if (!pInstance)
	{
		pInstance = new DBManager();
	}

	return pInstance;
}

void DBManager::init(const char* host, const char* user, const char* passwd)
{
	connect(host, user, passwd);
}

void DBManager::connect(const char* host, const char* user, const char* passwd)
{
	try {
		driver = get_driver_instance();

		conn = driver->connect(host, sql::SQLString(user), sql::SQLString(passwd));

		isConnected = true;
	}
	catch (sql::SQLException& excp) {
		manageException(excp);
		isConnected = false;
	}
}

void DBManager::manageException(sql::SQLException& _excep)
{
	if (_excep.getErrorCode() != 0)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << _excep.what();
		std::cout << " (MySQL error code: " << _excep.getErrorCode();
		std::cout << ", SQLState: " << _excep.getSQLState() << " )" << std::endl;
	}
}

DBManager::DBManager():
	driver(nullptr), conn(nullptr)
{}

DBManager::~DBManager()
{
	if (conn)
		delete conn;
}
