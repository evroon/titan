#pragma once

#include "Math.h"

class ArrayMath
{
public:


	template<typename T>
	static T min(const Array<T> array)
	{
		T result = INFINITY;

		for (int c = 0; c < array.size(); c++)
			if (array[c] < result)
				result = array[c];

		return result;
	}

	template<typename T>
	static int min_index(const Array<T> array)
	{
		T lowest = INFINITY;
		int result = -1;
		int c;

		for (c = 0; c < array.size(); c++)
			if (array[c] < lowest)
			{
				lowest = array[c];
				result = c;
			}

		return result;
	}

	template<typename T>
	static T max(const Array<T> array)
	{
		T result = 0.0;

		for (int c = 0; c < array.size(); c++)
			if (array[c] > result)
				result = array[c];

		return result;
	}

	template<typename T>
	static int max_index(const Array<T> array)
	{
		T highest = 0.0;
		int result = -1;
		int c;

		for (c = 0; c < array.size(); c++)
			if (array[c] < highest)
			{
				highest = array[c];
				result = c;
			}

		return result;
	}

	template<typename T>
	static T closest_to(T value, const Array<T> array)
	{
		Array<T> distances;

		for (int c = 0; c < array.size(); c++)
			distances.push_back(distance(value, array[c]));

		return min(distances);
	}

	template<typename T>
	static T closest_to_index(T value, const Array<T> array)
	{
		Array<T> distances;

		for (int c = 0; c < array.size(); c++)
			distances.push_back(Math::distance(value, array[c]));

		return static_cast<T>(min_index(distances));
	}
};