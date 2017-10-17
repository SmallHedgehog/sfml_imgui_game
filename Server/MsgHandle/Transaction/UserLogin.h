#pragma once

#include <sstream>
#include "../DataAccessObject/DBManager.h"


class UserLogin
{
public:
	// 查询某个用户是否在数据库中，如果存在获取该用户的密码
	static bool userIsExistsAndGetPasswd(const std::string& username, std::string& passwd)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			sql::ResultSet* res;
			std::string sql = "SELECT username, password FROM UserPasswd WHERE username=\"" +
				username + "\"";

			conn->setSchema("SFML_IMGUI_GAME");
			state = conn->createStatement();
			res = state->executeQuery(sql.c_str());

			while (res->next())
			{
				const std::string query = res->getString("username").c_str();
				if (query == username)
				{
					passwd = res->getString("password").c_str();
					return true;
				}
			}
		}
		return false;
	}

	// 查询某个用户是否在数据库中
	static bool userIsExists(const std::string& username)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			sql::ResultSet* res;
			std::string sql = "SELECT username FROM UserPasswd WHERE username=\"" +
				username + "\"";

			conn->setSchema("SFML_IMGUI_GAME");
			state = conn->createStatement();
			res = state->executeQuery(sql.c_str());
				
			while (res->next())
			{
				std::string query = res->getString("username").c_str();
				if (query == username)
					return true;
			}
			return false;
		}
		return true;
	}

	// 向数据库中插入一条用户数据记录
	static bool insertUserRecord(const std::string& username, const std::string& password)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			try {
				std::string sql = std::string("INSERT INTO UserPasswd values( \"") +
					username + "\", \"" + password + "\")";

				conn->setSchema("SFML_IMGUI_GAME");
				state = conn->createStatement();
				if (state->execute(sql.c_str()) != 0)
				{
					return false;
				}
				else
				{
					conn->commit();
					return true;
				}
			}
			catch (sql::SQLException& excep)
			{
				DBManager::manageException(excep);
			}
		}
		return false;
	}

	// 向用户在线信息表中插入一条记录
	static bool insertUserOnlineRecord(const std::string& username)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			try {
				std::string sql = std::string("INSERT INTO UserDataOnline(username, isOnline) values(\"") +
					username + "\", \"" + std::string("0") + "\")";

				conn->setSchema("SFML_IMGUI_GAME");
				state = conn->createStatement();
				if (state->execute(sql.c_str()) != 0)
				{
					return false;
				}
				else
				{
					conn->commit();
					return true;
				}
			}
			catch (sql::SQLException& excep)
			{
				DBManager::manageException(excep);
			}
		}

		return false;
	}

	// 向用户在线信息表中更新数据
	static bool updateUserOnlineRecord(const std::string& username, const int isOnline)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			try {
				std::stringstream stream;
				stream << isOnline;
				std::string sql = std::string("UPDATE UserDataOnline SET isOnline=") +
					stream.str() + " WHERE username=\"" + username + "\"";

				conn->setSchema("SFML_IMGUI_GAME");
				state = conn->createStatement();
				if (state->execute(sql.c_str()) != 0)
				{
					return false;
				}
				else
				{
					conn->commit();
					return true;
				}
			}
			catch (sql::SQLException& excep)
			{
				DBManager::manageException(excep);
			}
		}

		return false;
	}
};
