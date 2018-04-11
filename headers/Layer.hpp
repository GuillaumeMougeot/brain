#ifndef LAYER_HPP
#define LAYER_HPP

#include "CImg/CImg.h"
#include <iostream>

/// Layer abstract class:
/// Define the global framework for NeuronLayer and ConnectionLayer classes.

class Layer:
{
public:
  // Parameters:
  //  -sizeX and sizeY: dimension of the picture layer,
  //  -colors: number of color channels
  //  -lifeThreshold: reproduce or kill threshold
  //  -initThreshold: proportion of active neurons or connections
  Layer(unsigned int sizeX,
    unsigned int sizeY,
    unsigned int colors,
    float lifeThreshold,
    float initThreshold);

  ~Layer();

  // Kill "weak" neurons or connections and reproduce "strong" ones
  void Life();

  // TODO ? "Time" -> decrease all the transmition coefficients

  cimg_library::CImg<unsigned int>* getImage() {return m_image;}

  void setImage(cimg_library::CImg<unsigned int>* image);
  unsigned int operator()(unsigned int i, unsigned int j, unsigned int k)
  {return (*m_image)(i, j, k);}

  // Save image
  void Save(const char *const filename) {m_image->save(filename);}

private:
  // "m_image" will store the neuron or connection layer
  cimg_library::CImg<unsigned int>* m_image;
  unsigned int m_sizeX, m_sizeY, m_colors;
  float m_lifeThreshold, m_initThreshold;
};

#endif
