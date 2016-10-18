#include "GUIRectangle.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Rendering/Renderer.hpp"

using namespace NGE::GUI2;

GUIRectangle::GUIRectangle()
{
    position = Math::vec2i(0, 0);
    dimensions = Math::vec2i(10, 10);
    anchorPoint = GUITypes::BOTTOM_LEFT;
    percentageSize = false;
    shader = nullptr;
    parent = nullptr;

    isActive = false;
    isDraged = false;
    isClicked = false;
    isHover = false;
    isPressed = false;
    isReleased = false;

    isDraggable = false;

    SetOnActivateFunc(&GUIRectangle::OnActivate, this);
    SetOnDeactivateFunc(&GUIRectangle::OnDeactivate, this);
    SetOnHoverFunc(&GUIRectangle::OnHover, this);
    SetOnClickFunc(&GUIRectangle::OnClick, this);
    SetOnPressFunc(&GUIRectangle::OnPress, this);
    SetOnReleaseFunc(&GUIRectangle::OnRelease, this);

    SetOnDragFunc(&GUIRectangle::OnDrag, this);
    SetOnBeginDragFunc(&GUIRectangle::OnBeginDrag, this);
    SetOnEndDragFunc(&GUIRectangle::OnEndDrag, this);

    SetOnResizeFunc(&GUIRectangle::OnResize, this);
    SetOnBeginResizeFunc(&GUIRectangle::OnBeginResize, this);
    SetOnEndResizeFunc(&GUIRectangle::OnEndResize, this);
}

GUIRectangle::GUIRectangle(const Math::vec2i& position, const Math::vec2i& dimensions, const GUITypes::AnchorPoint anchorPoint)
{
    GUIRectangle();

    SetPosition(position);
    SetDimensions(dimensions);
    SetAnchorPoint(anchorPoint);
    shader = NULL;
}

bool GUIRectangle::Initialize()
{
    CreateRectangleGeometry();
    CreateRectangleVBO();
    CreateRectangleVBA();
}

void GUIRectangle::Update(float deltaTime) { }

void GUIRectangle::Render()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    shader->BindShader();
    {
        Math::mat4f modelviewMatrix;

        if (isDraged)
        {
            modelviewMatrix = translateMatrix;
        }

        shader->sendUniform4x4("modelviewMatrix", modelviewMatrix);
        shader->sendUniform4x4("projectionMatrix", NGE::Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(NGE::PROJECTION_MATRIX));
        shader->sendUniform("color", color);

        glBindVertexArray(vertexArray);
        {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
    }
    shader->UnbindShader();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void GUIRectangle::SetShader(NGE::Media::Shaders::GLSLProgram* shader)
{
    if (shader != NULL)
        this->shader = shader;
}

void GUIRectangle::SetPosition(const Math::vec2i& position)
{
    SetPosition(position.x, position.y);
}

void GUIRectangle::SetPosition(const int x, const int y)
{
    position.Set(x, y);
    needUpdate = true;
}

NGE::Math::vec2i GUIRectangle::GetPosition() const
{
    return position;
}

void GUIRectangle::SetDimensions(const Math::vec2i& dimensions)
{
    this->dimensions = dimensions;
    if (dimensions.x < 1)
        this->dimensions.x = 1;
    if (dimensions.y < 1)
        this->dimensions.y = 1;
    needUpdate = true;
}

void GUIRectangle::SetDimensions(const int width, const int height)
{
    SetDimensions(Math::vec2i(width, height));
}

NGE::Math::vec2i GUIRectangle::GetDimensions() const
{
    return dimensions;
}

NGE::Math::vec4i GUIRectangle::GetRealDimensions() const
{
    return realDimensions;
}

void GUIRectangle::SetAnchorPoint(const GUITypes::AnchorPoint anchorPoint)
{
    this->anchorPoint = anchorPoint;
    needUpdate = true;
}

GUITypes::AnchorPoint GUIRectangle::GetAnchorPoint() const
{
    return anchorPoint;
}

void GUIRectangle::SetParent(GUIRectangle* parent)
{
    this->parent = parent;
}

GUIRectangle* GUIRectangle::GetParent() const
{
    return parent;
}

void GUIRectangle::SetPercentageSize(bool value)
{
    percentageSize = value;
}

bool GUIRectangle::GetPercentageSize() const
{
    return percentageSize;
}

void GUIRectangle::SetDraggable(bool value)
{
    isDraggable = value;
}

bool GUIRectangle::IsDraggable()
{
    return isDraggable;
}

void GUIRectangle::SetResizeable(bool value)
{
    isResizeable = value;
}

bool GUIRectangle::IsResizeable()
{
    return isResizeable;
}

void GUIRectangle::CheckMouseEvent(NGE::Events::MouseEvent& event)
{
    if (event.GetX() >= realDimensions.x && event.GetX() <= realDimensions.z &&
        event.GetYInverse() >= realDimensions.y && event.GetYInverse() <= realDimensions.w)
    {
        //OnHoverFunc(&event);
        isHover = true;
    }

    if (event.GetAction() == NGE::Events::InputAction::PRESSED)
    {
        // Check if event happend inside the rectangle.
        if (event.GetX() >= realDimensions.x && event.GetX() <= realDimensions.z &&
            event.GetYInverse() >= realDimensions.y && event.GetYInverse() <= realDimensions.w)
        {
            if (!isActive)
            {
                //OnActivateFunc(&event);
                isActive = true;
            }

            //OnPressFunc(&event);
            isPressed = true;

            if (isPressed && event.GetPosition() != previousMouseEvent.GetPosition())
            {

                if (!isDraged && isDraggable && MouseInDragArea(event))
                {
                    //OnBeginDragFunc(&event);
                }

                if (isDraggable && MouseInDragArea(event))
                {
                    //OnDragFunc(&event);
                    isDraged = true;
                }

                if (!isResized && isResizeable)
                {
                    //OnBeginResizeFunc(&event);
                }

                if (isResizeable)
                {
                    //OnResizeFunc(&event);
                    isResized = true;
                }
            }
        }
        else
        {
            if (isActive)
            {
                /*if (isDraged && isDraggable)
                {
                    OnEndDragFunc(&event);
                }*/

                //if (isResized)
                //{
                //OnEndResizeFunc(&event);
                //}

                //OnDeactivateFunc(&event);

                isActive = false;
                isPressed = false;
                //isDraged = false;
                //isResized = false;
            }
        }
    }
    else if (event.GetAction() == NGE::Events::InputAction::RELEASED)
    {
        // Check if event happend inside the rectangle.
        if (event.GetX() >= realDimensions.x && event.GetX() <= realDimensions.z &&
            event.GetYInverse() >= realDimensions.y && event.GetYInverse() <= realDimensions.w)
        {
            if (isPressed)
            {
                //OnReleaseFunc(&event);
                isReleased = true;

                //OnClickFunc(&event);
                isClicked = true;
            }
        }

        if (isResized && isResizeable)
        {
            //OnEndResizeFunc(&event);
            isResized = false;
        }

        if (isDraged && isDraggable)
        {
            //OnEndDragFunc(&event);
            isDraged = false;
        }
    }

    previousMouseEvent = event;
}

void GUIRectangle::CheckKeyboardEvent(NGE::Events::KeyboardEvent& event) { }

void GUIRectangle::UpdateRectangle(bool force)
{
    if (needUpdate || force)
    {
        CalculateRealDimensions();
    }
}

bool GUIRectangle::CreateRectangleGeometry()
{
    vertices.reserve(4);
    vertices.push_back(NGE::Math::vec3f(realDimensions.x, realDimensions.y, 0.0f));
    vertices.push_back(NGE::Math::vec3f(realDimensions.z, realDimensions.y, 0.0f));
    vertices.push_back(NGE::Math::vec3f(realDimensions.z, realDimensions.w, 0.0f));
    vertices.push_back(NGE::Math::vec3f(realDimensions.x, realDimensions.w, 0.0f));

    colors.reserve(4);
    colors.push_back(Math::vec4f(1, 0, 0, 1));
    colors.push_back(Math::vec4f(0, 1, 0, 1));
    colors.push_back(Math::vec4f(0, 0, 1, 1));
    colors.push_back(Math::vec4f(1, 1, 0, 1));

    indices.reserve(6);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);

    return true;
}

bool GUIRectangle::CreateRectangleVBO()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (float) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (float) * 4 * colors.size(), &colors[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    vertices.clear();
    colors.clear();
    indices.clear();
}

bool GUIRectangle::CreateRectangleVBA()
{
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);
}

void GUIRectangle::CalculateRealDimensions()
{
    Math::vec4i parentDimensions;
    Math::vec2i copyDimensions;
    if (parent == NULL)
    {
        // viewport[2] - width
        // viewport[3] - height
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        parentDimensions.Set(0, 0, viewport[2], viewport[3]);
    }
    else
    {
        parentDimensions = parent->GetRealDimensions();
    }

    if (percentageSize)
    {
        // Calculate dimensions.
        copyDimensions.x = (dimensions.x * parentDimensions.z) / 100.0;
        copyDimensions.y = (dimensions.y * parentDimensions.w) / 100.0;
    }
    else
    {
        copyDimensions = dimensions;
    }

    switch (anchorPoint)
    {
        case GUITypes::AnchorPoint::BOTTOM_LEFT:
        {
            realDimensions.x = parentDimensions.x + position.x;
            realDimensions.y = parentDimensions.y + position.y;
            realDimensions.z = parentDimensions.x + position.x + copyDimensions.x;
            realDimensions.w = parentDimensions.y + position.y + copyDimensions.y;
            break;
        }

        case GUITypes::AnchorPoint::BOTTOM_RIGHT:
        {
            realDimensions.x = parentDimensions.z - position.x - copyDimensions.x;
            realDimensions.y = parentDimensions.y + position.y;
            realDimensions.z = parentDimensions.z - position.x;
            realDimensions.w = parentDimensions.y + position.y + copyDimensions.y;
            break;
        }

        case GUITypes::AnchorPoint::TOP_LEFT:
        {
            realDimensions.x = parentDimensions.x + position.x;
            realDimensions.y = parentDimensions.w - position.y - copyDimensions.y;
            realDimensions.z = parentDimensions.x + position.x + copyDimensions.y;
            realDimensions.w = parentDimensions.w - position.y;
            break;
        }

        case GUITypes::AnchorPoint::TOP_RIGHT:
        {
            realDimensions.x = parentDimensions.z - position.x - copyDimensions.x;
            realDimensions.y = parentDimensions.w - position.y - copyDimensions.y;
            realDimensions.z = parentDimensions.z - position.x;
            realDimensions.w = parentDimensions.w - position.y;
            break;
        }
    }

    CalculateDefaultDragArea();
}

void GUIRectangle::CalculateDefaultDragArea()
{
    NGE::Math::vec4f defaultDragArea;

    defaultDragArea.x = realDimensions.x;
    defaultDragArea.y = realDimensions.y + 160;
    defaultDragArea.z = realDimensions.z;
    defaultDragArea.w = realDimensions.w;

    dragAreas.push_back(defaultDragArea);
}

bool GUIRectangle::MouseInDragArea(NGE::Events::MouseEvent& event)
{
    for (std::vector<NGE::Math::vec4f>::iterator it = dragAreas.begin(); it != dragAreas.end(); ++it)
    {
        if (event.GetX() >= it->x && event.GetX() <= it->z &&
            event.GetYInverse() >= it->y && event.GetYInverse() <= it->w)
            return true;
    }

    return false;
}

void GUIRectangle::OnActivate(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnDeactivate(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnHover(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnPress(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnRelease(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnClick(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnDrag(NGE::Events::MouseEvent* event)
{
    int xChange = event->GetX() - previousMouseEvent.GetX();
    int yChange = event->GetYInverse() - previousMouseEvent.GetYInverse();

    int positionX, positionY;
    switch (anchorPoint)
    {
        case GUITypes::AnchorPoint::BOTTOM_LEFT:
        {
            positionX = position.x + xChange;
            positionY = position.y + yChange;
            break;
        }

        case GUITypes::AnchorPoint::TOP_LEFT:
        {
            positionX = position.x + xChange;
            positionY = position.y - yChange;
            break;
        }

        case GUITypes::AnchorPoint::BOTTOM_RIGHT:
        {
            positionX = position.x - xChange;
            positionY = position.y + yChange;
            break;
        }

        case GUITypes::AnchorPoint::TOP_RIGHT:
        {
            positionX = position.x - xChange;
            positionY = position.y - yChange;
            break;
        }
    }

    SetPosition(positionX, positionY);
    CalculateRealDimensions();

    Math::mat4f trans;
    trans.Translate(xChange, yChange, 0);
    translateMatrix *= trans;
}

void GUIRectangle::OnBeginDrag(NGE::Events::MouseEvent* event)
{
    translateMatrix.SetIdentity();
}

void GUIRectangle::OnEndDrag(NGE::Events::MouseEvent* event)
{
    Initialize();
}

void GUIRectangle::OnResize(NGE::Events::MouseEvent* event) { }

void GUIRectangle::OnBeginResize(NGE::Events::MouseEvent* event)
{
    startResizeMousePosition = event->GetPosition();
}

void GUIRectangle::OnEndResize(NGE::Events::MouseEvent* event)
{
    endResizeMousePosition = event->GetPosition();
    dimensions.x += endResizeMousePosition.x - startResizeMousePosition.x;
    dimensions.y -= endResizeMousePosition.y - startResizeMousePosition.y;

    CalculateRealDimensions();
    Initialize();
}