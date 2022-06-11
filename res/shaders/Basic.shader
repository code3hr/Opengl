#shader vertex
#version 430  core
//locating the attribute in the vectex, in this case 0 as seen in glVertexAttribPointer
layout(location = 0) in vec2 position; //note position have 2 dimention

void main()
{
gl_Position = vec4(position.xy,0.0,1.0); //casting to vec4

};



//writing frament shaderstd::string fragmentShader =
//#version 430 core
#shader fragment

void main()
{
gl_FragColor = vec4(0.2,0.3,0.8,1.0);

};