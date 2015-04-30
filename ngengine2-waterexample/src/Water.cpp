#include "Water.h"
#include <GL/glew.h>

Water::Water(unsigned int waterPlaneLength)
{
    this->waterPlaneLength = waterPlaneLength;
}

bool Water::Initialize(NGE::Media::Images::Texture* texture, NGE::Media::Shaders::GLSLProgram* shader)
{
    waterTexture = texture;
    waterShader = shader;
    passedTime = 0.f;

    InitializeGeometry();
    InitializeVBO();
    InitializeVBA();
}

void Water::Deinitialize()
{
    delete vertices;
    delete indices;

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void Water::Update(float dt)
{
    if (dt > 0.f)
        passedTime += dt;
}

void Water::Render(NGE::Math::mat4f* modelviewMatrix, NGE::Math::mat4f* projectionMatrix)
{
    if (waterShader == NULL && waterTexture == NULL)
        return;
    
    waterShader->BindShader();
    {
        waterShader->SendUniform4x4("projectionMatrix", projectionMatrix);
        waterShader->SendUniform4x4("viewMatrix", modelviewMatrix);
        
        NGE::Math::mat4f inverseViewMatrix = modelviewMatrix;
        inverseViewMatrix.SetInverse();
        waterShader->SendUniform3x3("inverseViewNormalMatrix", inverseViewMatrix.ExtractMatrix3());
        
        waterShader->SendUniform("waterPlaneLength", (float)waterPlaneLength);
        waterShader->SendUniform("passedTime", passedTime);
    }
    waterShader->UnbindShader();
}

void Water::SetShader(NGE::Media::Shaders::GLSLProgram* shader)
{
    waterShader = shader;
}

void Water::SetTexture(NGE::Media::Images::Texture* texture)
{
    waterTexture = texture;
}

unsigned int Water::GetWaterPlaneLength()
{
    return waterPlaneLength;
}

void Water::InitializeGeometry()
{
    vertices = new float[waterPlaneLength * waterPlaneLength * 4 * sizeof (float)];
    indices = new unsigned int[waterPlaneLength * (waterPlaneLength - 1) * 2 * sizeof (unsigned int)];

    for (int z = 0; z < waterPlaneLength; ++z)
    {
        for (int x = 0; x < waterPlaneLength; ++x)
        {
            vertices[(x + z * (waterPlaneLength)) * 4 + 0] = -(float) waterPlaneLength / 2 + 0.5f + (float) x;
            vertices[(x + z * (waterPlaneLength)) * 4 + 1] = 0.0f;
            vertices[(x + z * (waterPlaneLength)) * 4 + 2] = +(float) waterPlaneLength / 2 - 0.5f - (float) z;
            vertices[(x + z * (waterPlaneLength)) * 4 + 3] = 1.0f;
        }
    }

    for (int k = 0; k < waterPlaneLength; ++k)
    {
        for (int i = 0; i < waterPlaneLength; ++i)
        {
            if (k % 2 == 0)
            {
                indices[(i + k * waterPlaneLength) * 2 + 0] = i + (k + 1) * waterPlaneLength;
                indices[(i + k * waterPlaneLength) * 2 + 1] = i + k * waterPlaneLength;
            }
            else
            {
                indices[(i + k * waterPlaneLength) * 2 + 0] = waterPlaneLength - 1 - i + k * waterPlaneLength;
                indices[(i + k * waterPlaneLength) * 2 + 1] = waterPlaneLength - 1 - i + (k + 1) * waterPlaneLength;
            }
        }
    }
}

void Water::InitializeVBO()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, waterPlaneLength * waterPlaneLength * 4 * sizeof (float), (float*) vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, waterPlaneLength * (waterPlaneLength - 1) * 2 * sizeof (unsigned int), (unsigned int*) indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Water::InitializeVBA()
{
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);
}


