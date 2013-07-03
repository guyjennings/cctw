#ifndef CCTWVECTOR3D_H
#define CCTWVECTOR3D_H

template <typename T>
class CctwVector3D
{
public:
  CctwVector3D(T x, T y, T z);

private:
  T m_X;
  T m_Y;
  T m_Z;
};

#endif // CCTWVECTOR3D_H
