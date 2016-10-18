/* 
 * File:   GUITypes.hpp
 * Author: tku
 *
 * Created on 18 October 2016, 16:20
 */

#ifndef GUITYPES_HPP
#define GUITYPES_HPP

namespace NGE {
	namespace GUI2 {

		/**
		 * @class GUITypes class.
		 * @brief Class that keeps types used in GUI system in one place.
		 */
		class GUITypes {
		  public:

			/**
			 * @enum Type used to represents anchor point of GUI widget.
			 */
			enum AnchorPoint {
				BOTTOM_LEFT = 0,
				BOTTOM_RIGHT,
				TOP_LEFT,
				TOP_RIGHT
			};
		};
	}
}

#endif /* GUITYPES_HPP */

