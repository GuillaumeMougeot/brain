#include "Layer.hpp"
#include <algorithm> /* random_shuttle */
#include <vector>    /* vector */
#include <stdlib.h>  /* rand */

Layer::Layer(unsigned int sizeX,
  unsigned int sizeY,
  unsigned int colors,
  float lifeThreshold,
  float initThreshold)
{
  // Create an image of width "sizeX", height "sizeY", depth one and with
  // "colors" as image spectrum (3 = RGB, 1 = B&W)
  m_image = new cimg_library::CImg<unsigned int>(sizeX, sizeY, 1, colors);

  if (initThreshold > 1 || initThreshold < 0)
  {
    std::cout << "[Error][Layer.hpp] Your init threshold value is wrong.
      Please put a value strictly between zero and one." << std::endl;
    m_initThreshold = 0;
  } else {
    m_initThreshold = initThreshold;
  }

  // Initializing the image with random empty and non-empty boxes.
  // This should be improved later thanks to inheritance.
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      float alea = rand() / RAND_MAX;

      if (alea < initThreshold)
      {
        (*m_image)(i, j) = 124;
      }
    }
  }

  m_sizeX = sizeX;
  m_sizeY = sizeY;
  m_colors = colors;

  if (lifeThreshold > 1 || lifeThreshold < 0)
  {
    std::cout << "[Error][Layer.hpp] Your life threshold value is wrong.
      Please put a value strictly between zero and one." << std::endl;
    m_lifeThreshold = 0;
  } else {
    m_lifeThreshold = lifeThreshold;
  }

}

Layer::~Layer()
{
  delete m_image;
  m_image = 0;
}

void Layer::Life();
{
  // In the following: "neuron" can be replace by "connection"

  // We browse throught all of boxes into the layer to look at neuron state.
  // Each box can contains 4 differents values:
  //  -empty
  //  -a diying neuron
  //  -a normal neuron
  //  -a reproductive neuron
  for (int i = 0; i < m_sizeX; i++)
  {
    for (int j = 0; j < m_sizeY; j++)
    {
      // Reproduction process:
      //  If the life threshold is exceeded then we add a new neuron around the
      //  current one.
      if ((*m_image)(i, j) > m_lifeThreshold * 255)
      {
        // We would like to find a random empty place surrounding the neuron
        // and add a neuron there.
        std::vector<int> index1, index2;
        for (int i=-1; i<=1; i++)
        {
          index1.push_back(i);
          index2.push_back(i);
        }
        std::random_shuttle(index1.begin(), index1.end());
        std::random_shuttle(index2.begin(), index2.end());

        for(std::vector<int>::iterator it1=index1.begin(); it1!=index1.end(); it1++)
        {
          for(std::vector<int>::iterator it2=index2.begin(); it2!=index2.end(); it2++)
          {
            // We check if we are not looking out of the table ...
            if (!(i + it1 < 0 || i + it1 > sizeX || j + it2 < 0 || j + it2 > sizeY))
            {
              // ... then we can check if the box is free ...
              if ((*m_image)(i + it1, j + it2) == 0)
              {
                // ... and if it is the case we feel it with a new neuron.
                (*m_image)(i + it1, j + it2) = 124;
                // We then ended the loop:
                it1 = index1.end();
                it2 = index2.end();
              }
            }
          }
        }
      }

      // Killing process:
      //  If the life threshold of the neuron is too low we remove it.
      if ((*m_image)(i, j) < (1 - m_lifeThreshold) * 255)
      {
        (*m_image)(i, j) = 0;
      }
    }
  }
}


void Layer::setImage(cimg_library::CImg<unsigned int>* image)
{
  
}
