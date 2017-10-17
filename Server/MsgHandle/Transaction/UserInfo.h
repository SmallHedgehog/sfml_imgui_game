#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "../DataAccessObject/DBManager.h"


class UserInfo
{
public:
	// 在数据库中查找某一用户信息
	static bool findUserInfo(std::string& username, std::vector<std::string>& UserInfos)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			sql::ResultSet* res;
			std::string sql = "SELECT * FROM UserInfo WHERE username=\"" +
				username + "\"";

			conn->setSchema("SFML_IMGUI_GAME");
			state = conn->createStatement();
			res = state->executeQuery(sql.c_str());
			while (res->next())
			{
				UserInfos.push_back(res->getString("username").c_str());
				UserInfos.push_back(toString(res->getInt("level")));
				UserInfos.push_back(toString(res->getInt("expr")));
				UserInfos.push_back(toString(res->getInt("winCount")));
				UserInfos.push_back(toString(res->getInt("loseCount")));
				UserInfos.push_back(toString(res->getInt("ranCount")));
			}

			if (UserInfos.size() != 6)
				return false;
			return true;
		}
		return false;
	}

	// 设置用户信息
	static bool setUserInfo(std::string& username)
	{
		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			try {
				std::string sql = std::string("INSERT INTO UserInfo(username) values(\"") +
					username + "\")";

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

	static bool setUserInfo(std::string& username, std::vector<std::string>& UserInfos)
	{
		if (UserInfos.size() != 6)
			return false;

		DBManager* db = DBManager::getInstance();
		sql::Connection* conn = db->getConnection();
		if (conn)
		{
			sql::Statement* state;
			try {
				std::string sql = std::string("INSERT INTO UserInfo values(\"") +
					username + "\", \"" + UserInfos[0] + "\", \"" + UserInfos[1] + "\", \"" +
					UserInfos[2] + "\", \"" + UserInfos[3] + "\", \"" + UserInfos[4] + "\", \"" +
					UserInfos[5] + "\")";

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


private:
	static std::string toString(int val)
	{
		std::stringstream stream;
		stream << val;

		return stream.str();
	}
};
