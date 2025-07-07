#include <memory>
#include <new>
#include <utility>
#include <string>
template<typename T>
struct vector {
private:
	T* data = nullptr;
	size_t capacity{};
	size_t size{};
public:
	vector(size_t inputSize = 1)
		: data(static_cast<T*>(::operator new(inputSize * sizeof(T)))),
		capacity(inputSize), size(0) {
	}
	~vector() {
		clear();
		if (data) ::operator delete(data, capacity * sizeof(T));

	}
	template<typename... Args>
	void emplace_back(Args&&... args) {
		if (size == capacity) {
			reserve(capacity == 0 ? 1 : capacity * 2);
		}
		std::construct_at(data + size, std::forward<Args>(args)...);
		++size;
	}
	void reserve(size_t newCapacity) {
		if (newCapacity <= capacity) return;

		T* temp = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

		for (size_t i{}; i < size; ++i) {
			std::construct_at(temp + i, std::move(*(data + i)));
			std::destroy_at(data + i);

		}
		if (data) ::operator delete(data, capacity * sizeof(T));


		data = temp;
		capacity = newCapacity;
	}
	size_t get_size() const { return size; }
	size_t get_capacity() const { return capacity; }

	void clear() {
		for (size_t i{}; i < size; ++i) {
			std::destroy_at(data + i);
		}
		size = 0;
	}
	void pop_back() {
		if (size > 0) {
			--size;
			std::destroy_at(data + size);
		}
	}
	T& operator[](size_t i) { return *(data + i); };
	const T& operator[](size_t i) const { return *(data + i); };

	class iterator {
		T* ptr;
	public:
		iterator(T* p) : ptr(p) {}

		T& operator*() const { return *ptr; }
		iterator& operator++() { ++ptr; return *this; }
		iterator operator++(int) { iterator tmp = *this; ++ptr; return tmp; }

		bool operator==(const iterator& other) const { return ptr == other.ptr; }
		bool operator!=(const iterator& other) const { return ptr != other.ptr; }
	};

	iterator begin() { return iterator(data); }
	iterator end() { return iterator(data + size); }

	vector(const vector&) = delete;
	vector(vector&&) = delete;
	vector& operator=(const vector&) = delete;
	vector& operator=(vector&&) = delete;

};


int main() {
	vector<std::string> asd;
	asd.emplace_back("Hello World;\n");
}
