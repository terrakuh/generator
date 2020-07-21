#include "generator.hpp"

#include <cstdint>
#include <iostream>

inline generator<std::uint64_t> fibonacci()
{
	// state of the generator
	std::uint64_t a = 0, b = 1;

	return [a, b]() mutable {
		const auto c = a;
		a            = b;
		b            = b + c;

		// max value reached
		if (a < c) {
			throw stop_iteration{};
		}

		return a;
	};
}

int main()
{
	std::cout << "generating fibonacci sequence...\n";

	for (const auto i : fibonacci()) {
		std::cout << i << " ";
	}

	std::cout << std::endl;
}
