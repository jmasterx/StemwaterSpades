#include "Game/Database/Database.hpp"
namespace cge
{
	Database::~Database(void)
	{
	}

	Database::Database( 
		const std::string& url, const std::string& username, const std::string& password, const std::string& schema )
		: m_url(url),m_user(username),m_password(password),m_schema(schema),m_transaction(NULL)
	{

	}

	bool Database::execute( const std::string& statement )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute(statement);
			if(con)
			{
				releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				releaseConnection(con);
				con = NULL;
			}
			return false;
		}
		return true;
	}

	sql::Connection* Database::getConnection()
	{
		if(m_transaction == NULL)
		{
			sql::Connection* con = NULL;

			con = get_driver_instance()->connect(m_url,m_user,m_password);
			if(con == NULL)
			{
				return NULL;
			}
			con->setSchema(m_schema);
			return con;
		}

		return m_transaction;
	}

	void Database::releaseConnection( sql::Connection* con )
	{
		if(con == m_transaction)
			return;

		delete con;
	}

	bool Database::beginTransaction()
	{
		if(m_transaction != NULL)
		{
			return false;
		}

		m_transaction = getConnection();
		if(m_transaction)
		{
			m_transaction->setAutoCommit(false);
		}
		return true;
	}

	void Database::endTransaction( bool commit )
	{
		if(m_transaction && commit)
		{
			try
			{
				m_transaction->commit();
			}
			catch(sql::SQLException &e){
			}
		}
		delete m_transaction;
		m_transaction = NULL;
	}

	void Database::commitTransaction()
	{
		if(m_transaction)
		{
			try
			{
				m_transaction->commit();
			}
			catch(sql::SQLException &e){
			}
		}
	}

}
