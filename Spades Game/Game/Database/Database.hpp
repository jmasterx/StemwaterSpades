#ifndef CGE_DATABASE_HPP
#define CGE_DATABASE_HPP
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include "Game/serverplatform.hpp"
/* MySQL Connector/C++ specific headers */
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

namespace cge
{
	class Database
	{
		std::string m_url;
		std::string m_user;
		std::string m_password;
		std::string m_schema;
		sql::Connection* m_transaction;
	public:
		Database(const std::string& url, const std::string& username, const std::string& password, const std::string& schema);
		bool execute(const std::string& statement);
		sql::Connection* getConnection();
		void releaseConnection(sql::Connection* con);
		bool beginTransaction();
		void commitTransaction();
		void endTransaction(bool commit);

		~Database(void);
	};
}
#endif
