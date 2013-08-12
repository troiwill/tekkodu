#ifndef GENERAL_FNCS_H_
#define GENERAL_FNCS_H_

// C++ Library
#include <vector>

namespace GeneralFncs {

	//! Deletes and erases each dynamically created element
	template<typename T>
	inline void destroyAllPtrsInVector(std::vector<T*>& vec) {
		while (!vec.empty()) {
			if (vec[0] != NULL) {
				delete vec[0];
				vec[0] = NULL;
			}
			vec.erase(vec.begin());
		}
	}

	//! Deletes and erases a particular index in a vector
	template<typename T>
	inline void destroyPtrInVector(std::vector<T*>& vec, std::size_t elementIndex) {
		if (elementIndex < vec.size() && vec[elementIndex] != NULL) {
			delete vec[elementIndex];
			vec[elementIndex] = NULL;
			vec.erase(vec.begin() + elementIndex);
		}
	}

	//! Deletes the memory pointed by a pointer
	template<typename T>
	inline void deletePtr(T*& ptr) {
		if (ptr != NULL) {
			delete ptr;
			ptr = NULL;
		}
	}

	//! Returns a subset of a vector from the start position up to, but not including, the end position
	template <typename T>
	inline std::vector<T> subVector(const std::vector<T>& vec,
							 const std::size_t startPos,
							 const std::size_t endPos)
	{
		std::vector<T> subVec;
		const std::size_t kSize = vec.size();
		if (startPos >= kSize || endPos > kSize || startPos >= endPos)
			return subVec;
		for (std::size_t pos = startPos; pos < endPos; pos++)
			subVec.push_back(vec[pos]);
		return subVec;
	}
	
	/*
	// TODO (24/JUL/13) will both of these do the same thing? (with or without the pointer)
	//! Returns a subset of a vector from the start position up to, but not including, the end position
	template <typename T>
	std::vector<T*> subVector(const std::vector<T*>& vec,
							  const std::size_t startPos,
							  const std::size_t endPos)
	{
		std::vector<T*> subVec;
		const std::size_t kSize = vec.size();
		if (startPos >= kSize || endPos > kSize || startPos >= endPos)
			return subVec;
		for (std::size_t pos = startPos; pos < endPos; pos++)
			subVec.push_back(vec[pos]);
		return subVec;
	}
	*/
}

#endif // GENERAL_FNCS_H_