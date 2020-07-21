# Simple C++11 Generator

## Usage

```cpp
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

```

And compile it!

```sh
c++ fibonacci.cpp -o fibonacci -std=c++11
./fibonacci
```

## Installation

Just copy `generator.hpp` into your project or in your include folder and you're ready to start.

## License

This project is Public Domain.
