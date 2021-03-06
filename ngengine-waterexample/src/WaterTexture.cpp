/* 
 * File:   WaterTexture.cpp
 * Author: tku
 * 
 * Created on 20 czerwiec 2014, 16:03
 */

#include "WaterTexture.h"
#include <NGE/Tools/Logger/NewLogger.hpp>
#include <NGE/Math/Math.hpp>
#include <GL/glew.h>

unsigned int WaterTexture::Initialise(float waterPlaneLength)
{
    this->waterPlaneLength = waterPlaneLength;
    
    glGenTextures(1, &mirrorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mirrorTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize, textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &depthMirrorTexture);
    glBindRenderbuffer(GL_RENDERBUFFER, depthMirrorTexture);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureSize, textureSize);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &fboWaterTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, fboWaterTexture);

    // Attach the color buffer.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);

    // Attach the depth buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthMirrorTexture);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        //log_error("WaterTexture --> GL_FRAMEBUFFER_COMPLETE error: " + to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindVertexArray(0);

    vertices.reserve(4);
    vertices.push_back(NGE::Math::vec3f(-1.0f, -1.0f, 0.0f));
    vertices.push_back(NGE::Math::vec3f(1.0f, -1.0f, 0.0f));
    vertices.push_back(NGE::Math::vec3f(-1.0f, 1.0f, 0.0f));
    vertices.push_back(NGE::Math::vec3f(1.0f, 1.0f, 0.0f));

    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        vertices[i].x *= (textureSize / 2);
        vertices[i].y *= (textureSize / 2);
    }

    texCoords.reserve(4);
    texCoords.push_back(NGE::Math::vec2f(0.0f, 0.0f));
    texCoords.push_back(NGE::Math::vec2f(1.0f, 0.0f));
    texCoords.push_back(NGE::Math::vec2f(0.0f, 1.0f));
    texCoords.push_back(NGE::Math::vec2f(1.0f, 1.0f));

    indices.reserve(6);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(2);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof (GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * texCoords.size() * sizeof (GLfloat), &texCoords[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (GLuint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return mirrorTexture;
}

void WaterTexture::Update(float dt)
{
    passedTime = dt;
}

void WaterTexture::Render()
{
    static const unsigned int numberWaves = 4;
    static WaveParameters waveParameters[numberWaves];
    static WaveDirections waveDirections[numberWaves];
    static GLfloat overallSteepness = 0.2f;

    // Waves.
    memset(waveParameters, 0, sizeof (waveParameters));
    memset(waveDirections, 0, sizeof (waveDirections));

    // Wave one.
    waveParameters[0].speed = 0.05f;
    waveParameters[0].amplitude = 0.02f;
    waveParameters[0].wavelength = 0.3f;
    waveParameters[0].steepness = overallSteepness / (waveParameters[0].wavelength * waveParameters[0].amplitude * (float) numberWaves);
    waveDirections[0].x = +1.0f;
    waveDirections[0].z = +1.5f;

    // Wave two.
    waveParameters[1].speed = 0.1f;
    waveParameters[1].amplitude = 0.01f;
    waveParameters[1].wavelength = 0.4f;
    waveParameters[1].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (float) numberWaves);
    waveDirections[1].x = +0.8f;
    waveDirections[1].z = +0.2f;

    // Wave three.
    waveParameters[2].speed = 0.04f;
    waveParameters[2].amplitude = 0.035f;
    waveParameters[2].wavelength = 0.1f;
    waveParameters[2].steepness = overallSteepness / (waveParameters[2].wavelength * waveParameters[2].amplitude * (float) numberWaves);
    waveDirections[2].x = -0.2f;
    waveDirections[2].z = -0.1f;

    // Wave four.
    waveParameters[3].speed = 0.05f;
    waveParameters[3].amplitude = 0.007f;
    waveParameters[3].wavelength = 0.2f;
    waveParameters[3].steepness = overallSteepness / (waveParameters[3].wavelength * waveParameters[3].amplitude * (float) numberWaves);
    waveDirections[3].x = -0.4f;
    waveDirections[3].z = -0.3f;

    glViewport(0, 0, textureSize, textureSize);

    glBindFramebuffer(GL_FRAMEBUFFER, fboWaterTexture);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->BindShader();
    
    NGE::Math::mat4f modelviewMatrix;
    modelviewMatrix.LookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    shader->sendUniform4x4("modelviewMatrix", modelviewMatrix);

    NGE::Math::mat4f projectionMatrix;
    projectionMatrix.SetOrthographicProjection(-(float) textureSize / 2, (float) textureSize / 2, -(float) textureSize / 2, (float) textureSize / 2, 1.0f, 100.0f);
    shader->sendUniform4x4("projectionMatrix", projectionMatrix);

    shader->sendUniform("waterPlaneLength", waterPlaneLength);

    shader->sendUniform("passedTime", passedTime);
    shader->sendUniformArray4("waveParameters", 4 * numberWaves, (float*) waveParameters);
    shader->sendUniformArray2("waveDirections", 2 * numberWaves, (float*) waveDirections);

    glFrontFace(GL_CCW);

    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, parentWidth, parentHeight);
}

void WaterTexture::SetShader(NGE::Media::Shaders::GLSLProgram* shader)
{
    this->shader = shader;
}

void WaterTexture::Resize(int width, int height)
{
    parentWidth = width;
    parentHeight = height;
}


