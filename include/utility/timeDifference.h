#pragma once

struct TimeDifference {
	TimeDifference();

	long startMs;
	long lastMs;

	static long getCurrentTime();
	[[nodiscard]] long getDifferenceWithStart() const;
	[[nodiscard]] long getDifferenceWithLast() const;

	void updateLast();
};
