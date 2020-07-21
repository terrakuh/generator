#ifndef SIMPLE_GENERATOR_HPP_
#	define SIMPLE_GENERATOR_HPP_

#	include <functional>
#	include <iterator>
#	include <stdexcept>

class stop_iteration : std::runtime_error
{
public:
	stop_iteration() : std::runtime_error{ "" }
	{}
};

template<typename Type>
class generator
{
public:
	class iterator
	{
	public:
		typedef Type value_type;
		typedef std::ptrdiff_t difference_type;
		typedef const Type& reference;
		typedef const Type* pointer;
		typedef std::forward_iterator_tag iterator_category;

		iterator(const iterator& copy) = default;
		~iterator()
		{
			_destroy();
		}
		iterator& operator++()
		{
			if (_callback) {
				_destroy();

				try {
					new (&_data) Type{ _callback() };

					_valid = true;
				} catch (const stop_iteration&) {
					_callback = {};
				}
			}

			return *this;
		}
		iterator operator++(int)
		{
			iterator copy{ *this };

			++*this;

			return copy;
		}
		/**
		 * Derefences the iterator.
		 * 
		 * @return a reference to the next value
		 * @throw std::runtime_error if the operation is invalid
		 */
		reference operator*() const
		{
			if (!_valid) {
				throw std::runtime_error{ "invalid generator dereferencing" };
			}

			return *reinterpret_cast<const Type*>(&_data);
		}
		pointer operator->() const
		{
			return &**this;
		}
		bool operator==(const iterator& other) const noexcept
		{
			return static_cast<bool>(_callback) == static_cast<bool>(other._callback);
		}
		bool operator!=(const iterator& other) const noexcept
		{
			return !(*this == other);
		}
		iterator& operator=(const iterator& copy) = default;

	private:
		friend generator;

		std::function<Type()> _callback;
		typename std::aligned_storage<sizeof(Type), alignof(Type)>::type _data;
		bool _valid = false;

		iterator(std::function<Type()> callback)
		{
			_callback.swap(callback);

			++*this;
		}
		void _destroy() noexcept
		{
			if (_valid) {
				reinterpret_cast<Type*>(&_data)->~Type();

				_valid = false;
			}
		}
	};

	template<typename Function>
	generator(Function&& callback) : _callback{ std::forward<Function>(callback) }
	{}
	iterator begin() const
	{
		return _callback;
	}
	iterator end() const noexcept
	{
		return iterator{ {} };
	}

private:
	std::function<Type()> _callback;
};

#endif
