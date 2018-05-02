#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "NeuronLayer.hpp"
#include "ConnectionLayer.hpp"
#include <vector>


/// Define a neuron image layer

class Brain
{
public:
  // Params:
  //  -size is the amount of neuron layers.
  //  -sizeX and sizeY represent the NeuronLayer size, the connection
  // layer size is not parametrized yet.
  // The constructor initialize randomly a brain: the different layer are
  // TODO: take mutliple parent brain in parameter and initialize the child
  // brain depending on this parent brains.
  Brain(unsigned int size, unsigned int sizeX, unsigned int sizeY);

  // Kill or reproduce neurons and connections
  void Life();

  // Update charge through the brain (the biggest function)
  // The input image is a black and white picture.
  // It will transmit the picture signal throught the brain
  void Update(cimg_library::CImg<unsigned int> const* input);

  // The output corresponds to the last neuron layer.
  // Currently we are not scanning the brain to see how connections appear and
  // we are focusing on this output image only.
  cimg_library::CImg<unsigned int>* getOutput() {return m_neurons[m_size-1]};

  // TODO: Save and Load functions

private:
  // Update a connection layer given a neuron layer
  // The connection layer with the index "layerID" is loaded with the neuron
  // layer with the same index.
  // "size" corresponds to the square root of the number of connections.
  void ConnectionUpdate(int const layerID, int const size);

  // This method unloads the connection layer into the next neuron layer only,
  // without any back or side transmission.
  void DirectNeuronUpdate(int const layerID, int const size);

  // Unloads the connection layer into three different layers:
  // the previous, the current and the next layer
  void NeuronUpdate(int const layerID);

  // size is the amount of neuron layers.
  unsigned int m_size;

  // A neuron layer is a B&W picture containing neuron charges
  std::vector<Layer*> m_neurons;
  std::vector<Layer*> m_connections;
};

#endif
