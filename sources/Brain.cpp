#include "Brain.hpp"

Brain::Brain(unsigned int size, unsigned int sizeX, unsigned int sizeY)
{
  m_size = size;

  // There are "size" layers of neurons and "size-1" layers of connections.

  // The first connection layer do not need to have back or sides connections
  // There are two channels for both layers:
  //  -the first one contains the transmitting coefficient. This coefficient
  //   increases with the charge variation. This coefficient evoluate slower
  //   than the charge. The reproduction and the death of layer elements
  //   depends on this coefficient.
  //  -the second one contains the charge.
  m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9));
  // The "3" corresponds to the convolution coefficient: we will start with a
  // small value and will do several other test thereafter.
  m_connections.push_back(new Layer(sizeX*3, sizeY*3, 2, 0.9));

  for (unsigned int i = 1; i < size-1; i++)
  {
    m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9));
    m_connections.push_back(new Layer(sizeX*5, sizeY*5, 2, 0.9));
  }
  m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9));
}

void Brain::Life()
{
  for (int i=0; i<m_size-1; i++)
  {
    m_neurons[i].Life();
    m_connections[i].Life();
  }
  m_neurons[m_size-1].Life();
}

void Brain::Update(cimg_library::CImg<unsigned int> const* input)
{
  // Initializing of the first neuron layer.
  // This layer is going to be the only one that will change for every frame.
  m_neurons[0]->setImage(input);

  // First connection layer filling
  // For the first connection layer we are looking only towards the back
  // -We browse throught the first neuron layer
  // -We check every loaded neurons
  // -We transmit the signal, if the neuron is loaded, to the connection layer
  for (int i = 0; i < m_sizeX; i++)
  {
    for (int j = 0; j < m_sizeY; j++)
    {
      if ((*m_neurons[0])(i,j,1)==255)
      {
        // For every connections associated with the current neuron
        for (int k = -1; k < 1; k++)
        {
          for (int l = -1; l < 1; l++)
          {
            // We check if we are not out-of-bounds
            if (k + 3*i >= 0 && k + 3*i <= 255 && l + 3*j >=0 && l + 3*j <= 255)
            {

            }
          }
        }
      }
    }
  }
}
