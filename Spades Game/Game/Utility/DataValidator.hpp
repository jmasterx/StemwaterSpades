#ifndef CGE_DATA_VALIDATOR_HPP
#define CGE_DATA_VALIDATOR_HPP
#include <stdlib.h>
#include <string>
#include <vector>
namespace cge
{
	class DataValidator
	{
		std::vector<std::string> m_names;
	public:
		DataValidator(void);
		bool isEmailValid(const std::string& email) const;
		int getAge(int year, int month, int day) const;
		bool ageIsAtleast(int minAge, int year, int month, int day) const;
		bool dateIsValid(int year, int month, int day) const;
		bool usernameIsValid(const std::string& username) const;
		bool isPasswordValid(const std::string& password) const;
		bool isNameValid(const std::string& firstname, const std::string& lastName) const;
		int getMaxUsernameLength() const;
		int getMinUsernameLength() const;
		~DataValidator(void);
	};
}
#endif