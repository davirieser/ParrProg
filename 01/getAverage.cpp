#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#define DATA_FILE "MeasureValues.txt"

#define SQUARE(x) (x) * (x)

enum Filename { slow = 0, medium = 1, fast = 2 };
enum OLevel { O0 = 0, O1 = 1, O2 = 2, O3 = 3, Os = 4, Ofast = 5 };

int checkLineContains(char* lineStart, char* toFind);

template <class T> class FoundValues {
  public:
	T* values;
	uint16_t count = 0;
	uint16_t size;

	FoundValues(int size) {
		this->size = size;
		this->values = (float*)malloc(sizeof(T) * size);
		assert(values != NULL);
	}
	FoundValues() : FoundValues(64) {}

	~FoundValues() { free(this->values); }

	void insertElement(T value) {
		assert(count < size - 1);
		this->values[count++] = value;
	}

	T calculateAverage() {
		T average = 0;
		for(int i = 0; i < count; i++) {
			average += values[i];
		}
		return average / count;
	}

    T calculateEmpiricDeviation(){
        T deviation = 0;
        T mean = calculateAverage();
        for(int i = 0; i < count; i++){
            deviation += SQUARE(mean - values[i]);
        }
        return sqrt(deviation / count);
    }
};


int main() {
	FoundValues<float> foundValues[3][6][12];
	FILE* file = fopen(DATA_FILE, "r");
	if(file == NULL) {
		printf("File could not be opened.\n");
		return -1;
	}
	char buffer[256];
	// char *buffer = (char*)malloc(sizeof(char) * 256);
	Filename filename;
	OLevel optimisationLevel;
	uint threadNum = -1;
	while(fgets(buffer, 256, file) != NULL) {
		if(checkLineContains(buffer, "gcc") >= 0) {
			if(checkLineContains(buffer, "slow") >= 0)
				filename = Filename::slow;
			else if(checkLineContains(buffer, "medium") >= 0)
				filename = Filename::medium;
			else if(checkLineContains(buffer, "fast") >= 0)
				filename = Filename::fast;

			if(checkLineContains(buffer, "-O0") >= 0)
				optimisationLevel = OLevel::O0;
			else if(checkLineContains(buffer, "-O1") >= 0)
				optimisationLevel = OLevel::O1;
			else if(checkLineContains(buffer, "-O2") >= 0)
				optimisationLevel = OLevel::O2;
			else if(checkLineContains(buffer, "-O3") >= 0)
				optimisationLevel = OLevel::O3;
			else if(checkLineContains(buffer, "-Os") >= 0)
				optimisationLevel = OLevel::Os;
			else if(checkLineContains(buffer, "-Ofast") >= 0)
				optimisationLevel = OLevel::Ofast;
		} else if(checkLineContains(buffer, "OMP_NUM") >= 0) {
			int tempLength = strlen("OMP_NUM_THREADS=");
			char* endPtr = NULL;
			threadNum = strtol(buffer + tempLength, &endPtr, 10) - 1;
			// assert(*endPtr != ' ');
		}
		int foundAt = checkLineContains(buffer, "time:");
		if(foundAt > 0) {
			int tempLength = strlen("time: ");
			float value = strtof(buffer + foundAt + tempLength, NULL);
			// printf("Add %f to %d %d %d\n", value, filename, optimisationLevel, threadNum);
			foundValues[filename][optimisationLevel][threadNum].insertElement(value);
		}
	}

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 6; j++) {
			for(int k = 0; k < 12; k++) {
                printf("For %d %d %2d the average is %7.4f and a deviation of %7.4f\n", i, j, k, foundValues[i][j][k].calculateAverage(), foundValues[i][j][k].calculateEmpiricDeviation());
			}
		}
	}

	fclose(file);
}

/**
 * Returns the position where it was found, or -1 if nothing was found
 */
int checkLineContains(char* lineStart, char* toFind) {
	int lineLength = strlen(lineStart);
	int findLength = strlen(toFind);
	int initialLength = lineLength;

	bool rightSequenz = false;
	while(lineLength >= findLength && rightSequenz == false) {
		rightSequenz = true;
		for(int i = 0; i < findLength && rightSequenz == true; i++) {
			if(*(lineStart + i) != *(toFind + i)) {
				rightSequenz = false;
			}
		}
		lineStart++;
		if(!rightSequenz) {
			lineLength--;
		}
	}
	return rightSequenz ? initialLength - lineLength : -1;
}
