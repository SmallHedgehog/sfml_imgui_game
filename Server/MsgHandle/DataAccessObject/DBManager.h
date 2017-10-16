#pragma once

#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <sstream>

// using namespace sql;


class DBManager
{
public:
	DBManager();
	~DBManager();

	void init(const char* host, const char* user, const char* passwd);

	static DBManager* getInstance();
	void connect(const char* host, const char* user, const char* passwd);

	void static manageException(sql::SQLException& _excep);
	sql::Connection* getConnection() { return conn; }

protected:
	static DBManager* pInstance;

private:
	DBManager(const DBManager&) = delete;
	DBManager& operator = (const DBManager&) = delete;

	sql::Driver* driver;
	sql::Connection* conn;

	bool isConnected;
};
