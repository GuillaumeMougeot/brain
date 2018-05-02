#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

using namespace std;

int main ()
{
  srand (time(NULL));

  ifstream file("cifar/data_batch_2.bin", ios::binary);

  if(file)
  {

    // auto file_size = file.tellg();
    // unique_ptr<char[]> buffer(new char[file_size]);
    //
    // //Read the entire file at once
    // file.seekg(0, ios::beg);
    // file.read(buffer.get(), file_size);
    // file.close();
    unsigned char tplabel = 0;
    file.read((char*) &tplabel, sizeof(tplabel));

    /*
    std::size_t start = images.size();

    size_t size = 10000;
    size_t capacity = limit - images.size();

    if(capacity > 0 && capacity < size){
        size = capacity;
    }

    // Prepare the size for the new
    images.reserve(images.size() + size);
    labels.resize(labels.size() + size);

    for(std::size_t i = 0; i < size; ++i){
      labels[start + i] = buffer[i * 3073];

      images.push_back(func());

      for(std::size_t j = 1; j < 3073; ++j){
          images[start + i][j - 1] = buffer[i * 3073 + j];
      }
    }
    */
    cout << tplabel << endl;

    file.close();
  }
  else
    cerr << "[Error:main.cpp] Impossible to open the file." << endl;


  return 0;
}
