/* 
 * File:   GUIRectangle.hpp
 * Author: tku
 *
 * Created on 18 October 2016, 16:17
 */

#ifndef GUIRECTANGLE_HPP
#define GUIRECTANGLE_HPP

#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Math/Matrix4.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Events/MouseEvent.hpp"
#include "NGE/Events/KeyboardEvent.hpp"
#include "GUITypes.hpp"

namespace NGE
{
    namespace GUI2
    {

        /**
         * @class GUIRectangle class.
         * @brief Class that represents rectangle in graphics user interface.
         * 
         * GUIRectangle is basic class in GUI system. It represents simple
         * rectangle object - almost all widgets are based on this class and
         * inherits it.
         * This class implements all basic events that can happes with the rectangle,
         * as click, drag or resize. If you going to implement your own methods of
         * any event, remember to invoke methods from this class to keep the default
         * behaviour.
         * 
         * @note This class is abstract and cannot be used to represent simple rectangle.
         * To do so, create other class, inherit GUIRectangle and implement abstract
         * methods, which should be used to render rectangle.
         */
        class GUIRectangle
        {
          public:

            /**
             * Default rectangle constructor. It initialize all variables and sets
             * pointer to event functions. It is invoked even when parameterized constructor
             * is used.
             */
            GUIRectangle();

            /**
             * Parametrized constructor.
             * @param position Position of the rectangle, based on anchorPoint.
             * @param dimensions Dimensions (width and height) or the rectangle.
             * @param anchorPoint Anchor point of the rectangle used to calculate real position.
             */
            GUIRectangle(const Math::vec2i& position, const Math::vec2i& dimensions, const GUITypes::AnchorPoint anchorPoint = GUITypes::BOTTOM_LEFT);

            /**
             * Initialize the rectangle. This method invokes methods to
             * create rectangle geometry, initialize vertex buffer objects and
             * vertex buffer array.
             * @return True if everything went ok, otherwise false.
             */
            bool Initialize();

            /**
             * Update base on time - used for example to animate rectangle.
             * @param deltaTime Delta time.
             */
            virtual void Update(float deltaTime);

            /**
             * Render the geometry.
             */
            virtual void Render();

            /**
             * Set the shader used to render the rectangle.
             * @param shader Pointer to shader.
             */
            void SetShader(NGE::Media::Shaders::GLSLProgram* shader);

            /**
             * Set the rectangle position.
             * @param position Vector2<int> position. First parameter is x, second y.
             */
            void SetPosition(const Math::vec2i& position);

            /**
             * Set the rectangle position.
             * @param x X position.
             * @param y Y position.
             */
            void SetPosition(const int x, const int y);

            /**
             * Get the rectangle position.
             * @return Vector2<int> with position.
             */
            Math::vec2i GetPosition() const;

            /**
             * Set the dimensions of the rectangle - width and height.
             * @param dimensions Vector2<int> - x is width, y is hegith.
             */
            void SetDimensions(const Math::vec2i& dimensions);

            /**
             * Set the dimensions of the rectangle - width and height.
             * @param width Width of the rectangle.
             * @param height Height of the rectangle.
             */
            void SetDimensions(const int width, const int height);

            /**
             * Get the rectangle dimensions.
             * @return Vector2<int> with dimensions.
             */
            Math::vec2i GetDimensions() const;

            /**
             * Get the real dimensions of the rectangle. Real dimensions are position and
             * dimensions after calculating those parameters based on anchor point.
             * @return Vector4<int> where:\n
             *  x \-- real X position on the screen.\n
             *  y \-- real Y position on the screen.\n
             *  z \-- X position + width.\n
             *  w \-- Y position + height.
             */
            Math::vec4i GetRealDimensions() const;

            /**
             * Set the anchor point of the rectangle. Anchor point determines where
             * the rectangle will be created and rendered.
             * @param anchorPoint Anchor point.
             */
            void SetAnchorPoint(const GUITypes::AnchorPoint anchorPoint);

            /**
             * Get the anchor point of the rectangle.
             * @return Anchor point.
             */
            GUITypes::AnchorPoint GetAnchorPoint() const;

            /**
             * Sets the parent of the current rectangle.
             * @param parent Parent of the rectangle.
             */
            void SetParent(GUIRectangle* parent);
            
            /**
             * Get the parent of the rectangle.
             * @return Parent of the current rectangle or nullptr if parent is
             * not set.
             */
            GUIRectangle* GetParent() const;

            void SetPercentageSize(bool value = true);
            bool GetPercentageSize() const;

            void SetDraggable(bool value);
            bool IsDraggable();

            void SetResizeable(bool value);
            bool IsResizeable();
            
            /**
             * Get current drag areas for this rectangle.
             * @return Reference to vector of current drag areas.
             */
            std::vector<NGE::Math::vec4f>& GetDragAreas();

            /**
             * Check the mouse event.
             * @param event Event passed from the window to this class.
             */
            virtual void CheckMouseEvent(NGE::Events::MouseEvent& event);

            /**
             * Check the keyboard event.
             * @param event Event passed from the window to this class.
             */
            virtual void CheckKeyboardEvent(NGE::Events::KeyboardEvent& event);

            void UpdateRectangle(bool force = false);

          protected:
            /**
             * Create rectangle geometry. This method initialize "vertices" and "indices"
             * arrays.
             * @return True if everything went ok, otherwise false. 
             */
            bool CreateRectangleGeometry();

            /**
             * This method creates vertex buffer objects. It should be invoked after 
             * CreateRectangleGeometry().
             * @return True if everything went ok, otherwise false.
             */
            bool CreateRectangleVBO();

            /**
             * Create vertex array object. This method should be invoked after
             * CreateRectangleVBO().
             * @return True if everything went ok, otherwise false.
             */
            bool CreateRectangleVBA();

            /**
             * Calculate real dimensions based on position, dimensions and anchorPoint.
             */
            void CalculateRealDimensions();

            /**
             * Calculate the default drag area - it's 10% of the rectangle size from the
             * top of it. It should be called after CalculateRealDimensions() because
             * this method needs real size and position of the rectangle. It is called
             * automaticly after calculateing real dimensions.
             */
            void CalculateDefaultDragArea();

            /**
             * Check if event happend in one of the drag areas.
             * @param event Mouse event to check.
             * @return True if event occured in one of the ares, otherwise false.
             */
            bool MouseInDragArea(NGE::Events::MouseEvent& event);

          protected:
            // TODO: Remove this after testing.
            unsigned int vertexBuffer, colorBuffer, indexBuffer, vertexArray;
            /**
             * Array of vertices used by rectangle. It size should be 4.
             */
            std::vector<Math::vec3f> vertices;

            /**
             * Array of colors used by rectangle.
             */
            std::vector<Math::vec4f> colors;

            /**
             * Array of indices used by rectangle. It size should be 6, because
             * we render rectangle as two triangles.
             */
            std::vector<unsigned int> indices;

            /**
             * Pointer to shader that is used to render the rectangle.
             */
            NGE::Media::Shaders::GLSLProgram* shader;

            /**
             * Position of the rectangle depending on anchorPoint.
             * @see anchorPoint
             */
            Math::vec2i position;

            /**
             * Dimensions of the rectangle. X is width, Y is height.
             */
            Math::vec2i dimensions;

            /**
             * Real dimensions of the rectangle.
             */
            Math::vec4i realDimensions;

            /**
             * If this variable is set to true, than dimensions are treated as percentages
             * values. It means, that you can specific the size of the rectangle by 
             * percent of parent size. If the parent is NULL than we take window dimensions.
             */
            bool percentageSize;

            /**
             * Anchor point of the rectangle. It can be left bottom, right bottom,
             * left upper, right upper corner of the parent, or window if the parent
             * is not set.
             */
            GUITypes::AnchorPoint anchorPoint;

            /**
             * Pointer to the rectangle parent. It can be NULL.
             */
            GUIRectangle* parent;

            /**
             * Variable that indicates if the rectangle needs update. It is set to true
             * when e.g position changes.
             */
            bool needUpdate;

            /**
             * Variable that indicates if the rectangle can be dragged.
             */
            bool isDraggable;

            /**
             * Variable that indicates if the rectangle can be resized.
             */
            bool isResizeable;

            /**
             * Position of the mouse, when OnBeginResize event occurred.
             * It is used to calculate the size of the window after
             * resizing.
             */
            Math::vec2i startResizeMousePosition;

            /**
             * Position of the mouse when OnEndResize event occurred.
             * It is used to calculate the size of the window after
             * resizing.
             */
            Math::vec2i endResizeMousePosition;

            /**
             * Keeps the copy of previous mouse event. It is used to determine the actions
             * e.g click (which is press and release event).
             */
            NGE::Events::MouseEvent previousMouseEvent;

            /**
             * Keeps the array of drag areas. Drag area is an area of the rectangle
             * that can be clicked and drag rectangle around. By default there is
             * on drag area on top of the rectangle - 10% of the overall size.
             */
            std::vector<NGE::Math::vec4f> dragAreas;

            /**
             * Variable that indicates if the rectangle is active.
             */
            bool isActive;

            /**
             * Variable that indicates if the rectangle is pressed.
             */
            bool isPressed;

            /**
             * Variable that indicates if the rectangle is released.
             */
            bool isReleased;

            /**
             * Variable that indicates if the rectangle is clicked (pressed and released).
             */
            bool isClicked;

            /**
             * Variable that indicates if the mouse is over the rectangle.
             */
            bool isHover;

            /**
             * Variable that indicates if the rectangle is beeing draged.
             */
            bool isDraged;

            /**
             * Variable that indicates if the rectangle is beeing resized.
             */
            bool isResized;
			
//            boost::function<void (NGE::Events::MouseEvent*) > OnActivateFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnDeactivateFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnHoverFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnPressFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnReleaseFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnClickFunc;
//
//            boost::function<void (NGE::Events::MouseEvent*) > OnDragFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnBeginDragFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnEndDragFunc;
//
//            boost::function<void (NGE::Events::MouseEvent*) > OnResizeFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnBeginResizeFunc;
//            boost::function<void (NGE::Events::MouseEvent*) > OnEndResizeFunc;

          public:

            // TODO: Remove this after testing.
            NGE::Math::mat4f translateMatrix;

            void OnActivate(NGE::Events::MouseEvent* event);
            void OnDeactivate(NGE::Events::MouseEvent* event);
            void OnHover(NGE::Events::MouseEvent* event);
            void OnPress(NGE::Events::MouseEvent* event);
            void OnRelease(NGE::Events::MouseEvent* event);
            void OnClick(NGE::Events::MouseEvent* event);
            void OnDrag(NGE::Events::MouseEvent* event);
            void OnBeginDrag(NGE::Events::MouseEvent* event);
            void OnEndDrag(NGE::Events::MouseEvent* event);
            void OnResize(NGE::Events::MouseEvent* event);
            void OnBeginResize(NGE::Events::MouseEvent* event);
            void OnEndResize(NGE::Events::MouseEvent* event);

            template <class Operation, class T> void SetOnActivateFunc(const Operation& op, const T& t)
            {
                //OnActivateFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnDeactivateFunc(const Operation& op, const T& t)
            {
                //OnDeactivateFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnHoverFunc(const Operation& op, const T& t)
            {
                //OnHoverFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnPressFunc(const Operation& op, const T& t)
            {
                //OnPressFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnReleaseFunc(const Operation& op, const T& t)
            {
                //OnReleaseFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnClickFunc(const Operation& op, const T& t)
            {
                //OnClickFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnDragFunc(const Operation& op, const T& t)
            {
                //OnDragFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnBeginDragFunc(const Operation& op, const T& t)
            {
                //OnBeginDragFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnEndDragFunc(const Operation& op, const T& t)
            {
                //OnEndDragFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnResizeFunc(const Operation& op, const T& t)
            {
                //OnResizeFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnBeginResizeFunc(const Operation& op, const T& t)
            {
                //OnBeginResizeFunc = std::bind1st(std::mem_fun(op), t);
            }

            template <class Operation, class T> void SetOnEndResizeFunc(const Operation& op, const T& t)
            {
                //OnEndResizeFunc = std::bind1st(std::mem_fun(op), t);
            }

            // TODO: Remove that after testing
            NGE::Math::vec4f color;
        };
    }
}

#endif /* GUIRECTANGLE_HPP */

