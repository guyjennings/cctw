#include "cctwvector3d.h"

template <typename T>
CctwVector3D<T>::CctwVector3D(T x, T y, T z)
  : m_X(x),
    m_Y(y),
    m_Z(z)
{
}

template class CctwVector3D<int>;
template class CctwVector3D<double>;
