#include "generator.hpp"

#include <iostream>
#include <regex>
#include <string>

// the given parameters must outlive the returned generator because the regex_iterator takes their references
inline generator<std::smatch> find_all(const std::regex& regex, const std::string& src)
{
	std::sregex_iterator it{ src.begin(), src.end(), regex };

	return [it]() mutable {
		if (it == std::sregex_iterator{}) {
			throw stop_iteration{};
		}

		return *it++;
	};
}

int main()
{
	std::regex pattern{ R"(\d+)" };
	std::string src = "21.07.2020";

	for (const auto i : find_all(pattern, src)) {
		std::cout << i.str() << '\n';
	}
}
