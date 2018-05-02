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
  m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9, 1));
  // The "3" corresponds to the convolution coefficient: we will start with a
  // small value and will do several other test thereafter.
  m_connections.push_back(new Layer(sizeX*3, sizeY*3, 2, 0.9, 1));

  // We do the same with the two second connection layers
  m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9, 1));
  m_connections.push_back(new Layer(sizeX*3, sizeY*3, 2, 0.9, 1));

  for (unsigned int i = 2; i < size-1; i++)
  {
    // The transmission coefficient increase with depth
    if (i < 124)
    {
      m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9, i));
      m_connections.push_back(new Layer(sizeX*5, sizeY*5, 2, 0.9, i));
    } else {
      m_neurons.push_back(new Layer(sizeX, sizeY, 2, 0.9, 124));
      m_connections.push_back(new Layer(sizeX*5, sizeY*5, 2, 0.9, 124));
    }
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

// Update a connection layer given a neuron layer
// The connection layer with the index "layerID" is loaded with the neuron
// layer with the same index.
// "size" corresponds to the square root of the number of connections.
void Brain::ConnectionUpdate(int const layerID, int const size)
{
  for (int i = 0; i < m_sizeX; i++)
  {
    for (int j = 0; j < m_sizeY; j++)
    {
      if ((*m_neurons[layerID])(i,j,1)==255)
      {
        // For every connections associated with the current neuron
        for (int k = -1; k < 1; k++)
        {
          for (int l = -1; l < 1; l++)
          {
            // We check if we are not out-of-bounds
            if (k + size*i >= 0 && k + size*i <= m_sizeX * size && l + size*j >=0 && l + size*j <= m_sizeY * size)
            {
              // Load the connections
              if ((*m_connections[layerID])(k + size*i, l + size*j, 1) + (*m_neurons[layerID])(i,j,1) <= 255)
              {
                (*m_connections[layerID])(k + size*i, l + size*j, 1) += (*m_neurons[layerID])(i,j,0);
              }
              else
              {
                (*m_connections[layerID])(k + size*i, l + size*j, 1) = 255;
              }
            }
          }
        }
        // Unload the neuron
        (*m_neurons[layerID])(i,j,1) = 0;
      }
    }
  }
}


void Brain::DirectNeuronUpdate(int const layerID, int const size)
{
  // We browse the connections layer.
  for (int i = 0; i < m_sizeX; i++)
  {
    for (int j = 0; j < m_sizeY; j++)
    {
      for (int k = -1; k < 1; k++)
      {
        for (int l = -1; l < 1; l++)
        {
          // We check if we are not out-of-bounds
          if (k + size*i >= 0 && k + size*i <= m_sizeX * size && l + size*j >=0 && l + size*j <= m_sizeY * size)
          {
            // If the connection is ready to be unloaded.
            if ((*m_connections[layerID])(k + size*i, l + size*j, 1) == 255)
            {
              // If we not exceed 255
              if ((*m_connections[layerID])(k + size*i, l + size*j, 1) + (*m_neurons[layerID+1])(i + k,j + l,1) < 255)
              {
                (*m_neurons[layerID+1])(i + k,j + l,1) += (*m_connections[layerID])(k + size*i, l + size*j, 0);
              }
              else
              {
                (*m_neurons[layerID+1])(i + k,j + l,1) = 255;
              }

              // We unload the connection
              (*m_connections[layerID])(k + size*i, l + size*j, 1) = 0;
            }
          }
        }
      }
    }
  }
}


void Brain::NeuronUpdate(int const layerID)
{
  int rear_x[8] = {-2, -2, -2, -1, 2, 2, 2, 1};
  int rear_y[8] = {-2, -1, 2, 2, 2, 1, -1, -1};

  int rear_x_n[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
  int rear_y_n[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

  int side_x[8] = {-2, -2, 0, 1, 2, 2, 0, -1};
  int side_y[8] = {0, 1, 2, 2, 0, -1, -2, -2};

  int side_x_n[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int side_y_n[8] = {0, 1, 1, 1, 0, -1, -1, -1};

  // We browse the connections layer.
  for (int i = 0; i < m_sizeX; i++)
  {
    for (int j = 0; j < m_sizeY; j++)
    {
      // Feed the foward neuron layer
      for (int k = -1; k < 1; k++)
      {
        for (int l = -1; l < 1; l++)
        {
          // We check if we are not out-of-bounds
          if (k + 5*i >= 0 && k + 5*i <= m_sizeX * 5 && l + 5*j >=0 && l + 5*j <= m_sizeY * 5)
          {
            // If the connection is ready to be unloaded.
            if ((*m_connections[layerID])(k + 5*i, l + 5*j, 1) == 255)
            {
              // If we not exceed 255
              if ((*m_connections[layerID])(k + 5*i, l + 5*j, 1) + (*m_neurons[layerID+1])(i + k,j + l,1) < 255)
              {
                (*m_neurons[layerID+1])(i + k,j + l,1) += (*m_connections[layerID])(k + 5*i, l + 5*j, 0);
              }
              else
              {
                (*m_neurons[layerID+1])(i + k,j + l,1) = 255;
              }

              // We unload the connection
              (*m_connections[layerID])(k + 5*i, l + 5*j, 1) = 0;
            }
          }
        }
      }


      for (int k = 0; k < 8; k++)
      {
        // Feed the side neuron layer
        // We check if we are not out-of-bounds
        if (side_x[k] + 5*i >= 0 && side_x[k] + 5*i <= m_sizeX * 5 && side_y[k] + 5*j >=0 && side_y[k] + 5*j <= m_sizeY * 5)
        {
          // If the connection is ready to be unloaded.
          if ((*m_connections[layerID])(side_x[k] + 5*i, side_y[k] + 5*j, 1) == 255)
          {
            // If we not exceed 255
            if ((*m_connections[layerID])(side_x[k] + 5*i, side_y[k] + 5*j, 1) + (*m_neurons[layerID])(i + side_x_n[k],j + side_y_n[k],1) < 255)
            {
              (*m_neurons[layerID])(i + side_x_n[k],j + side_y_n[k],1) += (*m_connections[layerID])(side_x[k] + 5*i, side_y[k] + 5*j, 0);
            }
            else
            {
              (*m_neurons[layerID])(i + side_x_n[k],j + side_y_n[k],1) = 255;
            }

            // We unload the connection
            (*m_connections[layerID])(side_x[k] + 5*i, side_y[k] + 5*j, 1) = 0;
          }
        }

        // Feed the rear neuron layer
        // We check if we are not out-of-bounds
        if (rear_x[k] + 5*i >= 0 && rear_x[k] + 5*i <= m_sizeX * 5 && rear_y[k] + 5*j >=0 && rear_y[k] + 5*j <= m_sizeY * 5)
        {
          // If the connection is ready to be unloaded.
          if ((*m_connections[layerID])(rear_x[k] + 5*i, rear_y[k] + 5*j, 1) == 255)
          {
            // If we not exceed 255
            if ((*m_connections[layerID])(rear_x[k] + 5*i, rear_y[k] + 5*j, 1) + (*m_neurons[layerID-1])(i + rear_x[k],j + rear_y[k],1) < 255)
            {
              (*m_neurons[layerID-1])(i + rear_x_n[k],j + rear_y_n[k],1) += (*m_connections[layerID])(rear_x_n[k] + 5*i, rear_y_n[k] + 5*j, 0);
            }
            else
            {
              (*m_neurons[layerID-1])(i + rear_x_n[k],j + rear_y_n[k],1) = 255;
            }

            // We unload the connection
            (*m_connections[layerID])(rear_x[k] + 5*i, rear_y[k] + 5*j, 1) = 0;
          }
        }
      }
    }
  }
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
  ConnectionUpdate(0, 3);

  // Update of the second neurons layer.
  DirectNeuronUpdate(0, 3);

  // We do the same with the next neuron layer as a feedback is not necessary
  // so close to the input layer.
  ConnectionUpdate(1, 3);
  DirectNeuronUpdate(1, 3);

  for (unsigned int i = 2; i < m_size; i++)
  {
    ConnectionUpdate(i, 5);
    NeuronUpdate(i);
  }
}
