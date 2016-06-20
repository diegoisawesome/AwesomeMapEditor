/*
 * IndexedVertexShader.glsl
 * Copyright (C) 2015-2016 Pokedude
 *
 * Vertex shader to forward variables to the fragment shader.
 * This includes the coordinate position, the vertex position
 * and a tinting color for future day and night implementations.
 */


#version 330

// Input variables
layout(location = 0) in vec2 in_vertex;
layout(location = 1) in vec2 in_coords;

// Output variables (frag)
out vec2 frag_coords;

// Uniform variables
uniform mat4 uni_mvp;


/* Specifies the vertex position; forwards variables to fragment shader */
void main()
{
    frag_coords     = in_coords;
    gl_Position     = uni_mvp * vec4(in_vertex, 0.0, 1.0);
}
