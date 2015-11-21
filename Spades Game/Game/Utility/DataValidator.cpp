#include "Game/Utility/DataValidator.hpp"
#include "Game/Utility/TimeOfDay.hpp"
#include "Game/Utility/StringUtil.hpp"
#include "Game/Utility/UTF8.hpp"
#include <string.h>
#include <cctype>
namespace cge
{

	bool isCharacter(const char Character)
	{
		return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
		//Checks if a Character is a Valid A-Z, a-z Character, based on the ascii value
	}
	bool isNumber(const char Character)
	{
		return ( Character >= '0' && Character <= '9');
		//Checks if a Character is a Valid 0-9 Number, based on the ascii value
	}
	bool isValidEmailAddress(const char * EmailAddress)
	{
		if(!EmailAddress) // If cannot read the Email Address...
			return 0;
		if(!isCharacter(EmailAddress[0])) // If the First character is not A-Z, a-z
			return 0;
		int AtOffset = -1;
		int DotOffset = -1;
		unsigned int Length = strlen(EmailAddress); // Length = StringLength (strlen) of EmailAddress
		for(unsigned int i = 0; i < Length; i++)
		{
			if(EmailAddress[i] == '@') // If one of the characters is @, store it's position in AtOffset
				AtOffset = (int)i;
			else if(EmailAddress[i] == '.') // Same, but with the dot
				DotOffset = (int)i;
		}
		if(AtOffset == -1 || DotOffset == -1) // If cannot find a Dot or a @
			return 0;
		if(AtOffset > DotOffset) // If the @ is after the Dot
			return 0;
		return !(DotOffset >= ((int)Length-1)); //Chech there is some other letters after the Dot
	}



	bool spc_email_isvalid(const char *address) {
		int        count = 0;
		const char *c, *domain;
		static char *rfc822_specials = "()<>@,;:\\\"[]";

		/* first we validate the name portion (name@domain) */
		for (c = address;  *c;  c++) {
			if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == 
				'\"')) {
					while (*++c) {
						if (*c == '\"') break;
						if (*c == '\\' && (*++c == ' ')) continue;
						if (*c <= ' ' || *c >= 127) return 0;
					}
					if (!*c++) return 0;
					if (*c == '@') break;
					if (*c != '.') return 0;
					continue;
			}
			if (*c == '@') break;
			if (*c <= ' ' || *c >= 127) return 0;
			if (strchr(rfc822_specials, *c)) return 0;
		}
		if (c == address || *(c - 1) == '.') return 0;

		/* next we validate the domain portion (name@domain) */
		if (!*(domain = ++c)) return 0;
		do {
			if (*c == '.') {
				if (c == domain || *(c - 1) == '.') return 0;
				count++;
			}
			if (*c <= ' ' || *c >= 127) return 0;
			if (strchr(rfc822_specials, *c)) return 0;
		} while (*++c);

		return (count >= 1);
	}


	DataValidator::DataValidator( void )
	{
		m_names.push_back("admin");
		m_names.push_back("server");
		m_names.push_back("moderator");
		m_names.push_back("serveur");
		m_names.push_back("fuck");
		m_names.push_back("shit");
		m_names.push_back("cunt");
		m_names.push_back("cock");
		m_names.push_back("nigger");
		m_names.push_back("niger");
		m_names.push_back("penis");
		m_names.push_back("vagina");
		m_names.push_back("clitoris");
		m_names.push_back("pussy");
		m_names.push_back("fisting");
		m_names.push_back("whore");
		m_names.push_back("fag");
		m_names.push_back("asshole");
	}

	bool DataValidator::isEmailValid( const std::string& email ) const
	{
		if(!spc_email_isvalid(email.c_str()))
			return false;

		bool r = isValidEmailAddress(email.c_str());

		if(!r)
		{
			return false;
		}

		int atCount = 0;
		std::string valid_set("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@abcdefghijklmnopqrstuvwxyz._-");
		for(int i = 0; i < email.length(); i++)
		{
			if(email[i] == '@')
				atCount++;

			if(valid_set.find(email[i]) == std::string::npos)
				return false;
		}

		if(atCount > 1)
			return false;

		if(email.find(".@") != std::string::npos)
			return false;
		if(email.find("@.") != std::string::npos)
			return false;
		if(email.find("..") != std::string::npos)
			return false;
		return true;
	}

	DataValidator::~DataValidator( void )
	{

	}

	int DataValidator::getAge( int year, int month, int day ) const
	{
		int curYear = TimeOfDay::getYear();
		int curMonth = TimeOfDay::getMonth();
		int curDay = TimeOfDay::getMonthDay();

		int age = curYear - year;
		// Are we before the birth date this year? If so subtract one year.
		if (curMonth < month || (curMonth == month && curDay < day))
		{
			age = age -1;
		}

		return age;
	}

	bool DataValidator::ageIsAtleast( int minAge, int year, int month, int day ) const
	{
		return getAge(year,month,day) >= minAge;
	}

	bool DataValidator::dateIsValid( int year, int month, int day ) const
	{
		int monthLength[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		if(month <= 0 || day <= 0 || year <= 0)
			return false;

		if(day > 31 || month > 12)
			return false;

		return day <= monthLength[month];
	}

	bool DataValidator::usernameIsValid( const std::string& username ) const
	{
		std::string valid_set("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._- ");
		std::string un = username;
		StringUtil::toLower(un);
		char prev = 0;
		int persistCount = 0;
		int upperCount = 0;
		for(int i = 0; i < un.length(); i++)
		{
			if(valid_set.find(un[i]) == std::string::npos)
				return false;

			if(un[i] == prev)
			{
				persistCount++;
			}
			else
			{
				persistCount = 0;
			}

			prev = un[i];

			if(persistCount >= 3)
				return false;

			if(isupper(username[i]))
				upperCount++;

			if(upperCount >= 8)
				return false;
		}

		for(int i = 0; i < m_names.size(); i++)
		{
			if(un.find(m_names[i]) != std::string::npos)
			{
				return false;
			}
		}

		UTF8 utf8;
		if(utf8.length(un) > getMaxUsernameLength())
			return false;

		return true;
	}

	bool DataValidator::isPasswordValid( const std::string& password ) const
	{
		UTF8 utf8;
		return utf8.length(password) >= 8;
	}

	bool DataValidator::isNameValid( const std::string& firstname, const std::string& lastName ) const
	{
		UTF8 utf8;
		return utf8.length(firstname) <= 26 && utf8.length(lastName) <= 26;
	}

	int DataValidator::getMaxUsernameLength() const
	{
		return 14;
	}

	int DataValidator::getMinUsernameLength() const
	{
		return 3;
	}

}