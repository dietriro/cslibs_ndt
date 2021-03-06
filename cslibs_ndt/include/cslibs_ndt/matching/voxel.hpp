#ifndef CSLIBS_NDT_3D_VOXEL_HPP
#define CSLIBS_NDT_3D_VOXEL_HPP

#include <cslibs_math_3d/linear/pointcloud.hpp>
#include <cslibs_indexed_storage/storage.hpp>
#include <cslibs_indexed_storage/backend/kdtree/kdtree.hpp>
#include <cslibs_indexed_storage/backend/array/array.hpp>

namespace cis = cslibs_indexed_storage;

namespace cslibs_ndt {
namespace matching {
template<std::size_t Dim>
class EIGEN_ALIGN16 Voxel {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    using index_t = std::array<int, Dim>;
    using point_t = cslibs_math::linear::Vector<double, Dim>;
    using size_t  = std::array<std::size_t, Dim>;

    inline Voxel() :
        n_(1),
        n_1_(0)
    {
    }

    inline Voxel(const point_t &pt) :
        n_(2),
        n_1_(1),
        mean_(pt)
    {
    }

    inline virtual ~Voxel() = default;

    inline Voxel(const Voxel &other) :
        mean_(other.mean_)
    {
    }

   inline  Voxel(Voxel &&other) :
        mean_(std::move(other.mean_))
    {
    }

    inline Voxel& operator = (const Voxel &other)
    {
        mean_ = other.mean_;
        return *this;
    }

    inline Voxel& operator = (Voxel &&other)
    {
        mean_ = std::move(other.mean_);
        return *this;
    }

    inline point_t const & mean() const
    {
        return mean_;
    }

    inline void merge(const Voxel &other)
    {
        const std::size_t   _n  = n_1_ + other.n_1_;
        const point_t       _pt = (mean_ * static_cast<double>(n_1_) + other.mean_ * static_cast<double>(other.n_1_)) / static_cast<double>(_n);
        n_                      = _n + 1;
        n_1_                    = _n;
        mean_                   = _pt;
    }

    inline static index_t getIndex(const point_t &p, const double inverse_resolution)
    {
        index_t index;
        for(std::size_t i = 0 ; i < Dim ; ++i)
            index[i] = static_cast<int>(std::floor(p(i) * inverse_resolution));

        return index;
    }


private:
    std::size_t n_;
    std::size_t n_1_;
    point_t     mean_;
};

template<std::size_t Dim>
struct VoxelGrid
{
    using type = cis::Storage<Voxel<Dim>, typename Voxel<Dim>::index_t, cis::backend::array::Array>;
    using Ptr = std::shared_ptr<type>;
};
}
}


#endif // CSLIBS_NDT_3D_VOXEL_HPP
