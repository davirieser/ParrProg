#include <cstdio>
#include <assert.h>

class MyCSVHandler
{
private:
    std::FILE *file = NULL;
    int valuesPerLine;

public:
    /**
     * Creates an object to handle a csv and opens "filename". File extension also needed to be defined in "filename".
     * @param CSVHeander will be written only if the file does not exist.
     * @param valuesPerLine Is the number of header in CSVHeader.
     */
    MyCSVHandler(const char *fileName, const char *CSVHeader[], int valuesPerLine)
    {
        this->valuesPerLine = valuesPerLine;
        this->file = fopen(fileName, "r");
        bool writeHeader = this->file == NULL;
        if (this->file != NULL)
        {
            fclose(this->file);
        }
        this->file = fopen(fileName, "a");
        assert(this->file != NULL);

        if (writeHeader)
        {
            for (int i = 0; i < valuesPerLine; i++)
            {
                fprintf(this->file, "%s;", CSVHeader[i]);
            }
            fprintf(this->file, "\n");
        }
    }

    ~MyCSVHandler()
    {
        fclose(this->file);
    }

    /**
     * Will write the values stored in values_i and values_f into the csv and seperate it with a newline from the next write.
     * @param values_i: Array of integers to write to the csv.
     * @param values_f: Array of floats to write to the csv.
     * @param count_i: Number of integers in values_i.
     * @param count_f: Number of floats in values_i.
     * @param oder=NULL: An array of integers to specify the order in which the values should be selected.
     *      Positive indizes represent a position in values_i, negative values represent values from values_f with 1/-1 being the first value in both arrays.
     * @return true if values were written to file. \\
     * @return false if an error occured.
     */
    bool writeValues(int values_i[], int count_i, double values_f[], int count_f, int *order = NULL)
    {
        assert(this->file != NULL);
        if (count_i > 0 && values_i == NULL)
        {
            return false;
        }
        if (count_f > 0 && values_f == NULL)
        {
            return false;
        }
        fseek(file, 0, SEEK_END);
        if (order == NULL)
        {
            for (int i = 0; i < count_i; i++)
            {
                fprintf(this->file, "%d;", values_i[i]);
            }
            for (int i = 0; i < count_f; i++)
            {
                fprintf(this->file, "%f;", values_f[i]);
            }
        }
        else
        {
            for (int i = 0; i < count_i + count_f; i++)
            {
                if (order[i] == 0 || order[i] > count_i || order[i] < -count_f)
                    return false;
                if (order[i] > 0)
                {
                    fprintf(this->file, "%d;", values_i[order[i] - 1]);
                }
                else
                {
                    fprintf(this->file, "%lf;", values_f[(-order[i]) - 1]);
                }
            }
        }
        fprintf(this->file, "\n");
        return true;
    }
};