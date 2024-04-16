#include "main.h"
#include "restaurant.cpp"

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    freopen("output.txt", "w", stdout);
    string fileName = "test.txt";
    simulate(fileName);
    clock_t end = clock(); // ghi lại thời gian lúc sau
    cout << "Time run: " << (double)(end - begin) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
    // printMemoryUsage();
    return 0;
}
