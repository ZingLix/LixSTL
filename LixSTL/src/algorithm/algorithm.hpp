#ifndef ALGORITHM_H_
#define ALGORITHM_H_

namespace lix
{
template<class T>
T& max(T& lfs,T& rfs) {
	return lfs < rfs ? rfs : lfs;
}


}

#endif
