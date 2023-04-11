#pragma once

#include "haptic_constants.h"
#include "haptic_plane.hpp"

#include <types.hpp>

#include <map>
#include <Arduino.h>

namespace OH {
  class HapticBody {
   private:
    typedef std::map<oh_output_path_t, HapticPlane*> oh_output_components_map_t;
    std::map<oh_output_path_t, HapticPlane*> components{};

   public:
    HapticBody() {};

    void addComponent(const oh_output_path_t, HapticPlane*);
    oh_output_components_map_t* getComponents();

    void writeOutput(const oh_output_path_t, const oh_output_data_t&);

    void setup() {
      for (auto& component : this->components) {
        component.second->setup();
      }
    };
  };

  /**
   * Re-maps a point index to output coordinate.
   * @tparam _Tp The type of the point index.
   */
  template <typename _Tp>
  inline oh_output_point_t* mapPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max) {
    const oh_output_coord_t x_coord = map(x + 1, 0, x_max + 2, 0, OH_OUTPUT_COORD_MAX);
    const oh_output_coord_t y_coord = map(y + 1, 0, y_max + 2, 0, OH_OUTPUT_COORD_MAX);

    return new oh_output_point_t(x_coord, y_coord);
  }

  template <typename _Tp>
  std::map<oh_output_point_t, _Tp*> mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d);
}  // namespace OH